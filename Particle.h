#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Utility.h"

#define MASS 1.0f

class Particle {
public:
	Particle(float, Vector3f, Vector2f);

	void Draw(void);
	void ClearForces(void);

	const float m_Mass;
	Vector3f m_Position;
	Vector3f m_Velocity;
	Vector3f m_Acceleration;
	Vector3f m_ForceAccumulator;

	Vector2f m_TexCoord;
	Vector3f m_Normal;
};

#endif