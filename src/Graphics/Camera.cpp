#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "..//Game/gameHelper.h"

Camera::Camera()
{
	fov = 50.0f;
	cameraPos = glm::vec3(WINDOWSIZE.x / 2 , WINDOWSIZE.y + 350, 800.0f);
	cameraTarget = glm::vec3(WINDOWSIZE.x / 2, WINDOWSIZE.y / 2, -1);
	vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = -glm::cross(cameraDirection, cameraRight);

}


glm::mat4 Camera::GetPerspectiveProjection()
{
	return glm::perspective(glm::radians(fov), -(float)WINDOWSIZE.x / (float)WINDOWSIZE.y, -1.0f, 100.0f);;
}


mat4 Camera::GetOrthoPrjection()
{
	return glm::ortho(0.0f, (float)WINDOWSIZE.x, (float)WINDOWSIZE.y, 0.0f, -1.0f, 1.0f);
}

//View transformation , from world space to camer view space. 
mat4 Camera::GetViewMatrix()
{
	return ::lookAt(cameraPos, cameraTarget, cameraUp);
}

