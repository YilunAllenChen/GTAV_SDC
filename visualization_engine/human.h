/*
	Author: Yida Wang
	Class: ECE 4122
	Last Date Modified: November 27th, 2020

	Description: This is the header file of the human class
*/
#pragma once
#include <GL/glut.h>
#include <vector>
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
	void drawHuman(std::vector<GLdouble>);
private:
	// human's position
	GLfloat xPosition, yPosition, zPosition;
};

