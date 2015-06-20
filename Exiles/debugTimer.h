#pragma once

#include <SDL2/SDL.h>
#include <iostream>

class DebugTimer
{
public:
    void Start()
    {
        startTime = SDL_GetTicks();
    }
    
    void End(const std::string& message)
    {
        unsigned int endTime = SDL_GetTicks();
        std::cout << message << (endTime - startTime) << "ms" << std::endl;
    }
    
protected:
private:
    unsigned int startTime;
};
