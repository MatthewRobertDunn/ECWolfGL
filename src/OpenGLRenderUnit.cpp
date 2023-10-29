#include "OpenGLRenderUnit.h"
#include "Model3d.h"
#include <glew.h>
#include "OpenGlTextureLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace MatGl {

	void checkGLErrors() {
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			throw std::runtime_error("OpenGL Error: " + std::to_string(error));
		}
	}

	using namespace glm;
	MatGl::OpenGlRenderUnit::OpenGlRenderUnit(GameCamera* camera)
	{
		this->camera = camera;

		//vertex array
		glGenVertexArrays(1, &this->vertexArray);
		glBindVertexArray(this->vertexArray);
		//Create a buffer
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		//Create texture array
		// Create a sampler2DArray and bind textures to it
		glGenTextures(1, &this->textureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		this->shader = new Shader("./shader.vert", "./shader.frag");
		this->shader->use();

		checkGLErrors();

		this->LoadTextures(Model3d());
	}

	void MatGl::OpenGlRenderUnit::Load(Model3d renderUnit)
	{
		//Perform work required to load a VertexList + attributes

		int bufferInBytes = renderUnit.Vertices.size() * sizeof(Vertex);
		int stride = sizeof(Vertex);  //We stride a whole vertex at a time

		glBufferData(GL_ARRAY_BUFFER, bufferInBytes, renderUnit.Vertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0); //position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(vec3))); //color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(vec3) + sizeof(vec4))); //texture
		glEnableVertexAttribArray(2);

		this->vertexCount = renderUnit.Vertices.size();
		
		switch (renderUnit.ShapeType) {
			case ShapeType::Triangle:
				this->mode = GL_TRIANGLES;
			break;
			case ShapeType::TriangleStrip:
				this -> mode = GL_TRIANGLE_STRIP;
			break;
		}

		checkGLErrors();
	}

	void MatGl::OpenGlRenderUnit::Render()
	{
		//Camera stuff
		//Get the variable that represents our shader uniform
		unsigned int transformLoc = glGetUniformLocation(this->shader->ID, "mvp");
		//pass our matrix to shader
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(this->camera->ModelViewProjection));


		//Texture stuff
		// Get the location of the sampler2DArray uniform in the shader program
		GLint textureArrayLocation = glGetUniformLocation(this->shader->ID, "textureArray");

		// Set the uniform to use texture unit 0 (GL_TEXTURE0)
		glUniform1i(textureArrayLocation, 0);

		// Bind the texture array to GL_TEXTURE0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureArray);


		glDrawArrays(this->mode, 0, this->vertexCount);
	}

	void OpenGlRenderUnit::LoadTextures(Model3d renderUnit)
	{
		// Load two textures
		auto texture1 = LoadTexture("bark.bmp");
		auto texture2 = LoadTexture("bricks.bmp");
			
		int width = texture1->w;
		int height = texture2->h;
		int numLayers = 2;

		// Specify the number of layers (textures) in the array
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, numLayers);
		checkGLErrors();
		// Attach imageData1 to layer 0
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture1->pixels);
		checkGLErrors();
		// Attach imageData2 to layer 1
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture2->pixels);
		checkGLErrors();
		// Bind the texture array to a texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

		checkGLErrors();
	}

}
