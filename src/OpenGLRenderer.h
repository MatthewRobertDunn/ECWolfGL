#pragma once
#define GLM_FORCE_MESSAGES 
#include <SDL.h>
#include <gamemap.h>
#include "glew.h"
#include "OpenGLRenderUnit.h"
namespace MatGl {
	class OpenGlRenderer
	{
	public:
		OpenGlRenderer();
		void Render(GameMap* map, float playerX, float playerY, float playerAngle);
	private:
		OpenGlRenderUnit* renderUnit = nullptr;
	};
}
