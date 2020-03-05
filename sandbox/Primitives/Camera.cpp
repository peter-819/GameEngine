#include "Camera.h"
#include <gtx/transform.hpp>
#include <QtCore/qdebug.h>


Camera::Camera()
{
	CameraPosition = vec3(0.0f, 12.8692875f, 20.3249702f);
	ViewDirection = vec3(0.0116819320f, -0.627242148f, -0.778746963f);
	UP = vec3(0.0f, 1.0f, 0.0f);
	MousePosition = glm::vec2(-1, -1);
}

Camera::~Camera()
{
}

void Camera::mouseUpdate(glm::vec2 newMousePosition) {
	glm::vec2 MouseDelta = newMousePosition - MousePosition;
	if (MousePosition.x < 0 || MousePosition.y<0 || glm::length(MouseDelta) > MOUSE_DELTA_MAXIUM) {
		MousePosition = newMousePosition;
		return;
	}
	ViewDirection = glm::mat3(glm::rotate(-MouseDelta.x * MOUSE_VERTICAL_SPEED, UP)) * ViewDirection;
	ViewDirection = glm::mat3(glm::rotate(MouseDelta.y * MOUSE_HORIZONTAL_SPEED, glm::cross(UP, ViewDirection))) * ViewDirection;
	MousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(CameraPosition, CameraPosition + ViewDirection, UP);
}

void Camera::movePosition(MOVE_DIRECTION dir) {
	glm::vec3 StrafeDirection;
	switch (dir) {
	case MOVE_TOWARD:
		CameraPosition += ViewDirection * MOVE_SPEED;
		break;
	case MOVE_FORWARD:
		CameraPosition -= ViewDirection * MOVE_SPEED;
		break;
	case MOVE_LEFT:
		StrafeDirection = glm::cross(ViewDirection, UP);
		CameraPosition -= StrafeDirection * MOVE_SPEED;
		break;
	case MOVE_RIGHT:
		StrafeDirection = glm::cross(ViewDirection, UP);
		CameraPosition += StrafeDirection * MOVE_SPEED;
		break;
	case MOVE_UP:
		CameraPosition += UP * MOVE_SPEED;
		break;
	case MOVE_DOWN:
		CameraPosition -= UP * MOVE_SPEED;
		break;
	default:
		break;
	}
}