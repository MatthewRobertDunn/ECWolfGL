#include "WallGenerator.h"
#include "glm/ext/matrix_transform.hpp"

namespace MatGl {
	using namespace glm;

	const float WALL_HEIGHT = 1.0f;
	const float WALL_WIDTH = 1.0f;
	const float HALF_WIDTH = WALL_WIDTH * 0.5f;
	const float HALF_HEIGHT = WALL_HEIGHT * 0.5f;
	const float FLOOR_HEIGHT = 0.0f;


	const vec3 UP = vec3(0, 0, 1.0f);
	const vec2 WALL_SCALE = vec2(1.0, WALL_HEIGHT);
	const vec3 EAST = vec3(0.0f, 1.0f, 0.0f);
	const vec3 WEST = vec3(0.0f, -1.0f, 0.0f);
	const vec3 NORTH = vec3(-1.0f, 0.0f, 0.0f);
	const vec3 SOUTH = vec3(1.0f, 0.0f, 0.0f);
		
	VertexList GetBasicQuad(vec4 color, float layer, vec3 up, vec3 right, vec2 scale, vec3 pos = vec3(0.0f)) {
		auto normal = glm::normalize(glm::cross(up, right));
		up = 0.5f * up * scale.y;
		right = 0.5f * right * scale.x;
		VertexList wall{
			//Triangle 1
			Vertex{
					(up - right) + pos,  //TL
					normal,
					color,
					vec3(0.0,1.0,layer),
				},
			Vertex{
					(-up - right) + pos,		//BL
					normal,
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					(up + right) + pos, //TR
					normal,
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{	//Triangle 2
					(up + right) + pos, //TR
					normal,
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{
					(-up - right) + pos,		//BL
					normal,
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					(-up + right) + pos, //BR
					normal,
					color,
					vec3(1.0,0.0,layer),
				}
		};
		return wall;
	}

	void Transform(mat4 transform, VertexList& vertices) {
		for (auto& v : vertices) {
			auto result = transform * vec4(v.position, 1.0);
			v.position = vec3(result);
		}
	}

	/// <summary>
	/// Create a quad suitable for display as a hud element
	/// </summary>
	/// <param name="pos">position in opengl screen coords</param>
	/// <param name="color">rgba</param>
	/// <param name="layer">what texture to use</param>
	/// <param name="scale"></param>
	/// <returns></returns>
	VertexList CreateHudQuad(vec2 pos, vec4 color, float layer, vec2 scale) {
		const vec3 up = vec3(0.0, 1.0, 0.0);
		const vec3 right = vec3(1.0, 0.0, 0.0);
		return GetBasicQuad(color, layer, up, right, scale,vec3(pos,0.0f));
	}


	VertexList CreateSprite(vec2 pos, vec4 color, float layer, vec3 lookAt, vec2 scale, vec2 offset) {
		//scale = scale * 1.10f; //Not sure why we need this.
		vec3 spritePosition = vec3(pos, 0.0f);


		//Get plane that always faces the camera
		auto cameraRight = glm::cross(UP, lookAt);

		auto quad = GetBasicQuad(color, layer, UP, glm::normalize(cameraRight), scale);

		auto standingAt = vec3(pos, FLOOR_HEIGHT);

		//Calculate offset of sprite in terms of our camera basis vectors
		auto cameraOffset = -cameraRight * offset.x + UP * offset.y * WALL_HEIGHT;

		//For some reason we have to move slightly closer to the camera in the lookat direction
		cameraOffset = cameraOffset - 0.1340f * lookAt;

		auto translateMatrix = glm::translate(glm::mat4(1.0f), standingAt + cameraOffset);
		Transform(translateMatrix, quad);

		return quad;
	}


	VertexList CreateSouthWall(vec2 pos, vec4 color, float layer) {
		vec3 wallPos = vec3(pos.x + HALF_WIDTH, pos.y, HALF_HEIGHT + FLOOR_HEIGHT);
		return GetBasicQuad(color, layer, UP, SOUTH, WALL_SCALE, wallPos);
	}

	VertexList CreateNorthWall(vec2 pos, vec4 color, float layer) {
		vec3 wallPos = vec3(pos.x + 0.5f, pos.y, HALF_HEIGHT + FLOOR_HEIGHT);
		return GetBasicQuad(color, layer, UP, NORTH, WALL_SCALE, wallPos);
	}

	VertexList CreateEastWall(glm::vec2 pos, glm::vec4 color, float layer)
	{
		vec3 wallPos = vec3(pos.x, pos.y + HALF_WIDTH, HALF_HEIGHT + FLOOR_HEIGHT);
		return GetBasicQuad(color, layer, UP, EAST, WALL_SCALE, wallPos);
	}

	VertexList CreateWestWall(glm::vec2 pos, glm::vec4 color, float layer)
	{
		vec3 wallPos = vec3(pos.x, pos.y + HALF_WIDTH, HALF_HEIGHT + FLOOR_HEIGHT);
		return GetBasicQuad(color, layer, UP, WEST, WALL_SCALE, wallPos);
	}

	VertexList CreateFloor(glm::vec2 pos, glm::vec4 color, float layer)
	{
		vec3 wallPos = vec3(pos.x + HALF_WIDTH, pos.y + HALF_WIDTH, FLOOR_HEIGHT);
		return GetBasicQuad(color, layer, EAST, NORTH, vec2(1.0, 1.0), wallPos);
	}
	VertexList CreateCeiling(glm::vec2 pos, glm::vec4 color, float layer)
	{
		vec3 wallPos = vec3(pos.x + HALF_WIDTH, pos.y + HALF_WIDTH, WALL_HEIGHT + FLOOR_HEIGHT);
		return GetBasicQuad(color, layer, EAST, NORTH, vec2(1.0, 1.0), wallPos);
	}
}