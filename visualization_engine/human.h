#pragma once
#include <GL/glut.h>

class human
{
public:
	// Constructor
	human();
	void drawLeftLeg();
	void drawRightLeg();
	void drawBody();
	void drawNeck();
	void drawLeftArm();
	void drawRightArm();
	void drawHuman(GLdouble*);
private:
	// human's position
	GLfloat xPosition, yPosition, zPosition;
};

