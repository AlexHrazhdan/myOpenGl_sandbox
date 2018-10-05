#pragma once
#include <glm.hpp>
class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 UP;
	glm::vec2 oldMousePosition;
public:
	Camera();
	void mouseUpdate(const glm::vec2& newMousePosition);
	glm::mat4 Camera::getWorldToViewMatrix() const;
	void moveForward();
	void moweBackward();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
};

