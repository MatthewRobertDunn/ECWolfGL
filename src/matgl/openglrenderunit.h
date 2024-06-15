#pragma once
#include "glew.h"
#include "model3d.h"
#include "gamecamera.h"
#include "openglshader.h"
#include "lights.h"
namespace MatGl {
	typedef std::vector <SpotLight> SpotLightList;
	class OpenGlRenderUnit
	{
	public:
		OpenGlRenderUnit(Camera* camera, GLuint textureArray, Shader* shader);
		~OpenGlRenderUnit();
		void Load(Model3d model);
		glm::vec3 TopLeft;
		glm::vec3 BottomRight;
		void Render();
	private:
		void SetSpotlightsUniform();
		Shader* shader = nullptr;
		GLuint vertexArray = {};
		GLuint textureArray = {};
		GLuint buffer = {};
		GLsizei vertexCount = 0;
		GLenum mode = 0;
		Camera* camera = nullptr;
		SpotLightList spotLights;
		//todo: shaders etc if needed
	};
}
