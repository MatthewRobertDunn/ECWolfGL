#include "openglrenderer.h"
#include <array>
#include "wallgenerator.h"
#include <random>
#include "actor.h"
#include <r_sprites.h>
#include "matglmath.h"
#include <format>
#include <chrono>
#include <iostream>
#include <wl_agent.h>
#include "a_inventory.h"
namespace MatGl {
	using namespace glm;
	using namespace std::chrono;

	int floorTexture; //fixme
	OpenGlRenderer::OpenGlRenderer(GameCamera* camera, OpenGlTextureManager* textureManager, MatGlMap* matGlMap)
	{
		this->textureManager = textureManager;
		this->wallShader = new Shader("./shader.vert", "./shader.frag");
		this->spriteShader = new Shader("./shader.vert", "./sprites.frag");
		this->hudShader = new Shader("./hud.vert", "./hud.frag");
		this->renderUnit = new OpenGlRenderUnit(camera, textureManager->GetTextureArray(OpenGlTextureManager::WALL_TEXTURES), wallShader);
		this->camera = camera;
		this->viewFrustrum = new ViewFrustrum(20.0);
		this->matGlMap = matGlMap;
		floorTexture = textureManager->GetTextureArrayIndexForName(OpenGlTextureManager::WALL_TEXTURES, "FLOOR");
		this->hudCamera = new HudCamera();
	}


	//Right is 0 degrees
	//Up is 90 degrees
	//Positive x is right
	//Negative y is up
	void OpenGlRenderer::Render(float playerX, float playerY, float playerAngle)
	{
		auto start_time = high_resolution_clock::now();
		//this->RenderWalls(playerX, playerY, playerAngle);
		auto end_time = high_resolution_clock::now();
		double millisecs = duration<double, std::ratio<1, 1000>>(end_time - start_time).count();
		//std::cout << millisecs << "-";

		start_time = std::chrono::high_resolution_clock::now();
		RenderSprites();
		end_time = high_resolution_clock::now();
		millisecs = duration<double, std::ratio<1, 1000>>(end_time - start_time).count();
		//std::cout << millisecs << std::endl;

		RenderPlayer(playerX, playerY, playerAngle);
	}

	void OpenGlRenderer::RenderSprites()
	{
		const float TILE_WIDTH_PIXELS = matGlMap->GetHeader().tileSize;
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

			float tileXScale = (TILE_WIDTH_PIXELS * FixedToFloat(texture->xScale)) / FixedToFloat(actor->scaleX);
			float tileYScale = (TILE_WIDTH_PIXELS * FixedToFloat(texture->yScale)) / FixedToFloat(actor->scaleY);


			float scaleX = (texture->GetWidth() / tileXScale);
			float scaleY = (texture->GetHeight() / tileYScale);

			//Convert all these weird pixel coordinates to OpenGL ones
			float actualLeftOffset = 0.5f * (texture->GetWidth() / tileXScale) - 0.5f;
			float actualTopOffset = 0.5f * (texture->GetHeight() / tileYScale) - 0.5f;

			float desiredLeftOffset = (texture->LeftOffset / tileXScale) - 0.5f;
			float desiredTopOffset = (texture->TopOffset / tileYScale) - 0.5f;

			vec2 spriteOffset = vec2(desiredLeftOffset, desiredTopOffset) - vec2(actualLeftOffset, actualTopOffset);

			int textureIndex = textureManager->GetTextureArrayIndexForWolf(textureArray, texture->GetID());
			auto quad = CreateSprite(vec2(x, y), vec4(1.0, 1.0, 1.0, 1.0), textureIndex, this->camera->Direction, vec2(scaleX, scaleY), spriteOffset);
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
	}

	void OpenGlRenderer::RenderWalls(float playerX, float playerY, float playerAngle)
	{
		int tileX = floorf(playerX);
		int tileY = floorf(playerY);

		VertexList walls;

		vec2 playerPos = vec2(playerX, playerY) - 2.0f * vec2(camera->Direction);
		

		auto model = Model3d
		{
			.ShapeType = Triangle,
			.Vertices = VertexList(),
			.SpotLights = std::vector<SpotLight>()
		};
		
		
		this->viewFrustrum->RenderCells(playerAngle - 0.90f, playerAngle + 0.90f,
			[this, &model, playerPos](ivec2 pos) -> void {
				auto cubePos = playerPos + vec2(pos);
				auto spot = matGlMap->GetMatGlSpot(cubePos.x, cubePos.y);
				if (spot)
				{
					RenderMapSpot(spot, model.Vertices);
					model.SpotLights.insert(model.SpotLights.end(), spot->SpotLights.begin(), spot->SpotLights.end());
				}
			});


		this->renderUnit->Load(model);
		this->renderUnit->Render();
	}
	//Renders a single cube of tiles
	void OpenGlRenderer::RenderMapSpot(MatGlMapSpot* spot, VertexList& walls)
	{
		//Is this spot static or dynamic
		bool isStatic = !(spot->Spot->thinker || spot->Spot->pushAmount != 0 || spot->Spot->pushReceptor);

		auto cubeCached = &spot->Geometry;

		//If there is any pre-cached geometry then use it if the tile is not dynamic
		if (!cubeCached->empty()) {
			if (isStatic)
			{
				walls.insert(walls.end(), cubeCached->begin(), cubeCached->end());
				return;
			}
			else {
				cubeCached->clear();
			}
		}

		//Otherwise create it dynamically.
		VertexList cube;
		GenerateGeometryForMapSpot(spot->Spot, cube);

		if (isStatic) {
			cubeCached->insert(cubeCached->end(), cube.begin(), cube.end());
		}

		walls.insert(walls.end(), cube.begin(), cube.end());


		/*
		VertexList cube;
		bool isDynamic = GenerateGeometryForMapSpot(spot, cube);
		walls.insert(walls.end(), cube.begin(), cube.end());
		*/
	}


