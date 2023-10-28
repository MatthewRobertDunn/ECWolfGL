#include "OpenGLRenderer.h"
#include <array>
#include "WallGenerator.h"

namespace MatGl {
	using namespace glm;
	OpenGlRenderer::OpenGlRenderer()
	{
		this->renderUnit = new OpenGlRenderUnit();
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

		int tileX = floorf(playerX);
		int tileY = floorf(playerY);

		VertexList walls;

		for (int x = tileX - 10; x < tileX + 10; x++)
			for (int y = tileY - 10; y < tileY + 10; y++)
			{
				auto spot = map->GetSpot(tileX, tileY, 0);

				if (spot->sideSolid[SOUTH]) {
					auto wall = CreateSouthWall(vec2(x, y + 1), vec4(0.0, 0.0, 1.0, 0.5), vec3());
					walls.insert(walls.end(), walls.begin(), walls.end());
				}

				if (spot->sideSolid[NORTH]) {
					auto wall = CreateSouthWall(vec2(x, y - 1), vec4(0.0, 0.0, 1.0, 0.5), vec3());
					walls.insert(walls.end(), walls.begin(), walls.end());
				}
			}


		auto model = Model3d
		{
			TriangleStrip,
			walls
		};

		this->renderUnit->Load(model);
		this->renderUnit->Render();
	}
}