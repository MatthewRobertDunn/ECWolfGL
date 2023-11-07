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
			void RenderCells(float minAngle, float floatMaxAngle, std::function<void(glm::ivec2)> func);
		private:
			std::map<int, std::vector<glm::ivec2>> cells;
		};
}