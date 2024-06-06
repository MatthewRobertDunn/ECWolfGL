#include "openglmain.h"
using namespace MatGl::Globals;

void MatGl::Globals::ResizeCameras(float aspectRatio, int width, int height)
{
	if (Camera) {
		Camera->Resize(aspectRatio, width, height);
	}

	if (HudCamera) {
		HudCamera->Resize(aspectRatio, width, height);
	}
}
