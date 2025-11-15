#include "Engine/Engine.hpp"
#include <iostream>

int main() {
    Engine::Engine engine;
    
    if (!engine.Initialize()) {
        std::cerr << "[ENGINE] Failed to initialize engine.\n";
        return 1;
    }
    
    // Run the main game loop
    engine.Run();
    
    // Shutdown is called automatically in destructor
    return 0;
}
