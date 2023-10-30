#include "OpenGLRenderer.h"
#include <array>
#include "WallGenerator.h"
#include <random>
namespace MatGl {
	using namespace glm;

	GameMap::Plane::Map* GetSpot(GameMap* map, int x, int y)
	{
		if (x < 0 || y < 0)
			return nullptr;

		if (!map->IsValidTileCoordinate(x, y, 0)) {
			return nullptr;
		}

		return map->GetSpot(x, y, 0);
	}



	OpenGlRenderer::OpenGlRenderer(GameCamera* camera, OpenGlTextureManager* textureManager)
	{
		this->textureManager = textureManager;
		this->renderUnit = new OpenGlRenderUnit(camera, textureManager->GetTextureArray("WALL"));
		this->camera = camera;
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
				//srand(x * y);
				//double r = (double)rand() / ((double)RAND_MAX + 1);
				//double g = (double)rand() / ((double)RAND_MAX + 1);
				//double b = (double)rand() / ((double)RAND_MAX + 1);


				auto spot = GetSpot(map, x, y);
				if (!spot)
					continue;

				vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

				if (spot->tile)
				{
					if (spot->tile->offsetHorizontal || spot->tile->offsetVertical)
					{
						float max1 = std::max(spot->slideAmount[NORTH], spot->slideAmount[SOUTH]);
						float max2 = std::max(spot->slideAmount[EAST], spot->slideAmount[WEST]);
						float dooropen = std::max(max1, max2) / 65535.0f;
						//The more the door is open, the more transparent we get
						color = vec4(1.0, 1.0, 1.0, 1.0 - dooropen);
					}
				}


				if (spot->sideSolid[SOUTH]) {
					int texture = textureManager->GetTextureArrayIndexForWolf("WALL", spot->texture[SOUTH]);
					auto wall = CreateSouthWall(vec2(x, y + 1), color, texture);
					walls.insert(walls.end(), wall.begin(), wall.end());
				}

				if (spot->sideSolid[NORTH]) {
					int texture = textureManager->GetTextureArrayIndexForWolf("WALL", spot->texture[NORTH]);
					auto wall = CreateSouthWall(vec2(x, y), color, texture);
					walls.insert(walls.end(), wall.begin(), wall.end());
				}

				if (spot->sideSolid[EAST]) {
					int texture = textureManager->GetTextureArrayIndexForWolf("WALL", spot->texture[EAST]);
					auto wall = CreateEastWall(vec2(x, y), color, texture);
					walls.insert(walls.end(), wall.begin(), wall.end());
				}

				if (spot->sideSolid[WEST]) {
					int texture = textureManager->GetTextureArrayIndexForWolf("WALL", spot->texture[WEST]);
					auto wall = CreateWestWall(vec2(x + 1, y), color, texture);
					walls.insert(walls.end(), wall.begin(), wall.end());
				}
			}

		auto model = Model3d
		{
			Triangle,
			walls
		};

		this->renderUnit->Load(model);
		this->renderUnit->Render();
	}
}