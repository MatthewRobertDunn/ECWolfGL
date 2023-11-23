#pragma once
#define MATGL
//#define MATGLFORCE
#include "openglrenderer.h"
#include "openglsurface.h"
#include "gamecamera.h"
#include "opengltexturemanager.h"
#include "matglmap.h"
namespace MatGl{
	inline OpenGlRenderer* Renderer = nullptr;
	inline GameCamera* Camera = nullptr;
	inline OpenGlSurface* Surface = nullptr;
	inline OpenGlTextureManager* TextureManager = nullptr;
	inline MatGlMap* Map = nullptr;
}
