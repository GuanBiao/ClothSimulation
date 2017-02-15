#include <GL/glut.h>
#include "Cloth.h"

Cloth::Cloth() {
	// Build particles
	for (int i = 0; i < MESH_SIZE; i++) {
		vector<Particle> line;
		Vector3f origin(0.0f, STRUCTURAL_SPRING_REST_LENGTH * i, 0.0f);
		for (int j = 0; j < MESH_SIZE; j++) {
			Vector2f texCoord(j / (MESH_SIZE - 1.0f), i / (MESH_SIZE - 1.0f));
			Vector3f normal(0.0f, 0.0f, 1.0f);
			line.push_back(Particle(MASS, origin, texCoord));
			origin += Vector3f(STRUCTURAL_SPRING_REST_LENGTH, 0.0f, 0.0f);
		}
		m_Particles.push_back(line);
	}

	// Build structural springs
	for (int i = 0; i < MESH_SIZE - 1; i++) {
		for (int j = 0; j < MESH_SIZE; j++)
			m_StructuralSprings.push_back(Spring(&m_Particles[i][j], &m_Particles[i + 1][j], STRUCTURAL_SPRING_REST_LENGTH, STRUCTURAL_SPRING_STIFFNESS));
	}
	for (int i = 0; i < MESH_SIZE; i++) {
		for (int j = 0; j < MESH_SIZE - 1; j++)
			m_StructuralSprings.push_back(Spring(&m_Particles[i][j], &m_Particles[i][j + 1], STRUCTURAL_SPRING_REST_LENGTH, STRUCTURAL_SPRING_STIFFNESS));
	}

	// Build shear springs
	for (int i = 0; i < MESH_SIZE - 1; i++) {
		for (int j = 0; j < MESH_SIZE - 1; j++)
			m_ShearSprings.push_back(Spring(&m_Particles[i][j], &m_Particles[i + 1][j + 1], SHEAR_SPRING_REST_LENGTH, SHEAR_SPRING_STIFFNESS));
	}
	for (int i = 0; i < MESH_SIZE - 1; i++) {
		for (int j = 0; j < MESH_SIZE - 1; j++)
			m_ShearSprings.push_back(Spring(&m_Particles[i + 1][j], &m_Particles[i][j + 1], SHEAR_SPRING_REST_LENGTH, SHEAR_SPRING_STIFFNESS));
	}

	// Build flexion springs
	for (int i = 0; i < MESH_SIZE - 2; i++) {
		for (int j = 0; j < MESH_SIZE; j++)
			m_FlexionSprings.push_back(Spring(&m_Particles[i][j], &m_Particles[i + 2][j], FLEXION_SPRING_REST_LENGTH, FLEXION_SPRING_STIFFNESS));
	}
	for (int i = 0; i < MESH_SIZE; i++) {
		for (int j = 0; j < MESH_SIZE - 2; j++)
			m_FlexionSprings.push_back(Spring(&m_Particles[i][j], &m_Particles[i][j + 2], FLEXION_SPRING_REST_LENGTH, FLEXION_SPRING_STIFFNESS));
	}
}

