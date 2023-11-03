#pragma once
#include "Model3d.h"
#include <memory>
#include <glm/glm.hpp>
namespace MatGl {
	const float WALL_HEIGHT = 1.0f * 1.2039f;
	const float WALL_WIDTH = 1.0f;
	const float FLOOR_HEIGHT = -0.1;
	typedef std::vector<Vertex> VertexList;
	VertexList CreateSprite(glm::vec2 pos, glm::vec4 color, float layer, glm::vec3 cameraPos,  glm::vec2 scale);
	VertexList CreateSouthWall(glm::vec2 pos, glm::vec4 color, float layer);
	VertexList CreateNorthWall(glm::vec2 pos, glm::vec4 color, float layer);
	VertexList CreateEastWall(glm::vec2 pos, glm::vec4 color, float layer);
	VertexList CreateWestWall(glm::vec2 pos, glm::vec4 color, float layer);

}