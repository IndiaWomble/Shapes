#include "Angel.h"
#include <vector>
#include <iostream>

vec2 worldpoint(int x, int y);
std::vector<vec2> mousepoints;
int value = 0;
GLuint buffer;
GLint windowWidth, windowHeight;
int count = 0;
vec2 vertices[20];

void prepareArray()
{
	for (int j = 0; j < mousepoints.size(); j++)
	{
		vertices[j].x = mousepoints[j].x;
		vertices[j].y = mousepoints[j].y;
	}
}

void drawcircle()
{
	float x1, y1, x2, y2;
	float angle;
	double radius = 0.1;
	x1 = 0.5, y1 = 0.6;
	glColor3f(0.5, 0.5, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);
	for (angle = 1.0f; angle < 361.0f; angle = angle + 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}
}

void display()
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	prepareArray();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (value)
	{
	case 1: glDrawArrays(GL_TRIANGLES, 0, count);
		break;
	case 2:  glDrawArrays(GL_LINES, 0, count);
		break;
	case 3: drawcircle();
		break;
	}
	glEnd();
	glFlush();
}

void idle()
{
	glutPostRedisplay();
}

void mykey(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q')
		exit(0);
}

vec2 worldpoint(int x, int y)
{
	vec2 pos;
	float posx = (x / 640.0 * 2) - 1;
	float posy = ((640 - y) / 640.0 * 2) - 1;
	pos.x = posx;
	pos.y = posy;
	return pos;
}

void Init()
{
	GLuint abuffer;
	glGenVertexArrays(1, &abuffer);
	glBindVertexArray(abuffer);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLuint program;
	program = InitShader("vshader21.glsl", "fshader21.glsl");
	GLuint loc;
	loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glClearColor(0.5, 0.0, 1.45, 0.0);
}

void menu(int num)
{
	value = num;
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		vertices[count] = worldpoint(x, y);
		count++;
	}
}

int main(int ac, char** av)
{
	glutInit(&ac, av);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(150, 250);
	glutCreateWindow("Assignment 2");
	glutInitDisplayMode(GLUT_RGB);
	glewInit();
	Init();
	glutCreateMenu(menu);
	glutAddMenuEntry("Triangles", 1);
	glutAddMenuEntry("Lines", 2);
	glutAddMenuEntry("Circle", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(mykey);
	glutDisplayFunc(display);
	glutMainLoop();
}
