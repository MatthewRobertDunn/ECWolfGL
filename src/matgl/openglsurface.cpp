#include "openglsurface.h"
#include "openglerrors.h"
#include <sdl.h>
#include <iostream>

namespace MatGl {
	OpenGlSurface::OpenGlSurface(SDL_Window* window, int width, int height)
	{
		this->glContext = SDL_GL_CreateContext(window);
		
		
		//glewExperimental = true;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			// Problem: glewInit failed, something is seriously wrong.
			std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
			exit(1);
		}

		this->Resize(width, height);
	}

	void OpenGlSurface::Resize(int width, int height)
	{
#ifdef MATGL_RENDER_TEXTURE
		// Check if framebuffer already exists, delete it if true
		if (glIsFramebuffer(framebuffer)) {
			glDeleteFramebuffers(1, &framebuffer);
		}

		// Generate a new framebuffer
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// Check if depth buffer already exists, delete it if true
		if (glIsRenderbuffer(depthBuffer)) {
			glDeleteRenderbuffers(1, &depthBuffer);
		}

		// Generate a new renderbuffer for the depth buffer
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);


		// Check if texture already exists, delete it if true
		if (glIsTexture(glTexture)) {
			glDeleteTextures(1, &glTexture);
		}
		// Generate a new texture
		glGenTextures(1, &glTexture);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach the texture to the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTexture, 0);


		//Test if everything failed    
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			printf("failed to make complete framebuffer object %x", status);
		}

#endif // MATGL_RENDER_TEXTURE

		this->width = width;
		this->height = height;


		glViewport(0, 0, width, height);

#ifdef MATGL_RENDER_TEXTURE
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
#else
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_SM);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CheckGlErrors();
	}

	void OpenGlSurface::Render(void* texture)
	{
#ifdef MATGL_RENDER_TEXTURE
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture);
#endif
		//glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}