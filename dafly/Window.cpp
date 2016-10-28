#include "stdafx.h"
#include "Window.h"
#include "IdentitySphere.h"
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
const glm::vec4 BLACK = { 0, 0, 0, 1 };
const glm::vec4 SKYBLUE = { 0.529f, 0.808f, 0.922f, 1 };
const float MATERIAL_SHININESS = 60.f;
const glm::vec4 FADED_WHITE_RGBA = { 0.3f, 0.3f, 0.3f, 1.f };
const glm::vec4 GRASS_GREEN_RGBA = { 0.016f, 0.208f, 0.125f, 1 };
const glm::vec3 SUNLIGHT_DIRECTION = {-1.f, 0.2f, 0.7f};

void SetupOpenGLState()
{
    // включаем механизмы трёхмерного мира.
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    //glCullFace(GL_BACK);

    // включаем систему освещения
    glEnable(GL_LIGHTING);

    // включаем текстурирование в старом стиле (OpenGL 1.1)
    glEnable(GL_TEXTURE_2D);
}
}

CWindow::CWindow()
    : m_sunlight(GL_LIGHT0)
{
    SetBackgroundColor(SKYBLUE);

	const glm::vec4 WHITE_RGBA = { 1, 1, 1, 1 };
	m_material.SetAmbient(GRASS_GREEN_RGBA);
	m_material.SetDiffuse(GRASS_GREEN_RGBA);
	m_material.SetSpecular(FADED_WHITE_RGBA);
	m_material.SetShininess(MATERIAL_SHININESS);

	m_landscape.Tesselate({ -1000, 1000 }, { -1000, 1000 }, 10.f);

    m_sunlight.SetDirection(SUNLIGHT_DIRECTION);
    m_sunlight.SetDiffuse(WHITE_RGBA);
    m_sunlight.SetAmbient(0.3f * WHITE_RGBA);
    m_sunlight.SetSpecular(WHITE_RGBA);
}

void CWindow::OnWindowInit(const glm::ivec2 &size)
{
    (void)size;
    SetupOpenGLState();
}

void CWindow::OnUpdateWindow(float deltaSeconds)
{
    m_camera.Update(deltaSeconds);
	m_landscape.Update(deltaSeconds);
}

void CWindow::OnDrawWindow(const glm::ivec2 &size)
{
    SetupView(size);

    m_sunlight.Setup();
	m_material.Setup();
	m_landscape.Draw();
}

void CWindow::SetupView(const glm::ivec2 &size)
{
    glViewport(0, 0, size.x, size.y);

    // Матрица вида возвращается камерой и составляет
    // начальное значение матрицы GL_MODELVIEW.
    glLoadMatrixf(glm::value_ptr(m_camera.GetViewTransform()));

    // Матрица перспективного преобразования вычисляется функцией
    // glm::perspective, принимающей угол обзора, соотношение ширины
    // и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
    const float fieldOfView = glm::radians(70.f);
    const float aspect = float(size.x) / float(size.y);
    const float zNear = 0.01f;
    const float zFar = 1000.f;
    const glm::mat4 proj = glm::perspective(fieldOfView, aspect, zNear, zFar);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(proj));
    glMatrixMode(GL_MODELVIEW);
}

void CWindow::OnKeyDown(const SDL_KeyboardEvent &event)
{
    m_camera.OnKeyDown(event);
}

void CWindow::OnKeyUp(const SDL_KeyboardEvent &event)
{
    m_camera.OnKeyUp(event);
}
