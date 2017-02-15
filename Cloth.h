#ifndef _CLOTH_H_
#define _CLOTH_H_

#include <vector>
#include "Spring.h"

#define MESH_SIZE 17
#define ACCELERATION_OF_GRAVITY Vector3f(0.0f, -9.8f, 0.0f)
#define SPRING_DAMPING_COEFFICIENT 1.0f
#define FLUID_VELOCITY Vector3f(0.0f, -30.0f, -30.0f)
#define FLUID_DAMPING_COEFFICIENT 1.0f
#define TIME_STEP 0.01f
#define CRITICAL_DEFORMATION_RATE 0.1f

extern GLuint textureObj;

class Cloth {
public:
	Cloth();

	void Draw(void);
	void Simulate(void);
	void ApplyEulerMethod(void);
	void ApplyDynamicInverseProcedure(void);
	void ComputeNormal(void);

	vector< vector<Particle> > m_Particles;
	vector<Spring> m_StructuralSprings;
	vector<Spring> m_ShearSprings;
	vector<Spring> m_FlexionSprings;
};

#endif