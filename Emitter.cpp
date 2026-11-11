//------------------------------------------------------------------------------
//
// File Name:	Emitter.cpp
// Author(s):	Trey Mongeon
// Project:		Long March
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "GraphicsSetup.h"
#include "Emitter.h"
#include "Particle.h"
#include "Random.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Matrices.h"
#include "Camera.h"
#include "PubSub.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "Mesh.h"
#include "Render.h"

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

Emitter::Emitter(float frequency, unsigned int maxParticles,
    const glm::vec4& position, float baseDuration, float xSpawnDist,
    float ySpawnDist, float xVelocityFactor, float yVelocityFactor,
    float zVelocityFactor, const glm::vec4& baseVelocity,
    float xAccelerationFactor, float yAccelerationFactor,
    float zAccelerationFactor, const glm::vec4& baseAcceleration,
    const glm::vec4& baseColor, const glm::vec4& colorIncrement,
    float baseScale, float baseRotation, float baseRotationalVelocity,
    float scalingFactor, float durationFactor, float rotationalVelocityFactor,
    float opacityDecayRate, float accelerationDuration,
    bool useAdditiveBlending, bool writeToDepth,
    bool readFromDepth, Texture* texture, Mesh* mesh, Camera* cam)
    : m_frequency(frequency), m_internalTimer(m_frequency),
    m_maxParticles(maxParticles), m_position(position),
    m_baseDuration(baseDuration), m_xSpawnDist(xSpawnDist),
    m_ySpawnDist(ySpawnDist), m_xVelocityFactor(xVelocityFactor),
    m_yVelocityFactor(yVelocityFactor),
    m_zVelocityFactor(zVelocityFactor), m_baseVelocity(baseVelocity),
    m_xAccelerationFactor(xAccelerationFactor),
    m_yAccelerationFactor(yAccelerationFactor),
    m_zAccelerationFactor(zAccelerationFactor),
    m_baseAcceleration(baseAcceleration),
    m_baseColor(baseColor), m_colorIncrement(colorIncrement),
    m_baseScale(baseScale), m_baseRotation(baseRotation),
    m_baseRotationalVelocity(baseRotationalVelocity),
    m_scalingFactor(scalingFactor), m_durationFactor(durationFactor),
    m_rotationalVelocityFactor(rotationalVelocityFactor),
    m_opacityDecayRate(opacityDecayRate),
    m_accelerationDuration(accelerationDuration),
    m_useAdditiveBlending(useAdditiveBlending),
    m_writeToDepth(writeToDepth), m_readFromDepth(readFromDepth),
    m_texture(texture), m_mesh(mesh), m_camera(cam)
{
}

Emitter::Emitter(std::string name) : Emitter()
{
  JSON stream = JSON("Data/Emitters/" + name + ".json");
  Load(stream);
}

Emitter::Emitter(): Emitter(0.f, 0.f, glm::vec4{ 0.f, 0.f, 0.f, 0.f },
  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, glm::vec4{ 0.f, 0.f, 0.f, 0.f }, 0.f, 0.f, 0.f, glm::vec4{ 0.f, 0.f, 0.f, 0.f },
  glm::vec4{ 0.f, 0.f, 0.f, 0.f }, glm::vec4{ 0.f, 0.f, 0.f, 0.f }, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, false, false,
  false, nullptr, nullptr)
{
}