void Cloth::Draw(void) {
/*	// Draw springs
	for (int i = 0; i < m_StructuralSprings.size(); i++)
		m_StructuralSprings[i].Draw();
	for (int i = 0; i < m_ShearSprings.size(); i++)
		m_ShearSprings[i].Draw();
	for (int i = 0; i < m_FlexionSprings.size(); i++)
		m_FlexionSprings[i].Draw();

	// Draw particles
	for (int i = 0; i < MESH_SIZE; i++) {
		for (int j = 0; j < MESH_SIZE; j++)
			m_Particles[i][j].Draw();
	}/**/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	for (int i = 0; i < MESH_SIZE - 1; i++) {
		for (int j = 0; j < MESH_SIZE - 1; j++) {
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(m_Particles[i][j].m_Normal.x, m_Particles[i][j].m_Normal.y, m_Particles[i][j].m_Normal.z);
			glTexCoord2f(m_Particles[i][j].m_TexCoord.x, m_Particles[i][j].m_TexCoord.y);
			glVertex3f(m_Particles[i][j].m_Position.x, m_Particles[i][j].m_Position.y, m_Particles[i][j].m_Position.z);
			
			glNormal3f(m_Particles[i + 1][j].m_Normal.x, m_Particles[i + 1][j].m_Normal.y, m_Particles[i + 1][j].m_Normal.z);
			glTexCoord2f(m_Particles[i + 1][j].m_TexCoord.x, m_Particles[i + 1][j].m_TexCoord.y);
			glVertex3f(m_Particles[i + 1][j].m_Position.x, m_Particles[i + 1][j].m_Position.y, m_Particles[i + 1][j].m_Position.z);
			
			glNormal3f(m_Particles[i][j + 1].m_Normal.x, m_Particles[i][j + 1].m_Normal.y, m_Particles[i][j + 1].m_Normal.z);
			glTexCoord2f(m_Particles[i][j + 1].m_TexCoord.x, m_Particles[i][j + 1].m_TexCoord.y);
			glVertex3f(m_Particles[i][j + 1].m_Position.x, m_Particles[i][j + 1].m_Position.y, m_Particles[i][j + 1].m_Position.z);
			
			glNormal3f(m_Particles[i + 1][j + 1].m_Normal.x, m_Particles[i + 1][j + 1].m_Normal.y, m_Particles[i + 1][j + 1].m_Normal.z);
			glTexCoord2f(m_Particles[i + 1][j + 1].m_TexCoord.x, m_Particles[i + 1][j + 1].m_TexCoord.y);
			glVertex3f(m_Particles[i + 1][j + 1].m_Position.x, m_Particles[i + 1][j + 1].m_Position.y, m_Particles[i + 1][j + 1].m_Position.z);
			glEnd();
		}
	}
}

void Cloth::Simulate(void) {
	// Clear the force accumulator for all particles
	for (int i = 0; i < MESH_SIZE; i++) {
		for (int j = 0; j < MESH_SIZE; j++)
			m_Particles[i][j].ClearForces();
	}

	// Compute spring (internal) forces applied on each particle by Hooke's Law
	// For structural springs
	for (int i = 0; i < m_StructuralSprings.size(); i++) {
		Vector3f force = m_StructuralSprings[i].ComputeForce();
		m_StructuralSprings[i].m_Particle1->m_ForceAccumulator += force;
		m_StructuralSprings[i].m_Particle2->m_ForceAccumulator += -force;
	}
	// For shear springs
	for (int i = 0; i < m_ShearSprings.size(); i++) {
		Vector3f force = m_ShearSprings[i].ComputeForce();
		m_ShearSprings[i].m_Particle1->m_ForceAccumulator += force;
		m_ShearSprings[i].m_Particle2->m_ForceAccumulator += -force;
	}
	// For flexion springs
	for (int i = 0; i < m_FlexionSprings.size(); i++) {
		Vector3f force = m_FlexionSprings[i].ComputeForce();
		m_FlexionSprings[i].m_Particle1->m_ForceAccumulator += force;
		m_FlexionSprings[i].m_Particle2->m_ForceAccumulator += -force;
	}

	// Compute the normal vector of each particle
	ComputeNormal();

	// Compute external forces applied on each particle
	Vector3f weight = ACCELERATION_OF_GRAVITY * MASS;
	for (int i = 0; i < MESH_SIZE; i++) {
		for (int j = 0; j < MESH_SIZE; j++) {
			Vector3f viscousDamping = -m_Particles[i][j].m_Velocity * SPRING_DAMPING_COEFFICIENT;

			// Let the wind blow for a while
			static float timer = 0.0f;
			timer += TIME_STEP;
			Vector3f viscousFluidForce;
			if (timer > 200.0f)
				viscousFluidForce = Vector3f(0.0f, 0.0f, 0.0f);
			else
				viscousFluidForce = m_Particles[i][j].m_Normal * (m_Particles[i][j].m_Normal.dot(FLUID_VELOCITY - m_Particles[i][j].m_Velocity)) * FLUID_DAMPING_COEFFICIENT;

			m_Particles[i][j].m_ForceAccumulator += weight + viscousDamping + viscousFluidForce;
		}
	}

	// Set two hanging points
	m_Particles[MESH_SIZE - 1][0].m_ForceAccumulator = Vector3f(0.0f, 0.0f, 0.0f);
	m_Particles[MESH_SIZE - 1][MESH_SIZE - 1].m_ForceAccumulator = Vector3f(0.0f, 0.0f, 0.0f);

	// Compute the new cloth position by Euler's Method
	ApplyEulerMethod();

	// Adjust "super-elongated" springs
	ApplyDynamicInverseProcedure();
}

