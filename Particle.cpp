#include <GL/glut.h>
#include "Particle.h"

Particle::Particle(float mass, Vector3f position, Vector2f texCoord) : m_Mass(mass) {
	m_Position = position;
	m_Velocity = Vector3f(0.0f, 0.0f, 0.0f);
	m_Acceleration = Vector3f(0.0f, 0.0f, 0.0f);
	m_ForceAccumulator = Vector3f(0.0f, 0.0f, 0.0f);

	m_TexCoord = texCoord;
	m_Normal = Vector3f(0.0f, 0.0f, 0.0f);
}

void Particle::ClearForces(void) {
	m_ForceAccumulator = Vector3f(0.0f, 0.0f, 0.0f);
}

void Particle::Draw(void) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glVertex3f(m_Position.x, m_Position.y, m_Position.z);
	glEnd();
}