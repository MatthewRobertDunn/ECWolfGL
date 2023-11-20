#pragma once
#include "openglrenderer.h"
#include "openglsurface.h"
#include "gamecamera.h"
#include "opengltexturemanager.h"
namespace MatGl{
	inline OpenGlRenderer* Renderer;
	inline GameCamera* Camera;
	inline OpenGlSurface* Surface;
	inline MatGlTextureManager* TextureManager;
}
