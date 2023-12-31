#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "lights.h"
namespace MatGl {

	//standardizes the data we pass to all vertex shaders.
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec3 texture;
	};

	typedef std::vector<Vertex> VertexList;

	//Provides abstraction over commonly supported rendering data
	enum ShapeType {
		Triangle,
		TriangleStrip,
	};



	/// <summary>
	/// Render unit encapsulates everything that is required to render a 3d model, vertices textures + shaders etc
	/// It shouldn't contain opengl specific stuff
	/// </summary>
	struct Model3d
	{
		ShapeType ShapeType;
		std::vector<Vertex> Vertices;
		std::vector<SpotLight> SpotLights;
		//TODO: Different shaders, texture array set?
	};
}


