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
			
	//Deletes all render units then empties the render unit list
	void OpenGlHudRenderer::Clear(int left, int top, int right, int bottom, int palcolor, uint32 color)
	{
		for (auto& unit : this->renderUnits)
		{
			delete unit;
		}
		this->renderUnits.clear();
		depth = 0.0f;
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
		spriteUnit->TopLeft = quad[0].position;
		spriteUnit->BottomRight = quad[5].position;
		AddQuad(spriteUnit);
	}

	//After removing the overlapped quads, the new quad is added to this->quad
	//Overlap is checked by using the coordinates of the vertexes in each quad in the list
	//Vertexes are always stored in quad in the following order TL, BL, TR, TR, BL, BR
	void OpenGlHudRenderer::AddQuad(OpenGlRenderUnit* quad)
	{
		auto topLeft = quad->TopLeft;
		auto bottomRight = quad->BottomRight;

		std::vector<decltype(this->renderUnits)::iterator> quadsToRemove;

		for (auto it = this->renderUnits.begin(); it != this->renderUnits.end(); ++it)
		{
			const auto& existingQuad = *it;
			auto existingTopLeft = existingQuad->TopLeft;
			auto existingBottomRight = existingQuad->BottomRight;
		
			bool overlap = existingTopLeft.x >= topLeft.x
						&& existingTopLeft.y >= topLeft.y
						&& existingBottomRight.x <= bottomRight.x
						&& existingBottomRight.y <= bottomRight.y;

			if (overlap)
			{
				quadsToRemove.push_back(it);
				delete existingQuad;
			}
		}

		for (auto it = quadsToRemove.rbegin(); it != quadsToRemove.rend(); ++it)
		{
			this->renderUnits.erase(*it);
		}

		this->renderUnits.push_back(quad);
	}
	void OpenGlHudRenderer::Render()
	{
		for (auto & unit : this->renderUnits)
		{
			unit->Render();
		}

		depth = 0;
	}
}