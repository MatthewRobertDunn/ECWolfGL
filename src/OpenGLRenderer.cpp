#include "OpenGLRenderer.h"

namespace MatGl {


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




		/*
		float points[] = { -1.0f, -1.0f, 0.0f,
				   1.0f, -1.0f, 0.0f,
				   0.0f,  1.0f, 0.0f };

		*/

		float points[] = { 0.0f + playerX, 1.0f + playerY, 1.0f ,
						0.0f + playerX , -1.0f + playerY, 1.0f ,
						20.0f + playerX,  0.0f + playerY, 1.0f };

		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
}