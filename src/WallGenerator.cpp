#include "WallGenerator.h"
#include <glm/ext/matrix_transform.hpp>

namespace MatGl {
	using namespace glm;

	VertexList GetBasicQuad(vec4 color, float layer) {
		VertexList wall{
			//Triangle 1
			Vertex{
					vec3(-0.5f, 0.0f ,0.5f) ,  //TL
					color,
					vec3(0.0,1.0,layer),
				},
			Vertex{
					vec3(-0.5f, 0.0f , -0.5f),		//BL
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					vec3(0.5f, 0.0f , 0.5f), //TR
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{
					vec3(0.5f, 0.0f , 0.5f), //TR
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{
					vec3(-0.5f, 0.0f , -0.5f),			//BL
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					vec3(0.5f, 0.0f , -0.5f), //BR
					color,
					vec3(1.0,0.0,layer),
				}
		};
		return wall;
	}

	VertexList GetBasicQuad(vec4 color, float layer, vec3 up, vec3 right, vec2 scale) {
		up = 0.5f * up * scale.y;
		right = 0.5f * right * scale.x;
		VertexList wall{
			//Triangle 1
			Vertex{
					up - right,  //TL
					color,
					vec3(0.0,1.0,layer),
				},
			Vertex{
					-up - right,		//BL
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					up + right, //TR
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{	//Triangle 2
					up + right, //TR
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{
					-up - right,		//BL
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					-up + right, //BR
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


	VertexList CreateSprite(vec2 pos, vec4 color, float layer, vec3 cameraPos, vec2 scale) {
		vec3 up = vec3(0, 0, 1);
		vec3 spritePosition = vec3(pos, 0.0f);


		//Get plane that always faces the camera
		//auto rightVector = glm::cross(cameraPos - spritePosition, up);
		auto diff = cameraPos - spritePosition;
		auto rightVector = vec3(diff.y, -diff.x, 0);

		scale.y = scale.y * WALL_HEIGHT;
		//auto quad = GetBasicQuad(color, layer);
		auto quad = GetBasicQuad(color, layer, up, glm::normalize(rightVector), scale);

		//Scale the sprite
		//auto scaleMatrix = glm::scale(glm::mat4(1.0f), vec3(scale.x, 1.0, scale.y * WALL_HEIGHT));
		//Transform(scaleMatrix, quad);

		//figure out how high we are off the floor
		float aboveFloorBy = quad[1].position.z - FLOOR_HEIGHT + 0.05f;
		auto standingAt = vec3(pos, -aboveFloorBy);
		vec2 wallNormal = vec2(0.0, 1.0);

		auto translateMatrix = glm::translate(glm::mat4(1.0f), standingAt);
		Transform(translateMatrix, quad);

		return quad;
	}


	VertexList CreateSouthWall(vec2 pos, vec4 color, float layer) {
		pos = pos + vec2(0.5, 0.0);
		auto quad = GetBasicQuad(color, layer);
		auto const translateMatrix = glm::translate(glm::mat4(1.0f), vec3(pos, WALL_HEIGHT * 0.5 + FLOOR_HEIGHT));
		auto const scaleMatrix = glm::scale(glm::mat4(1.0f), vec3(1.0, 1.0, WALL_HEIGHT));
		auto transform = translateMatrix * scaleMatrix;
		Transform(transform, quad);
		return quad;
	}



	VertexList CreateNorthWall(vec2 pos, vec4 color, float layer) {
		pos = pos + vec2(0.5, 0.0);
		auto quad = GetBasicQuad(color, layer);
		auto const translateMatrix = glm::translate(glm::mat4(1.0f), vec3(pos, WALL_HEIGHT * 0.5 + FLOOR_HEIGHT));
		auto const scaleMatrix = glm::scale(glm::mat4(1.0f), vec3(1.0, 1.0, WALL_HEIGHT));
		auto const rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		auto transform = translateMatrix * scaleMatrix * rotateMatrix;
		Transform(transform, quad);
		return quad;
	}
	VertexList CreateEastWall(glm::vec2 pos, glm::vec4 color, float layer)
	{
		pos = pos + vec2(0.0, 0.5f);
		auto quad = GetBasicQuad(color, layer);
		auto const translateMatrix = glm::translate(glm::mat4(1.0f), vec3(pos, WALL_HEIGHT * 0.5 + FLOOR_HEIGHT));
		auto const scaleMatrix = glm::scale(glm::mat4(1.0f), vec3(1.0, 1.0, WALL_HEIGHT));
		auto const rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		auto transform = translateMatrix * scaleMatrix * rotateMatrix;
		Transform(transform, quad);
		return quad;
	}
	VertexList CreateWestWall(glm::vec2 pos, glm::vec4 color, float layer)
	{
		pos = pos + vec2(0.0, 0.5f);
		auto quad = GetBasicQuad(color, layer);
		auto const translateMatrix = glm::translate(glm::mat4(1.0f), vec3(pos, WALL_HEIGHT * 0.5 + FLOOR_HEIGHT));
		auto const scaleMatrix = glm::scale(glm::mat4(1.0f), vec3(1.0, 1.0, WALL_HEIGHT));
		auto const rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		auto transform = translateMatrix * scaleMatrix * rotateMatrix;
		Transform(transform, quad);
		return quad;
	}
}