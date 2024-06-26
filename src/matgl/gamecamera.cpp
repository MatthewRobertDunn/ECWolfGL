#include "gamecamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <c_cvars.h>

MatGl::Camera::Camera(float aspectRatio, int width, int height)
{
	this->Width = width;
	this->Height = height;
	this->AspectRatio = aspectRatio;
}

void MatGl::Camera::Resize(float aspectRatio, int width, int height)
{
	this->Width = width;
	this->Height = height;
	this->AspectRatio = aspectRatio;
}

float MatGl::Camera::ConvertRatio(Aspect ratio, int width, int height)
{
	switch (ratio)
	{
	case ASPECT_16_9:
		return 16.0 / 9.0;
		break;
	case ASPECT_16_10:
		return 16.0 / 10.0;
		break;
	case ASPECT_17_10:
		return 17.0 / 10.0;
		break;
	case ASPECT_4_3:
		return 4.0 / 3.0;
		break;
	case ASPECT_5_4:
		return 5.0 / 4.0;
		break;
	case ASPECT_64_27:
		return 64.0 / 27.0;
		break;
	case ASPECT_32_9:
		return 32.0 / 9.0;
		break;
	default:
		return (float)width / (float)height;
		break;
	}
}

MatGl::GameCamera::GameCamera(float aspectRatio, int width, int height) : Camera(aspectRatio, width, height)
{
}

void MatGl::GameCamera::SetCamera(float playerX, float playerY, float playerAngle)
{

	const float wallHeight = 1.2039f;
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, wallHeight));

	glm::vec3 standingAt = glm::vec4(playerX, playerY, 0.5f, 0.0f);
	glm::vec3 direction = glm::normalize(glm::vec3(cos(playerAngle), -sin(playerAngle), 0.0));
	

	//Apply our model transform to our camera coords  
	this->GlPosition = glm::vec3(model * glm::vec4(standingAt, 1.0));
	this->GlDirection = glm::vec3(model * glm::vec4(direction, 1.0));
	glm::vec3 glLookingAt = GlPosition + GlDirection;

	glm::mat4 view = glm::lookAt(GlPosition,
		glLookingAt,
		glm::vec3(0.0f, 0.0f, 1.0f)); //up is (0,0,1.0)

	

	//45 degree perspective view, 0 is near, 100 is far, cutoff 8:6 ratio
	glm::mat4 projection = glm::perspective(glm::radians(58.0f), AspectRatio, 0.001f, 25.0f);

	//Combine into one transform.
	this->Model = model;
	this->ModelViewProjection = projection * view * model;
	this->Projection = projection;
	this->View = view;
	this->Position = standingAt;
	this->Direction = direction;

}

MatGl::HudCamera::HudCamera(float aspectRatio, int width, int height) : Camera(aspectRatio, width, height)
{
	//translate into the middle
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.f, 0.0f));

	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);  //left, right, bottom, top
	this->ModelViewProjection = projection * model;
}

void MatGl::HudCamera::SetDepth(float depth)
{
	//translate into the middle
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.f,  depth));
	glm::mat4 projection = glm::ortho(0.0f, (float)Width, (float)Height, 0.0f);  //left, right, bottom, top
	this->ModelViewProjection = projection * model;
}
