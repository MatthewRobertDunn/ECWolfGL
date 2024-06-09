#include "openglsdlfb.h"
#include "openglmain.h"
#include "version.h"
#include "v_pfx.h"
#include "wl_main.h"

#define vid_adapter 0
#define rgamma 1.f
#define ggamma 1.f
#define bgamma 1.f
#define vid_forcesurface 0
#define vid_displaybits 32
namespace MatGl
{

	//===========================================================================

// CheckRatio -- From ZDoom
//
// Tries to guess the physical dimensions of the screen based on the
// screen's pixel dimensions.

	void ScaleWithAspect(int& w, int& h, int Width, int Height)
	{
		int resRatio = CheckRatio(Width, Height);
		int screenRatio;
		CheckRatio(w, h, &screenRatio);
		if (resRatio == screenRatio)
			return;

		double yratio;
		switch (resRatio)
		{
		case 0: yratio = 4. / 3.; break;
		case 1: yratio = 16. / 9.; break;
		case 2: yratio = 16. / 10.; break;
		case 3: yratio = 17. / 10.; break;
		case 4: yratio = 5. / 4.; break;
		case 5: yratio = 64. / 27.; break;
		case 6: yratio = 32. / 9.; break;
		default: return;
		}
		double y = w / yratio;
		if (y > h)
			w = (int)(h * yratio);
		else
			h = (int)(y);
	}

	OpenGlSDLFB::OpenGlSDLFB(int width, int height, bool fullscreen, SDL_Window* oldwin)
		: DFrameBuffer(width, height)
	{
		int i;

		NeedPalUpdate = false;
		NeedGammaUpdate = false;
		UpdatePending = false;
		NotPaletted = false;
		FlashAmount = 0;
		Renderer = NULL;
		Texture = NULL;

		if (oldwin)
		{
			// In some cases (Mac OS X fullscreen) SDL2 doesn't like having multiple windows which
			// appears to inevitably happen while compositor animations are running. So lets try
			// to reuse the existing window.
			Screen = oldwin;
			SDL_SetWindowSize(Screen, width, height);
			SetFullscreen(fullscreen);
		}
		else
		{
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			Screen = SDL_CreateWindow(GetGameCaption(),
				SDL_WINDOWPOS_UNDEFINED_DISPLAY(vid_adapter), SDL_WINDOWPOS_UNDEFINED_DISPLAY(vid_adapter),
				width, height, (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) | SDL_WINDOW_OPENGL);

			if (Screen == NULL)
				return;
		}

#ifdef _WIN32
		extern void ForceSDLFocus(SDL_Window * win);
		ForceSDLFocus(Screen);
#endif

		ResetSDLRenderer();

#ifdef __ANDROID__
		extern void PostSDLCreateRenderer(SDL_Window*);
		PostSDLCreateRenderer(Screen);
#endif

		for (i = 0; i < 256; i++)
		{
			GammaTable[0][i] = GammaTable[1][i] = GammaTable[2][i] = i;
		}


		memcpy(SourcePalette, GPalette.BaseColors, sizeof(PalEntry) * 256);
		UpdateColors();

#ifdef __APPLE__
		SetVSync(vid_vsync);
#endif
	}

	OpenGlSDLFB::~OpenGlSDLFB()
	{
		if (Renderer)
		{
			if (Texture)
				SDL_DestroyTexture(Texture);
			if (OpenGlTexture)
				SDL_DestroyTexture(OpenGlTexture);

			SDL_DestroyRenderer(Renderer);
		}

		if (Screen)
		{
			SDL_DestroyWindow(Screen);
		}
	}

	bool OpenGlSDLFB::IsValid()
	{
		return DFrameBuffer::IsValid() && Screen != NULL;
	}

	int OpenGlSDLFB::GetPageCount()
	{
		return 1;
	}

	bool OpenGlSDLFB::Lock(bool buffered)
	{
		return DSimpleCanvas::Lock();
	}

	bool OpenGlSDLFB::Relock()
	{
		return DSimpleCanvas::Lock();
	}

	void OpenGlSDLFB::Unlock()
	{
		if (UpdatePending && LockCount == 1)
		{
			Update();
		}
		else if (--LockCount <= 0)
		{
			Buffer = NULL;
			LockCount = 0;
		}
	}

