#include "OpenGLRenderer.h"
#include <array>
#include "WallGenerator.h"
#include <random>
#include "actor.h"
#include <r_sprites.h>
#include "MatGlMath.h"
#include <format>
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
		this->wallShader = new Shader("./shader.vert", "./shader.frag");
		this->renderUnit = new OpenGlRenderUnit(camera, textureManager->GetTextureArray(OpenGlTextureManager::WALL_TEXTURES), wallShader);
		this->camera = camera;

	}


	//Right is 0 degrees
	//Up is 90 degrees
	//Positive x is right
	//Negative y is up
	void OpenGlRenderer::Render(GameMap* map, float playerX, float playerY, float playerAngle)
	{
		this->RenderWalls(map, playerX, playerY, playerAngle);

		std::map<std::string, VertexList> quads;

		for (AActor::Iterator iter = AActor::GetIterator(); iter.Next();)
		{
			AActor* actor = iter;

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
			auto quad = CreateSprite(vec2(x, y), vec4(1.0, 1.0, 1.0, 1.0), textureIndex, this->camera->CameraPosition, vec2(scaleX, scaleY));
			auto list = &quads[textureArray];
			list->insert(list->end(), quad.begin(), quad.end());
		}

		for (const auto& quad : quads) {
			auto spriteUnit = new OpenGlRenderUnit(camera, textureManager->GetTextureArray(quad.first), wallShader);
			spriteUnit->DepthMaskEnabled = GL_FALSE;
			auto model = Model3d
			{
				Triangle,
				quad.second
			};

			spriteUnit->Load(model);
			spriteUnit->Render();
			delete spriteUnit;
		}
	}




	void OpenGlRenderer::RenderWalls(GameMap* map, float playerX, float playerY, float playerAngle)
	{
		int tileX = floorf(playerX);
		int tileY = floorf(playerY);

		VertexList walls;

		for (int x = tileX - 10; x < tileX + 10; x++)
			for (int y = tileY - 10; y < tileY + 10; y++)
			{
				auto spot = GetSpot(map, x, y);
				if (!spot)
					continue;

				RenderMapSpot(spot, walls);
			}

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

		//North things are flipped
		//West things are flipped
		auto textureArray = OpenGlTextureManager::WALL_TEXTURES;

		if (spot->sideSolid[MapTile::South]) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::South]);
			auto wall = CreateSouthWall(vec2(x, y + 1), color, texture);
			walls.insert(walls.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::North]) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::North]);
			auto wall = CreateNorthWall(vec2(x, y), color, texture);
			walls.insert(walls.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::East]) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::East]);
			auto wall = CreateEastWall(vec2(x, y), color, texture);
			walls.insert(walls.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::West]) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::West]);
			auto wall = CreateWestWall(vec2(x + 1, y), color, texture);
			walls.insert(walls.end(), wall.begin(), wall.end());
		}
	}
}