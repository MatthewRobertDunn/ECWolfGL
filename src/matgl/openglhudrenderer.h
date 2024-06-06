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
	class OpenGlHudRenderer
	{
	public:
		OpenGlHudRenderer(HudCamera* camera, OpenGlTextureManager* textureManager);
		void STACK_ARGS DrawTexture(FTexture* img, double x, double y, int tags_first, ...);
	private:
		Shader* hudShader = nullptr;
		HudCamera* camera = nullptr;
		OpenGlTextureManager* textureManager = nullptr;
		HudCamera* hudCamera = nullptr;
	};
}
