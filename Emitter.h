//------------------------------------------------------------------------------
//
// File Name:	Emitter.h
// Author(s):	Trey Mongeon
// Project:		Long March
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Particle.h"
#include <vector>
#include "glm/glm.hpp"
#include "Textures.h"
#include "Mesh.h"
#include "Camera.h"
#include "BaseObject.h"
#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
class EmitterComponent;
class Shader;
//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Typedefs:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Classes:
//------------------------------------------------------------------------------


class Emitter : public BaseObject
{
public:
	friend EmitterComponent;

	Emitter(float frequency, unsigned int maxParticles, const glm::vec4& position,
		float baseDuration, float xSpawnDist, float ySpawnDist,
		float xVelocityFactor, float yVelocityFactor, float zVelocityFactor,
		const glm::vec4& baseVelocity,
		float xAccelerationFactor, float yAccelerationFactor,
		float zAccelerationFactor, const glm::vec4& baseAcceleration,
		const glm::vec4& baseColor, const glm::vec4& colorIncrement, float baseScale,
		float baseRotation, float baseRotationalVelocity, float scalingFactor,
		float durationFactor, float rotationalVelocityFactor,
		float opacityDecayRate, float accelerationDuration, bool useAdditiveBlending,
		bool writeToDepth, bool readFromDepth, 
		Texture* texture, Mesh* mesh, Camera* cam = nullptr);

	Emitter(const Emitter& rhs) : m_frequency(rhs.m_frequency), 
		m_internalTimer(m_frequency), m_maxParticles(rhs.m_maxParticles),
		m_position(rhs.m_position), m_baseDuration(rhs.m_baseDuration),
		m_xSpawnDist(rhs.m_xSpawnDist), m_ySpawnDist(rhs.m_ySpawnDist),
		m_xVelocityFactor(rhs.m_xVelocityFactor),
		m_yVelocityFactor(rhs.m_yVelocityFactor),
		m_zVelocityFactor(rhs.m_zVelocityFactor),
		m_baseVelocity(rhs.m_baseVelocity),
		m_xAccelerationFactor(rhs.m_xAccelerationFactor),
		m_yAccelerationFactor(rhs.m_yAccelerationFactor),
		m_zAccelerationFactor(rhs.m_zAccelerationFactor),
		m_baseAcceleration(rhs.m_baseAcceleration),
		m_baseColor(rhs.m_baseColor), m_colorIncrement(rhs.m_colorIncrement),
		m_baseScale(rhs.m_baseScale), m_baseRotation(rhs.m_baseRotation),
		m_baseRotationalVelocity(rhs.m_baseRotationalVelocity),
		m_scalingFactor(rhs.m_scalingFactor), m_durationFactor(rhs.m_durationFactor),
		m_rotationalVelocityFactor(rhs.m_rotationalVelocityFactor),
		m_opacityDecayRate(rhs.m_opacityDecayRate),
		m_accelerationDuration(rhs.m_accelerationDuration),
		m_useAdditiveBlending(rhs.m_useAdditiveBlending),
		m_writeToDepth(rhs.m_writeToDepth), m_readFromDepth(rhs.m_readFromDepth),
		m_texture(rhs.m_texture), m_mesh(rhs.m_mesh), m_camera(rhs.m_camera), m_baseVelocityModifier(rhs.m_baseVelocityModifier)
	{
	}

	Emitter(std::string name);
	Emitter();

	Emitter(JSON& stream) { Load(stream); }

	~Emitter() = default;

	// Function for loading an emitter from JSON
	void Load(JSON& stream) override;

	// Function for saving an emitter to JSON
	operator json() const;


	// Position should be the position of the emitters parent entity
	void Update(const glm::vec4 position, float dt);

	void Render(Shader* shader);

	// Returns true if a new particle was created
	bool Emit();


	void SetCamera(Camera* cam);
	void SetCamera();

	inline float GetFrequency() const { return m_frequency; }

	inline unsigned int GetMaxParticles() const { return m_maxParticles; }

	inline glm::vec4 GetPosition() const { return m_position; }

	inline unsigned int GetParticleCount() const { return static_cast<unsigned int>(m_particleList.size()); }

	inline float GetBaseDuration() const { return m_baseDuration; }

	inline float GetXSpawnDist() const { return m_xSpawnDist; }

	inline float GetYSpawnDist() const { return m_ySpawnDist; }

