#include "ViewFrustrum.h"
#include "glm/glm.hpp"
#include "math.h"
#include <numbers> // std::numbers
#include <stdexcept>

using namespace glm;
using namespace std::numbers;
namespace MatGl {

	MatGl::ViewFrustrum::ViewFrustrum(float renderDistance)
	{
		for (int x = -renderDistance; x < renderDistance; x++)
			for (int y = -renderDistance; y < renderDistance; y++) {
				vec2 cell = vec2(x, y);
				if (length(cell) > renderDistance) {
					continue;
				}

				//Get distance this cell is.
				const int distance = length(cell);

				if (distance == 0) {
					continue;
				}

				//returns -PI to +PI
				float theta = glm::atan(cell.y, cell.x);

				if (theta < 0) {
					theta = theta + 2 * pi;
				}

				//Get radial this cell is on
				int radial = (int)((theta / (2 * pi)) * NumberRadials);

				//Get our list on this radial
				auto radialList = &this->cells[radial];

				//Is our radial list long enough?
				if (radialList->size() < distance) {
					radialList->resize(renderDistance);
				}

				int index = distance - 1;
				auto testCell = radialList->at(index);
				if (testCell.x > 0.1f && testCell.y > 0.1f) {
					throw std::runtime_error("Overwrote cell");
				}

				radialList->at(index) = ivec2(x, -y); //Put this cell on this radial.
			}
	}
	void ViewFrustrum::RenderCells(float minAngle, float floatMaxAngle, std::function<void(glm::ivec2)> func)
	{
		int startRadial = (int)((minAngle / (2 * pi)) * NumberRadials);
		int endRadial = (int)((floatMaxAngle / (2 * pi)) * NumberRadials);
		int maxDistance = this->cells[0].size();
		func(ivec2(0,0));
		for (int distance = 0; distance < maxDistance; distance++) {
			for (int r = startRadial; r < endRadial; r++) {
				int radial = r % NumberRadials;
				if (radial < 0) {
					radial = radial + NumberRadials;
				}
				if (!this->cells.contains(radial)) {
					continue;
				}
				auto cell = this->cells[radial][distance];
				if (cell.x != 0 || cell.y != 0) {
					func(cell);
				}
			}
		}
	}
}