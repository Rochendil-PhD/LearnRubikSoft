#ifndef _CUBICLE_HPP_
#define _CUBICLE_HPP_

#include "..\lib\glew\glew.h"
#include "..\lib\freeglut\freeglut.h"
#include "..\lib\glfw\glfw3.h"
#include "..\lib\glm\glm.hpp"
#include "..\lib\glm\gtc\matrix_transform.hpp"
#include "..\lib\glm\gtc\constants.hpp"
#include "..\lib\glm\gtx\transform.hpp"

#include "Common.hpp"

const float timeDelay = 0.25f;

class Cubicle
{
private:
    short ID;
	void calculateTrueCoordFromID();

    //position and cube rotations
	glm::mat4 Model;
	glm::vec3 trueposition;
	glm::vec3 position;

	//this is for the spin animation
	float timeSpined;
	glm::vec3 rotationAxis;
	float rotationAngle;
	glm::mat4 lastModel;
    // this is to quickly calculate a cubicle curent rotation
    //glm::vec3 upVector; //x =
public:
    Cubicle(short ID);
	void rotate(short, cubicleColor);
	void rotateAnimation(float);
	void rotateFinished();
	float* getModel();
	glm::vec3 getPosition();
	bool isOK();
};

#endif // _CUBICLE_HPP_
