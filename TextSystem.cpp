//------------------------------------------------------------------------------
//
// File Name:	TextSystem.cpp
// Author(s):	Trey Mongeon
// Project:		Long March
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Render.h"
#include "TextSystem.h"
#include "Entity.h"
#include "Transform.h"
#include "Shader.h"
#include "Sprite.h"
#include "glfw/glfw3.h"
#include "SpriteSheet.h"
#include "Camera.h"
#include "PubSub.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GraphicsSetup.h"
#include "ScreenTextSystem.h"
#include "Engine.h"


TextSystem::TextSystem() :	System("TextSystem",
							SystemType::stText),
							m_mesh(nullptr),
							m_texture(),
							m_textShaderWorld()
{
}

// Call within Init
void TextSystem::InitMesh(float xHalfSize, float yHalfSize, float rows, float cols)
{
	m_mesh = new SpriteSheet(xHalfSize, yHalfSize, rows, cols);
}

// Call within Init
void TextSystem::InitTexture(const char* filePath)
{
	m_texture = new Texture(filePath);
}

bool TextSystem::Init()
{
	m_textShaderWorld = new Shader("shaders/vertexShaderTextWorldSpace.glsl",
		"shaders/fragmentShaderText.glsl");
	m_textShaderWorld->AddMapUniform("UVoffset");
	m_textShaderWorld->AddMapUniform("color");
	m_textShaderWorld->AddMapUniform("kerningOffset");
	m_textShaderWorld->AddMapUniform("cols");
	m_textShaderWorld->AddMapUniform("projection");
	m_textShaderWorld->AddMapUniform("view");
	m_textShaderWorld->AddMapUniform("model");

	m_textShaderWorld ->Use();
	m_textShaderWorld->Setfloat("cols", 16.0f);
	m_textShaderWorld->StopUsing();

	InitTexture("assets/images/Roboto_Mono_black-2048x1024.png");
	InitMesh(1.0f, 1.0f, 8.0f, 16.0f);

	return true;
}

void TextSystem::Update(double dt)
{
}

void TextSystem::Render()
{
	m_textShaderWorld->Use();
	RenderTextWorldSpace();
	m_textShaderWorld->StopUsing();

	m_TextInfoList_world.clear();
}


