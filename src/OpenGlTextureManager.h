#pragma once
#include "textures/textures.h"
#include "glew.h"
#include <map>
#include <string>
#include <vector>
namespace MatGl {
	struct Vec2dInt {
		Vec2dInt(int x, int y) {
			this->x = x;
			this->y = y;
		}
		int x;
		int y;

		bool const operator==(const Vec2dInt& o) const {
			return x == o.x && y == o.y;
		}

		bool const operator<(const Vec2dInt& o) const {
			return x < o.x || (x == o.x && y < o.y);
		}
	};
	
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
		static const std::string WALL_TEXTURES;
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
			TextureArrayMap textureMap;
	};
}