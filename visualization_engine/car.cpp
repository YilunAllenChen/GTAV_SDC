#include "car.h"
extern GLUquadricObj* quadratic;

static GLfloat colorBlue[4] = { 12.0f / 255.0f, 153.0f / 255.0f, 255.0f / 255.0f, 1.0f };
static GLfloat colorGreen[4] = { 0.157f, 0.784f, 0.667f, 1.0f };
static GLfloat colorPink[4] = { 1.0f, 0.737f, 0.894f, 1.0f };
static GLfloat colorRed[4] = { 1.0f, 0.137f, 0.0f, 1.0f };
static GLfloat colorYellow[4] = { 1.0f, 1.0f, 0.06f, 1.0f };

car::car() :
	xPosition{ 0.0f }, yPosition{ 0.0f }, zPosition{ 0.0f }
{

}

void car::drawCar(std::vector<GLdouble> coordinate)
{

    glPushMatrix();
    // glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(coordinate[0], coordinate[1], 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue);
    /*glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);
    glVertex3f(40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, 15.0f);*/
    // left side of car
    glVertex3f(-120.0f, 0.0f, 45.0f);
    glVertex3f(120.0f, 0.0f, 45.0f);
    glVertex3f(120.0f, -45.0f, 45.0f);
    glVertex3f(-120.0f, -45.0f, 45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    
    /*glVertex3f(40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);
    glVertex3f(40.0f, -15.0f, 15.0f);
    glVertex3f(40.0f, -15.0f, -15.0f);*/
    // back of car
    glVertex3f(120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, 0.0f, 45.0f);
    glVertex3f(120.0f, -45.0f, 45.0f);
    glVertex3f(120.0f, -45.0f, -45.0f); 
    glEnd();
    glBegin(GL_POLYGON);
    /*glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);*/
    // front of car
    glVertex3f(-120.0f, 0.0f, -45.0f);
    glVertex3f(-120.0f, 0.0f, 45.0f);
    glVertex3f(-120.0f, -45.0f, 45.0f);
    glVertex3f(-120.0f, -45.0f, -45.0f);
    glEnd();
    glBegin(GL_POLYGON);
    /*glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, -15.0f, -15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);*/
    // right side of car
    glVertex3f(-120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, -45.0f, -45.0f);
    glVertex3f(-120.0f, -45.0f, -45.0f);
    glEnd();
    glBegin(GL_POLYGON);
   /* glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);*/
    // top outter of car
    glVertex3f(-120.0f, 0.0f, 45.0f);
    glVertex3f(-120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, 0.0f, -45.0f);
    glVertex3f(120.0f, 0.0f, 45.0f);
    glEnd();
    glBegin(GL_POLYGON);
    /*glVertex3f(-40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);
    glVertex3f(40.0f, -15.0f, -15.0f);
    glVertex3f(40.0f, -15.0f, 15.0f);*/
    // bottom of car
    glVertex3f(-120.0f, -45.0f, 45.0f);
    glVertex3f(-120.0f, -45.0f, -45.0f);
    glVertex3f(120.0f, -45.0f, -45.0f);
    glVertex3f(120.0f, -45.0f, 45.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorYellow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorYellow);
    /*glVertex3f(-20.0f, 0.0f, 15.0f);
    glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(20.0f, 10.0f, 15.0f);
    glVertex3f(25.0f, 0.0f, 15.0f);*/
    // left car window
    glVertex3f(-60.0f, 0.0f, 45.0f);
    glVertex3f(-30.0f, 30.0f, 45.0f);
    glVertex3f(60.0f, 30.0f, 45.0f);
    glVertex3f(75.0f, 0.0f, 45.0f);
    glEnd();
    glBegin(GL_POLYGON);
    /*glVertex3f(-20.0f, 0.0f, -15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, -15.0f);*/
    // right car window
    glVertex3f(-60.0f, 0.0f, -45.0f);
    glVertex3f(-30.0f, 30.0f, -45.0f);
    glVertex3f(60.0f, 30.0f, -45.0f);
    glVertex3f(75.0f, 0.0f, -45.0f);
    glEnd();
    glBegin(GL_POLYGON);
    /*glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, 15.0f);*/
    // top of the car
    glVertex3f(-30.0f, 30.0f, 45.0f);
    glVertex3f(-30.0f, 30.0f, -45.0f);
    glVertex3f(60.0f, 30.0f, -45.0f);
    glVertex3f(60.0f, 30.0f, 45.0f);
    glEnd();
    glBegin(GL_POLYGON);
    /*glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(-20.0f, 0.0f, 15.0f);
    glVertex3f(-20.0f, 0.0f, -15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);*/
    // front window
    glVertex3f(-30.0f, 30.0f, 45.0f);
    glVertex3f(-60.0f, 0.0f, 45.0f);
    glVertex3f(-60.0f, 0.0f, -45.0f);
    glVertex3f(-30.0f, 30.0f, -45.0f);
    glEnd();
    glBegin(GL_POLYGON);
    /*glVertex3f(20.0f, 10.0f, 15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, 15.0f);*/
    // back window
    glVertex3f(60.0f, 30.0f, 45.0f);
    glVertex3f(60.0f, 30.0f, -45.0f);
    glVertex3f(75.0f, 0.0f, -45.0f);
    glVertex3f(75.0f, 0.0f, 45.0f);
    glEnd();
    glBegin(GL_POLYGON);
    /*glVertex3f(-30.0f, -15.0f, 15.0f);
    glVertex3f(-30.0f, -15.0f, -15.0f);
    glVertex3f(30.0f, -15.0f, -15.0f);
    glVertex3f(30.0f, -15.0f, 15.0f);*/
    glVertex3f(-90.0f, -45.0f, 45.0f);
    glVertex3f(-90.0f, -45.0f, -45.0f);
    glVertex3f(90.0f, -45.0f, -45.0f);
    glVertex3f(90.0f, -45.0f, 45.0f);
    glEnd();


    glBegin(GL_3D);
    //glTranslated(-20.0f, -15.0f, 15.0f);
    glTranslated(-60.0f, -45.0f, 45.0f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorPink);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorPink);
    //glutSolidTorus(2, 5, 5, 100);
    glutSolidTorus(6, 15, 5, 100);




    glTranslated(0.0f, 0.0f, -90.0f);
    glutSolidTorus(6, 15, 5, 100);
    glTranslated(135.0f, 0.0f, 0.0f);
    glutSolidTorus(6, 15, 5, 100);
    glTranslated(0.0f, 0.0f, 90.0f);
    glutSolidTorus(6, 15, 5, 100);
    glEnd();

    glPopMatrix();
}