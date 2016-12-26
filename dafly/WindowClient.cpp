#include "stdafx.h"
#include "WindowClient.h"
#include "Renderer3D.h"

using glm::mat4;
using glm::vec3;

namespace
{
const float SURFACE_SCALE = 100.f;

void SetupOpenGLState()
{
    // включаем механизмы трёхмерного мира.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}
}

CWindowClient::CWindowClient(CWindow &window)
    : CAbstractWindowClient(window)
    , m_defaultVAO(CArrayObject::do_bind_tag())
	, m_surface(SURFACE_SCALE)
	, m_sunlight(GL_LIGHT0)
	, m_player(*this, { 0, 2000, 0 }, { M_PI, 0, 0 })
	, m_camera(m_player)
	, m_gun(m_player)
{
    const glm::vec3 SUNLIGHT_DIRECTION = {-1.f, -1.f, -1.f};
    const glm::vec4 WHITE_RGBA = { 1, 1, 1, 1 };
	const glm::vec4 BLACK_RGBA = { 0, 0, 0, 1 };
	const glm::vec4 SKYBLUE_RGBA = { 0.529f, 0.808f, 0.922f, 1.f };

    window.SetBackgroundColor(SKYBLUE_RGBA);
    SetupOpenGLState();

    m_sunlight.SetDirection(SUNLIGHT_DIRECTION);
    m_sunlight.SetDiffuse(WHITE_RGBA);
    m_sunlight.SetSpecular(WHITE_RGBA);
}

bool CWindowClient::CheckCollisions(const glm::vec3 & position) const
{
	return m_surface.CheckCollision(position);
}

void CWindowClient::OnUpdateWindow(float deltaSeconds)
{
	m_player.Update(deltaSeconds);
	m_camera.Update(deltaSeconds);
	//m_gun.Update(deltaSeconds);
}

void CWindowClient::OnDrawWindow()
{
    SetupView(GetWindow().GetWindowSize());
    SetupLight0();

    CRenderer3D renderer(m_programContext);
	m_surface.Draw(renderer);
	//m_gun.Draw(m_programContext, renderer);
}

void CWindowClient::OnKeyDown(const SDL_KeyboardEvent &event)
{
	m_player.OnKeyDown(event);// || m_gun.OnKeyDown(event);
}

void CWindowClient::OnKeyUp(const SDL_KeyboardEvent &event)
{
	m_player.OnKeyUp(event);// || m_gun.OnKeyUp(event);
}

void CWindowClient::SetupView(const glm::ivec2 &size)
{
    const mat4 view = m_camera.GetViewTransform();

    // Матрица перспективного преобразования вычисляется функцией
    // glm::perspective, принимающей угол обзора, соотношение ширины
    // и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
    const float fieldOfView = glm::radians(60.f);
    const float aspect = float(size.x) / float(size.y);
    const float zNear = 1.f;
    const float zFar = 10000.f;
    const mat4 proj = glm::perspective(fieldOfView, aspect, zNear, zFar);

    glViewport(0, 0, size.x, size.y);

    m_programContext.SetView(view);
    m_programContext.SetProjection(proj);
}

void CWindowClient::SetupLight0()
{
    CProgramContext::SLightSource light0;
    light0.specular = m_sunlight.GetSpecular();
    light0.diffuse = m_sunlight.GetDiffuse();
    light0.position = m_sunlight.GetUniformPosition();
    m_programContext.SetLight0(light0);
}
