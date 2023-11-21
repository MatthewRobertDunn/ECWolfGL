#pragma once
#define GLM_FORCE_MESSAGES 
#include <SDL.h>
#include <gamemap.h>
#include "glew.h"
#include "openglrenderunit.h"
#include "opengltexturemanager.h"
#include "wallgenerator.h"
#include "viewfrustrum.h"
#include "actordef.h"
#include "matglmap.h"
namespace MatGl {
	class OpenGlRenderer
	{
	public:
		OpenGlRenderer(GameCamera* camera, OpenGlTextureManager* textureManager, MatGlMap* matGlMap);
		void Render(float playerX, float playerY, float playerAngle);
	private:
		void RenderSprites();
		void RenderWalls(float playerX, float playerY, float playerAngle);
		void RenderPlayer(float playerX, float playerY, float playerAngle);
		void RenderMapSpot(MatGlMapSpot* spot, VertexList& walls);
		void GenerateGeometryForMapSpot(GameMap::Plane::Map* spot, VertexList& cube);
		void DrawPlayerSprite(AActor* actor, const Frame* frame, fixed offsetX, fixed offsetY);
		OpenGlTextureManager* textureManager = nullptr;
		OpenGlRenderUnit* renderUnit = nullptr;
		GameCamera* camera = nullptr;
		Shader* wallShader = nullptr;
		Shader* spriteShader = nullptr;
		Shader* hudShader = nullptr;
		ViewFrustrum* viewFrustrum = nullptr;
		MatGlMap* matGlMap = nullptr;
	};
}
