#include "OpenGLSurface.h"
#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace MatGl {
	OpenGlSurface::OpenGlSurface(SDL_Window* window, int width, int height)
	{
		this->width = width;
		this->height = height;

		this->glContext = SDL_GL_CreateContext(window);

		//glewExperimental = true;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			// Problem: glewInit failed, something is seriously wrong.
			std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
			exit(1);
		}

		//glClearColor(0.0f, 0.0f, 1.0f, 0.5f);

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


		glGenTextures(1, &glTexture);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTexture, 0);

		//Test if everything failed    
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			printf("failed to make complete framebuffer object %x", status);
		}

		this->shader = new Shader("./shader.vert", "./shader.frag");
				
		this->shader->use();

		this->SetCamera(0.0, 0.0, 0.0);
	}

	void OpenGlSurface::Render(void* texture)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture);
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	}

	void OpenGlSurface::SetCamera(float playerX, float playerY, float playerAngle)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(playerY, 0.0f, playerX));
		//Get the variable that represents our shader uniform
		unsigned int transformLoc = glGetUniformLocation(this->shader->ID, "mvp");
		//set it to our translation matrix
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	}
}