// Function for loading an emitter from JSON
void Emitter::Load(JSON& stream)
{
  m_frequency = stream.Get<float>("Frequency");
  m_maxParticles = stream.Get<int>("MaxParticles");
  m_baseDuration = stream.Get<float>("BaseDuration");

  m_xSpawnDist = stream.Get<float>("xSpawnDist");
  m_ySpawnDist = stream.Get<float>("ySpawnDist");

  m_xVelocityFactor = stream.Get<float>("xVelocityFactor");
  m_yVelocityFactor = stream.Get<float>("yVelocityFactor");
  m_zVelocityFactor = stream.Get<float>("zVelocityFactor");
  m_baseVelocity = glm::vec4 { 0 };
  if (stream.Contains("BaseVelocity")) {
    m_baseVelocity = glm::vec4(stream.GetVec3("BaseVelocity"), 0.0f);
  }

  m_xAccelerationFactor = stream.Get<float>("xAccelerationFactor");
  m_yAccelerationFactor = stream.Get<float>("yAccelerationFactor");
  m_zAccelerationFactor = stream.Get<float>("zAccelerationFactor");
  m_baseAcceleration = glm::vec4 { 0 };
  if (stream.Contains("BaseAcceleration")) {
    m_baseAcceleration = glm::vec4(stream.GetVec3("BaseAcceleration"), 0.0f);
  }

  m_baseColor = glm::vec4 { 0 };
  if (stream.Contains("BaseColor")) {
    m_baseColor = glm::vec4(stream.GetVec4("BaseColor"));
  }
  m_colorIncrement = glm::vec4 { 0 };
  if (stream.Contains("ColorIncrement")) {
    m_colorIncrement = glm::vec4(stream.GetVec4("ColorIncrement"));
  }

  m_baseScale = stream.Get<float>("BaseScale");
  m_baseRotation = stream.Get<float>("BaseRotation");
  m_baseRotationalVelocity = stream.Get<float>("BaseRotationalVelocity");
  m_scalingFactor = stream.Get<float>("ScalingFactor");
  m_durationFactor = stream.Get<float>("DurationFactor");
  m_rotationalVelocityFactor = stream.Get<float>("RotationalVelocityFactor");
  m_opacityDecayRate = stream.Get<float>("OpacityDecayRate");
  m_accelerationDuration = stream.Get<float>("AccelerationDuration");
  m_useAdditiveBlending= stream.Get<bool>("UseAdditiveBlending");
  m_writeToDepth = stream.Get<bool>("WriteToDepth");
  m_readFromDepth = stream.Get<bool>("ReadFromDepth");

  m_texture = nullptr;
  if (stream.Contains("Texture")) {
    m_textureKey = stream.Get<std::string>("Texture");
    std::string texPath = "assets/images/" + m_textureKey;
    m_texture = GraphicsLibrary::GetTexture(texPath);
  }

  m_meshKey = stream.Get<std::string>("Mesh");
  m_mesh = GraphicsLibrary::GetMesh(m_meshKey);
  if (!m_mesh) {
    m_mesh = new Mesh(0.5f, 0.5f);
    GraphicsLibrary::RegisterMesh(stream.Get<std::string>("ParticleMesh"), m_mesh);
  }
}

