/*
    Author: Yida Wang
    Class: ECE 4122
    Last Date Modified: November 27th, 2020

    Description: This is the car class cpp file. This file contains 
    drawing of a car.
*/
#include "car.h"

extern GLUquadricObj* quadratic;

static GLfloat colorBlue[4] = { 12.0f / 255.0f, 153.0f / 255.0f, 255.0f / 255.0f, 1.0f };
static GLfloat colorGreen[4] = { 0.157f, 0.784f, 0.667f, 1.0f };
static GLfloat colorPink[4] = { 1.0f, 0.737f, 0.894f, 1.0f };
static GLfloat colorRed[4] = { 1.0f, 0.137f, 0.0f, 1.0f };
static GLfloat colorYellow[4] = { 1.0f, 1.0f, 0.06f, 1.0f };

/////////////////////////////////////////////////////////////
// default constructor
/////////////////////////////////////////////////////////////
car::car() :
	xPosition{ 0.0f }, yPosition{ 0.0f }, zPosition{ 0.0f }
{

}

void car::drawTopPart()
{
    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorYellow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorYellow);
    // left car window
    glVertex3f(-60.0f, 0.0f, 45.0f);
    glVertex3f(-30.0f, 30.0f, 45.0f);
    glVertex3f(60.0f, 30.0f, 45.0f);
    glVertex3f(75.0f, 0.0f, 45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // right car window
    glVertex3f(-60.0f, 0.0f, -45.0f);
    glVertex3f(-30.0f, 30.0f, -45.0f);
    glVertex3f(60.0f, 30.0f, -45.0f);
    glVertex3f(75.0f, 0.0f, -45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // top of the car
    glVertex3f(-30.0f, 30.0f, 45.0f);
    glVertex3f(-30.0f, 30.0f, -45.0f);
    glVertex3f(60.0f, 30.0f, -45.0f);
    glVertex3f(60.0f, 30.0f, 45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // front window
    glVertex3f(-30.0f, 30.0f, 45.0f);
    glVertex3f(-60.0f, 0.0f, 45.0f);
    glVertex3f(-60.0f, 0.0f, -45.0f);
    glVertex3f(-30.0f, 30.0f, -45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // back window
    glVertex3f(60.0f, 30.0f, 45.0f);
    glVertex3f(60.0f, 30.0f, -45.0f);
    glVertex3f(75.0f, 0.0f, -45.0f);
    glVertex3f(75.0f, 0.0f, 45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-90.0f, -45.0f, 45.0f);
    glVertex3f(-90.0f, -45.0f, -45.0f);
    glVertex3f(90.0f, -45.0f, -45.0f);
    glVertex3f(90.0f, -45.0f, 45.0f);
    glEnd();
}

void car::drawBottomPart()
{
    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue);
    // left side of car
    glVertex3f(-120.0f, 0.0f, 45.0f);
    glVertex3f(120.0f, 0.0f, 45.0f);
    glVertex3f(120.0f, -45.0f, 45.0f);
    glVertex3f(-120.0f, -45.0f, 45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // back of car
    glVertex3f(120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, 0.0f, 45.0f);
    glVertex3f(120.0f, -45.0f, 45.0f);
    glVertex3f(120.0f, -45.0f, -45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // front of car
    glVertex3f(-120.0f, 0.0f, -45.0f);
    glVertex3f(-120.0f, 0.0f, 45.0f);
    glVertex3f(-120.0f, -45.0f, 45.0f);
    glVertex3f(-120.0f, -45.0f, -45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // right side of car
    glVertex3f(-120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, -45.0f, -45.0f);
    glVertex3f(-120.0f, -45.0f, -45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // top outter of car
    glVertex3f(-120.0f, 0.0f, 45.0f);
    glVertex3f(-120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, 0.0f, 45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    // bottom of car
    glVertex3f(-120.0f, -45.0f, 45.0f);
    glVertex3f(-120.0f, -45.0f, -45.0f);
    glVertex3f(120.0f, -45.0f, -45.0f);
    glVertex3f(120.0f, -45.0f, 45.0f);
    glEnd();
}

void car::drawWheels()
{
    // draw wheels
    glBegin(GL_3D);
    glTranslated(-60.0f, -45.0f, 45.0f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorPink);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorPink);
    glutSolidTorus(6, 15, 5, 100);
    glTranslated(0.0f, 0.0f, -90.0f);
    glutSolidTorus(6, 15, 5, 100);
    glTranslated(135.0f, 0.0f, 0.0f);
    glutSolidTorus(6, 15, 5, 100);
    glTranslated(0.0f, 0.0f, 90.0f);
    glutSolidTorus(6, 15, 5, 100);
    glEnd();
}

void car::drawCar(std::vector<GLdouble> coordinate)
{
    glPushMatrix();
    glTranslatef(coordinate[0], coordinate[1], 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawTopPart();
    drawBottomPart();
    drawWheels();
    glPopMatrix();
}