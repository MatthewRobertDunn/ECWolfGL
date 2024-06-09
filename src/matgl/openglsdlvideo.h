#pragma once
#include "v_video.h"

namespace MatGl
{
	class OpenGlSDLVideo : public IVideo
	{
	public:
		OpenGlSDLVideo(int parm);
		~OpenGlSDLVideo();

		EDisplayType GetDisplayType() { return DISPLAY_Both; }
		void SetWindowedScale(float scale);
		DFrameBuffer* CreateFrameBuffer(int width, int height, bool fs, DFrameBuffer* old);
		void StartModeIterator(int bits, bool fs);
		bool NextMode(int* width, int* height, bool* letterbox);

	private:
		int IteratorMode;
		int IteratorBits;
		bool IteratorFS;
	};
}