void TextSystem::RenderTextWorldSpace()
{

	// Loop through list of text scructs and tries to render them
	for (int i = 0; i < m_TextInfoList_world.size(); ++i)
	{
		Camera* cam = m_TextInfoList_world[i].second;

		if (!cam) {
			continue;
		}

		int uKerningOffset = m_textShaderWorld->GetMapUniform("kerningOffset");
		float kerningOffset = ROBOTO_MONO_KERNING;

		glUniform1f(uKerningOffset, kerningOffset * m_TextInfoList_world[i].first.m_scale.x);

		for (int n = 0; n < m_TextInfoList_world[i].first.m_text.size(); ++n)
		{
			for (int j = 0; j < m_TextInfoList_world[i].first.m_text[n].size(); ++j)
			{
				// Using ASCII values to get the index of the character to then index into the sprite sheet
				int index = m_TextInfoList_world[i].first.m_text[n][j] - ' ';
				if (dynamic_cast<SpriteSheet*>(m_mesh))
					dynamic_cast<SpriteSheet*>(m_mesh)->SetFrameIndex(index);

				if (m_mesh == nullptr)
				{
					PUB_BASE("Engine").PublishData("Debug", DataMessage<std::string>
						("TextSystem::Render Error:: m_mesh could not be cast to a spritesheet or was null"));
					return;
				}

				glm::vec2 uvOffset = m_mesh->GetUVoffset();

				// Bind vertex array
				glBindVertexArray(m_mesh->MeshGetVertexArray());

				if (m_texture == NULL)
				{
					PUB_BASE("Engine").PublishData("Debug", DataMessage<std::string>
						("TextSystem::Render Error:: null texture passed for world text rendering"));
					glBindTexture(GL_TEXTURE_2D, 0);
					return;
				}
				else
				{
					// Bind the texture
					glBindTexture(GL_TEXTURE_2D, m_texture->GetID());
					if (m_textShaderWorld->GetUVOffset() != -1)
					{
						// Passing uvOffset to shader
						glUniform2f(m_textShaderWorld->GetUVOffset(), uvOffset.x, uvOffset.y);
					}
				}

				// Passing color to shader
				glUniform4f(m_textShaderWorld->GetColor(), m_TextInfoList_world[i].first.m_color.r,
					m_TextInfoList_world[i].first.m_color.g,
					m_TextInfoList_world[i].first.m_color.b, m_TextInfoList_world[i].first.m_color.a);

				// Model matrix is dependent on which text alignment is used
				glm::mat4 modelMat = glm::mat4(1.0f);
				

				//RIGHT ALIGNED TEXT
				if (m_TextInfoList_world[i].first.rightAligned)
				{
					modelMat = glm::translate(modelMat,
						glm::vec3{ m_TextInfoList_world[i].first.m_pos.x - (m_TextInfoList_world[i].first.m_text[n].size() - j) * m_TextInfoList_world[i].first.m_scale.x * 1.2f,
						m_TextInfoList_world[i].first.m_pos.y - n * m_TextInfoList_world[i].first.m_scale.y * m_TextInfoList_world[i].first.m_lineSpacing,
						m_TextInfoList_world[i].first.m_pos.z });
				}
				//CENTER ALIGNED TEXT
				else if (m_TextInfoList_world[i].first.centerAligned)
				{
					modelMat = glm::translate(modelMat,
						glm::vec3{ m_TextInfoList_world[i].first.m_pos.x + j * m_TextInfoList_world[i].first.m_scale.x * 1.2f - ((static_cast<float>(m_TextInfoList_world[i].first.m_text[n].size())/2.0f) * m_TextInfoList_world[i].first.m_scale.x * 1.2f) + (0.5f * m_TextInfoList_world[i].first.m_scale.x * 1.2f),
						m_TextInfoList_world[i].first.m_pos.y - n * m_TextInfoList_world[i].first.m_scale.y * m_TextInfoList_world[i].first.m_lineSpacing,
						m_TextInfoList_world[i].first.m_pos.z });
				}
				//LEFT ALIGNED TEXT
				else
				{
					modelMat = glm::translate(modelMat,
						glm::vec3{ m_TextInfoList_world[i].first.m_pos.x + j * m_TextInfoList_world[i].first.m_scale.x * 1.2f,
						m_TextInfoList_world[i].first.m_pos.y - n * m_TextInfoList_world[i].first.m_scale.y * m_TextInfoList_world[i].first.m_lineSpacing,
						m_TextInfoList_world[i].first.m_pos.z });
				}

				// MVP matrices need to be given to the shader
				modelMat = glm::scale(modelMat, glm::vec3(m_TextInfoList_world[i].first.m_scale, 1.0f));

				m_textShaderWorld->SetMatrixUniform(m_textShaderWorld->GetModel(), modelMat);

				m_textShaderWorld->SetMatrixUniform(m_textShaderWorld->GetView(), cam->_getView());

				m_textShaderWorld->SetMatrixUniform(m_textShaderWorld->GetProjection(), cam->_getProjection());

				// We dont want text to draw under anything so turn it off for drawing it.
				if (!m_TextInfoList_world[i].first.enableDepthTest) {
					disableDepthTest();
				}

				glDrawArrays(GL_TRIANGLES, 0, m_mesh->MeshGetVertexCount());

				if (!m_TextInfoList_world[i].first.enableDepthTest) {
					enableDepthTest();
				}


				// Stop using things
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
}

void TextSystem::Exit()
{
	delete m_mesh;
	delete m_textShaderWorld;
	delete m_texture;
}

// Screen space text is handed off to the screen space text system, which is basically the same
void TextSystem::RequestDrawText(const std::vector<std::string> text, const glm::vec2& pos,
	const glm::vec2& scale, const glm::vec4& color, bool rightAlign, float lineSpacing)
{
	ScreenTextSystem* screenTS = dynamic_cast<ScreenTextSystem*>(EngineGetSystem(SystemType::stScreenText));
	screenTS->RequestDrawText(text, pos, scale, color, rightAlign, lineSpacing);
}

// Call this function to draw text!
void TextSystem::RequestDrawTextWorld(const std::vector<std::string> text, const glm::vec3& pos,
	const glm::vec2& scale, const glm::vec4& color, Camera* cam, bool enableDepthTest, bool rightAlign, bool centerAlign, float lineSpacing) {
	TextInfo info;
	info.m_text = text;
	info.m_pos = pos;
	info.m_scale = scale;
	info.m_color = color;
	info.m_lineSpacing = lineSpacing;
	info.enableDepthTest = enableDepthTest;
	info.rightAligned = rightAlign;
	info.centerAligned = centerAlign;

	m_TextInfoList_world.push_back(std::make_pair(info, cam));
}
