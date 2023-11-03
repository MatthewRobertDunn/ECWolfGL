#pragma once
#define GLM_FORCE_MESSAGES 
#include <SDL.h>
#include <gamemap.h>
#include "glew.h"
#include "OpenGLRenderUnit.h"
#include <OpenGlTextureManager.h>
#include <WallGenerator.h>
namespace MatGl {
	class OpenGlRenderer
	{
	public:
		OpenGlRenderer(GameCamera* camera, OpenGlTextureManager* textureManager);
		void Render(GameMap* map, float playerX, float playerY, float playerAngle);
	private:
		void RenderWalls(GameMap* map, float playerX, float playerY, float playerAngle);
		void RenderMapSpot(GameMap::Plane::Map* spot, VertexList& walls);
		OpenGlTextureManager* textureManager = nullptr;
		OpenGlRenderUnit* renderUnit = nullptr;
		GameCamera* camera = nullptr;
		Shader* wallShader = nullptr;
	};
}
