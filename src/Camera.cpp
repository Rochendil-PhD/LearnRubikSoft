#include "Camera.hpp"

const float maxPosZ =  3.0;
const float minPosZ = -3.0;
const float PI = glm::pi<float>();
const float deltaAlpha = 2*PI / 3;
const float changeFaceAngle = PI / 3;
const float deltaHeight = 8;

float Camera::alpha;
float Camera::height;
short Camera::faceAngle;
glm::vec3 Camera::position;
glm::vec3 Camera::upDirection;
glm::mat4 Camera::View;

void Camera::init()
{
	height = 2;
	alpha = PI / 4;
	faceAngle = 0;
	//frontalFace = RED;
	upDirection = glm::vec3(0, 1, 0);
	updateView();
}

void Camera::updateView()
{
	float zoomResolve;
	float radius = 5;

	// This is needed to fix the zoom when moving camera up or down
	if (height > 0)
		zoomResolve = height / 5;
	else
		zoomResolve = -height / 5;

	// Update the matrix
	position = glm::vec3((radius - zoomResolve) * sin(alpha), height, (radius - zoomResolve) * cos(alpha));
	View = glm::lookAt(position, glm::vec3(0, 0, 0), upDirection);
}

void Camera::moveCamera(cameraMove direction, float dt)
{
	switch (direction)
	{
		case cmLEFT:
			// Rotate on OY clockwise
			alpha -= dt*deltaAlpha;
			if (alpha < 0) alpha += 2*PI;
			updateView();
			break;
		case cmRIGHT:
			// Rotate on OY clockwise
			alpha += dt*deltaAlpha;
			if (alpha >= 2 * PI) alpha -= 2*PI;
			updateView();
			break;
		case cmUP:
			// Prevent overspin
			if (height < maxPosZ)
			{
				height += dt*deltaHeight;
				updateView();
			}
			break;
		case cmDOWN:
			// Prevent overspin
			if (height > minPosZ)
			{
				height -= dt*deltaHeight;
				updateView();
			}
			break;
	}

	// Recalculate which face is in front of the cube
	calcFace();
}
void Camera::calcFace() {
    // Calculate angle diference
	float fangle = faceAngle * PI / 2;
    float dangle = alpha - fangle;

    // Fix trigonometric period
    if (dangle >= 2 * PI) dangle -= 2*PI;
    if (dangle < 0) dangle += 2*PI;

    // Change
    if (dangle > changeFaceAngle && dangle < PI) {
        frontColChange(1);
        ++faceAngle;
    }
	if (dangle < 2 * PI - changeFaceAngle && dangle > PI) {
        frontColChange(0);
        --faceAngle;
    }

    faceAngle %= 4;
    if (faceAngle < 0) faceAngle += 4;
}

float*Camera::getCamView(){
	return &View[0][0];
}
