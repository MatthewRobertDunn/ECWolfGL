#include "OpenGLRenderer.h"
#include <array>
#include "WallGenerator.h"
#include <random>
#include "actor.h"
#include <r_sprites.h>
#include "MatGlMath.h"
#include <format>
#include <chrono>
#include <iostream>
#include "MapCleaner.h"
namespace MatGl {
	using namespace glm;
	using namespace std::chrono;


	OpenGlRenderer::OpenGlRenderer(GameCamera* camera, OpenGlTextureManager* textureManager)
	{
		this->textureManager = textureManager;
		this->wallShader = new Shader("./shader.vert", "./shader.frag");
		this->spriteShader = new Shader("./shader.vert", "./sprites.frag");
		this->renderUnit = new OpenGlRenderUnit(camera, textureManager->GetTextureArray(OpenGlTextureManager::WALL_TEXTURES), wallShader);
		this->camera = camera;
		this->viewFrustrum = new ViewFrustrum(20.0);

	}


	//Right is 0 degrees
	//Up is 90 degrees
	//Positive x is right
	//Negative y is up
	void OpenGlRenderer::Render(GameMap* map, float playerX, float playerY, float playerAngle)
	{
		auto start_time = high_resolution_clock::now();
		this->RenderWalls(map, playerX, playerY, playerAngle);
		auto end_time = high_resolution_clock::now();
		double millisecs = duration<double, std::ratio<1, 1000>>(end_time - start_time).count();
		std::cout << millisecs << std::endl;
		start_time = std::chrono::high_resolution_clock::now();

		std::map<std::string, VertexList> quads;
		for (AActor::Iterator iter = AActor::GetIterator(); iter.Next();)
		{
			AActor* actor = iter;

			if (actor->sprite == SPR_NONE)
				continue;

			FTexture* texture = GetActorSprite(actor);

			if (!texture || texture->GetID().GetIndex() <= 1) {
				continue;
			}


			//Figure out key for our texture dictionary
			std::string textureArray = std::format("wolf/{}/{}", texture->GetWidth(), texture->GetHeight());


			float x = FixedToFloat(actor->x);
			float y = FixedToFloat(actor->y);

			float scaleX = (texture->GetWidth() / 64.0f) * 1.10f;
			float scaleY = (texture->GetHeight() / 64.0f) * 1.10f;

			int textureIndex = textureManager->GetTextureArrayIndexForWolf(textureArray, texture->GetID());
			auto quad = CreateSprite(vec2(x, y), vec4(1.0, 1.0, 1.0, 1.0), textureIndex, this->camera->Direction, vec2(scaleX, scaleY));
			auto list = &quads[textureArray];
			list->insert(list->end(), quad.begin(), quad.end());
		}

		for (const auto& quad : quads) {
			auto spriteUnit = new OpenGlRenderUnit(camera, textureManager->GetTextureArray(quad.first), this->spriteShader);
			//spriteUnit->DepthMaskEnabled = GL_TRUE;
			auto model = Model3d
			{
				Triangle,
				quad.second
			};

			spriteUnit->Load(model);
			spriteUnit->Render();
			delete spriteUnit;
		}

		end_time = high_resolution_clock::now();
		millisecs = (end_time - start_time) / milliseconds(1);
	}




	void OpenGlRenderer::RenderWalls(GameMap* map, float playerX, float playerY, float playerAngle)
	{
		int tileX = floorf(playerX);
		int tileY = floorf(playerY);

		VertexList walls;

		/*
		for (int x = tileX - 20; x < tileX + 20; x++)
			for (int y = tileY - 20; y < tileY + 20; y++)
			{
				auto spot = GetSpot(map, x, y);
				if (!spot)
					continue;

				RenderMapSpot(spot, walls);
			}
			*/

		vec2 playerPos = vec2(playerX, playerY) - 2.0f * vec2(camera->Direction);
		this->viewFrustrum->RenderCells(playerAngle - 0.90f, playerAngle + 0.90f,
			[this, map, &walls, playerPos](ivec2 pos) -> void {
				auto wallPos = playerPos + vec2(pos);
				auto spot = GetSpot(map, wallPos.x, wallPos.y);
				if (spot)
				{
					RenderMapSpot(spot, walls);
				}
			});

		auto model = Model3d
		{
			Triangle,
			walls
		};

		this->renderUnit->Load(model);
		this->renderUnit->Render();
	}

	//Renders a single cube of tiles
	void OpenGlRenderer::RenderMapSpot(GameMap::Plane::Map* spot, VertexList& walls)
	{
		int x = spot->GetX();
		int y = spot->GetY();
		vec4 color = vec4(1.0, 1.0, 1.0, 1.0);


		float max1 = std::max(spot->slideAmount[NORTH], spot->slideAmount[SOUTH]);
		float max2 = std::max(spot->slideAmount[EAST], spot->slideAmount[WEST]);
		float dooropen = std::max(max1, max2) / 65535.0f;

		bool offsetHorizontal = spot->tile && spot->tile->offsetHorizontal;
		bool offsetVertical = spot->tile && spot->tile->offsetVertical;

		//North things are flipped
		//West things are flipped
		auto textureArray = OpenGlTextureManager::WALL_TEXTURES;

		if (spot->sideSolid[MapTile::South]) {
			auto pos = !offsetHorizontal ? vec2(x, y + 1) : vec2(x + dooropen, y + 1 - 0.5f);
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::South]);
			auto wall = CreateSouthWall(pos, color, texture);
			walls.insert(walls.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::North] && !offsetHorizontal) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::North]);
			auto wall = CreateNorthWall(vec2(x, y), color, texture);
			walls.insert(walls.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::East]) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::East]);
			auto pos = !offsetVertical ? vec2(x, y) : vec2(x + 0.5f, y + dooropen);
			auto wall = CreateEastWall(pos, color, texture);
			walls.insert(walls.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::West] && !offsetVertical) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::West]);
			auto wall = CreateWestWall(vec2(x + 1, y), color, texture);
			walls.insert(walls.end(), wall.begin(), wall.end());
		}
	}
}