#include "OpenGLRenderer.h"
#include <array>

namespace MatGl {

	const float WALL_HEIGHT = 1.0f * 1.193f;
	const float WALL_WIDTH = 1.0f;
	const float FLOOR_HEIGHT = -0.1;

	std::array<float, 12> CreateSouthWall(float x, float y) {
		return std::array<float, 12> {
			0.0f + x, 0.0f + y, FLOOR_HEIGHT + WALL_HEIGHT,
				0.0f + x, 0.0f + y, FLOOR_HEIGHT,
				WALL_WIDTH + x, 0.0f + y, FLOOR_HEIGHT + WALL_HEIGHT,
				WALL_WIDTH + x, 0.0f + y, FLOOR_HEIGHT,
		};
	}

	std::array<float, 12> CreateNorthWall(float x, float y) {
		return std::array<float, 12> {
			WALL_WIDTH + x, 0.0f + y, WALL_HEIGHT,
				WALL_WIDTH + x, 0.0f + y, 0.0f,
				0.0f + x, 0.0f + y, WALL_HEIGHT,
				0.0f + x, 0.0f + y, 0.0f,
		};
	}

	std::array<float, 9> GetTriangle(float playerX, float playerY) {
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


		auto points = CreateSouthWall(28.0, 58.0);

		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
}