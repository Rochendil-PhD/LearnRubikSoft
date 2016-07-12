#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "..\lib\glew\glew.h"
#include "..\lib\freeglut\freeglut.h"
#include "..\lib\glfw\glfw3.h"
#include "..\lib\glm\glm.hpp"
#include "..\lib\glm\gtc\matrix_transform.hpp"
#include "..\lib\glm\gtc\constants.hpp"

#include "Cubicle.hpp"
#include "Common.hpp"

class Camera
{
private:
	static float alpha;
	static short faceAngle; // we don't hold angle, we calculate it. only 4 posibilities, short is safer
	static float height;
	static glm::vec3 position;
	static glm::vec3 upDirection;
	static glm::mat4 View;
	static void updateView();
	static void calcFace();
public:
	static void init();
	enum cameraMove {cmLEFT, cmRIGHT, cmUP, cmDOWN};
	static void moveCamera(cameraMove direction, float dt);
	static float* getCamView();

	static float* setFace();
	static float* getFace();
};
#endif // _CAMERA_HPP_
