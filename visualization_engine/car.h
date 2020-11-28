/*
	Author: Yida Wang
	Class: ECE 4122
	Last Date Modified: November 27th, 2020

	Description: This is the header file of the car class
*/
#pragma once
#include <GL/glut.h>
#include <vector>

class car
{
public:
	// Constructor
	car();
	void drawTopPart();
	void drawBottomPart();
	void drawWheels();
	void drawCar(std::vector<GLdouble>);
private:
	// car's position
	GLfloat xPosition, yPosition, zPosition;
};

