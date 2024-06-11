#include "openglhudrenderer.h"
#include <array>
#include "wallgenerator.h"
#include <random>
#include "actor.h"
#include <r_sprites.h>
#include "matglmath.h"
#include <format>
#include <chrono>
#include <iostream>
#include <wl_agent.h>
#include "a_inventory.h"
#include "openglsdlfb.h"
#include "openglmain.h"
namespace MatGl {
	using namespace glm;
	using namespace std::chrono;
	float depth = 0.0;

	OpenGlHudRenderer::OpenGlHudRenderer(int width, int height, bool fullscreen, SDL_Window* oldwin) : OpenGlSDLFB(width, height, fullscreen, oldwin)
	{
		this->hudShader = new Shader("./hud.vert", "./hud.frag");
	}

	void STACK_ARGS OpenGlHudRenderer::DrawTextureV(FTexture* img, double x, double y, uint32 tag, va_list tags)
	{
		//Figure out key for our texture dictionary
		std::string textureArray = std::format("wolf/{}/{}", img->GetWidth(), img->GetHeight());

		//Create a quad to hold this hud
		int textureIndex = MatGl::Globals::TextureManager->GetTextureArrayIndexForWolf(textureArray, img->GetID());

		DrawParms parms;

		if (!ParseDrawTextureTags(img, x, y, tag, tags, &parms, false))
		{
			return;
		}

		float PIXELS_X = 320.0f;
		float PIXELS_Y = 200.0f;

		auto screenSize = vec2(hudCamera->Width, hudCamera->Height);

		
		double x0 = parms.x - parms.left * parms.destwidth / parms.texwidth;
		double y0 = parms.y - parms.top * parms.destheight / parms.texheight;

		auto spriteScale = vec2(parms.destwidth, parms.destheight);
		auto spriteOffset = vec2(x0, y0) + spriteScale * 0.5f;
		
		depth += 0.001;
		auto quad = CreateHudQuad(vec3(spriteOffset,depth), vec4(1.0, 1.0, 1.0, 1.0), textureIndex, spriteScale);

		//render it using our HUD shader
		auto spriteUnit = new OpenGlRenderUnit(this->hudCamera, MatGl::Globals::TextureManager->GetTextureArray(textureArray), this->hudShader);

		//Create 3d model, we are using triangles.
		auto model = Model3d
		{
			Triangle,
			quad
		};

		spriteUnit->Load(model);
		renderUnits.push_back(spriteUnit);
	}
	void OpenGlHudRenderer::Render()
	{
		for (auto & unit : this->renderUnits)
		{
			unit->Render();
			delete unit;
		}

		this->renderUnits.clear();

		depth = 0.0f;
	}
}