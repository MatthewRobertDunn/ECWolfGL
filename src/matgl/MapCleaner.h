#pragma once
#include "gamemap.h"
namespace MatGl {
	GameMap::Plane::Map* GetSpot(GameMap* map, int x, int y);
	void CleanMap(GameMap* map);
}