#pragma once
#include <GL/glut.h>
#include <vector>

class car
{
public:
	// Constructor
	car();

	void drawCar(std::vector<GLdouble>);
private:
	// car's position
	GLfloat xPosition, yPosition, zPosition;
};

