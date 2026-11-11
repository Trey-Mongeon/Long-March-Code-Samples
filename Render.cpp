//------------------------------------------------------------------------------
//
// File Name:	Render.cpp
// Author(s):	Trey Mongeon
// Project:		Long March
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "GraphicsSetup.h"
#include "Render.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "PubSub.h"
#include "GLWrappers.h"
#include "Matrices.h"
#include "TraceSystem.h"

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

void Draw(Shader* shader, Mesh* mesh, const glm::mat4& model, Camera* cam, Texture* texture, glm::vec2 uvOffset,
	glm::vec4 color)
{
	if (mesh == nullptr)
	{
		Trace("Rendering", "Draw called without mesh, did not draw", WarningLevel::wlWarning);
		return;
	}

	if (shader == nullptr)
	{
		Trace("Rendering", "Draw called without shader, used default", WarningLevel::wlMessage);
		shader = shader->BaseShader();
	}

	// Use the shader that was passed in
	shader->Use();

	BindVertexArray(mesh->MeshGetVertexArray());

	if (texture == nullptr)
	{
		// Allow null textures. Just unbind
		Unbind2DTexture();
	}
	else
	{
		Bind2DTexture(texture);
		if (shader->GetUVOffset() != -1)
		{
			shader->Set2float("UVoffset", uvOffset.x, uvOffset.y);
		}
	}

	if (shader->GetColor() != -1)
	{
		shader->Set4float("color", color.r, color.g, color.b, color.a);
	}

	// Give shader mvp matrices
	shader->SetMatrixUniform(shader->GetModel(), model);

	shader->SetMatrixUniform(shader->GetView(), cam->_getView());											
																											
	shader->SetMatrixUniform(shader->GetProjection(), cam->_getProjection());

	// Draw
	DefaultDrawTriangleList(mesh);

	// Stop using things
	shader->StopUsing();
	UnbindVertexArray();
	Unbind2DTexture();
}

void DrawParticles(Shader* shader, std::vector<Vertex3D> verts, Camera* cam, Texture* texture)
{
	if (verts.empty())
	{
		return;
	}

	if (shader == nullptr)
	{
		Trace("Rendering", "DrawPartices called without shader", WarningLevel::wlWarning);
		return;
	}

	// Use the passed in shader
	shader->Use();

	// Create a mesh from the vertices of the particles
	Mesh mesh(verts);

	// Bind the mesh vertex array
	BindVertexArray(mesh.MeshGetVertexArray());

	if (texture == nullptr)
	{
		// Unbind texture if null
		Unbind2DTexture();
	}
	else
	{
		Bind2DTexture(texture);
		if (shader->GetUVOffset() != -1)
		{
			shader->Set2float("UVoffset", 0.0, 0.0);
		}
	}

	// Give shader view and projection matrices
	shader->SetMatrixUniform(shader->GetView(), cam->_getView());

	shader->SetMatrixUniform(shader->GetProjection(), cam->_getProjection());

	// Draw
	DefaultDrawTriangleList(&mesh);

	// Stop using things
	shader->StopUsing();
	UnbindVertexArray();
	Unbind2DTexture();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

