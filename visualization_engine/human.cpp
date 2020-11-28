/*
	Author: Yida Wang
	Class: ECE 4122
	Last Date Modified: November 27th, 2020

	Description: This is the human class cpp file. This file contains
	drawing of a human.
*/
#include "human.h"

extern GLUquadricObj* quadratic;

static GLfloat colorRed[4] = { 1.0f, 0.137f, 0.0f, 1.0f };

/////////////////////////////////////////////////////////////
// default constructor
/////////////////////////////////////////////////////////////
human::human() :
	xPosition{ 0.0f }, yPosition{ 0.0f }, zPosition{ 0.0f }
{

}

void human::drawLeftLeg()
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(-2.0, 0.0f, 0.0f);
	gluCylinder(quadratic, 1.5f, 1.5f, 25.0f, 32, 32);
	glPopMatrix();
}

void human::drawRightLeg()
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(2.0f, 0.0f, 0.0f);
	gluCylinder(quadratic, 1.5f, 1.5f, 25.0f, 32, 32);
	glPopMatrix();
}

void human::drawBody()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);

	// front
	glVertex3f(-5.0, 2.5, 20.0);
	glVertex3f(-5.0, -2.5, 20.0);
	glVertex3f(5.0, -2.5, 20.0);
	glVertex3f(5.0, 2.5, 20.0);
	// back
	glVertex3f(5.0, 2.5, 0.0);
	glVertex3f(5.0, -2.5, 0.0);
	glVertex3f(-5.0, -2.5, 0.0);
	glVertex3f(-5.0, 2.5, 0.0);
	// right
	glVertex3f(5.0, -2.5, 20.0);
	glVertex3f(5.0, -2.5, 0.0);
	glVertex3f(5.0, 2.5, 0.0);
	glVertex3f(5.0, 2.5, 20.0);
	// left
	glVertex3f(-5.0, 2.5, 0.0);
	glVertex3f(-5.0, -2.5, 0.0);
	glVertex3f(-5.0, -2.5, 20.0);
	glVertex3f(-5.0, 2.5, 20.0);
	// top
	glVertex3f(-5.0, 2.5, 0.0);
	glVertex3f(-5.0, 2.5, 20.0);
	glVertex3f(5.0, 2.5, 20.0);
	glVertex3f(5.0, 2.5, 0.0);
	// bottom
	glVertex3f(-5.0, -2.5, 0.0);
	glVertex3f(-5.0, -2.5, 20.0);
	glVertex3f(5.0, -2.5, 20.0);
	glVertex3f(5.0, -2.5, 0.0);
	glEnd();
}
void human::drawNeck()
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	gluCylinder(quadratic, 1.0f, 1.0f, 2.5f, 32, 32);
	glPopMatrix();
}

void human::drawLeftArm()
{
	glPushMatrix();
	glTranslated(-5.0f, 0.0f, -1.5f);
	glutSolidSphere(2.5f, 100, 100);
	glRotatef(-150.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 1.5f, 1.5f, 10.0f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-10.0f, 0.0f, -9.83975f);
	glutSolidSphere(1.5f, 100, 100);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 1.5f, 1.5f, 10.0f, 32, 32);
	glPopMatrix();
}

void human::drawRightArm()
{
	glPushMatrix();
	glTranslated(5.0f, 0.0f, -1.5f);
	glutSolidSphere(2.5f, 100, 100);
	glRotatef(150.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 1.5f, 1.5f, 10.0f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(10.0f, 0.0f, -9.83975f);
	glutSolidSphere(1.5f, 100, 100);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 1.5f, 1.5f, 10.0f, 32, 32);
	glPopMatrix();
}

void human::drawHuman(std::vector<GLdouble> coordinate)
{
	// set material color and use diffuse and specular material
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorRed);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorRed);
	glPushMatrix();
	glTranslatef(coordinate[0], coordinate[1], 0.0f);
	drawLeftLeg();
	drawRightLeg();
	glTranslatef(0.0f, 0.0f, 25.0f);
	drawBody();
	glTranslatef(0.0f, 0.0f, 20.0f);
	drawNeck();
	drawLeftArm();
	drawRightArm();
	glTranslatef(0.0f, 0.0f, 5.0f);
	// draw head
	glutSolidSphere(3.0f, 100, 100);
	glPopMatrix();
}