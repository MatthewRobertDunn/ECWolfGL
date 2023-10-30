#pragma once
#include <OpenGLRenderer.h>
#include <OpenGLSurface.h>
#include <GameCamera.h>
#include <OpenGlTextureManager.h>
namespace MatGl{
	inline OpenGlRenderer* Renderer;
	inline GameCamera* Camera;
	inline OpenGlSurface* Surface;
	inline OpenGlTextureManager* TextureManager;
}