// Function for saving an emitter to JSON
Emitter::operator json() const
{
  json jsonConversion;
  jsonConversion["Frequency"] = GetFrequency();
  jsonConversion["MaxParticles"] = GetMaxParticles();
  jsonConversion["BaseDuration"] = GetBaseDuration();

  jsonConversion["xSpawnDist"] = GetXSpawnDist();
  jsonConversion["ySpawnDist"] = GetYSpawnDist();

  jsonConversion["xVelocityFactor"] = GetxVelocityFactor();
  jsonConversion["yVelocityFactor"] = GetyVelocityFactor();
  jsonConversion["zVelocityFactor"] = GetzVelocityFactor();

  glm::vec4 tempVel = GetBaseVelocity();
  jsonConversion["BaseVelocity"].push_back(tempVel.x);
  jsonConversion["BaseVelocity"].push_back(tempVel.y);
  jsonConversion["BaseVelocity"].push_back(tempVel.z);

  jsonConversion["xAccelFactor"] = GetxAccelerationFactor();
  jsonConversion["yAccelFactor"] = GetyAccelerationFactor();
  jsonConversion["zAccelFactor"] = GetzAccelerationFactor();

  glm::vec4 tempAccel = GetBaseAcceleration();
  jsonConversion["BaseAcceleration"].push_back(tempAccel.x);
  jsonConversion["BaseAcceleration"].push_back(tempAccel.y);
  jsonConversion["BaseAcceleration"].push_back(tempAccel.z);

  glm::vec4 tempColor = GetBaseColor();
  jsonConversion["BaseColor"].push_back(tempColor.r);
  jsonConversion["BaseColor"].push_back(tempColor.g);
  jsonConversion["BaseColor"].push_back(tempColor.b);
  jsonConversion["BaseColor"].push_back(tempColor.a);

  glm::vec4 tempColInc = GetColorIncrement();
  jsonConversion["ColorIncrement"].push_back(tempColInc.r);
  jsonConversion["ColorIncrement"].push_back(tempColInc.g);
  jsonConversion["ColorIncrement"].push_back(tempColInc.b);
  jsonConversion["ColorIncrement"].push_back(tempColInc.a);

  jsonConversion["BaseScale"] = GetBaseScale();
  jsonConversion["BaseRotation"] = GetBaseRotation();
  jsonConversion["BaseRotationalVelocity"] = GetBaseRotationalVelocity();
  jsonConversion["ScalingFactor"] = GetScalingFactor();
  jsonConversion["DurationFactor"] = GetDurationFactor();
  jsonConversion["RotationalVelocityFactor"] = GetRotationalVelocityFactor();
  jsonConversion["AccelerationDuration"] = GetAccelerationDuration();
  jsonConversion["OpacityDecayRate"] = GetOpacityDecayRate();
  jsonConversion["UseAdditiveBlending"] = GetAdditiveBlendingFlag();

  jsonConversion["Mesh"] = m_meshKey;
  jsonConversion["Texture"] = m_textureKey;

  return jsonConversion;
}

void Emitter::Update(const glm::vec4 position, float dt)
{
	m_position = position;

    if (m_camera == nullptr)
    {
        Scene* activeScene = SceneSystem::GetUpdatingScene();

        Camera* cam = activeScene->GetCamera();

        SetCamera(cam);
    }

    // Update all particles
    for (std::size_t i = 0; i < m_particleList.size(); ++i)
    {
        m_particleList[i].Update(dt);
    }

    // Delete particles if they have reached the end of their life
    std::vector<Particle>::iterator it = m_particleList.begin();

    while (it != m_particleList.end())
    {
        if (!it->IsAlive())
        {
            it = m_particleList.erase(it);
        }
        else
        {
            ++it;
        }
    }
    m_internalTimer -= dt;
}

void Emitter::Render(Shader* shader)
{
    if (!m_writeToDepth)
    {
        disableDepthWriting();
    }

    if (!m_readFromDepth)
    {
        disableDepthTest();
    }

    if (m_useAdditiveBlending)
    {
        setAdditiveBlending();
    }

    std::vector<Vertex3D> vertices;
    vertices.reserve(m_particleList.size() * 6);

    for (std::size_t i = 0; i < m_particleList.size(); ++i)
    {
        // Add all particle vertices to vertex array
        m_particleList[i].AddToVertices(vertices);
    }

    if (!vertices.empty())
    {
        // Draw all particles in special draw call for particle batch rendering
        DrawParticles(shader, vertices, m_camera, m_texture);
    }

    if (!m_writeToDepth)
    {
        enableDepthWriting();
    }

    if (!m_readFromDepth)
    {
        enableDepthTest();
    }

    if (m_useAdditiveBlending)
    {
        defaultBlendMode();
    }
}

