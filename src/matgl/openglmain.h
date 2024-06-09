#pragma once
//#define MATGL
//#define MATGLFORCE
#include "openglrenderer.h"
#include "openglsurface.h"
#include "gamecamera.h"
#include "opengltexturemanager.h"
#include "matglmap.h"
#include "openglhudrenderer.h"
using namespace MatGl;
namespace MatGl::Globals{
	inline OpenGlRenderer* Renderer = nullptr;
	inline OpenGlHudRenderer* HudRenderer = nullptr;
	inline GameCamera* Camera = nullptr;
	inline MatGl::HudCamera* HudCamera = nullptr;
	inline OpenGlSurface* Surface = nullptr;
	inline OpenGlTextureManager* TextureManager = nullptr;
	inline MatGlMap* Map = nullptr;
	void ResizeCameras(float aspectRatio, int width, int height);
}
