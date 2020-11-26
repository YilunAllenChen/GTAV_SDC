#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "human.h"
#include <thread>

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

typedef int SOCKET;
#endif

#define PI 3.141592653589793238
#define PORT 62000
// function prototypes


//void init();
//void display();
//void changeSize();
//void keyboard(unsigned char, int, int);
//void changeSize(int, int);
//void timer(int);
//void drawGround();
// Window size
static GLsizei g_Width = 1000;
static GLsizei g_Height = 800;
// camera control variables
static GLfloat g_fieldOfView = 45.0f;
static GLfloat g_nearPlane = 0.1f;
static GLfloat g_farPlane = 100000.0f;
const GLfloat g_defaultCameraX = 0.000001f;
const GLfloat g_defaultCameraY = 800.0f;
const GLfloat g_defaultCameraZ = 400.0f;
static GLfloat g_cameraX = g_defaultCameraX;
static GLfloat g_cameraY = g_defaultCameraY;
static GLfloat g_cameraZ = g_defaultCameraZ;
const GLfloat g_defaultCameraLookAtX = 0.0f;
const GLfloat g_defaultCameraLookAtY = 0.0f;
const GLfloat g_defaultCameraLookAtZ = 0.0f;
static GLfloat g_cameraLookAtX = g_defaultCameraLookAtX;
static GLfloat g_cameraLookAtY = g_defaultCameraLookAtY;
static GLfloat g_cameraLookAtZ = g_defaultCameraLookAtZ;
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


static GLdouble selfLocation[2] = { 0.0f, 500.0f };

int state = 1;
static GLdouble testDistance = 500;
static GLdouble testAngle = 40;
// static GLdouble testLocation[2];
static GLdouble testLocation[2] = { selfLocation[0] - testDistance * sin(testAngle * PI / 180) , selfLocation[1] - testDistance * cos(testAngle * PI / 180) };


/////////////////////////////////////////////////
// Cross-platform socket initialize
int sockInit(void)
{
#ifdef _WIN32
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
	return 0;
#endif
}
/////////////////////////////////////////////////
// Cross-platform socket quit
int sockQuit(void)
{
#ifdef _WIN32
	return WSACleanup();
#else
	return 0;
#endif
}
/////////////////////////////////////////////////
// Cross-platform socket close
int sockClose(SOCKET sock)
{

	int status = 0;

#ifdef _WIN32
	status = shutdown(sock, SD_BOTH);
	if (status == 0)
	{
		status = closesocket(sock);
	}
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0)
	{
		status = close(sock);
	}
#endif

	return status;

}
/////////////////////////////////////////////////
// Output error message and exit
void error(const char* msg)
{
	perror(msg);
	// Make sure any open sockets are closed before calling exit
	exit(1);
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
	// lighting effects
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, colorOffWhite);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, colorOffWhite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, colorDarkWhite);
	glShadeModel(GL_SMOOTH);
}
int sockfd, newsockfd;

typedef struct ve_packet_t {
	uint32_t type;
	float distance;
	float angle;
} ve_packet;

void receivePack()
{
	int sockfd;
	ve_packet_t packet_buffer;
	
	struct sockaddr_in servaddr, cliaddr;
	sockInit();

	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		// int WSAGetLastError();
		printf("erro code: %d\n", WSAGetLastError());
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address 
	if (bind(sockfd, (const struct sockaddr*)&servaddr,
		sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len, n;


	len = sizeof(cliaddr);  //len is value/resuslt 
	while (1)
	{
		printf("size of ve_packet %d \n", sizeof(ve_packet));
		n = recvfrom(sockfd, (char*)&packet_buffer, 1023,
			0, (struct sockaddr*)&cliaddr,
			&len);
		printf("Packet Received: type: %d , distance: %f , angle %f\n", packet_buffer.type, packet_buffer.distance, packet_buffer.angle);
		testDistance = packet_buffer.distance;
		testAngle = packet_buffer.angle;
	}

	sockClose(sockfd);
	sockQuit();
}

void display()
{

	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// reset transformations
	glLoadIdentity();
	// set the camera center at (g_cameraX, g_cameraY, g_cameraZ) and looking
	// at the center of the maze, with the z-axis pointing up
	gluLookAt(g_cameraX, g_cameraY, g_cameraZ, g_cameraLookAtX, g_cameraLookAtY, g_cameraLookAtZ, 0.0, 0.0, 1.0);
	// drawAxis();
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, colorDarkWhite);
	// drawGround();
	human self;
	self.drawHuman(selfLocation);


	human player1;

	// testLocation[0] = { selfLocation[0] - testDistance * sin(testAngle * PI / 180) };
	// testLocation[1] = { selfLocation[1] - testDistance * cos(testAngle * PI / 180) };
	// testLocation[1] = 300.0f;
	testDistance = sqrt((testLocation[0] - selfLocation[0]) * (testLocation[0] - selfLocation[0]) + (testLocation[1] - selfLocation[1]) * (testLocation[1] - selfLocation[1]));

	if (testLocation[0] < 400 && testLocation[0] > -400)
	{
		player1.drawHuman(testLocation);
	}
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
	/*switch (state)
	{
	case 1:
		if (testLocation[0] < 500.0f)
		{
			testLocation[0] += 5.0f;
		}
		else
		{
			state = -1;
		}
		break;
	case -1:
		if (testLocation[0] > -500.0f)
		{
			testLocation[0] -= 5.0f;
		}
		else
		{
			state = 1;
		}
		break;
	default:
		break;
	}*/
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
// Press WASD to control the camera look at location.
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
		g_cameraLookAtX = g_defaultCameraLookAtX;
		g_cameraLookAtY = g_defaultCameraLookAtY;
		g_cameraLookAtZ = g_defaultCameraLookAtZ;
		ViewingAngle = atan(g_defaultCameraY / g_defaultCameraX) * 180.0 / PI;
		glutPostRedisplay();
		break;
	case 'w':
	case 'W':
		g_cameraLookAtZ += 10.0f;
		break;
	case 's':
	case 'S':
		g_cameraLookAtZ -= 10.0f;
		break;
	case 'a':
	case 'A':
		g_cameraLookAtX += 10.0f;
		break;
	case 'd':
	case 'D':
		g_cameraLookAtX -= 10.0f;
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
	default:
		break;
	}
}
void drawGround()
{
	glPushMatrix();
	// glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlue);
	// glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue);
	glColor3f(12.0f / 255.0f, 153.0f / 255.0f, 255.0f / 255.0f);
	glBegin(GL_QUADS);
	glVertex3f(-180.0, 200.0, 0.0);
	glVertex3f(-180.0, -220.0, 0.0);
	glVertex3f(180.0, -220.0, 0.0);
	glVertex3f(180.0, 200.0, 0.0);
	glEnd();
	glPopMatrix();
}


int main(int argc, char** argv)
{
	std::thread Receive(receivePack);
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

	printf("\n\
-----------------------------------------------------------------------\n\
  Welcome to the GTAV SDC:\n\
  - This program creates visualization scene of GTAV\n\
  - Press the r or R key to rotate the camera clockwise\n\
  - Press the e or E key to rotate it counterclockwise\n\
  - Press u and i to control camera x direction\n\
  - Press j and k to control camera y direction\n\
  - Press n and m to control camera z direction\n\
  - Press wasd or WASD to control camera look at location\n\
  - Press ESC, q, and Q to quit\n\
-----------------------------------------------------------------------\n");
	
	glutMainLoop();

	gluDeleteQuadric(quadratic);

	Receive.join();
	return 0;
}



