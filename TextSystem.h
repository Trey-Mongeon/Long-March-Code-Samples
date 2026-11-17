//------------------------------------------------------------------------------
//
// File Name:	TextSystem.h
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

#include "Textures.h"
#include "System.h"
#include "glm/glm.hpp"
#include <vector>
#include "Mesh.h"
#include "Camera.h"

#define ROBOTO_MONO_KERNING 0.3f

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
class Shader;

//------------------------------------------------------------------------------
// Public Classess:
//------------------------------------------------------------------------------

class TextSystem : public System
{
public:
	
	TextSystem();
					 
	void InitMesh(float xHalfSize, float yHalfSize, float rows, float cols);

	void InitTexture(const char* filePath);

	bool Init() override;

	void Update(double dt) override;

	void Render() override;

	void Exit() override;

	void RequestDrawText(const std::vector<std::string> text, const glm::vec2& pos, 
		const glm::vec2& scale, const glm::vec4& color, bool rightAlign = false, float lineSpacing = 2.0f);

	void RequestDrawTextWorld(const std::vector<std::string> text, const glm::vec3& pos,
		const glm::vec2& scale, const glm::vec4& color, Camera* cam, bool enableDepthTest,
		bool rightAlign = false, bool centerAlign = false, float lineSpacing = 2.0f);

private:

	struct TextInfo
	{
		std::vector<std::string> m_text;
		glm::vec3 m_pos;
		glm::vec2 m_scale;
		glm::vec4 m_color;
		float m_lineSpacing;
		bool enableDepthTest = false;
		bool rightAligned = false;
		bool centerAligned = false;
	};

	Mesh* m_mesh;

	// Text shader
	Shader* m_textShaderWorld;

	// Font texture
	Texture* m_texture;

	// List of TextInfo structs that should be drawn
	std::vector<std::pair<TextInfo, Camera*>> m_TextInfoList_world;

	// Render things that are waiting in the text info list
	void RenderTextWorldSpace();
};
