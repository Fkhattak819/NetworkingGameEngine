#pragma once

namespace Engine {
    class Window {
    public:
        Window();
        ~Window();
        
        // Initialize the window
        bool Initialize(int width = 1280, int height = 720, const char* title = "MyEngine");
        
        // Shutdown the window
        void Shutdown();
        
        // Check if window should close
        bool ShouldClose() const;
        
        // Poll window events (call once per frame)
        void PollEvents();
        
        // Swap buffers (call after rendering)
        void SwapBuffers();
        
        // Get window dimensions
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        
        // Check if window is initialized
        bool IsInitialized() const { return m_IsInitialized; }
        
    private:
        void* m_WindowHandle;  // GLFWwindow* (using void* to avoid including GLFW in header)
        int m_Width;
        int m_Height;
        bool m_IsInitialized;
    };
}