	inline float GetxVelocityFactor() const { return m_xVelocityFactor; }

	inline float GetyVelocityFactor() const { return m_yVelocityFactor; }

	inline float GetzVelocityFactor() const { return m_zVelocityFactor; }

	inline glm::vec4 GetBaseVelocity() const { return m_baseVelocity; }

	inline float GetxAccelerationFactor() const { return m_xAccelerationFactor; }

	inline float GetyAccelerationFactor() const { return m_yAccelerationFactor; }

	inline float GetzAccelerationFactor() const { return m_zAccelerationFactor; }

	inline glm::vec4 GetBaseAcceleration() const { return m_baseAcceleration; }

	inline glm::vec4 GetBaseColor() const { return m_baseColor; }

	inline glm::vec4 GetColorIncrement() const { return m_colorIncrement; }

	inline float GetBaseScale() const { return m_baseScale; }

	inline float GetBaseRotation() const { return m_baseRotation; }

	inline float GetBaseRotationalVelocity() const { return m_baseRotationalVelocity; }

	inline float GetScalingFactor() const { return m_scalingFactor; }

	inline float GetDurationFactor() const { return m_durationFactor; }

	inline float GetRotationalVelocityFactor() const { return m_rotationalVelocityFactor; }

	inline float GetOpacityDecayRate() const { return m_opacityDecayRate; }

	inline float GetAccelerationDuration() const { return m_accelerationDuration; }

	inline bool GetAdditiveBlendingFlag() const { return m_useAdditiveBlending; }

	Mesh* GetMesh();

	Texture* GetTexture();

	Camera* GetCamera();

	void SetMaxParticles(unsigned int maxParticles);

	void SetBaseScale(float scale);

	void SetPosition(glm::vec4 newpos);

	void SetScalingFactor(float factor);

	// Multiplies the base velocity components by scale
	void ScaleBaseVelocity(float scale);

	// Sets the velocity randomness
	void ScaleVelocityFactor(float scale);

private:

	// Frequency of particle emission
	float m_frequency;

	// Emits a particle when <= 0 and gets set to frequency value
	float m_internalTimer;

	unsigned int m_maxParticles;

	glm::vec4 m_position;

	std::vector<Particle> m_particleList;

	// Particles will spawn with this value as their lifetime timer.
	float m_baseDuration;

	float m_xSpawnDist;
	float m_ySpawnDist;

	// Max velocity that can be added or subtracted from the base velocity
	// base + (rand from -1 to 1) * factor
	float m_xVelocityFactor;

	float m_yVelocityFactor;

	float m_zVelocityFactor;

	glm::vec4 m_baseVelocity;

	// Useful for changing velocity with scale
	float m_baseVelocityModifier = 1;
	float m_factorVelocityModifier = 1;

	// Acceleration
	float m_xAccelerationFactor;
	
	float m_yAccelerationFactor;
	
	float m_zAccelerationFactor;

	// Velocity += acceleration * dt | kinematic eqn: velocity = initial velocity + acceleration * time
	glm::vec4 m_baseAcceleration;

	// Color
	glm::vec4 m_baseColor;

	// Each update the particles color will have increment * dt added
	glm::vec4 m_colorIncrement;

	// Scale
	float m_baseScale;

	// Rotation
	float m_baseRotation;

	// Rotational velocity
	float m_baseRotationalVelocity;

	// Max scale that will be added to or subtracted from the base scale 
	// base + (rand from -1 to 1) * factor
	float m_scalingFactor;

	// Max duration that will be added to or subtracted from the base duration
	// base + (rand from -1 to 1) * factor
	float m_durationFactor;

	// Max rotational velocity that will be added to or subtracted from the base rotational velocity
	// base + (rand from -1 to 1) * factor
	float m_rotationalVelocityFactor;

	// In the last second of a particles life it will lose opacity at this rate * dt
	float m_opacityDecayRate;

	// Specify how long you want the particle to accelerate
	float m_accelerationDuration;

	// The particles will use the additive blending mode
	bool m_useAdditiveBlending;

	// Should particles from this emitter write to the depth buffer
	bool m_writeToDepth;

	// Should particles from this emitter read from the depth buffer
	bool m_readFromDepth;

	std::string m_textureKey;
	Texture* m_texture;

	std::string m_meshKey;
	Mesh* m_mesh;

	Camera* m_camera;
	

};


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
