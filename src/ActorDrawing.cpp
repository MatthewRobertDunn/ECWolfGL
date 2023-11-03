#include "ActorDrawing.h"
#include "r_sprites.h"
#include "wl_agent.h"
#include "wl_play.h"
#include "wl_main.h"
namespace MatGl
{

	unsigned int CalcRotate(AActor* ob)
	{
		angle_t angle, viewangle;

		// this isn't exactly correct, as it should vary by a trig value,
		// but it is close enough with only eight rotations

		viewangle = players[ConsolePlayer].camera->angle + (centerx - ob->viewx) / 8;

		angle = viewangle - ob->angle;

		angle += ANGLE_180 + ANGLE_45 / 2;

		return angle / ANGLE_45;
	}


    FTexture* GetTextureForActor(AActor* actor)
    {
		return nullptr;
    }
}
