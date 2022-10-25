#include "Camera.h"
#include "JGame.h"
#include "Actors.h"

using namespace junebug;

Camera::Camera() : Camera(Vector2::Zero, Vector2((float)JGame::Get()->GetScreenWidth(), (float)JGame::Get()->GetScreenHeight()))
{
}

Camera::Camera(Vector2 pos, Vector2 size) : Camera(pos, size, Vector2::Zero, Vector2::Zero)
{
}

Camera::Camera(Vector2 pos, Vector2 size, Vector2 screenPos, Vector2 screenSize) : pos(pos), size(size), screenPos(screenPos), screenSize(screenSize)
{
    JGame::Get()->AddCamera(this);
}

Camera::~Camera()
{
    JGame::Get()->RemoveCamera(this);
}

void Camera::Render()
{
}