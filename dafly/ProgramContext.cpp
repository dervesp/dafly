#include "stdafx.h"
#include "ProgramContext.h"

namespace
{
glm::mat4 GetNormalMatrix(const glm::mat4 &modelView)
{
    return glm::transpose(glm::inverse(modelView));
}
}

CProgramContext::CProgramContext()
{
	CTexture2DLoader loader;

	//m_grassTexture = loader.Load("res/grass01.jpg");
	//m_grassSpecularTexture = loader.Load("res/grass01_s.jpg");
	//m_grassNormalTexture = loader.Load("res/grass01_n.jpg");
	m_diffuseTexture = loader.Load("res/texture_diffuse.jpg");

    const auto vertShader = CFilesystemUtils::LoadFileAsString("res/phong.vert");
    const auto fragShader = CFilesystemUtils::LoadFileAsString("res/phong.frag");
    m_programEarth.CompileShader(vertShader, ShaderType::Vertex);
    m_programEarth.CompileShader(fragShader, ShaderType::Fragment);
    m_programEarth.Link();
}

void CProgramContext::Use()
{
	//glActiveTexture(GL_TEXTURE2);
	//m_grassNormalTexture->Bind();
	//glActiveTexture(GL_TEXTURE1);
	//m_grassSpecularTexture->Bind();
	glActiveTexture(GL_TEXTURE1);
	m_diffuseTexture->Bind();
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_programEarth.Use();
	m_programEarth.FindUniform("material.diffuse") = glm::vec4(0.f, 0.353f, 0.051f, 1);
	m_programEarth.FindUniform("material.specular") = glm::vec4(0, 0, 0, 1);
	m_programEarth.FindUniform("material.emissive") = glm::vec4(0, 0, 0, 1);   
	m_programEarth.FindUniform("textureDiffuseMap") = 1; // GL_TEXTURE0
    //m_programEarth.FindUniform("specularMap") = 1; // GL_TEXTURE1
    //m_programEarth.FindUniform("normalMap") = 2; // GL_TEXTURE1


	const glm::mat4 mv = m_view * m_model;
	m_programEarth.FindUniform("view") = m_view;
	m_programEarth.FindUniform("modelView") = mv;
	m_programEarth.FindUniform("normalModelView") = GetNormalMatrix(mv);
	m_programEarth.FindUniform("projection") = m_projection;


	m_programEarth.FindUniform("light0.position") = m_light0.position;
	m_programEarth.FindUniform("light0.diffuse") = m_light0.diffuse;
    m_programEarth.FindUniform("light0.specular") = m_light0.specular;
}

CVertexAttribute CProgramContext::GetPositionAttr() const
{
    return m_programEarth.FindAttribute("vertex");
}

CVertexAttribute CProgramContext::GetNormalAttr() const
{
    return m_programEarth.FindAttribute("normal");
}

CVertexAttribute CProgramContext::GetTexCoordAttr() const
{
    return m_programEarth.FindAttribute("textureUV");
}

const glm::mat4 &CProgramContext::GetModel() const
{
    return m_model;
}

const glm::mat4 &CProgramContext::GetView() const
{
    return m_view;
}

const glm::mat4 &CProgramContext::GetProjection() const
{
    return m_projection;
}

const CProgramContext::SLightSource &CProgramContext::GetLight0() const
{
    return m_light0;
}

void CProgramContext::SetModel(const glm::mat4 &value)
{
    m_model = value;
}

void CProgramContext::SetView(const glm::mat4 &value)
{
    m_view = value;
}

void CProgramContext::SetProjection(const glm::mat4 &value)
{
    m_projection = value;
}

void CProgramContext::SetLight0(const CProgramContext::SLightSource &source)
{
    m_light0 = source;
}
