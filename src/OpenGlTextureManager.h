#pragma once
#include "textures/textures.h"
#include "glew.h"
namespace MatGl {
	class OpenGlTextureManager
	{
		public: 
			OpenGlTextureManager();
			void LoadTextures(FTextureManager* wolfTextures);
			GLuint WallTextureArray = {};
		
		private:	
			
	};
}