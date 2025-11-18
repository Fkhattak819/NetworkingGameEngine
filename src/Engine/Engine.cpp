#include "Engine/Engine.hpp"
#include "Platform/Window.hpp"
#include <iostream>
#include <chrono>

// OpenGL includes
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

namespace Engine {
    Engine::Engine()
        : m_IsRunning(false)
        , m_IsInitialized(false)
        , m_Accumulator(0.0f)
        , m_Window(nullptr)
        , m_VAO(0)
        , m_VBO(0)
        , m_ShaderProgram(0)
    {
    }
    
    Engine::~Engine() {
        if (m_IsInitialized) {
            Shutdown();
        }
    }
    
    bool Engine::Initialize() {
        if (m_IsInitialized) {
            std::cout << "[ENGINE] Already initialized.\n";
            return true;
        }
        
        std::cout << "[ENGINE] Initializing...\n";
        
        // Initialize Window subsystem
        m_Window = new Window();
        if (!m_Window->Initialize(1280, 720, "MyEngine")) {
            std::cerr << "[ENGINE] Failed to initialize window.\n";
            delete m_Window;
            m_Window = nullptr;
            return false;
        }
        
        // Initialize triangle rendering
        InitTriangle();
        
        // TODO: Initialize other subsystems here
        // - Input subsystem
        // - Renderer subsystem
        // - Time subsystem
        
        m_IsInitialized = true;
        m_IsRunning = true;
        
        std::cout << "[ENGINE] Initialization complete.\n";
        return true;
    }
    
    void Engine::Shutdown() {
        if (!m_IsInitialized) {
            return;
        }
        
        std::cout << "[ENGINE] Shutting down...\n";
        
        // Cleanup triangle resources
        CleanupTriangle();
        
        // Shutdown Window subsystem
        if (m_Window) {
            m_Window->Shutdown();
            delete m_Window;
            m_Window = nullptr;
        }
        
        // TODO: Shutdown other subsystems here
        
        m_IsRunning = false;
        m_IsInitialized = false;
        
        std::cout << "[ENGINE] Shutdown complete.\n";
    }
    
    void Engine::Run() {
        if (!m_IsInitialized) {
            std::cerr << "[ENGINE] Error: Engine not initialized. Call Initialize() first.\n";
            return;
        }
        
        // High-resolution clock for timing
        auto lastTime = std::chrono::high_resolution_clock::now();
        m_Accumulator = 0.0f;
        
        std::cout << "[ENGINE] Starting main loop...\n";
        
        while (m_IsRunning) {
            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            float deltaTime = duration;
            
            // Clamp delta time to prevent large spikes
            const float MAX_DELTA_TIME = 0.1f; // 100ms max
            if (deltaTime > MAX_DELTA_TIME) {
                deltaTime = MAX_DELTA_TIME;
            }
            
            // Add to accumulator for fixed timestep
            m_Accumulator += deltaTime;
            
            // Process fixed timestep updates
            while (m_Accumulator >= FIXED_DELTA_TIME) {
                FixedUpdate(FIXED_DELTA_TIME);
                m_Accumulator -= FIXED_DELTA_TIME;
            }
            
            // Variable timestep update
            Update(deltaTime);
            
            // Render
            Render();
            
            // Swap buffers (present frame)
            if (m_Window) {
                m_Window->SwapBuffers();
            }
            
            // Poll window events (handles input, window close, etc.)
            if (m_Window) {
                m_Window->PollEvents();
                
                // Check if window should close
                if (m_Window->ShouldClose()) {
                    Stop();
                }
            }
        }
        
        std::cout << "[ENGINE] Main loop ended.\n";
    }
    
    void Engine::Update(float deltaTime) {
        // Variable timestep update logic goes here
        // This is called every frame with variable delta time
        (void)deltaTime; // Suppress unused parameter warning for now
    }
    
    void Engine::FixedUpdate(float fixedDeltaTime) {
        // Fixed timestep update logic goes here
        // This is called at a fixed rate (60 Hz) for deterministic simulation
        (void)fixedDeltaTime; // Suppress unused parameter warning for now
    }
    
    void Engine::Render() {
        // Clear the screen with a dark blue color
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);  // Dark blue background
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Set viewport (in case window was resized)
        if (m_Window) {
            glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
        }
        
        // Draw triangle
        if (m_ShaderProgram != 0 && m_VAO != 0) {
            glUseProgram(m_ShaderProgram);
            glBindVertexArray(m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw 3 vertices as a triangle
            glBindVertexArray(0);
        }
    }
    
    void Engine::InitTriangle() {
        // Simple vertex shader source code
        const char* vertexShaderSource = R"(
            #version 410 core
            layout (location = 0) in vec3 aPos;
            
            void main() {
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            }
        )";
        
        // Simple fragment shader source code
        const char* fragmentShaderSource = R"(
            #version 410 core
            out vec4 FragColor;
            
            void main() {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);  // Orange color
            }
        )";
        
        // Compile vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        
        // Check vertex shader compilation
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "[RENDER] Vertex shader compilation failed: " << infoLog << "\n";
            return;
        }
        
        // Compile fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        
        // Check fragment shader compilation
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "[RENDER] Fragment shader compilation failed: " << infoLog << "\n";
            glDeleteShader(vertexShader);
            return;
        }
        
        // Create shader program
        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, vertexShader);
        glAttachShader(m_ShaderProgram, fragmentShader);
        glLinkProgram(m_ShaderProgram);
        
        // Check shader program linking
        glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
            std::cerr << "[RENDER] Shader program linking failed: " << infoLog << "\n";
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return;
        }
        
        // Delete shaders (they're linked into the program now)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        // Triangle vertices (normalized device coordinates: -1 to 1)
        float vertices[] = {
             0.0f,  0.5f, 0.0f,  // Top vertex
            -0.5f, -0.5f, 0.0f,  // Bottom left
             0.5f, -0.5f, 0.0f   // Bottom right
        };
        
        // Generate and bind VAO
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        
        // Generate and bind VBO
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        std::cout << "[RENDER] Triangle initialized.\n";
    }
    
    void Engine::CleanupTriangle() {
        if (m_VAO != 0) {
            glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
        }
        if (m_VBO != 0) {
            glDeleteBuffers(1, &m_VBO);
            m_VBO = 0;
        }
        if (m_ShaderProgram != 0) {
            glDeleteProgram(m_ShaderProgram);
            m_ShaderProgram = 0;
        }
    }
}

