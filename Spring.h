#ifndef _SPRING_H_
#define _SPRING_H_

#include "Particle.h"

#define STRUCTURAL_SPRING_REST_LENGTH 5.0f
#define SHEAR_SPRING_REST_LENGTH (STRUCTURAL_SPRING_REST_LENGTH * sqrtf(2.0f))
#define FLEXION_SPRING_REST_LENGTH (STRUCTURAL_SPRING_REST_LENGTH * 2)

#define STRUCTURAL_SPRING_STIFFNESS 20.0f
#define SHEAR_SPRING_STIFFNESS 20.0f
#define FLEXION_SPRING_STIFFNESS 20.0f

class Spring {
public:
	Spring(Particle *, Particle *, float, float);

	void Draw(void);
	float ComputeLength(void);
	Vector3f ComputeForce(void);

	Particle *m_Particle1;
	Particle *m_Particle2;
	const float m_RestLength;
	const float m_Stiffness;
};

#endif