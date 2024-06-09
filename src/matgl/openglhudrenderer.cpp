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
namespace MatGl {
	using namespace glm;
	using namespace std::chrono;

	float depth = 0.0;

	OpenGlHudRenderer::OpenGlHudRenderer(HudCamera* hudCamera, OpenGlTextureManager* textureManager) : DFrameBuffer(hudCamera->Width, hudCamera->Height)
	{
		this->textureManager = textureManager;
		this->camera = hudCamera;
		this->hudShader = new Shader("./hud.vert", "./hud.frag");
	}

	void STACK_ARGS OpenGlHudRenderer::DrawTextureV(FTexture* img, double x, double y, uint32 tag, va_list tags)
	{
		//Figure out key for our texture dictionary
		std::string textureArray = std::format("wolf/{}/{}", img->GetWidth(), img->GetHeight());

		//Create a quad to hold this hud
		int textureIndex = textureManager->GetTextureArrayIndexForWolf(textureArray, img->GetID());

		float PIXELS_X = 320.0f;
		float PIXELS_Y = 200.0f;

		float baseRatio = 4.0f / 3.0f;
		float ratioChange = baseRatio / camera->AspectRatio;


		float tileXScale = PIXELS_X * FixedToFloat(img->xScale) / ratioChange;
		float tileYScale = PIXELS_Y * FixedToFloat(img->yScale);

		float scaleX = img->GetWidth() / tileXScale;
		float scaleY = img->GetHeight() / tileYScale;


		auto glCurrentOffset = -vec2(0.5 * scaleX, 0.5 * scaleY);

		vec2 glOffsets = vec2(x, y);

		//Scale 0 to width to 0 to 1.0
		glOffsets.x /= this->camera->Width;
		glOffsets.y /= this->camera->Height;

		vec2 spriteOffset = glOffsets - glCurrentOffset;

		depth += 0.001;
		auto quad = CreateHudQuad(vec3(spriteOffset,depth), vec4(1.0, 1.0, 1.0, 1.0), textureIndex, vec2(scaleX, scaleY));

		//render it using our HUD shader
		auto spriteUnit = new OpenGlRenderUnit(this->camera, textureManager->GetTextureArray(textureArray), this->hudShader);

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
		std::cout << this->renderUnits.size() << std::endl;
		for (auto & unit : this->renderUnits)
		{
			unit->Render();
			delete unit;
		}

		this->renderUnits.clear();

		depth = 0.0f;
	}

	// Inherited via DFrameBuffer
	bool OpenGlHudRenderer::Lock(bool buffered)
	{
		return false;
	}
	void OpenGlHudRenderer::Update()
	{
	}
	PalEntry* OpenGlHudRenderer::GetPalette()
	{
		return nullptr;
	}
	void OpenGlHudRenderer::GetFlashedPalette(PalEntry palette[256])
	{
	}
	void OpenGlHudRenderer::UpdatePalette()
	{
	}
	bool OpenGlHudRenderer::SetGamma(float gamma)
	{
		return false;
	}
	bool OpenGlHudRenderer::SetFlash(PalEntry rgb, int amount)
	{
		return false;
	}
	void OpenGlHudRenderer::GetFlash(PalEntry& rgb, int& amount)
	{
	}
	int OpenGlHudRenderer::GetPageCount()
	{
		return 0;
	}
	bool OpenGlHudRenderer::IsFullscreen()
	{
		return false;
	}
	void OpenGlHudRenderer::PaletteChanged()
	{
	}
	int OpenGlHudRenderer::QueryNewPalette()
	{
		return 0;
	}
	bool OpenGlHudRenderer::Is8BitMode()
	{
		return false;
	}
}