#include "matglmap.h"
#include <iostream>

namespace MatGl {
	MatGl::MatGlMap::MatGlMap(GameMap* map)
	{
		this->gameMap = map;
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

	const GameMap::Header& MatGlMap::GetHeader() const
	{
		return gameMap->GetHeader();
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
}
