// CS3241Lab1.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>
#include "GL\glut.h"

#define MAXPTNO 1000
#define NLINESEGMENT 32
#define NOBJECTONCURVE 8

using namespace std;

// Global variables that you can use
struct Point {
	int x,y;
};

// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];
Point ptC1List[MAXPTNO];

// Display options
bool displayControlPoints = true;
bool displayControlLines = true;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;
bool displayBezierCurve = true;
	
void drawRightArrow()
{
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0,0);
		glVertex2f(100,0);
		glVertex2f(95,5);
		glVertex2f(100,0);
		glVertex2f(95,-5);
	glEnd();
}

void drawRectangle() {
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
		glVertex2f(-10,5);
		glVertex2f(-10,-5);
		glVertex2f(10, -5);
		glVertex2f(10,5);
	glEnd();
}

void drawBezierCurve() {
	for (int i = 0; i < nPt; i +=3) {
		if (i + 3 >= nPt) {
			break;
		}

		Point pt1 = ptList[i];
		Point pt2 = ptList[i + 1];
		Point pt3 = ptList[i + 2];
		Point pt4 = ptList[i + 3];

		if (i - 2 > 0 && C1Continuity) {
			Point oldpt3 = ptList[i - 1];

			pt2.x = 2*pt1.x - oldpt3.x;
			pt2.y = 2*pt1.y - oldpt3.y;

			ptC1List[i + 1] = pt2; 
		}

		
			glBegin(GL_LINE_STRIP);
				glColor3f(0, 0, 1);
				for (int j = 0; j < NLINESEGMENT + 1; j++) {
					float t = j/32.0;
					float bzX = pow((1 - t), 3)*pt1.x + 3*pow((1 - t), 2)*pow(t, 1)*pt2.x + 3*pow((1 - t), 1)*pow(t, 2)*pt3.x + pow(t, 3)*pt4.x;
					float bzY = pow((1 - t), 3)*pt1.y + 3*pow((1 - t), 2)*pow(t, 1)*pt2.y + 3*pow((1 - t), 1)*pow(t, 2)*pt3.y + pow(t, 3)*pt4.y;
					glVertex2d(bzX, bzY);
				}
			glEnd();
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	if(displayBezierCurve){
		drawBezierCurve();
	}

	if(displayControlPoints) {

		glPointSize(5);
		glBegin(GL_POINTS);
		for(int i=0;i<nPt; i++) {
			if (C1Continuity) {
				if (i > 3 && i%3 == 1) {
					glColor3f(1,0,0);
					glVertex2d(ptC1List[i].x,ptC1List[i].y);
					glColor3f(0.7,0.7,0.7);
					glVertex2d(ptList[i].x,ptList[i].y);
				} else {
					glColor3f(0,0,0);
					glVertex2d(ptList[i].x,ptList[i].y);
				}
			} else {
				glColor3f(0,0,0);
				glVertex2d(ptList[i].x,ptList[i].y);
			}

		}
		glEnd();
		glPointSize(1);

	}

	if(displayControlLines) {
		glBegin(GL_LINE_STRIP);
		for(int i=0;i<nPt; i++)
		{
			if (C1Continuity) {
				glColor3f(0,1,0);
				glVertex2d(ptC1List[i].x,ptC1List[i].y);
			} else {
				glColor3f(0,1,0);
				glVertex2d(ptList[i].x,ptList[i].y);
			}
		}
		glEnd();
	}

	for(int i = 3; i < nPt; i += 3) {
		for(float j = 0; j < NOBJECTONCURVE+1; j++) {

			Point pt4 = ptList[i];
			Point pt3 = ptList[i - 1];
			Point pt2 = ptList[i - 2];
			Point pt1 = ptList[i - 3];

			if (C1Continuity) {
				pt4 = ptC1List[i];
				pt3 = ptC1List[i - 1];
				pt2 = ptC1List[i - 2];
				pt1 = ptC1List[i - 3];
			}

			float t = j/NOBJECTONCURVE;

			float bzX = pow((1 - t), 3)*pt1.x + 3*pow((1 - t), 2)*pow(t, 1)*pt2.x + 3*pow((1 - t), 1)*pow(t, 2)*pt3.x + pow(t, 3)*pt4.x;
			float bzY = pow((1 - t), 3)*pt1.y + 3*pow((1 - t), 2)*pow(t, 1)*pt2.y + 3*pow((1 - t), 1)*pow(t, 2)*pt3.y + pow(t, 3)*pt4.y;

			float tx = 3*((pt2.x - pt1.x) + (t*((2*pt3.x) - (4*pt2.x) + 2*pt1.x)) + (pow(t, 2)*(pt4.x - (3*pt3.x) + (3*pt2.x) - pt1.x)));
			float ty = 3*((pt2.y - pt1.y) + (t*((2*pt3.y)-(4*pt2.y) + (2*pt1.y))) + (pow(t, 2)*(pt4.y - (3*pt3.y) + (3*pt2.y) - pt1.y)));

			glPushMatrix();

			float angle = 180*atan2(ty, tx)/3.142;
			glTranslatef(bzX,bzY,0);
			glRotatef(angle,0,0,1);

			if (displayTangentVectors) {
				drawRightArrow();
			}

			if (displayObjects) {
				drawRectangle();
			}

			glPopMatrix();
		}
	}

	glPopMatrix();
	glutSwapBuffers ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile()
{

	std::ifstream file;
    file.open("savefile.txt");
	file >> nPt;

	if(nPt>MAXPTNO)
	{
		cout << "Error: File contains more than the maximum number of points." << endl;
		nPt = MAXPTNO;
	}

	for(int i=0;i<nPt;i++)
	{
		file >> ptList[i].x;
		file >> ptList[i].y;
		file >> ptC1List[i].x;
		file >> ptC1List[i].y;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void writeFile()
{
	std::ofstream file;
    file.open("savefile.txt");
    file << nPt << endl;

	for(int i=0;i<nPt;i++)
	{
		file << ptList[i].x << " ";
		file << ptList[i].y << endl;
		file << ptC1List[i].x << " ";
		file << ptC1List[i].y << endl;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
		case 'R':
			readFile();
		break;

		case 'w':
		case 'W':
			writeFile();
		break;

		case 'T':
		case 't':
			displayTangentVectors = !displayTangentVectors;
		break;

		case 'o':
		case 'O':
			displayObjects = !displayObjects;
		break;

		case 'p':
		case 'P':
			displayControlPoints = !displayControlPoints;
		break;

		case 'L':
		case 'l':
			displayControlLines = !displayControlLines;
		break;

		case 'C':
		case 'c':
			C1Continuity = !C1Continuity;
		break;

		case 'e':
		case 'E':
			// Do something to erase all the control points added
			nPt = 0;
		break;

		case 'Q':
		case 'q':
			exit(0);
		break;

		case 'B':
		case 'b':
			displayBezierCurve = !displayBezierCurve;

		default:
		break;
	}

	glutPostRedisplay();
}



void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_LEFT_BUTTON)&&(state == GLUT_UP))
	{
		if(nPt==MAXPTNO)
		{
			cout << "Error: Exceeded the maximum number of points." << endl;
			return;
		}
		ptList[nPt].x=x;
		ptList[nPt].y=y;
		ptC1List[nPt].x=x;
		ptC1List[nPt].y=y;
		nPt++;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 4"<< endl<< endl;
	cout << "Left mouse click: Add a control point"<<endl;
	cout << "Q: Quit" <<endl;
	cout << "P: Toggle displaying control points" <<endl;
	cout << "L: Toggle displaying control lines" <<endl;
	cout << "E: Erase all points (Clear)" << endl;
	cout << "C: Toggle C1 continuity" <<endl;	
	cout << "T: Toggle displaying tangent vectors" <<endl;
	cout << "O: Toggle displaying objects" <<endl;
	cout << "R: Read in control points from \"savefile.txt\"" <<endl;
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
	cout << "D: Toggle Bezier curve" <<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS3241 Assignment 4");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
