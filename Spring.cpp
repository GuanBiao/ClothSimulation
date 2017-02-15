#include <GL/glut.h>
#include "Spring.h"

Spring::Spring(Particle *p1, Particle *p2, float restLength, float stiffness) : m_RestLength(restLength), m_Stiffness(stiffness) {
	m_Particle1 = p1;
	m_Particle2 = p2;
}

void Spring::Draw(void) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(m_Particle1->m_Position.x, m_Particle1->m_Position.y, m_Particle1->m_Position.z);
	glVertex3f(m_Particle2->m_Position.x, m_Particle2->m_Position.y, m_Particle2->m_Position.z);
	glEnd();
}

float Spring::ComputeLength(void) {
	float diff_x = m_Particle1->m_Position.x - m_Particle2->m_Position.x;
	float diff_y = m_Particle1->m_Position.y - m_Particle2->m_Position.y;
	float diff_z = m_Particle1->m_Position.z - m_Particle2->m_Position.z;

	return sqrtf(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
}

Vector3f Spring::ComputeForce(void) {
	Vector3f link = m_Particle2->m_Position - m_Particle1->m_Position;
	float length = link.length();
	Vector3f force = (link - (link / length) * m_RestLength) * m_Stiffness;

	return force;
}