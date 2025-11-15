#pragma once

namespace Engine {
    class Engine {
    public:
        Engine();
        ~Engine();
        
        // Initialize the engine
        bool Initialize();
        
        // Shutdown the engine
        void Shutdown();
        
        // Main game loop - runs until the engine should stop
        void Run();
        
        // Variable timestep update (called every frame)
        void Update(float deltaTime);
        
        // Fixed timestep update (called at fixed intervals, typically 60 Hz)
        void FixedUpdate(float fixedDeltaTime);
        
        // Render (called every frame)
        void Render();
        
        // Check if engine is running
        bool IsRunning() const { return m_IsRunning; }
        
        // Request engine to stop
        void Stop() { m_IsRunning = false; }
        
    private:
        bool m_IsRunning;
        bool m_IsInitialized;
        
        // Fixed timestep accumulator
        float m_Accumulator;
        static constexpr float FIXED_DELTA_TIME = 1.0f / 60.0f; // 60 Hz
    };
}

