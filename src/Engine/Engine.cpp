#include "Engine/Engine.hpp"
#include <iostream>
#include <chrono>

namespace Engine {
    Engine::Engine()
        : m_IsRunning(false)
        , m_IsInitialized(false)
        , m_Accumulator(0.0f)
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
        
        // TODO: Initialize subsystems here
        // - Window subsystem
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
        
        // TODO: Shutdown subsystems here
        
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
            
            // TODO: Check window events, input, etc. to determine if we should stop
            // For now, stop after a short test period (remove this once window system is added)
            static int frameCount = 0;
            frameCount++;
            if (frameCount >= 60) { // Run for ~1 second at 60 FPS
                Stop();
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
        // Render logic goes here
        // This is called every frame
    }
}

