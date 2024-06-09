#pragma once
#include "v_video.h"


namespace MatGl
{
	class OpenGlSDLFB : public DFrameBuffer
	{
		//DECLARE_CLASS(OpenGlSDLFB, DFrameBuffer)
	public:
		OpenGlSDLFB(int width, int height, bool fullscreen, SDL_Window* oldwin);
		~OpenGlSDLFB();
		bool Lock(bool buffer);
		void Unlock();
		bool Relock();
		void ForceBuffering(bool force);
		bool IsValid();
		void Update();
		PalEntry* GetPalette();
		void GetFlashedPalette(PalEntry pal[256]);
		void UpdatePalette();
		bool SetGamma(float gamma);
		bool SetFlash(PalEntry rgb, int amount);
		void GetFlash(PalEntry& rgb, int& amount);
		void SetFullscreen(bool fullscreen);
		int GetPageCount();
		bool IsFullscreen();
		void PaletteChanged() { }
		int QueryNewPalette() { return 0; }
		bool Is8BitMode() { return true; }
		friend class OpenGlSDLVideo;
		virtual void SetVSync(bool vsync);
		virtual void ScaleCoordsFromWindow(SWORD& x, SWORD& y);
	private:
		PalEntry SourcePalette[256];
		BYTE GammaTable[3][256];
		PalEntry Flash;
		int FlashAmount;
		float Gamma;
		bool UpdatePending;
		SDL_Window* Screen;
		SDL_Renderer* Renderer;
		union
		{
			SDL_Texture* Texture;
			SDL_Surface* Surface;
		};

		SDL_Texture* OpenGlTexture = NULL; //Matty

		bool UsingRenderer;
		bool NeedPalUpdate;
		bool NeedGammaUpdate;
		bool NotPaletted;

		void UpdateColors();
		void ResetSDLRenderer();

		OpenGlSDLFB() {}
	};
}