void Cloth::ApplyEulerMethod(void) {
	for (int i = 0; i < MESH_SIZE; i++) {
		for (int j = 0; j < MESH_SIZE; j++) {
			m_Particles[i][j].m_Acceleration = m_Particles[i][j].m_ForceAccumulator / m_Particles[i][j].m_Mass;
			m_Particles[i][j].m_Velocity = m_Particles[i][j].m_Velocity + m_Particles[i][j].m_Acceleration * TIME_STEP;
			m_Particles[i][j].m_Position = m_Particles[i][j].m_Position + m_Particles[i][j].m_Velocity * TIME_STEP;
		}
	}
}

void Cloth::ApplyDynamicInverseProcedure(void) {
	// For structural springs
	for (int i = 0; i < m_StructuralSprings.size(); i++) {
		float length = m_StructuralSprings[i].ComputeLength();
		float deformationRate = (length - m_StructuralSprings[i].m_RestLength) / m_StructuralSprings[i].m_RestLength;
		if (deformationRate > CRITICAL_DEFORMATION_RATE) {
			// Case 1: spring linking a fixed particle and a loose particle
			if (m_StructuralSprings[i].m_Particle1 == &m_Particles[MESH_SIZE - 1][0] || m_StructuralSprings[i].m_Particle1 == &m_Particles[MESH_SIZE - 1][MESH_SIZE - 1]) {
				float m = 1.1f * m_StructuralSprings[i].m_RestLength;
				float n = length - m;
				Vector3f newPosition = (m_StructuralSprings[i].m_Particle1->m_Position * n + m_StructuralSprings[i].m_Particle2->m_Position * m) / length;
				m_StructuralSprings[i].m_Particle2->m_Position = newPosition;
			}
			else if (m_StructuralSprings[i].m_Particle2 == &m_Particles[MESH_SIZE - 1][0] || m_StructuralSprings[i].m_Particle2 == &m_Particles[MESH_SIZE - 1][MESH_SIZE - 1]) {
				float m = 1.1f * m_StructuralSprings[i].m_RestLength;
				float n = length - m;
				Vector3f newPosition = (m_StructuralSprings[i].m_Particle2->m_Position * n + m_StructuralSprings[i].m_Particle1->m_Position * m) / length;
				m_StructuralSprings[i].m_Particle1->m_Position = newPosition;
			}
			// Case 2: spring linking two loose particles
			else {
				Vector3f middle = (m_StructuralSprings[i].m_Particle1->m_Position + m_StructuralSprings[i].m_Particle2->m_Position) / 2.0f;
				float n = 0.55f * m_StructuralSprings[i].m_RestLength;
				float m = 0.5f * length - n;
				// Change the position of one particle
				m_StructuralSprings[i].m_Particle1->m_Position = (m_StructuralSprings[i].m_Particle1->m_Position * n + middle * m) / (0.5f * length);
				// Change the position of the other particle
				m_StructuralSprings[i].m_Particle2->m_Position = (m_StructuralSprings[i].m_Particle2->m_Position * n + middle * m) / (0.5f * length);
			}
		}
	}

	// For shear springs
	for (int i = 0; i < m_ShearSprings.size(); i++) {
		float length = m_ShearSprings[i].ComputeLength();
		float deformationRate = (length - m_ShearSprings[i].m_RestLength) / m_ShearSprings[i].m_RestLength;
		if (deformationRate > CRITICAL_DEFORMATION_RATE) {
			// Case 1: spring linking a fixed particle and a loose particle
			if (m_ShearSprings[i].m_Particle1 == &m_Particles[MESH_SIZE - 1][0] || m_ShearSprings[i].m_Particle1 == &m_Particles[MESH_SIZE - 1][MESH_SIZE - 1]) {
				float m = 1.1f * m_ShearSprings[i].m_RestLength;
				float n = length - m;
				Vector3f newPosition = (m_ShearSprings[i].m_Particle1->m_Position * n + m_ShearSprings[i].m_Particle2->m_Position * m) / length;
				m_ShearSprings[i].m_Particle2->m_Position = newPosition;
			}
			else if (m_ShearSprings[i].m_Particle2 == &m_Particles[MESH_SIZE - 1][0] || m_ShearSprings[i].m_Particle2 == &m_Particles[MESH_SIZE - 1][MESH_SIZE - 1]) {
				float m = 1.1f * m_ShearSprings[i].m_RestLength;
				float n = length - m;
				Vector3f newPosition = (m_ShearSprings[i].m_Particle2->m_Position * n + m_ShearSprings[i].m_Particle1->m_Position * m) / length;
				m_ShearSprings[i].m_Particle1->m_Position = newPosition;
			}
			// Case 2: spring linking two loose particles
			else {
				Vector3f middle = (m_ShearSprings[i].m_Particle1->m_Position + m_ShearSprings[i].m_Particle2->m_Position) / 2.0f;
				float n = 0.55f * m_ShearSprings[i].m_RestLength;
				float m = 0.5f * length - n;
				// Change the position of one particle
				m_ShearSprings[i].m_Particle1->m_Position = (m_ShearSprings[i].m_Particle1->m_Position * n + middle * m) / (0.5f * length);
				// Change the position of the other particle
				m_ShearSprings[i].m_Particle2->m_Position = (m_ShearSprings[i].m_Particle2->m_Position * n + middle * m) / (0.5f * length);
			}
		}
	}
}

