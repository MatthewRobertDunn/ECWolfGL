#pragma once
#define GLM_FORCE_MESSAGES 
#include <SDL.h>
#include <gamemap.h>
namespace MatGl {
	class MatGlMap
	{
	public:
		MatGlMap(GameMap* map);
		GameMap::Plane::Map* GetSpot(int x, int y);
		const GameMap::Header& GetHeader() const;
		void Load();
	private:
		GameMap* gameMap;
		void CleanMap();
	};
}
#pragma once