bool Emitter::Emit()
{
  if (m_camera == nullptr) {
    SetCamera();
  }
    if (m_particleList.size() >= m_maxParticles  ||
        m_position == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) && m_particleList.size() <= 1)
        // Second line of if statement is last minute fix to prevent particles from spawning at 0,0,0 before pos is initialized
    {
        return false;
    }
    m_internalTimer = m_frequency;

    float scalingFactor;
    if (GetRandomTF())
    {
        scalingFactor = -m_scalingFactor;
    }
    else
    {
        scalingFactor = m_scalingFactor;
    }

    float rotVelFactor;
    if (GetRandomTF())
    {
        rotVelFactor = -m_rotationalVelocityFactor;
    }
    else
    {
        rotVelFactor = m_rotationalVelocityFactor;
    }


    // LEAST EFFICIENT
    // 
    // Bad because wed be making a rotation matrix for each particle
    /*
    glm::mat4 rotMat = Ident();
    rotMat = glm::rotate(rotMat, glm::radians(GetRandomFloat() * 360.0f)
        , glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec4 startingOffset = rotMat * glm::vec4(m_spawnDist, 0.0f, 0.0f, 1.0f);
    */

    // MORE EFFICIENT
    //
 /*   float mag = 2.0f;
    glm::vec4 startVec;
    while (mag > 1.0f) 
    {
        startVec = glm::vec4(GetRandomFloatNeg1To1(),
            GetRandomFloatNeg1To1(), 0.0f, 1.0f);

        mag = sqrt(startVec.x * startVec.x + startVec.y * startVec.y);
    }*/

    // Gen two rand nums, normalize it, then after, mult 
    // with spawn dist instead of discarding things in a loop

    // BEST WAY
    glm::vec4 startVec = glm::vec4(GetRandomFloatNeg1To1(),
        GetRandomFloatNeg1To1(), 0.0f, 0.0f);

    startVec = glm::normalize(startVec);
    startVec.x *= m_xSpawnDist;
    startVec.y *= m_ySpawnDist;

    Particle newParticle
    (
        this,
        m_position + startVec,

        m_baseColor,
        m_colorIncrement,
        m_baseVelocity * m_baseVelocityModifier + glm::vec4
        (
            GetRandomFloatNeg1To1() * m_xVelocityFactor * m_factorVelocityModifier,
            GetRandomFloatNeg1To1() * m_yVelocityFactor * m_factorVelocityModifier,
            GetRandomFloatNeg1To1() * m_zVelocityFactor * m_factorVelocityModifier,
            0.0f
        ),
        m_baseAcceleration + glm::vec4
        (
            GetRandomFloatNeg1To1() * m_xAccelerationFactor,
            GetRandomFloatNeg1To1() * m_yAccelerationFactor,
            GetRandomFloatNeg1To1() * m_zAccelerationFactor,
            0.0f
        ),

        m_baseDuration + GetRandomFloatNeg1To1() * m_durationFactor,

        m_baseScale + GetRandomFloat() * scalingFactor,

        m_baseRotation + GetRandomFloat() * 0.0f, // m_RotaionFactor could replace the 0 when added

        m_baseRotationalVelocity + GetRandomFloat() * rotVelFactor,

        m_opacityDecayRate, m_accelerationDuration, m_camera
        );

        m_particleList.push_back(newParticle);

        return true;
}

Mesh* Emitter::GetMesh()
{
    return this->m_mesh;
}

Texture* Emitter::GetTexture()
{
    return this->m_texture;
}

Camera* Emitter::GetCamera()
{
    return this->m_camera;
}

void Emitter::SetCamera(Camera* cam)
{
    m_camera = cam;
}

void Emitter::SetCamera()
{
    Scene* activeScene = SceneSystem::GetUpdatingScene();

    m_camera = activeScene->GetCamera();
}

void Emitter::SetMaxParticles(unsigned int maxParticles)
{
    m_maxParticles = maxParticles;
}

void Emitter::SetBaseScale(float scale)
{
    m_baseScale = scale;
}

void Emitter::SetPosition(glm::vec4 newpos)
{
    m_position = newpos;
}

void Emitter::SetScalingFactor(float factor)
{
    m_scalingFactor = factor;
}

void Emitter::ScaleBaseVelocity(float scale)
{
    m_baseVelocityModifier = scale;
}

void Emitter::ScaleVelocityFactor(float scale)
{
  m_factorVelocityModifier = scale;
}
