#pragma once
#define GL_GLEXT_PROTOTYPES
#include <sdl.h>
#include "glew.h"
#include "openglshader.h"
//#define MATGL_RENDER_TEXTURE
namespace MatGl {
	class OpenGlSurface
	{
	public:
		OpenGlSurface(SDL_Window* window, int width, int height);
		void Resize(int width, int height);
		void Render(void* texture);
	private:
		SDL_GLContext  glContext;
		GLuint framebuffer = 0;
		GLuint glTexture = 0;
		GLuint depthBuffer = 0;
		int width = 0;
		int height = 0;
	};
}