	void OpenGlSDLFB::Update()
	{
		if (LockCount != 1)
		{
			if (LockCount > 0)
			{
				UpdatePending = true;
				--LockCount;
			}
			return;
		}

		DrawRateStuff();

		if (NeedGammaUpdate)
		{
			bool Windowed = false;
			NeedGammaUpdate = false;
			CalcGamma((Windowed || rgamma == 0.f) ? Gamma : (Gamma * rgamma), GammaTable[0]);
			CalcGamma((Windowed || ggamma == 0.f) ? Gamma : (Gamma * ggamma), GammaTable[1]);
			CalcGamma((Windowed || bgamma == 0.f) ? Gamma : (Gamma * bgamma), GammaTable[2]);
			NeedPalUpdate = true;
		}

		if (NeedPalUpdate)
		{
			NeedPalUpdate = false;
			UpdateColors();
		}

#if 0
#ifndef __APPLE__
		if (vid_maxfps && !cl_capfps)
		{
			SEMAPHORE_WAIT(FPSLimitSemaphore)
		}
#endif
#endif

		Buffer = NULL;
		LockCount = 0;
		UpdatePending = false;

		//BlitCycles.Reset();
		//SDLFlipCycles.Reset();
		//BlitCycles.Clock();

		void* pixels = NULL;
		int pitch = NULL;


		if (UsingRenderer)
		{

			if (SDL_LockTexture(Texture, NULL, &pixels, &pitch))
			{
				exit(0);
				return;
			}
		}
		else
		{
			if (SDL_LockSurface(Surface))
				return;

			pixels = Surface->pixels;
			pitch = Surface->pitch;
		}

		if (NotPaletted)
		{

			GPfx.Convert(MemBuffer, Pitch,
				pixels, pitch, Width, Height,
				FRACUNIT, FRACUNIT, 0, 0);

		}
		else
		{
			if (pitch == Pitch)
			{
				memcpy(pixels, MemBuffer, Width * Height);
			}
			else
			{
				for (int y = 0; y < Height; ++y)
				{
					memcpy((BYTE*)pixels + y * pitch, MemBuffer + y * Pitch, Width);
				}
			}
		}

		if (UsingRenderer)
		{
			void* openGlPixels = NULL;
			int openGlPitch;
			if (!SDL_LockTexture(OpenGlTexture, NULL, &openGlPixels, &openGlPitch))
			{
				MatGl::Globals::HudRenderer->Render();
				MatGl::Globals::Surface->Render(openGlPixels);
				SDL_UnlockTexture(OpenGlTexture);
			}
			//SDLFlipCycles.Clock();
			SDL_RenderClear(Renderer);
			SDL_UnlockTexture(Texture);
			SDL_RenderCopy(Renderer, Texture, NULL, NULL);
			SDL_RenderCopy(Renderer, OpenGlTexture, NULL, NULL);

#ifdef __ANDROID__
			// Hack control overlay in
			extern void frameControls();
			frameControls();
#endif

			SDL_RenderPresent(Renderer);
			//SDLFlipCycles.Unclock();
		}
		else
		{
			SDL_UnlockSurface(Surface);

			//SDLFlipCycles.Clock();
			SDL_UpdateWindowSurface(Screen);
			//SDLFlipCycles.Unclock();
		}
	}

	void OpenGlSDLFB::UpdateColors()
	{
		if (NotPaletted)
		{
			PalEntry palette[256];

			for (int i = 0; i < 256; ++i)
			{
				palette[i].r = GammaTable[0][SourcePalette[i].r];
				palette[i].g = GammaTable[1][SourcePalette[i].g];
				palette[i].b = GammaTable[2][SourcePalette[i].b];
			}
			if (FlashAmount)
			{
				DoBlending(palette, palette,
					256, GammaTable[0][Flash.r], GammaTable[1][Flash.g], GammaTable[2][Flash.b],
					FlashAmount);
			}
			GPfx.SetPalette(palette);
		}
		else
		{
			SDL_Color colors[256];

			for (int i = 0; i < 256; ++i)
			{
				colors[i].r = GammaTable[0][SourcePalette[i].r];
				colors[i].g = GammaTable[1][SourcePalette[i].g];
				colors[i].b = GammaTable[2][SourcePalette[i].b];
			}
			if (FlashAmount)
			{
				DoBlending((PalEntry*)colors, (PalEntry*)colors,
					256, GammaTable[2][Flash.b], GammaTable[1][Flash.g], GammaTable[0][Flash.r],
					FlashAmount);
			}
			SDL_SetPaletteColors(Surface->format->palette, colors, 0, 256);
		}
	}

	PalEntry* OpenGlSDLFB::GetPalette()
	{
		return SourcePalette;
	}

	void OpenGlSDLFB::UpdatePalette()
	{
		NeedPalUpdate = true;
	}

	bool OpenGlSDLFB::SetGamma(float gamma)
	{
		Gamma = gamma;
		NeedGammaUpdate = true;
		return true;
	}

	bool OpenGlSDLFB::SetFlash(PalEntry rgb, int amount)
	{
		Flash = rgb;
		FlashAmount = amount;
		NeedPalUpdate = true;
		return true;
	}

	void OpenGlSDLFB::GetFlash(PalEntry& rgb, int& amount)
	{
		rgb = Flash;
		amount = FlashAmount;
	}

	// Q: Should I gamma adjust the returned palette?
	void OpenGlSDLFB::GetFlashedPalette(PalEntry pal[256])
	{
		memcpy(pal, SourcePalette, 256 * sizeof(PalEntry));
		if (FlashAmount)
		{
			DoBlending(pal, pal, 256, Flash.r, Flash.g, Flash.b, FlashAmount);
		}
	}

