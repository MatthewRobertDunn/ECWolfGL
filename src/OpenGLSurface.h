#pragma once
#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#include <glew.h>
#include "OpenGlShader.h"
namespace MatGl {
	class OpenGlSurface
	{
	public:
		OpenGlSurface(SDL_Window* window, int width, int height);
		void Render(void* texture);
		void SetCamera(float playerX, float playerY, float playerAngle);
	private:
		SDL_GLContext  glContext;
		GLuint framebuffer = 0;
		GLuint glTexture = 0;
		Shader* shader = nullptr;
		int width = 0;
		int height = 0;
	};
}