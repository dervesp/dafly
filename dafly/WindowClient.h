#pragma once
#include "libutil.h"
#include "LandscapeSurface.h"
#include "ProgramContext.h"
#include "ControlledActor.h"
#include "ICollider.h"
#include "Gun.h"
#include <vector>

class CWindowClient
        : public CAbstractWindowClient, ICollider
{
public:
    CWindowClient(CWindow &window);

protected:
    // IWindowClient interface
    void OnUpdateWindow(float deltaSeconds) override;
    void OnDrawWindow() override;
    void OnKeyDown(const SDL_KeyboardEvent &) override;
    void OnKeyUp(const SDL_KeyboardEvent &) override;

	// ICollider interface
	bool CheckCollisions(const glm::vec3 & position) const;

private:
    void SetupView(const glm::ivec2 &size);
    void SetupLight0();

    // Данный VAO будет объектом по-умолчанию.
    // Его привязка должна произойти до первой привязки VBO.
    //  http://stackoverflow.com/questions/13403807/
    CArrayObject m_defaultVAO;

	CLandscapeSurface m_surface;
	CControlledActor m_player;
    CActorCamera m_camera;
	CGun m_gun;
    CDirectedLightSource m_sunlight;
    CProgramContext m_programContext;
};
