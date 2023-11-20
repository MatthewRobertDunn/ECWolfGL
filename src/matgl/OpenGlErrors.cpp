#include "OpenGlErrors.h"
#include <sstream>
#include <iostream>
#include <glew.h>
void MatGl::CheckGlErrors()
{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::string erstring =  "OpenGL Error: " + std::to_string(error);
			std::cout << erstring;
			throw std::runtime_error(erstring);
		}
}
