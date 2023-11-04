#pragma once
#include <glm/glm.hpp>
namespace MatGl {
	class GameCamera
	{
	public:
		GameCamera(int width, int height);
		void SetCamera(float playerX, float playerY, float playerAngle);
		glm::mat4 ModelViewProjection;
		glm::mat4 View;
		glm::mat4 Projection;
		glm::vec3 CameraPosition;
		glm::vec3 Direction;

		int width, height;
	};
}