#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>
#include <stdio.h>
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];
static GLdouble zoomFactor = 1.0;
static GLint height;
static GLuint pixelBuffer;
void makeCheckImage(void)
{
	int i, j, c;
	for (i = 0; i < checkImageHeight; i++)
	{
		for (j = 0; j < checkImageWidth; j++)
		{
			c = ((i & 0x8) == 0 ^ ((j & 0x8) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;

		}
	}
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB, GL_UNSIGNED_BYTE, checkImage);
	glFlush();
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	height = (GLint)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
void motion(int x, int y)
{
	static GLint screeny;
	screeny = height - y;
	glRasterPos2i(x, screeny);
	glPixelZoom(zoomFactor, zoomFactor);
	glCopyPixels(0, 0, checkImageWidth, checkImageHeight, GL_COLOR);
	glPixelZoom(1.0, 1.0);
	glFlush();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
	case 'R':
		zoomFactor = 1.0;
		glutPostRedisplay();
		printf("reset\n");
		break;
	case 'z':
		zoomFactor += 0.5;
		if (zoomFactor >= 3.0)
			zoomFactor = 3.0;

		printf("+=0.5\n");
		break;

	case 'Z':
		zoomFactor -= 0.5;
		if (zoomFactor <= 0.5)
			zoomFactor = 0.5;

		printf("-=0.5\n");
		break;
	case 27:
		break;
	default:
		break;
	}
}
int main(int argv, char **argc)
{
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argc[0]);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}