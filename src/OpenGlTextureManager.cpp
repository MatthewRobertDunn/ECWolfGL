#include "OpenGlTextureManager.h"
#include <iostream>
#include "textures/bitmap.h"
#include <vector>
#include <OpenGlErrors.h>
#include <OpenGlTextureLoader.h>
#include <format>
#include <memory>
#include <glm/glm.hpp>
#include <MatGlMath.h>
using namespace glm;
namespace MatGl {
	//prototypes of private functions
	std::map<ivec2, WolfTextureList, IVec2Compare> GroupTextures(FTextureManager* wolfTextures);
	GLuint CreateTextureArray();

	//Texture to use when rendering walls.
	const std::string OpenGlTextureManager::WALL_TEXTURES = "wolf/64/64";

	OpenGlTextureManager::OpenGlTextureManager()
	{

	}

	void MatGl::OpenGlTextureManager::LoadTextures(FTextureManager* wolfTextures)
	{
		const int totalTextures = wolfTextures->NumTextures();
		int width = 64;
		int height = 64;

		//Get map of all textures and their count
		auto groupedTextures = GroupTextures(wolfTextures);

		//Initialize an opengl TextureArray for each one
		for (const auto& p : groupedTextures)
		{
			auto size = p.first;
			auto textures = p.second;
			//Figure out key for our texture dictionary
			std::string key = std::format("wolf/{}/{}", size.x, size.y);

			std::cout << "Loading textures " << key << std::endl;
			//Create a texture array
			MatGlTextureArray newTextureArray = { CreateTextureArray(), TextureLayerMap() };
			//Allocate storage.
			glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, size.x, size.y, textures.size());
			CheckGlErrors();

			//Load all textures into the array
			const int bufferSize = size.x * size.y * 4;
			std::vector<uint8_t> openGlTextureData(bufferSize); //opengl texture data in rgba (32bit) format.
			int layer = 0;
			for (auto& currentTexture : p.second) {
				std::cout << currentTexture->Name << ",";
				FCopyInfo inf = { OP_OVERWRITE, BLEND_NONE, {0}, 0, 0 };
				FBitmap bmp((BYTE*)(openGlTextureData.data()), size.x * 4, size.x, size.y);
				currentTexture->CopyTrueColorPixels(&bmp, 0, 0, 0, &inf);
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, size.x, size.y, 1, GL_RGBA, GL_UNSIGNED_BYTE, openGlTextureData.data());
				newTextureArray.TextureLayerMap.insert(std::pair<FTextureID, int>(currentTexture->id, layer));
				layer++;
				CheckGlErrors();
			};

			std::cout << std::endl;

			this->textureMap[key] = newTextureArray;
		}
	}

	GLuint OpenGlTextureManager::GetTextureArray(std::string texturePack)
	{
		return this->textureMap[texturePack].TextureArray;
	}
	int OpenGlTextureManager::GetTextureArrayIndexForWolf(std::string texturePack, FTextureID wolfId)
	{
		return this->textureMap[texturePack].TextureLayerMap[wolfId];
	}

	//Group all textures by their width and height
	std::map<ivec2, WolfTextureList, IVec2Compare> GroupTextures(FTextureManager* wolfTextures) {
		
		std::map<ivec2, WolfTextureList, IVec2Compare> result;
		
		const int totalTextures = wolfTextures->NumTextures();

		for (int i = 1; i < totalTextures; i++) {
			auto currentTexture = wolfTextures->ByIndex(i);

			//Skip any textures without pixel data for some reason
			if (currentTexture->GetPixels() == nullptr)
			{
				continue;
			}

			auto key = ivec2(currentTexture->GetWidth(), currentTexture->GetHeight());
			result[key].push_back(currentTexture);
		}

		return result;
	}


	GLuint CreateTextureArray() {
		//Create texture array
		GLuint textureArray;

		glGenTextures(1, &textureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureArray;
	}
}