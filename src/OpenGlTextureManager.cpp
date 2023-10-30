#include "OpenGlTextureManager.h"
#include <iostream>
#include "textures/bitmap.h"
#include <vector>
#include <OpenGlErrors.h>
#include <OpenGlTextureLoader.h>
namespace MatGl {

	void Fucky(int layer) {
		
			// Load two textures
			auto texture1 = LoadTexture("bark.bmp");
			int width = 64;
			int height = 64;
			// Attach imageData2 to layer 1
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture1->pixels);
			CheckGlErrors();
	}

	MatGl::OpenGlTextureManager::OpenGlTextureManager()
	{
		//Create texture array
		glGenTextures(1, &this->WallTextureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->WallTextureArray);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void MatGl::OpenGlTextureManager::LoadTextures(FTextureManager* wolfTextures)
	{
		const int totalTextures = wolfTextures->NumTextures();
		int width = 64;
		int height = 64;

		std::vector<int> wallTextures;
		//Count all the wall textures
		for (int i = 1; i < totalTextures; i++) {
			auto currentTexture = wolfTextures->ByIndex(i);
			//We are only loading 64x64 textures atm
			if (currentTexture->GetWidth() != 64 || currentTexture->GetHeight() != 64)
				continue;
			wallTextures.push_back(i);
		};

		// Specify the number of layers (textures) in the array
		BYTE openGlTextureData[64 * 64 * 4]; //opengl texture data in rgba format
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, wallTextures.size());
		CheckGlErrors();

		int layer = 0;
		for (auto& i : wallTextures) {
			auto currentTexture = wolfTextures->ByIndex(i);
			std::cout << currentTexture->Name << ",";
			FCopyInfo inf = { OP_OVERWRITE, BLEND_NONE, {0}, 0, 0 };
			FBitmap bmp((BYTE*)(&openGlTextureData), width * 4, width, height);
			currentTexture->CopyTrueColorPixels(&bmp, 0, 0, 0, &inf);
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, (void*)(&openGlTextureData));
			layer++;
			CheckGlErrors();
		};

		// Bind the texture array to a texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, WallTextureArray);
		CheckGlErrors();
	}
}