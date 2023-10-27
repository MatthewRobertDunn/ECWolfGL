#include "OpenGLRenderer.h"
#include <array>
#include "WallGenerator.h"

namespace MatGl {

	std::array<float, 12> CreateSouthWall2(float x, float y) {
		return std::array<float, 12> {
			0.0f + x, 0.0f + y, FLOOR_HEIGHT + WALL_HEIGHT,
				0.0f + x, 0.0f + y, FLOOR_HEIGHT,
				WALL_WIDTH + x, 0.0f + y, FLOOR_HEIGHT + WALL_HEIGHT,
				WALL_WIDTH + x, 0.0f + y, FLOOR_HEIGHT,
		};
	}

	std::array<float, 12> CreateNorthWall2(float x, float y) {
		return std::array<float, 12> {
			WALL_WIDTH + x, 0.0f + y, WALL_HEIGHT,
				WALL_WIDTH + x, 0.0f + y, 0.0f,
				0.0f + x, 0.0f + y, WALL_HEIGHT,
				0.0f + x, 0.0f + y, 0.0f,
		};
	}

	std::array<float, 9> GetTriangle2(float playerX, float playerY) {
		return std::array<float, 9> { 0.0f + playerX, 1.0f + playerY, 1.0f,
			0.0f + playerX, -1.0f + playerY, 1.0f,
			20.0f + playerX, 0.0f + playerY, 1.0f };
	}


	OpenGlRenderer::OpenGlRenderer()
	{
		//vertex array
		glGenVertexArrays(1, &this->vertexArrayObject);
		glBindVertexArray(this->vertexArrayObject);
		//Create a buffer
		glGenBuffers(1, &triangle);
		glBindBuffer(GL_ARRAY_BUFFER, triangle);
	}

	//Right is 0 degrees
	//Up is 90 degrees
	//Positive x is right
	//Negative y is up
	void OpenGlRenderer::Render(GameMap* map, float playerX, float playerY, float playerAngle)
	{
		//TODO: iterate through all cells in viewing frustrum
		//Update the camera matrix with playerX,playerY and playerAngle
		//Check tile for what walls are enabled
		//For each enabled wall
		//make two triangles in the right place or maybe apply a rotation + translation matrix to a premade wall probs
		//Put color or texture on there

		//map->GetSpot(2,4,0);

		//auto wall = CreateSouthWall(28.0, 58.0);


		auto wall = CreateSouthWall(vec2(28.0, 58.0), vec4(0.0, 0.0, 1.0, 0.5), vec3());

		//Perform work required to load a VertexList + attributes

		int bufferInBytes = wall.size() * sizeof(Vertex);
		int stride = sizeof(Vertex);  //We stride a whole vertex at a time
		
		glBufferData(GL_ARRAY_BUFFER, bufferInBytes, wall.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0); //position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(vec3))); //color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(vec3) + sizeof(vec4))); //texture
		glEnableVertexAttribArray(2);
	}
}