#include "human.h"

extern GLUquadricObj* quadratic;

static GLfloat colorRed[4] = { 1.0f, 0.137f, 0.0f, 1.0f };

human::human() :
	xPosition{ 0.0f }, yPosition{ 0.0f }, zPosition{ 0.0f }
{

}

void human::drawLeftLeg()
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(-4.0, 0.0f, 0.0f);
	gluCylinder(quadratic, 3.0f, 3.0f, 50.0f, 32, 32);
	glPopMatrix();
}

void human::drawRightLeg()
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(4.0f, 0.0f, 0.0f);
	gluCylinder(quadratic, 3.0f, 3.0f, 50.0f, 32, 32);
	glPopMatrix();
}

void human::drawBody()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);

	// front
	// glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-10.0, 5.0, 40.0);
	glVertex3f(-10.0, -5.0, 40.0);
	glVertex3f(10.0, -5.0, 40.0);
	glVertex3f(10.0, 5.0, 40.0);
	// back
	// glColor3f(0.0, 1.0, 0.0);
	glVertex3f(10.0, 5.0, 0.0);
	glVertex3f(10.0, -5.0, 0.0);
	glVertex3f(-10.0, -5.0, 0.0);
	glVertex3f(-10.0, 5.0, 0.0);
	// right
	// glColor3f(0.0, 0.0, 1.0);
	glVertex3f(10.0, -5.0, 40.0);
	glVertex3f(10.0, -5.0, 0.0);
	glVertex3f(10.0, 5.0, 0.0);
	glVertex3f(10.0, 5.0, 40.0);
	// left
	// glColor3f(1.0, 1.0, 0.0);
	glVertex3f(-10.0, 5.0, 0.0);
	glVertex3f(-10.0, -5.0, 0.0);
	glVertex3f(-10.0, -5.0, 40.0);
	glVertex3f(-10.0, 5.0, 40.0);
	// top
	// glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-10.0, 5.0, 0.0);
	glVertex3f(-10.0, 5.0, 40.0);
	glVertex3f(10.0, 5.0, 40.0);
	glVertex3f(10.0, 5.0, 0.0);
	// bottom
	// glColor3f(1.0, 0.0, 1.0);
	glVertex3f(-10.0, -5.0, 0.0);
	glVertex3f(-10.0, -5.0, 40.0);
	glVertex3f(10.0, -5.0, 40.0);
	glVertex3f(10.0, -5.0, 0.0);
	glEnd();
}
void human::drawNeck()
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	gluCylinder(quadratic, 2.0f, 2.0f, 5.0f, 32, 32);
	glPopMatrix();
}

void human::drawLeftArm()
{
	glPushMatrix();
	glTranslated(-10.0f, 0.0f, -3.0f);
	glutSolidSphere(5.0f, 100, 100);
	glRotatef(-150.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 3.0f, 3.0f, 20.0f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-20.0f, 0.0f, -19.6795f);
	glutSolidSphere(3.0f, 100, 100);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 3.0f, 3.0f, 20.0f, 32, 32);
	glPopMatrix();
}

void human::drawRightArm()
{
	glPushMatrix();
	glTranslated(10.0f, 0.0f, -3.0f);
	glutSolidSphere(5.0f, 100, 100);
	glRotatef(150.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 3.0f, 3.0f, 20.0f, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(20.0f, 0.0f, -19.6795f);
	glutSolidSphere(3.0f, 100, 100);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 3.0f, 3.0f, 20.0f, 32, 32);
	glPopMatrix();
}

void human::drawHuman(GLdouble* coordinate)
{
	// set material color and use diffuse and specular material
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorRed);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colorRed);
	glPushMatrix();
	glTranslatef(coordinate[0], coordinate[1], 0.0f);
	drawLeftLeg();
	drawRightLeg();
	glTranslatef(0.0f, 0.0f, 50.0f);
	drawBody();
	glTranslatef(0.0f, 0.0f, 40.0f);
	drawNeck();
	drawLeftArm();
	drawRightArm();
	glTranslatef(0.0f, 0.0f, 10.0f);
	// draw head
	glutSolidSphere(6.0f, 100, 100);
	glPopMatrix();
}