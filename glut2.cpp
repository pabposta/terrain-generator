#include<gl/glut.h>
#include<gl/glu.h>
#include<gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


float angle = 0.0;
int iterations = 4 * 4 * 4 * 4 * 4 * 4 + 1;
GLfloat **lines;
int squares = 4 * 4 * 4 + 1;
GLfloat **matrix;
int **flags;


GLfloat randomatic (GLfloat max = 1.0)
{

	GLfloat value;


	value = 2 * (GLfloat) rand() / (GLfloat) RAND_MAX * max - max;


	return value;

}



void divideSquare (GLfloat **matrix, int ax, int ay, int bx, int by)
{

	int abx, aby;
	GLfloat mean, mean1, mean2, mean3, mean4;
	GLfloat deviation = 1.6 * (bx - ax) / squares;
	GLfloat middleDeviation = 0.4 * (bx - ax) / squares;
	
		
	if (bx - ax < 2)
		return;

	abx = ax + (bx - ax) / 2;
	aby = ay + (by - ay) / 2;
	
	mean1 = (matrix[ax][ay] + matrix[bx][ay]) / 2.0;
	mean2 = (matrix[bx][ay] + matrix[bx][by]) / 2.0;
	mean3 = (matrix[bx][by] + matrix[ax][by]) / 2.0;
	mean4 = (matrix[ax][by] + matrix[ax][ay]) / 2.0;
	mean = (mean1 + mean2 +mean3 + mean4) / 4.0;
	if (flags[abx][ay] == 0)
	{
		matrix[abx][ay] = mean1 + randomatic (deviation);
		flags[abx][ay] = 1;
	}
	if (flags[bx][aby] == 0)
	{
		matrix[bx][aby] = mean2 + randomatic (deviation);
		flags[bx][aby] = 1;
	}
	if (flags[abx][by] == 0)
	{
		matrix[abx][by] = mean3 + randomatic (deviation);		
		flags[abx][by] = 1;
	}
	
	if (flags[ax][aby] == 0)
	{
		matrix[ax][aby] = mean4 + randomatic (deviation);	
		flags[ax][aby] = 1;
	}
	matrix[abx][aby] = mean + randomatic (middleDeviation);

	divideSquare (matrix, ax, ay, abx, aby);
	divideSquare (matrix, abx, ay, bx, aby);
	divideSquare (matrix, ax, aby, abx, by);
	divideSquare (matrix, abx, aby, bx, by);


}


void createTerrain (void)
{

	int i, j;
	float height = 0.0;
	GLfloat min = -0.11;


	srand ((unsigned) time (NULL));

	matrix = new GLfloat *[squares];
	for (i = 0; i < squares; i++)
		matrix[i] = new GLfloat[squares];
	
	matrix[0][0] = height;
	matrix[0][squares - 1] = height;
	matrix[squares - 1][squares - 1] = height;
	matrix[squares - 1][0] = height;
	
	flags = new int *[squares];
	for (i = 0; i < squares; i++)
	{

		flags[i] = new int[squares];
		for (j = 0; j < squares; j++)
			flags[i][j] = 0;

	}
	
	divideSquare (matrix, 0, 0, squares - 1, squares - 1);
	
	for (i = 0; i < squares; i++)
		for (j = 0; j < squares; j++)
			if (matrix[i][j] < min)
				matrix[i][j] = min;

	
}


void calculateColor (int i, int j, GLfloat &r, GLfloat &g, GLfloat &b)
{
			
	GLfloat rt, gt, bt;


	r = 0.0; 
	g = 0.2;
	b = 0.0;

	if (matrix[i][j] < -0.1)
	{
		g = 0.0;
		b = 0.3;
	}
	
	r += matrix[i][j] * 0.15;
	g += matrix[i][j] * 0.15;
	b += matrix[i][j] * 0.06;

	rt = matrix[i][j] * -2.0;
	if (rt < 0.0)
		rt = 0.0;
	r += rt;
	
	gt = matrix[i][j] * -2.0;
	if (gt < 0.0)
		gt = 0.0;
	g += gt;
		
	bt = matrix[i][j] * -2.0;
	if (bt < 0.0)
		bt = 0.0;
	b += bt;

	
}


