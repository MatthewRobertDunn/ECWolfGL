#pragma once
#include "textures/textures.h"
#include "glew.h"
#include <map>
#include <string>
#include <vector>
#include "glm/glm.hpp"
namespace MatGl {
	typedef std::vector<FTexture*> WolfTextureList;
	typedef std::map<FTextureID, int> TextureLayerMap;

	struct MatGlTextureArray {
		GLuint TextureArray;
		TextureLayerMap TextureLayerMap;
	};
	typedef std::map<std::string, MatGlTextureArray> TextureArrayMap;

	class MatGlTextureManager
	{
	public:
		virtual void LoadTextures() = 0; //pure virtual method
		static const std::string WALL_TEXTURES;
	};


	//OpenGL Specific version of TextureManager
	class OpenGlTextureManager : public MatGlTextureManager
	{
		public:
			OpenGlTextureManager(FTextureManager* wolfTextures);
			void LoadTextures() override;
			//Returns reference to an opengl sampler2DArray
			GLuint GetTextureArray(std::string texturePack);
			//Returns open gl texture array index for a given wolf texture identifier
			int GetTextureArrayIndexForWolf(std::string texturePack, FTextureID wolfId);
			int GetTextureArrayIndexForName(std::string texturePack, std::string name);

		private:
			TextureArrayMap textureMap;
			FTextureManager* wolfTextures;
	};
}