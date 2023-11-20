#pragma once
#include "glew.h"
#include "model3d.h"
#include "gamecamera.h"
#include "openglshader.h"
#include "lights.h"
namespace MatGl {
	class OpenGlRenderUnit
	{
	public:
		OpenGlRenderUnit(GameCamera* camera, GLuint textureArray, Shader* shader);
		~OpenGlRenderUnit();
		void Load(Model3d model);
		void Render();
	private:
		void SetSpotlightsUniform();
		Shader* shader = nullptr;
		GLuint vertexArray = {};
		GLuint textureArray = {};
		GLuint buffer = {};
		GLsizei vertexCount = 0;
		GLenum mode = 0;
		GameCamera* camera = nullptr;
		std::vector <SpotLight> *spotLights;
		//todo: shaders etc if needed
	};
}
