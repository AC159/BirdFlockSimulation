//#include <windows.h>
//#include "gl/glew.h"
//#include <gl/freeglut.h>
//
//float angle = 0.0f;
//
//void display()
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	// Reset transformations
//	glLoadIdentity();
//
//	// Set the camera
//	gluLookAt(0.5f, 0.0f, 1.0f,
//			  0.0f, 0.0f, 0.0f,
//			  0.0f, 0.0f, 1.0f);
//
//	glRotatef(angle, 0.0f, 1.0f, 0.0f);
//
//	glBegin(GL_TRIANGLES);
//	glColor3f(0.5f, 0.5f, 0.5f);
//
//	glVertex3f(0.0f, 0.5f, 0.0f);
//	glVertex3f(-0.5f, -0.5f, 0.0f);
//	glVertex3f(0.5f, -0.5f, 0.0f);
//
//	glEnd();
//
//	angle += 0.1f;
//
//	glutSwapBuffers();
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowSize(1200, 650);
//
//	glutCreateWindow("Bird Flock Simulation");
//
//	glutDisplayFunc( display );
//	glutIdleFunc( display ); // function that gets called when the window is idle
//
//	glutMainLoop();
//
//	return 0;
//}
