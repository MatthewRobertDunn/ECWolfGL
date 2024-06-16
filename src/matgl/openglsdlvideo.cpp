#include "openglsdlvideo.h"
#include "c_cvars.h"
#include "openglhudrenderer.h"

namespace MatGl
{
	struct MiniModeInfo
	{
		WORD Width, Height;
	};
	// Dummy screen sizes to pass when windowed
	static MiniModeInfo WinModes[] =
	{
		{ 320, 200 },
		{ 320, 240 },
		{ 400, 225 },	// 16:9
		{ 400, 300 },
		{ 480, 270 },	// 16:9
		{ 480, 360 },
		{ 512, 288 },	// 16:9
		{ 512, 384 },
		{ 640, 270 },   // 64:27 (~21:9)
		{ 640, 360 },	// 16:9
		{ 640, 400 },
		{ 640, 480 },
		{ 720, 480 },	// 16:10
		{ 720, 540 },
		{ 800, 450 },	// 16:9
		{ 800, 480 },
		{ 800, 500 },	// 16:10
		{ 800, 600 },
		{ 848, 480 },	// 16:9
		{ 960, 600 },	// 16:10
		{ 960, 720 },
		{ 1024, 288 },  // 32:9
		{ 1024, 576 },	// 16:9
		{ 1024, 600 },	// 17:10
		{ 1024, 640 },	// 16:10
		{ 1024, 768 },
		{ 1088, 612 },	// 16:9
		{ 1152, 648 },	// 16:9
		{ 1152, 720 },	// 16:10
		{ 1152, 864 },
		{ 1280, 360 },  // 32:9
		{ 1280, 540 },  // 64:27 (~21:9)
		{ 1280, 720 },	// 16:9
		{ 1280, 854 },
		{ 1280, 800 },	// 16:10
		{ 1280, 960 },
		{ 1280, 1024 },	// 5:4
		{ 1360, 768 },	// 16:9
		{ 1366, 768 },
		{ 1400, 787 },	// 16:9
		{ 1400, 875 },	// 16:10
		{ 1400, 1050 },
		{ 1440, 900 },
		{ 1440, 960 },
		{ 1440, 1080 },
		{ 1600, 900 },	// 16:9
		{ 1600, 1000 },	// 16:10
		{ 1600, 1200 },
		{ 1680, 1050 },	// 16:10
		{ 1720, 720 },  // 43:18 (~21:9)
		{ 1920, 540 },  // 32:9
		{ 1920, 1080 },
		{ 1920, 1200 },
		{ 2048, 864 }, // 64:27 (~21:9)
		{ 2048, 1536 },
		{ 2240, 1400 }, // 16:10
		{ 2304, 1440 }, // 16:10
		{ 2560, 720 },  // 32:9
		{ 2560, 1080 }, // 64:27 (~21:9)
		{ 2560, 1440 },
		{ 2560, 1600 },
		{ 2560, 2048 },
		{ 2880, 1800 },
		{ 3072, 1920 }, // 16:10
		{ 3200, 1800 },
		{ 3440, 1440 }, // 43:18 (~21:9)
		{ 3840, 1080 }, // 32:9
		{ 3840, 2160 },
		{ 3840, 2400 },
		{ 4096, 2160 },
		{ 4096, 2304 }, // 16:9
		{ 4480, 2520 }, // 16:9
		{ 5120, 1440 }, // 32:9
		{ 5120, 2880 }
	};

	void I_ClosestResolution(int* width, int* height, int bits)
	{
		int twidth, theight;
		int cwidth = 0, cheight = 0;
		int iteration;
		DWORD closest = 0xFFFFFFFFu;

		for (iteration = 0; iteration < 2; iteration++)
		{
			Video->StartModeIterator(bits, screen ? screen->IsFullscreen() : vid_fullscreen);
			while (Video->NextMode(&twidth, &theight, NULL))
			{
				if (twidth == *width && theight == *height)
					return;

				if (iteration == 0 && (twidth < *width || theight < *height))
					continue;

				DWORD dist = (twidth - *width) * (twidth - *width)
					+ (theight - *height) * (theight - *height);

				if (dist < closest)
				{
					closest = dist;
					cwidth = twidth;
					cheight = theight;
				}
			}
			if (closest != 0xFFFFFFFFu)
			{
				*width = cwidth;
				*height = cheight;
				return;
			}
		}
	}

	OpenGlSDLVideo::OpenGlSDLVideo(int parm)
	{
		IteratorBits = 0;
		IteratorFS = false;
	}

	OpenGlSDLVideo::~OpenGlSDLVideo()
	{
	}

	void OpenGlSDLVideo::StartModeIterator(int bits, bool fs)
	{
		IteratorMode = 0;
		IteratorBits = bits;
		IteratorFS = fs;
	}

	bool OpenGlSDLVideo::NextMode(int* width, int* height, bool* letterbox)
	{
		if (IteratorBits != 8)
			return false;

		if ((unsigned)IteratorMode < sizeof(WinModes) / sizeof(WinModes[0]))
			{
				*width = WinModes[IteratorMode].Width;
				*height = WinModes[IteratorMode].Height;
				++IteratorMode;
				return true;
			}
		return false;
	}

	DFrameBuffer* OpenGlSDLVideo::CreateFrameBuffer(int width, int height, bool fullscreen, DFrameBuffer* old)
	{
		static int retry = 0;
		static int owidth, oheight;

		PalEntry flashColor;
		int flashAmount;

		SDL_Window* oldwin = NULL;

#if __ANDROID__
		// Always fullscreen in Android
		fullscreen = true;
#endif

		if (old != NULL)
		{ // Reuse the old framebuffer if its attributes are the same
			OpenGlHudRenderer* fb = static_cast<OpenGlHudRenderer*> (old);
			if (fb->Width == width &&
				fb->Height == height)
			{
				bool fsnow = (SDL_GetWindowFlags(fb->Screen) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;

				if (fsnow != fullscreen)
				{
					fb->SetFullscreen(fullscreen);
				}
				return old;
			}

			oldwin = fb->Screen;
			fb->Screen = NULL;

			old->GetFlash(flashColor, flashAmount);
			old->ObjectFlags |= OF_YesReallyDelete;
			if (screen == old) screen = NULL;
			delete old;
		}
		else
		{
			flashColor = 0;
			flashAmount = 0;
		}

		OpenGlHudRenderer* fb = new OpenGlHudRenderer(width, height, fullscreen, oldwin);

		// If we could not create the framebuffer, try again with slightly
		// different parameters in this order:
		// 1. Try with the closest size
		// 2. Try in the opposite screen mode with the original size
		// 3. Try in the opposite screen mode with the closest size
		// This is a somewhat confusing mass of recursion here.

		while (fb == NULL || !fb->IsValid())
		{
			if (fb != NULL)
			{
				delete fb;
			}

			switch (retry)
			{
			case 0:
				owidth = width;
				oheight = height;
			case 2:
				// Try a different resolution. Hopefully that will work.
				I_ClosestResolution(&width, &height, 8);
				break;

			case 1:
				// Try changing fullscreen mode. Maybe that will work.
				width = owidth;
				height = oheight;
				fullscreen = !fullscreen;
				break;

			default:
				// I give up!
				I_FatalError("Could not create new screen (%d x %d)", owidth, oheight);
			}

			++retry;
			fb = static_cast<OpenGlHudRenderer*>(CreateFrameBuffer(width, height, fullscreen, NULL));
		}
		retry = 0;

		fb->SetFlash(flashColor, flashAmount);

		return fb;
	}

	void OpenGlSDLVideo::SetWindowedScale(float scale)
	{
	}
}