	void OpenGlRenderer::GenerateGeometryForMapSpot(GameMap::Plane::Map* spot, VertexList& cube)
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
			cube.insert(cube.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::North] && !offsetHorizontal) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::North]);
			auto wall = CreateNorthWall(vec2(x, y), color, texture);
			cube.insert(cube.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::East]) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::East]);
			auto pos = !offsetVertical ? vec2(x, y) : vec2(x + 0.5f, y + dooropen);
			auto wall = CreateEastWall(pos, color, texture);
			cube.insert(cube.end(), wall.begin(), wall.end());
		}

		if (spot->sideSolid[MapTile::West] && !offsetVertical) {
			int texture = textureManager->GetTextureArrayIndexForWolf(textureArray, spot->texture[MapTile::West]);
			auto wall = CreateWestWall(vec2(x + 1, y), color, texture);
			cube.insert(cube.end(), wall.begin(), wall.end());
		}


		{
			auto wall = CreateFloor(vec2(x, y), vec4(0.5, 0.5, 0.5, 1.0), -1);
			cube.insert(cube.end(), wall.begin(), wall.end());
		}

		{
			auto wall = CreateCeiling(vec2(x, y), vec4(0.4, 0.4, 0.4, 1.0), -1);
			cube.insert(cube.end(), wall.begin(), wall.end());
		}
	}


	const int ConsolePlayer = 0;
	void OpenGlRenderer::RenderPlayer(float playerX, float playerY, float playerAngle)
	{
		for (unsigned int i = 0; i < player_t::NUM_PSPRITES; ++i)
		{
			if (!players[ConsolePlayer].psprite[i].frame)
				return;

			fixed xoffset, yoffset;
			players[ConsolePlayer].BobWeapon(&xoffset, &yoffset);

			this->DrawPlayerSprite(players[ConsolePlayer].ReadyWeapon, players[ConsolePlayer].psprite[i].frame, players[ConsolePlayer].psprite[i].sx + xoffset, players[ConsolePlayer].psprite[i].sy + yoffset);
		}
	}

	void OpenGlRenderer::DrawPlayerSprite(AActor* actor, const Frame* frame, fixed offsetX, fixed offsetY)
	{
		auto texture = MatGl::GetPlayerSprite(actor, frame);
		if (!texture) {
			return;
		}

		//Figure out key for our texture dictionary
		std::string textureArray = std::format("wolf/{}/{}", texture->GetWidth(), texture->GetHeight());

		//Create a quad to hold this hud
		int textureIndex = textureManager->GetTextureArrayIndexForWolf(textureArray, texture->GetID());


		float PIXELS_X = 320.0f;
		float PIXELS_Y = 200.0f;

		float baseRatio = 4.0f / 3.0f;
		float ratioChange = baseRatio / camera->AspectRatio;

		
		float tileXScale = PIXELS_X * FixedToFloat(texture->xScale) / ratioChange;
		float tileYScale = PIXELS_Y * FixedToFloat(texture->yScale);

		float scaleX = texture->GetWidth() / tileXScale;
		float scaleY = texture->GetHeight() / tileYScale;


		auto glCurrentOffset = -vec2(0.5 * scaleX, 0.5 * scaleY);

		auto offsets = MatGl::GetWeaponOffsets(texture, offsetX, offsetY);
		vec2 glOffsets = vec2(FixedToFloat(offsets.first), FixedToFloat(offsets.second));

		//Scale 0 to width to 0 to 1.0
		glOffsets.x /= this->camera->Width;
		glOffsets.y /= this->camera->Height;
				
		vec2 spriteOffset = glOffsets - glCurrentOffset;

		auto quad = CreateHudQuad(spriteOffset, vec4(1.0, 1.0, 1.0, 1.0), textureIndex, vec2(scaleX, scaleY));

		//render it using our HUD shader
		auto spriteUnit = new OpenGlRenderUnit(this->hudCamera, textureManager->GetTextureArray(textureArray), this->hudShader);

		//Create 3d model, we are using triangles.
		auto model = Model3d
		{
			Triangle,
			quad
		};

		spriteUnit->Load(model);
		spriteUnit->Render();
		delete spriteUnit;
	}
}