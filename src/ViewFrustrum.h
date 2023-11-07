#pragma once
#include <map>
#include "glm/glm.hpp"
#include <vector>
#include <functional>
namespace MatGl {

	class ViewFrustrum {
		public:
			static const int NumberRadials = 360;
			ViewFrustrum(float renderDistance);
			void RenderCells(float minAngle, float floatMaxAngle, std::function<void(glm::vec2)> func);
		private:
			std::vector<std::vector<glm::vec2>> cells;
		};
}