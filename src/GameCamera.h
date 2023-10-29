#pragma once
#include <glm/glm.hpp>
namespace MatGl {
	class GameCamera
	{
	public:
		GameCamera(int width, int height);
		void SetCamera(float playerX, float playerY, float playerAngle);
		glm::mat4 ModelViewProjection;
		int width, height;
	};
}