#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "human.h"
#define PI 3.141592653589793238


// Window size
static GLsizei g_Width = 1000;
static GLsizei g_Height = 800;
// camera control variables
static GLfloat g_fieldOfView = 45.0f;
static GLfloat g_nearPlane = 0.1f;
static GLfloat g_farPlane = 100000.0f;
const GLfloat g_defaultCameraX = 282.84f;
const GLfloat g_defaultCameraY = -489.90f;
const GLfloat g_defaultCameraZ = 300.0f;
static GLfloat g_cameraX = g_defaultCameraX;
static GLfloat g_cameraY = g_defaultCameraY;
static GLfloat g_cameraZ = g_defaultCameraZ;
static GLdouble g_cameraXYDistance = sqrt((GLdouble)g_cameraX * (GLdouble)g_cameraX + (GLdouble)g_cameraY * (GLdouble)g_cameraY);
// degree in radian
static GLdouble ViewingAngle = atan(g_defaultCameraY / g_defaultCameraX) * 180.0 / PI;
static GLfloat light_position[] = { 0.0f, -100.0f, 200.0f, 0.0f };

// colors for lighting effects
static GLfloat colorDarkWhite[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
// static GLfloat colorOffWhite[4] = { 1.0f, 248.0f / 255.0f, 220.0f / 255.0f, 1.0f };
static GLfloat colorOffWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
// static GLfloat colorWhite[4] = { .60, 0.60, 0.60, 1.0f };
// static GLfloat colorBlack[4] = { 0.01, 0.01, 0.01, 1.0f };
static GLfloat colorBlue[4] = { 12.0f / 255.0f, 153.0f / 255.0f, 255.0f / 255.0f, 1.0f };
static GLfloat colorGreen[4] = { 0.157f, 0.784f, 0.667f, 1.0f };
static GLfloat colorPink[4] = { 1.0f, 0.737f, 0.894f, 1.0f };
static GLfloat colorRed[4] = { 1.0f, 0.137f, 0.0f, 1.0f };
static GLfloat colorOrange[4] = { 1.0f, 0.745f, 0.05f, 1.0f };
static GLfloat colorSilver[4] = { 0.75f, 0.75f, 0.75f, 1.0f };
// define shininess for reflection
GLfloat shininess[] = { 5 };
// quadratic for drawing sphere and cylinder
GLUquadricObj* quadratic{ nullptr };

// function prototypes
void init();
void display();
void changeSize();
void keyboard(unsigned char, int, int);
void changeSize(int, int);
void timer(int);


int main(int argc, char** argv)
{
	printf("\n\
-----------------------------------------------------------------------\n\
  Welcome to the GTAV SDC:\n\
  - This program creates visualization scene of GTAV\n\
  - Press the r or R key to rotate the camera clockwise\n\
  - Press the e or E key to rotate it counterclockwise\n\
  - Press u and i to control camera x direction\n\
  - Press j and k to control camera y direction\n\
  - Press n and m to control camera z direction\n\
  - Press ESC, q, and Q to quit\n\
-----------------------------------------------------------------------\n");
	// general initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutCreateWindow("GTAV SDC");
	init();
	// register callbacks
	glutDisplayFunc(display);	// display callback
	glutReshapeFunc(changeSize);	// window reshape callback
	glutKeyboardFunc(keyboard);		// keyboard callback
	glutTimerFunc(0, timer, 0);		// timer callback

	// enter GLUT event processing cycle
	glutMainLoop();
	gluDeleteQuadric(quadratic);
	return 0;
}

void init()
{
	// set the background to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// enable depth test to hide the object behind other object
	glEnable(GL_DEPTH_TEST);
	if (!quadratic)
	{
		quadratic = gluNewQuadric();
	}
	glShadeModel(GL_SMOOTH);

}

void display()
{
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// reset transformations
	glLoadIdentity();
	// set the camera center at (g_cameraX, g_cameraY, g_cameraZ) and looking
	// at the center of the maze, with the z-axis pointing up
	gluLookAt(g_cameraX, g_cameraY, g_cameraZ, 0.0, 0.0, -50.0, 0.0, 0.0, 1.0);
	// drawAxis();
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-180.0, 200.0, 0.0);
	glVertex3f(-180.0, -220.0, 0.0);
	glVertex3f(180.0, -220.0, 0.0);
	glVertex3f(180.0, 200.0, 0.0);
	glEnd();
	human player1;
	player1.drawHuman(0.0f, 0.0f);

	//for (float i = -3; i < 3; i++)
	//{
	//	for (float j = -3; j < 3; j++)
	//	{
	//		glPushMatrix();
	//		// glTranslatef(i * 7.5, j * 7.5, 0);
	//		player1.drawHuman(i * 75, j * 75);
	//		glPopMatrix();
	//	}
	//}
	glutSwapBuffers();
}

