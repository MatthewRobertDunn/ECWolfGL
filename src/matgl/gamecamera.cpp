#include "gamecamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

MatGl::GameCamera::GameCamera(int width, int height)
{
	this->width = width;
	this->height = height;
	this->SetCamera(0.0, 0.0, 0.0);
}

void MatGl::GameCamera::SetCamera(float playerX, float playerY, float playerAngle)
{
	glm::vec3 standingAt = glm::vec3(playerX, playerY, 0.6f);
	glm::vec3 direction = glm::normalize(glm::vec3(cos(playerAngle), -sin(playerAngle), 0.0));
	glm::vec3 lookingAt = direction + standingAt;
	const float wallHeight = 1.2039f;


	glm::mat4 view = glm::lookAt(standingAt,
		lookingAt,
		glm::vec3(0.0f, 0.0f, -1.0f)); //up is (0,0,1.0)

	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, wallHeight));



	//45 degree perspective view, 0 is near, 100 is far, cutoff 8:6 ratio
	glm::mat4 projection = glm::perspective(glm::radians(58.0f), (float)width / (float)height, 0.001f, 25.0f);

	//Combine into one transform.
	this->Model = model;
	this->ModelViewProjection = projection * view * model;
	this->Projection = projection;
	this->View = view;
	this->CameraPosition = standingAt;
	this->Direction = direction;
	
}

void MatGl::GameCamera::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
}