void Cloth::ComputeNormal(void) {
	// Initialize the normal for all particles
	for (int i = 0; i < MESH_SIZE; i++) {
		for (int j = 0; j < MESH_SIZE; j++)
			m_Particles[i][j].m_Normal = Vector3f(0.0f, 0.0f, 0.0f);
	}

	// Compute the normal of each particle
	for (int i = 0; i < MESH_SIZE - 1; i++) {
		for (int j = 0; j < MESH_SIZE - 1; j++) {
			// Compute the normal of the upper triangle defined by the position of three particles
			Vector3f v1 = m_Particles[i + 1][j + 1].m_Position - m_Particles[i][j].m_Position;
			Vector3f v2 = m_Particles[i + 1][j].m_Position - m_Particles[i][j].m_Position;
			Vector3f normal = v1.cross(v2).normalize();
			// Accumulate the normal
			m_Particles[i][j].m_Normal += normal;
			m_Particles[i + 1][j].m_Normal += normal;
			m_Particles[i + 1][j + 1].m_Normal += normal;

			// Compute the normal of the lower triangle defined by the position of another three particles
			v1 = m_Particles[i][j + 1].m_Position - m_Particles[i][j].m_Position;
			v2 = m_Particles[i + 1][j + 1].m_Position - m_Particles[i][j].m_Position;
			normal = v1.cross(v2).normalize();
			// Accumulate the normal
			m_Particles[i][j].m_Normal += normal;
			m_Particles[i][j + 1].m_Normal += normal;
			m_Particles[i + 1][j + 1].m_Normal += normal;
		}
	}

	// Normalize the normal for all particles
	for (int i = 0; i < MESH_SIZE; i++) {
		for (int j = 0; j < MESH_SIZE; j++)
			m_Particles[i][j].m_Normal = m_Particles[i][j].m_Normal.normalize();
	}
}