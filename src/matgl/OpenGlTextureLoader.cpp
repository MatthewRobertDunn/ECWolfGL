#include "OpenGlTextureLoader.h"
#include "glew.h"
#include <SDL.h>
#include <iostream>
#include <sstream>
#include <memory>
namespace MatGl
{
	void SdlSurfaceDeleter::operator()(SDL_Surface* surface)
	{
		if (surface)
		{
			SDL_FreeSurface(surface);
		}
	}

	// Function to load a texture from a file and create an OpenGL texture object
	std::unique_ptr<SDL_Surface, SdlSurfaceDeleter> LoadTexture(const char* filePath) {
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load the texture using SDL2
		SDL_Surface* surface = SDL_LoadBMP(filePath);
		if (!surface) {
			std::stringstream ss;
			ss << "Failed to load texture: " << SDL_GetError() << std::endl;
			throw std::runtime_error(ss.str());
		}

		return std::unique_ptr<SDL_Surface, SdlSurfaceDeleter>(surface, SdlSurfaceDeleter());
	}
}