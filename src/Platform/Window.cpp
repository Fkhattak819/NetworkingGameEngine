#include "Platform/Window.hpp"
#include <iostream>
#include <GLFW/glfw3.h>

namespace Engine {
    Window::Window()
        : m_WindowHandle(nullptr)
        , m_Width(0)
        , m_Height(0)
        , m_IsInitialized(false)
    {
    }
    
    Window::~Window() {
        if (m_IsInitialized) {
            Shutdown();
        }
    }
    
    bool Window::Initialize(int width, int height, const char* title) {
        if (m_IsInitialized) {
            std::cout << "[WINDOW] Already initialized.\n";
            return true;
        }
        
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "[WINDOW] Failed to initialize GLFW.\n";
            return false;
        }
        
        // Configure GLFW
        #ifdef __APPLE__
        // macOS only supports OpenGL 4.1
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #else
        // Linux/Windows can use OpenGL 4.5
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #endif
        
        // Create window
        GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            std::cerr << "[WINDOW] Failed to create GLFW window.\n";
            const char* description;
            int code = glfwGetError(&description);
            if (description) {
                std::cerr << "[WINDOW] Error code: " << code << " - " << description << "\n";
            }
            glfwTerminate();
            return false;
        }
        
        // Make context current
        glfwMakeContextCurrent(window);
        
        m_WindowHandle = window;
        m_Width = width;
        m_Height = height;
        m_IsInitialized = true;
        
        std::cout << "[WINDOW] Initialized: " << width << "x" << height << " - " << title << "\n";
        return true;
    }
    
    void Window::Shutdown() {
        if (!m_IsInitialized) {
            return;
        }
        
        if (m_WindowHandle) {
            glfwDestroyWindow(static_cast<GLFWwindow*>(m_WindowHandle));
            m_WindowHandle = nullptr;
        }
        
        glfwTerminate();
        m_IsInitialized = false;
        
        std::cout << "[WINDOW] Shutdown complete.\n";
    }
    
    bool Window::ShouldClose() const {
        if (!m_IsInitialized || !m_WindowHandle) {
            return true;
        }
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_WindowHandle));
    }
    
    void Window::PollEvents() {
        if (m_IsInitialized) {
            glfwPollEvents();
        }
    }
    
    void Window::SwapBuffers() {
        if (m_IsInitialized && m_WindowHandle) {
            glfwSwapBuffers(static_cast<GLFWwindow*>(m_WindowHandle));
        }
    }
}

