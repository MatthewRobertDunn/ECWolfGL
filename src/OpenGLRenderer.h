#pragma once
#define GLM_FORCE_MESSAGES 
#include <SDL.h>
#include <gamemap.h>
#include "glew.h"
#include "OpenGLRenderUnit.h"
#include "OpenGlTextureManager.h"
#include "WallGenerator.h"
#include "ViewFrustrum.h"
#include "actordef.h"
namespace MatGl {
	class OpenGlRenderer
	{
	public:
		OpenGlRenderer(GameCamera* camera, OpenGlTextureManager* textureManager);
		void Render(GameMap* map, float playerX, float playerY, float playerAngle);
	private:
		void RenderSprites(GameMap* map);
		void RenderWalls(GameMap* map, float playerX, float playerY, float playerAngle);
		void RenderPlayer(float playerX, float playerY, float playerAngle);
		void RenderMapSpot(GameMap::Plane::Map* spot, VertexList& walls);
		void DrawPlayerSprite(AActor* actor, const Frame* frame, fixed offsetX, fixed offsetY);
		OpenGlTextureManager* textureManager = nullptr;
		OpenGlRenderUnit* renderUnit = nullptr;
		GameCamera* camera = nullptr;
		Shader* wallShader = nullptr;
		Shader* spriteShader = nullptr;
		ViewFrustrum* viewFrustrum = nullptr;
	};
}
