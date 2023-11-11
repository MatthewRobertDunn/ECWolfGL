#include "OpenGLRenderUnit.h"
#include "Model3d.h"
#include <glew.h>
#include "OpenGlTextureLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <OpenGlErrors.h>

namespace MatGl {
	using namespace glm;
	MatGl::OpenGlRenderUnit::OpenGlRenderUnit(GameCamera* camera, GLuint textureArray, Shader* shader)
	{
		this->camera = camera;
		this->textureArray = textureArray;

		//vertex array
		glGenVertexArrays(1, &this->vertexArray);
		glBindVertexArray(this->vertexArray);
		//Create a buffer
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		this->shader = shader;
		CheckGlErrors();
	}

	OpenGlRenderUnit::~OpenGlRenderUnit()
	{
		if (this->vertexArray)
			glDeleteVertexArrays(1, &this->vertexArray);

		if (this->buffer)
			glDeleteBuffers(1, &this->buffer);
	}

	void MatGl::OpenGlRenderUnit::Load(Model3d renderUnit)
	{
		//Perform work required to load a VertexList + attributes

		//Activate current vertex array;
		glBindVertexArray(this->vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffer);

		int bufferInBytes = renderUnit.Vertices.size() * sizeof(Vertex);
		int stride = sizeof(Vertex);  //We stride a whole vertex at a time

		glBufferData(GL_ARRAY_BUFFER, bufferInBytes, renderUnit.Vertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0); //position vec3
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(vec3))); //normal vec3
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(vec3))); //color vec4
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(vec3) + sizeof(vec4))); //texture
		glEnableVertexAttribArray(3);

		this->vertexCount = renderUnit.Vertices.size();

		switch (renderUnit.ShapeType) {
		case ShapeType::Triangle:
			this->mode = GL_TRIANGLES;
			break;
		case ShapeType::TriangleStrip:
			this->mode = GL_TRIANGLE_STRIP;
			break;
		}

		CheckGlErrors();
	}

	void MatGl::OpenGlRenderUnit::Render()
	{
		//Activate shader
		this->shader->use();

		//Activate current vertex array;
		glBindVertexArray(this->vertexArray);

		//Camera stuff
		shader->SetMat4("mvp", this->camera->ModelViewProjection);
		shader->SetMat4("model", this->camera->Model);

		//Ambient light
		shader->SetVec4("ambientLight", vec4(0.1f, 0.1f, 0.1f, 1.0f));

		// spotLight
		shader->SetVec3("spotLight.position", this->camera->CameraPosition);
		shader->SetVec3("spotLight.direction", this->camera->Direction);
		shader->SetVec3("spotLight.ambient", 1.0f, 1.0f, 1.0f);
		shader->SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		shader->SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shader->SetFloat("spotLight.constant", 1.0f);
		shader->SetFloat("spotLight.linear", 0.09f * 0.5f);
		shader->SetFloat("spotLight.quadratic", 0.032f * 0.5f);
		shader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(70.0f)));



		//Get the variable that represents our camera position
		shader->SetVec3("cameraPosition", this->camera->CameraPosition);


		//Texture stuff
		// Get the location of the sampler2DArray uniform in the shader program
		GLint textureArrayLocation = glGetUniformLocation(this->shader->ID, "textureArray");

		// Set the uniform to use texture unit 0 (GL_TEXTURE0)
		glUniform1i(textureArrayLocation, 0);

		// Bind the texture array to GL_TEXTURE0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureArray);

		CheckGlErrors();

		glDrawArrays(this->mode, 0, this->vertexCount);
	}
}
