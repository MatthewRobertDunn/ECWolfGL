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
#include "v_video.h"
namespace MatGl {
	class OpenGlHudRenderer : public DFrameBuffer
	{
	public:
		OpenGlHudRenderer(HudCamera* camera, OpenGlTextureManager* textureManager);
		//void STACK_ARGS DrawTexture(FTexture* img, double x, double y, int tags_first, ...);
		void Render();

		// Inherited via DFrameBuffer
		bool Lock(bool buffered) override;
		void Update() override;
		PalEntry* GetPalette() override;
		void GetFlashedPalette(PalEntry palette[256]) override;
		void UpdatePalette() override;
		bool SetGamma(float gamma) override;
		bool SetFlash(PalEntry rgb, int amount) override;
		void GetFlash(PalEntry& rgb, int& amount) override;
		int GetPageCount() override;
		bool IsFullscreen() override;
		void PaletteChanged() override;
		int QueryNewPalette() override;
		bool Is8BitMode() override;
	protected:
		void STACK_ARGS DrawTextureV(FTexture* img, double x, double y, uint32 tag, va_list tags) override;
	private:
		Shader* hudShader = nullptr;
		HudCamera* camera = nullptr;
		OpenGlTextureManager* textureManager = nullptr;
		std::vector<OpenGlRenderUnit*> renderUnits;
	};
}
