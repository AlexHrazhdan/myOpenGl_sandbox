#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/transform.hpp>


Camera::Camera() : viewDirection(0.0f, 0.0f, -1.0f), UP(0.0f, 1.0f, 0.0f)
{
	
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
		
	viewDirection = glm::mat3(glm::rotate(glm::radians(-0.5f*mouseDelta.x), UP))*viewDirection;
	viewDirection = glm::mat3(glm::rotate(glm::radians(-0.5f*mouseDelta.y), glm::cross(viewDirection,UP)))*viewDirection;
	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);

}
const float movementSpeed = 0.1f;
void Camera::moveForward()
{
	position += viewDirection*movementSpeed;
}
void Camera::moweBackward()
{
	position += -viewDirection*movementSpeed;
}
void Camera::moveLeft()
{
	glm::vec3 sideVec = glm::normalize(glm::cross(viewDirection, UP));
	position -= sideVec*movementSpeed;
}
void Camera::moveRight()
{
	glm::vec3 sideVec = glm::normalize(glm::cross(viewDirection, UP));
	position += sideVec*movementSpeed;
}
void Camera::moveUp() 
{
	position += UP*movementSpeed;
}
void Camera::moveDown()
{
	position -= UP*movementSpeed;
}