//------------------------------------------------------------------------------
//
// File Name:	Particle.cpp
// Author(s):	Trey Mongeon
// Project:		Long March
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Particle.h"
#include "Render.h"
#include "Emitter.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "DrawEnviorment.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


Particle::Particle(Emitter* parent, const glm::vec4& position,
	const glm::vec4& color, const glm::vec4& colorIncrement,
	const glm::vec4& velocity, const glm::vec4& acceleration,
	const float& duration, const float& scale, const float& rotation,
	const float& rotationalVelocity, const float& opacityDecayRate,
	const float& accelerationDuration, Camera* cam) : m_parent(parent), m_position(position),
	m_color(color), m_colorIncrement(colorIncrement),
	m_velocity(velocity), m_acceleration(acceleration),
	m_duration(duration), m_scale(scale), m_rotation(rotation),
	m_rotationalVelocity(rotationalVelocity),
	m_opacityDecayRate(opacityDecayRate),
	m_accelerationDuration(accelerationDuration),
	m_cam(cam)
{
}

void Particle::Update(float dt)
{
	m_duration -= dt;
	m_accelerationDuration -= dt;
	m_position += m_velocity * dt;
	m_rotation += m_rotationalVelocity * dt;
	if (m_accelerationDuration > 0)
	{
		m_velocity += m_acceleration * dt;
	}

	m_color += m_colorIncrement * dt;

	if (m_duration <= 1.0f)
	{
		m_color.a -= m_opacityDecayRate * dt;
	}
}

// DEPRECATED: No longer in use to reduce draw calls, particles using batch rendering. See Particle::AddToVertices
void Particle::Render()
{
	glm::mat4 modelMat = glm::mat4(1.0f);

	glm::vec3 pos(m_position);

	modelMat = glm::translate(modelMat, pos);

	modelMat = glm::rotate(modelMat, glm::radians(m_rotation), 
		glm::vec3(0.0f, 0.0f, 1.0f));

	modelMat = glm::scale(modelMat, glm::vec3(m_scale, m_scale, 1.0f));

	Draw(nullptr, m_parent->GetMesh(), modelMat, m_cam, 
		m_parent->GetTexture(), glm::vec2(0.0f), m_color);
}

void Particle::AddToVertices(std::vector<Vertex3D>& vertices) const
{
	// Model matrix for this particle
	glm::mat4 modelMat = glm::mat4(1.0f);

	glm::vec3 pos(m_position);

	modelMat = glm::translate(modelMat, pos);

	modelMat = glm::rotate(modelMat, glm::radians(m_rotation),
		glm::vec3(0.0f, 0.0f, 1.0f));

	modelMat = glm::scale(modelMat, glm::vec3(m_scale, m_scale, 1.0f));

	// Model space verts
	glm::vec3 tl_m = glm::vec3(-0.5f, 0.5f, 0.0f);
	glm::vec3 tr_m = glm::vec3(0.5f, 0.5f, 0.0f);
	glm::vec3 bl_m = glm::vec3(-0.5f, -0.5f, 0.0f);
	glm::vec3 br_m = glm::vec3(0.5f, -0.5f, 0.0f);

	// World space verts
	glm::vec4 tl_w = modelMat * glm::vec4(tl_m, 1.0f);
	glm::vec4 tr_w = modelMat * glm::vec4(tr_m, 1.0f);
	glm::vec4 bl_w = modelMat * glm::vec4(bl_m, 1.0f);
	glm::vec4 br_w = modelMat * glm::vec4(br_m, 1.0f);

	// Vertex UV coords
	glm::vec2 uvTL = glm::vec2(0.0f, 1.0f);
	glm::vec2 uvTR = glm::vec2(1.0f, 1.0f);
	glm::vec2 uvBL = glm::vec2(0.0f, 0.0f);
	glm::vec2 uvBR = glm::vec2(1.0f, 0.0f);

	// Add two triangles to vertices
	// Triangle 1
	vertices.push_back(Vertex3D(glm::vec3(br_w.x, br_w.y, br_w.z), uvBR, m_color));
	vertices.push_back(Vertex3D(glm::vec3(tr_w.x, tr_w.y, tr_w.z), uvTR, m_color));
	vertices.push_back(Vertex3D(glm::vec3(tl_w.x, tl_w.y, tl_w.z), uvTL, m_color));
	// Triangle 2										 
	vertices.push_back(Vertex3D(glm::vec3(tl_w.x, tl_w.y, tl_w.z), uvTL, m_color));
	vertices.push_back(Vertex3D(glm::vec3(bl_w.x, bl_w.y, bl_w.z), uvBL, m_color));
	vertices.push_back(Vertex3D(glm::vec3(br_w.x, br_w.y, br_w.z), uvBR, m_color));
}

bool Particle::IsAlive() const
{
	if (m_duration <= 0.0f)
	{
		return false;
	}
	return true;
}

glm::vec4 Particle::GetPosition() const
{
	return m_position;
}

glm::vec4 Particle::GetColor() const
{
	return m_color;
}

glm::vec4 Particle::GetVelocity() const
{
	return m_velocity;
}

float Particle::GetDuration() const
{
	return m_duration;
}

float Particle::GetScale() const
{
	return m_scale;
}

float Particle::GetRotation() const
{
	return m_rotation;
}

float Particle::GetRotationalVelocity() const
{
	return m_rotationalVelocity;
}

void Particle::SetPosition(const glm::vec4& position)
{
	m_position = position;
}

void Particle::SetColor(const glm::vec4& color)
{
	m_color = color;
}

void Particle::SetVelocity(const glm::vec4& velocity)
{
	m_velocity = velocity;
}

void Particle::SetDuration(const float& duration)
{
	m_duration = duration;
}

void Particle::SetScale(const float& scale)
{
	m_scale = scale;
}

void Particle::SetRotation(const float& rotation)
{
	m_rotation = rotation;
}

void Particle::SetRotationalVelocity(const float& rotationalVelocity)
{
	m_rotationalVelocity = rotationalVelocity;
}

void Particle::SetCamera(Camera* cam) {
	m_cam = cam;
}