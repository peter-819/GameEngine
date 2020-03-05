#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>

using glm::vec3;

enum MOVE_DIRECTION {
	MOVE_TOWARD,
	MOVE_FORWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};

class Camera
{
private:
	const float MOUSE_DELTA_MAXIUM = 20.0f;
	const float MOUSE_VERTICAL_SPEED = 0.0015f;
	const float MOUSE_HORIZONTAL_SPEED = 0.003f;
	const float MOVE_SPEED = 0.1f;

	vec3 CameraPosition;
	vec3 ViewDirection;
	vec3 UP;
	glm::vec2 MousePosition;
public:
	Camera();
	~Camera();
	void mouseUpdate(glm::vec2 newMousePosition);
	glm::mat4 getWorldToViewMatrix() const;
	void movePosition(MOVE_DIRECTION dir);
	vec3 getPosition();
};

#endif