#include "Cubicle.hpp"

#define EPS 0.001

const float cubeScale = 1.0f / 3;

Cubicle::Cubicle(short i) : ID(i)
{
    calculateTrueCoordFromID();
	position = trueposition;
	Model = glm::translate(position.x*(1 - cubeScale), position.y*(1 - cubeScale), position.z*(1 - cubeScale))
        * glm::scale(cubeScale, cubeScale, cubeScale);
	timeSpined = timeDelay + 1; //extra 1 for security measures
}

void Cubicle::calculateTrueCoordFromID()
{
	trueposition = glm::vec3(float(ID % 3 - 1), float((ID / 9) - 1), float((ID / 3) % 3 - 1));
}

float* Cubicle::getModel() {
	return &Model[0][0];
}
glm::vec3 Cubicle::getPosition() {
	return position;
}

bool Cubicle::isOK() {
	return (position.x == trueposition.x) && (position.y == trueposition.y) && (position.z == trueposition.z);
}

void Cubicle::rotate(short times, cubicleColor face)
{
	short axis = short(face) / 2; // 0 = x, 1 = z, 2 = y
	short reverse = face % 2;
	if (reverse) times = 4 - times; // we spin axis and face in diferent directions

	glm::mat3 rotatePosition;
	
	switch (axis)
	{
		case 0: rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f); 
			rotatePosition = glm::mat3(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);
			break;
		case 1: rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
			rotatePosition = glm::mat3(0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			break;
		case 2: rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
			rotatePosition = glm::mat3(0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			break;
	}

	switch (times)
	{
		case 1:
			rotationAngle = 90.0f;
			position = rotatePosition * position;
			break;
		case 2:
			rotationAngle = -180.0f;
			position = rotatePosition * rotatePosition * position;
			break;
		case 3:
			rotationAngle = -90.0f;
			position = rotatePosition * rotatePosition * rotatePosition * position;
			break;
	}
	lastModel = Model;
	timeSpined = 0;
}

void Cubicle::rotateAnimation(float dt)
{
	if (timeSpined < timeDelay)
	{ 
		Model = glm::rotate(rotationAngle * dt / timeDelay, rotationAxis) * Model;
		timeSpined += dt;

		if (timeSpined >= timeDelay - EPS)
		{
			rotateFinished();
		}
	}
}

void Cubicle::rotateFinished()
{
	Model = glm::rotate(rotationAngle, rotationAxis) * lastModel;
}