void renderTerrain (void)
{

	int i, j;
	GLfloat r = 1.0, g = 1.0, b = 1.0;
	GLfloat scale = 3.0;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	
	glRotatef (-60.0, 1.0, 0.0, 0.0);

	for (i = 0; i < squares - 1; i++)
		for (j = 0; j < squares - 1; j++)
		{
			
			
		

			/*
			calculateColor (i, j, r, g, b);
			glColor3f (r, g, b);
			 
			glBegin(GL_LINES);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) i / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) j / (GLfloat) squares, matrix[i][j]);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) (i + 1) / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) j / (GLfloat) squares, matrix[i + 1][j]);
			glEnd();
			glBegin (GL_LINES);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) (i + 1) / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) j / (GLfloat) squares, matrix[i + 1][j]);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) (i + 1) / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) (j + 1) / (GLfloat) squares, matrix[i + 1][j + 1]);
			glEnd();
			glBegin (GL_LINES);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) (i + 1) / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) (j + 1) / (GLfloat) squares, matrix[i + 1][j + 1]);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) i / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) (j + 1) / (GLfloat) squares, matrix[i][j + 1]);				
			glEnd();
			glBegin (GL_LINES);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) i / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) (j + 1) / (GLfloat) squares, matrix[i][j + 1]);				
				glVertex3f(-scale + 2.0 * scale * (GLfloat) i / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) j / (GLfloat) squares, matrix[i][j]);
			glEnd();
			*/
			glBegin (GL_QUADS);
				calculateColor (i, j, r, g, b);
				glColor3f (r, g, b);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) i / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) j / (GLfloat) squares, matrix[i][j]);
				calculateColor (i + 1, j, r, g, b);
				glColor3f (r, g, b);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) (i + 1) / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) j / (GLfloat) squares, matrix[i + 1][j]);
				calculateColor (i + 1, j + 1, r, g, b);
				glColor3f (r, g, b);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) (i + 1) / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) (j + 1) / (GLfloat) squares, matrix[i + 1][j + 1]);
				calculateColor (i, j + 1, r, g, b);
				glColor3f (r, g, b);
				glVertex3f(-scale + 2.0 * scale * (GLfloat) i / (GLfloat) squares, -scale + 2.0 * scale * (GLfloat) (j + 1) / (GLfloat) squares, matrix[i][j + 1]);	
			glEnd();

		}

	glPopMatrix();
	glutSwapBuffers();


}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,1.0,10.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);


}




void partitionLine (GLfloat **lines, int index1, int index2)
{

	int step;
	int index14, index24, index34;
	GLfloat temp, magnitude, height[2], b, c;;


	if (index2 - index1 < 4)
		return;

	// calculate new points
	step = (index2 - index1) / 4;
	index14 = index1 + step;
	index24 = index14 + step;
	index34 = index2 - step;
	
	lines[index14][0] = lines[index1][0] + (lines[index2][0] - lines[index1][0]) / 3.0;
	lines[index14][1] = lines[index1][1] + (lines[index2][1] - lines[index1][1]) / 3.0;
	
		
	height[0] = lines[index2][0] - lines[index1][0];
	height[1] = lines[index2][1] - lines[index1][1];
	
	temp = height[0];
	height[0] = -height[1];
	height[1] = temp;
	magnitude = sqrt (pow (height[0], 2) + pow (height[1], 2));
	height[0] /= magnitude;
	height[1] /= magnitude;
	
	c = sqrt (pow (lines[index2][0] - lines[index1][0], 2) + pow (lines[index2][1] - lines[index1][1], 2)) / 3.0;
	b = c / 2;
	magnitude = sqrt (pow (c, 2) - pow (b, 2));
	height[0] *= magnitude;
	height[1] *= magnitude;

	lines[index24][0] = lines[index1][0] + (lines[index2][0] - lines[index1][0]) / 2.0 + height[0];
	lines[index24][1] = lines[index1][1] + (lines[index2][1] - lines[index1][1]) / 2.0 + height[1];

	
	lines[index34][0] = lines[index1][0] + 2.0 * (lines[index2][0] - lines[index1][0]) / 3.0;
	lines[index34][1] = lines[index1][1] + 2.0 * (lines[index2][1] - lines[index1][1]) / 3.0;
	
	// call recursive function
	partitionLine (lines, index1, index14);
	partitionLine (lines, index14, index24);
	partitionLine (lines, index24, index34);
	partitionLine (lines, index34, index2);

}


GLfloat **createFractal (void)
{

	int i;

	
	lines = new GLfloat *[iterations];
	for (i = 0; i < iterations; i++)
		lines[i] = new GLfloat[2];
	
	lines[0][0] = -2.0;
	lines[0][1] = 0.0;
	lines[iterations - 1][0] = 2.0;
	lines[iterations - 1][1] = 0.0;

	partitionLine (lines, 0, iterations - 1);


	return lines;

}

void renderFractal(void) {
	int i;
	static GLfloat r = 1.0, g = 1.0, b = 1.0;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	
	for (i = 0; i < iterations - 1; i++)
	{
		glColor3f (r, g, b);
		glBegin(GL_LINES);
			glVertex3f(lines[i][0], lines[i][1], 0.0);
			glVertex3f(lines[i + 1][0], lines[i + 1][1], 0.0);
		glEnd();
		r -= 0.1;
		if (r < 0.0)
			r = 1.0;
		g -= 0.05;
		if (g < 0.0)
			g = 1.0;
		b -= 0.15;
		if (b < 0.0)
			b = 1.0;
		

	}
	glPopMatrix();
	glutSwapBuffers();
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,640);
	glutCreateWindow("3D Tech - GLUT Tutorial");
	glutDisplayFunc(renderTerrain);
	glutIdleFunc(renderTerrain);
	glutReshapeFunc(changeSize);
	
	createTerrain();

	glutMainLoop();
}

