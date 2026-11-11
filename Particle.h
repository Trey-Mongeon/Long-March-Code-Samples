//------------------------------------------------------------------------------
//
// File Name:	Particle.h
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

#include "glm/glm.hpp"
#include "Textures.h"
#include "Transform.h"
#include "Vertex.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Emitter;
class Camera;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Typedefs:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Classes:
//------------------------------------------------------------------------------

class Particle
{
public:

	Particle(Emitter* parent, const glm::vec4& position, const glm::vec4& color,
		const glm::vec4& colorIncrement, const glm::vec4& velocity,
		const glm::vec4& acceleration, const float& duration, const float& scale,
		const float& rotation, const float& rotationalVelocity,
		const float& opacityDecayRate, const float& accelerationDuration,
		Camera* cam);

	~Particle() = default;

	void Update(float dt);

	void Render();

	// For adding to vertex array for batch rendering
	void AddToVertices(std::vector<Vertex3D>& vertices) const;

	bool IsAlive() const;

	glm::vec4 GetPosition() const;

	glm::vec4 GetColor() const;

	glm::vec4 GetVelocity() const;

	float GetDuration() const;

	float GetScale() const;

	float GetRotation() const;

	float GetRotationalVelocity() const;

	void SetPosition(const glm::vec4& position);

	void SetColor(const glm::vec4& color);

	void SetVelocity(const glm::vec4& velocity);

	void SetDuration(const float& duration);

	void SetScale(const float& scale);

	void SetRotation(const float& rotation);

	void SetRotationalVelocity(const float& rotationalVelocity);

	void SetCamera(Camera* cam);
private:

	Emitter* m_parent;

	glm::vec4 m_position;
	glm::vec4 m_color;
	glm::vec4 m_colorIncrement;
	glm::vec4 m_velocity;
	glm::vec4 m_acceleration;

	float m_duration;
	float m_scale;
	float m_rotation;

	float m_rotationalVelocity;
	float m_opacityDecayRate;
	float m_accelerationDuration;

	Camera* m_cam;
};


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
