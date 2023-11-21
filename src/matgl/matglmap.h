#pragma once
#define GLM_FORCE_MESSAGES 
#include <SDL.h>
#include <gamemap.h>
#include <vector>
#include "lights.h"
#include "model3d.h"
#include <vector>
namespace MatGl {

	struct MatGlMapSpot {
		GameMap::Plane::Map* Spot;
		VertexList Geometry;
		std::vector<MatGl::SpotLight> SpotLights;
	};


	class MatGlMap
	{
	public:
		MatGlMap(GameMap* map);
		GameMap::Plane::Map* GetSpot(int x, int y);
		MatGlMapSpot* GetMatGlSpot(int x, int y);

		const GameMap::Header& GetHeader() const;
		void Load();
		void LoadLights();
		std::vector<MatGl::SpotLight> SpotLights;

		

	private:
		const GameMap* gameMap = nullptr;
		const GameMap::Header* gameMapHeader = nullptr;
		std::vector<MatGlMapSpot> mapSpots;
		void CleanMap();

	};
}
#pragma once
