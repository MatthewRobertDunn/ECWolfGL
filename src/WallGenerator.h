#pragma once
#include "Model3d.h"
#include <memory>
#include <glm/glm.hpp>
namespace MatGl {
	typedef std::vector<Vertex> VertexList;
	VertexList CreateSprite(glm::vec2 pos, glm::vec4 color, float layer, glm::vec3 cameraPos,  glm::vec2 scale, glm::vec2 offset);
	VertexList CreateHudQuad(glm::vec2 pos, glm::vec4 color, float layer, glm::vec2 scale);
	VertexList CreateSouthWall(glm::vec2 pos, glm::vec4 color, float layer);
	VertexList CreateNorthWall(glm::vec2 pos, glm::vec4 color, float layer);
	VertexList CreateEastWall(glm::vec2 pos, glm::vec4 color, float layer);
	VertexList CreateWestWall(glm::vec2 pos, glm::vec4 color, float layer);
	VertexList CreateFloor(glm::vec2 pos, glm::vec4 color, float layer);
	VertexList CreateCeiling(glm::vec2 pos, glm::vec4 color, float layer);

}