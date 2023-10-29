#pragma once
#include <glew.h>
#include "Model3d.h"
#include "GameCamera.h"
#include "OpenGlShader.h"
namespace MatGl {
	class OpenGlRenderUnit
	{
	public:
		OpenGlRenderUnit(GameCamera* camera);
		void Load(Model3d model);
		void Render();
	private:
		void LoadTextures(Model3d renderUnit);
		Shader* shader = nullptr;
		GLuint vertexArray = {};
		GLuint textureArray = {};
		GLuint buffer = {};
		GLsizei vertexCount = 0;
		GLenum mode = 0;
		GameCamera* camera = nullptr;
		//todo: shaders etc if needed
	};
}
