/*
** r_sprites.h
**
**---------------------------------------------------------------------------
** Copyright 2011 Braden Obrzut
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**---------------------------------------------------------------------------
**
**
*/

#ifndef __R_SPRITES_H__
#define __R_SPRITES_H__

#include "actor.h"
#include "zstring.h"

enum SpecialSprites
{
	SPR_NONE,

	NUM_SPECIAL_SPRITES
};

bool R_CheckSpriteValid(unsigned int spr);
class FTexture *R_GetAMSprite(AActor *actor, angle_t rotangle, bool &flip);
unsigned int R_GetSprite(const char* spr);
void R_GetSpriteHitlist(BYTE* hitlist);
void R_InitSprites();
void R_LoadSprite(const FString &name);

void ScaleSprite(AActor *actor, int xcenter, const Frame *frame, unsigned height);
void Scale3DSprite(AActor *actor, const Frame *frame, unsigned height);
void R_DrawPlayerSprite(AActor *actor, const Frame *frame, fixed offsetX, fixed offsetY);

// For FArchive
unsigned int R_GetNumLoadedSprites();
uint32_t R_GetNameForSprite(unsigned int index);

// Thinker for S3DNA style zoom in sprites. (Game over, you win, cast call)
class SpriteZoomer : public Thinker
{
	DECLARE_CLASS(SpriteZoomer, Thinker)

private:
	const Frame *frame;
	FTextureID texID;
	int frametics;
	unsigned short count;
	unsigned short zoomtime;

public:
	SpriteZoomer(FTextureID texID, unsigned short zoomtime);
	SpriteZoomer(const Frame *frame, unsigned short zoomtime);

	void Draw();
	void Tick();
};

void R_DrawZoomer(FTextureID texID);

#include <utility>
namespace MatGl {
	FTexture* GetActorSprite(AActor* actor);
	FTexture* GetPlayerSprite(AActor* actor, const Frame* frame);
	std::pair<fixed, fixed> GetWeaponOffsets(FTexture* tex, fixed offsetX, fixed offsetY);
}

#endif