	void OpenGlSDLFB::SetFullscreen(bool fullscreen)
	{
#ifdef __ANDROID__
		fullscreen = true;
#endif

		if (IsFullscreen() == fullscreen)
			return;

		SDL_SetWindowFullscreen(Screen, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
		if (!fullscreen)
		{
			// Restore proper window size
			SDL_SetWindowSize(Screen, Width, Height);
		}

		ResetSDLRenderer();
	}

	bool OpenGlSDLFB::IsFullscreen()
	{
		return (SDL_GetWindowFlags(Screen) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
	}

	void fillTexture(SDL_Renderer* renderer, SDL_Texture* texture, int r, int g, int b, int a)
	{
		SDL_SetRenderTarget(renderer, texture);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderTarget(renderer, NULL);
	}


	void OpenGlSDLFB::ResetSDLRenderer()
	{
		if (Renderer)
		{
			if (Texture)
				SDL_DestroyTexture(Texture);
			if (OpenGlTexture)
				SDL_DestroyTexture(OpenGlTexture);

			SDL_DestroyRenderer(Renderer);
		}

		UsingRenderer = !vid_forcesurface;
		if (UsingRenderer)
		{
			Renderer = SDL_CreateRenderer(Screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE |
				(vid_vsync ? SDL_RENDERER_PRESENTVSYNC : 0));

			SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);

			if (!Renderer)
				return;

			SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);

			Uint32 fmt;
			switch (vid_displaybits)
			{
			default: fmt = SDL_PIXELFORMAT_ARGB8888; break;
			case 30: fmt = SDL_PIXELFORMAT_ARGB2101010; break;
			case 24: fmt = SDL_PIXELFORMAT_RGB888; break;
			case 16: fmt = SDL_PIXELFORMAT_RGB565; break;
			case 15: fmt = SDL_PIXELFORMAT_ARGB1555; break;
			}
			Texture = SDL_CreateTexture(Renderer, fmt, SDL_TEXTUREACCESS_STREAMING, Width, Height);
			OpenGlTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, Width, Height);
			SDL_SetTextureBlendMode(OpenGlTexture, SDL_BLENDMODE_BLEND);

#if defined(MATGL) || defined(MATGLFORCE)
			float aspectRatio = MatGl::GameCamera::ConvertRatio(r_ratio, Width, Height);
			if (!MatGl::Globals::Surface) {
				MatGl::Globals::Surface = new MatGl::OpenGlSurface(Screen, Width, Height);
				MatGl::Globals::Camera = new MatGl::GameCamera(aspectRatio, Width, Height);
				MatGl::Globals::HudCamera = new MatGl::HudCamera(aspectRatio, Width, Height);
			}
			else {
				MatGl::Globals::Surface->Resize(Width, Height);
				MatGl::Globals::ResizeCameras(aspectRatio, Width, Height);
			}
#endif
			{
				NotPaletted = true;

				Uint32 format;
				SDL_QueryTexture(Texture, &format, NULL, NULL, NULL);

				Uint32 Rmask, Gmask, Bmask, Amask;
				int bpp;
				SDL_PixelFormatEnumToMasks(format, &bpp, &Rmask, &Gmask, &Bmask, &Amask);
				GPfx.SetFormat(bpp, Rmask, Gmask, Bmask);
			}
		}
		else
		{
			Surface = SDL_GetWindowSurface(Screen);

			if (Surface->format->palette == NULL)
			{
				NotPaletted = true;
				GPfx.SetFormat(Surface->format->BitsPerPixel, Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask);
			}
			else
				NotPaletted = false;
		}

		// In fullscreen, set logical size according to animorphic ratio.
		// Windowed modes are rendered to fill the window (usually 1:1)
		if (IsFullscreen())
		{
			int w, h;
			SDL_GetWindowSize(Screen, &w, &h);
			ScaleWithAspect(w, h, Width, Height);
			SDL_RenderSetLogicalSize(Renderer, w, h);
		}
	}

	void OpenGlSDLFB::SetVSync(bool vsync)
	{
		ResetSDLRenderer();
	}

	void OpenGlSDLFB::ScaleCoordsFromWindow(SWORD& x, SWORD& y)
	{
		int w, h;
		SDL_GetWindowSize(Screen, &w, &h);

		// Detect if we're doing scaling in the Window and adjust the mouse
		// coordinates accordingly. This could be more efficent, but I
		// don't think performance is an issue in the menus.
		if (IsFullscreen())
		{
			int realw = w, realh = h;
			ScaleWithAspect(realw, realh, SCREENWIDTH, SCREENHEIGHT);
			if (realw != SCREENWIDTH || realh != SCREENHEIGHT)
			{
				double xratio = (double)SCREENWIDTH / realw;
				double yratio = (double)SCREENHEIGHT / realh;
				if (realw < w)
				{
					x = (SWORD)((x - (w - realw) / 2) * xratio);
					y = (SWORD)(y * yratio);
				}
				else
				{
					y = (SWORD)((y - (h - realh) / 2) * yratio);
					x = (SWORD)(x * xratio);
				}
			}
		}
		else
		{
			x = (SWORD)(x * Width / w);
			y = (SWORD)(y * Height / h);
		}
	}
}