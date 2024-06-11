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
#include "openglsdlfb.h"
namespace MatGl {
	class OpenGlHudRenderer : public OpenGlSDLFB
	{
	public:
		OpenGlHudRenderer(int width, int height, bool fullscreen, SDL_Window* oldwin);
	protected:
		void Render() override;
		void STACK_ARGS DrawTextureV(FTexture* img, double x, double y, uint32 tag, va_list tags) override;
	private:
		Shader* hudShader = nullptr;
		std::vector<OpenGlRenderUnit*> renderUnits;
	};
}
