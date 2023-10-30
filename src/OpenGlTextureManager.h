#pragma once
#include "textures/textures.h"
#include "glew.h"
#include <map>
#include <string>
namespace MatGl {
	class MatGlTextureManager
	{
	public:
		virtual void LoadTextures(FTextureManager* wolfTextures) = 0; //pure virtual method
	};


	//OpenGL Specific version of TextureManager
	class OpenGlTextureManager : public MatGlTextureManager
	{
		public:
			OpenGlTextureManager();
			void LoadTextures(FTextureManager* wolfTextures) override;
			//Returns reference to an opengl sampler2DArray
			GLuint GetTextureArray(std::string texturePack);
			//Returns open gl texture array index for a given wolf texture identifier
			int GetTextureArrayIndexForWolf(std::string texturePack, FTextureID wolfId);

		private:
			GLuint WallTextureArray = {};
			std::map<FTextureID, int> wallMap;
	};
}