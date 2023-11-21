#include "matglmap.h"
#include <iostream>
#include "actor.h"
#include "r_sprites.h"
#include "glm/glm.hpp"
#include "matglmath.h"
using namespace glm;
namespace MatGl {
	MatGl::MatGlMap::MatGlMap(GameMap* map)
	{
		this->gameMap = map;
		gameMapHeader = &map->GetHeader();
	}

	GameMap::Plane::Map* MatGlMap::GetSpot(int x, int y)
	{
		if (x < 0 || y < 0)
			return nullptr;

		if (!gameMap->IsValidTileCoordinate(x, y, 0)) {
			return nullptr;
		}

		return gameMap->GetSpot(x, y, 0);
	}

	MatGlMapSpot* MatGlMap::GetMatGlSpot(int x, int y)
	{
		if (x < 0 || y < 0)
			return nullptr;

		if (!gameMap->IsValidTileCoordinate(x, y, 0)) {
			return nullptr;
		}

		return &this->mapSpots[y * gameMapHeader->width + x];
	}

	const GameMap::Header& MatGlMap::GetHeader() const
	{
		return gameMap->GetHeader();
	}

	void MatGlMap::Load()
	{
		this->CleanMap();
		const auto header = this->gameMap->GetHeader();
		this->mapSpots.resize(header.width * header.height);

		for (int i = 0; i < header.width * header.height; i++) {
			this->mapSpots[i].Spot = &this->gameMap->GetPlane(0).map[i];
		}
		
	}

	void MatGl::MatGlMap::CleanMap()
	{
		std::cout << "Cleaning Map";
		for (int x = 0; x < gameMap->GetHeader().width; x++)
			for (int y = 0; y < gameMap->GetHeader().height; y++) {
				auto spot = GetSpot(x, y);
				bool offsetHorizontal = spot->tile && spot->tile->offsetHorizontal;
				bool offsetVertical = spot->tile && spot->tile->offsetVertical;


				//offsetHorizontal is true for a north-south door. [|]
				if (offsetVertical) {
					//If spot north of this door has a solid south face, turn it off
					auto testSpot = GetSpot(x, y - 1);
					if (testSpot && testSpot->sideSolid[MapTile::South] && spot->sideSolid[MapTile::North]) {
						testSpot->sideSolid[MapTile::South] = false;
						std::cout << ".";
					}

					//If spot south of this door has a solid north face, turn it off
					testSpot = GetSpot(x, y + 1);
					if (testSpot && testSpot->sideSolid[MapTile::North] && spot->sideSolid[MapTile::South]) {
						testSpot->sideSolid[MapTile::North] = false;
						std::cout << ".";
					}
				}

				//offserVertical is true for a east-west door. [--]
				if (offsetHorizontal) {

					//If spot east of this door has a solid west face, turn it off
					auto testSpot = GetSpot(x + 1, y);
					if (testSpot && testSpot->sideSolid[MapTile::West] && spot->sideSolid[MapTile::East]) {
						testSpot->sideSolid[MapTile::East] = false;
						std::cout << ".";
					}

					//If spot west of this door has a solid east face, turn it off
					testSpot = GetSpot(x - 1, y);
					if (testSpot && testSpot->sideSolid[MapTile::East] && spot->sideSolid[MapTile::West]) {
						testSpot->sideSolid[MapTile::West] = false;
						std::cout << ".";
					}
				}
			}

		std::cout << std::endl;
	}

	//Scans the map and loads any sprites of lights and converts them to opengl lights
	void MatGlMap::LoadLights()
	{
		this->SpotLights.clear();

		for (AActor::Iterator iter = AActor::GetIterator(); iter.Next();)
		{
			AActor* actor = iter;
			
			if (actor->sprite == SPR_NONE)
				continue;

			FTexture* texture = GetActorSprite(actor);

			if (!texture || texture->GetID().GetIndex() <= 1) {
				continue;
			}

			if (strcmp(texture->Name, "GLMPA0") != 0) {
				continue;
			}

			this->SpotLights.push_back(SpotLight{
				.Position = vec3(FixedToFloat(actor->x), FixedToFloat(actor->y),0.99f),
				.Direction = vec3(0.0f,0.0f,-1.0f),
				.CutOff = glm::cos(glm::radians(12.5f)),
				.OuterCutOff = glm::cos(glm::radians(30.0f)),
				.Constant = 1.0f,
				.Linear = 0.09f * 0.5f,
				.Quadratic = 0.032f * 0.5f,
				.Ambient = vec3(1.0f),
				.Diffuse = vec3(1.0f),
				.Specular = vec3(1.0f),
			});

		}
	}
}
