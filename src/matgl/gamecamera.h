#pragma once
#include "glm/glm.hpp"
#include "c_cvars.h"
namespace MatGl {
	class GameCamera
	{
	public:
		GameCamera(float aspectRatio, int width, int height);
		void SetCamera(float playerX, float playerY, float playerAngle);
		void Resize(float aspectRatio, int width, int height);
		static float ConvertRatio(Aspect ratio, int width, int height);
		glm::mat4 ModelViewProjection;
		glm::mat4 Model;
		glm::mat4 View;
		glm::mat4 Projection;
		glm::vec3 CameraPosition;
		glm::vec3 Direction;
		int Width, Height;
		float AspectRatio;
	};
}