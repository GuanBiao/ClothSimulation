#include <GL/glut.h>
#include <FreeImage.h>
#include "Cloth.h"

GLuint textureObj;

void DisplayCallback(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float center = (MESH_SIZE - 1) * STRUCTURAL_SPRING_REST_LENGTH / 2.0f;
	gluLookAt(center, center, center * 3, center, center, 0.0f, 0.0f, 1.0f, 0.0f); // Look at the center of the cloth
	static Cloth cloth;
	cloth.Simulate();
	cloth.Draw();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.333f, 1.0f, center * 20);

	glutSwapBuffers();
}

void SetTextures(const char *fileName) {
	FIBITMAP *bitmap = FreeImage_Load(FIF_JPEG, fileName, JPEG_DEFAULT);
	GLubyte *pixels = FreeImage_GetBits(bitmap);
	int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);

	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FreeImage_Unload(bitmap);
}

void SetLightSource(void) {
	GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat position[] = {0.0f, 0.0f, 1.0f, 0.0f};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void SetMaterial(void) {
	GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("Cloth Simulation");
	glutDisplayFunc(DisplayCallback);
	glutIdleFunc(DisplayCallback);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	SetTextures("Fabric_Texture.jpg");
	SetLightSource();
	SetMaterial();

	glutMainLoop();

	return 0;
}