#pragma once
#include <SDL.h>
#include "glew.h"
#include <memory>

namespace MatGl {

    //Custom resource deleter for sdl surface.
    struct SdlSurfaceDeleter {
        //Called by unique_ptr to destroy/free the Resource
        void operator()(SDL_Surface* surface);
    };

    std::unique_ptr<SDL_Surface, SdlSurfaceDeleter> LoadTexture(const char* filePath);
}