void changeSize(int w, int h)
{
	float ratio = ((float)w / (float)h);	// window aspect ratio
	glMatrixMode(GL_PROJECTION);		// projection matrix is active
	glLoadIdentity();	// reset the projection
	gluPerspective(g_fieldOfView, ratio, g_nearPlane, g_farPlane);	// perspective transformation
	glMatrixMode(GL_MODELVIEW);	// return to modelview mode
	glViewport(0, 0, w, h);	// set viewport to entire window
}
//----------------------------------------------------------------------
// Update with each idle event
// display function is been call 60 times in 1 second
// frame rate is 60 frames per second
//
//----------------------------------------------------------------------
void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}

//----------------------------------------------------------------------
// User-input callbacks
//
// processNormalKeys: ESC, q, and Q cause program to exit
// pressSpecialKey: r or R = camera clockwise rotation, e or E = camera 
// counter clockwise rotation
// Press z or Z to reset back to the starting camera location
// Press u and i to control camera x direction
// Press j and k to control camera y direction
// Press n and m to control camera z direction
// releaseSpecialKey: Set incremental motion to zero
//----------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
	case 'R':
		ViewingAngle += 5.0;
		g_cameraXYDistance = sqrt((GLdouble)g_cameraX * (GLdouble)g_cameraX + (GLdouble)g_cameraY * (GLdouble)g_cameraY);
		g_cameraX = (GLfloat)g_cameraXYDistance * (GLfloat)cos(ViewingAngle * PI / 180.0);
		g_cameraY = (GLfloat)g_cameraXYDistance * (GLfloat)sin(ViewingAngle * PI / 180.0);
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		ViewingAngle -= 5.0;
		g_cameraXYDistance = sqrt((GLdouble)g_cameraX * (GLdouble)g_cameraX + (GLdouble)g_cameraY * (GLdouble)g_cameraY);
		g_cameraX = (GLfloat)g_cameraXYDistance * (GLfloat)cos(ViewingAngle * PI / 180.0);
		g_cameraY = (GLfloat)g_cameraXYDistance * (GLfloat)sin(ViewingAngle * PI / 180.0);
		glutPostRedisplay();
		break;
	case 27:
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'z':
	case 'Z':
		g_cameraX = g_defaultCameraX;
		g_cameraY = g_defaultCameraY;
		g_cameraZ = g_defaultCameraZ;
		ViewingAngle = atan(g_defaultCameraY / g_defaultCameraX) * 180.0 / PI;
		glutPostRedisplay();
		break;
	case 'w':
		g_cameraX -= 10.0f;
		g_cameraY += 10.0f;
		g_cameraZ -= 10.0f;
		break;
	case 'u':
		g_cameraX -= 10.0f;
		break;
	case 'i':
		g_cameraX += 10.0f;
		break;
	case 'j':
		g_cameraY += 10.0f;
		break;
	case 'k':
		g_cameraY -= 10.0f;
		break;
	case 'n':
		g_cameraZ -= 10.0f;
		break;
	case 'm':
		g_cameraZ += 10.0f;
		break;
	case 's':
		g_cameraX += 10.0f;
		g_cameraY -= 10.0f;
		g_cameraZ += 10.0f;
		break;
	default:
		break;
	}
}