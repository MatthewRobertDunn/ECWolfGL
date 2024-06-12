#pragma once
#include "glm/glm.hpp"
#include "c_cvars.h"
namespace MatGl {

	class Camera {
		public:
			Camera(float aspectRatio, int width, int height);
			void Resize(float aspectRatio, int width, int height);
			static float ConvertRatio(Aspect ratio, int width, int height);
			int Width, Height;
			float AspectRatio;
			glm::mat4 ModelViewProjection;
			glm::mat4 Model;
			glm::mat4 View;
			glm::mat4 Projection;
			glm::vec3 Position;
			glm::vec3 Direction;
			//Same as the above except in opengl coordinates
			glm::vec3 GlPosition;
			glm::vec3 GlDirection;

	};

	class GameCamera : public Camera
	{
	public:
		GameCamera(float aspectRatio, int width, int height);
		void SetCamera(float playerX, float playerY, float playerAngle);
	};


	class HudCamera : public Camera {
	public:
		HudCamera(float aspectRatio, int width, int height);
	};
}