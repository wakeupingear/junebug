#include "Camera.h"
#include "JGame.h"
#include "Actors.h"
#include "Sprite.h"

using namespace junebug;

Camera::Camera() : Camera(Vec2<>::Zero, Vec2(1.0f, 1.0f))
{
}

Camera::Camera(Vec2<float> pos, Vec2<float> size) : Camera(pos, size, Vec2<>::Zero, size)
{
}
Camera::Camera(Vec2<float> pos, Vec2<int> size) : Camera(pos, Vec2<float>((float)size.x, (float)size.y))
{
}
Camera::Camera(Vec2<int> pos, Vec2<int> size) : Camera(Vec2<float>((float)pos.x, (float)pos.y), Vec2<float>((float)size.x, (float)size.y))
{
}

Camera::Camera(Vec2<float> pos, Vec2<float> size, Vec2<float> screenPos, Vec2<float> screenSize) : pos(pos), size(size), screenPos(screenPos), screenSize(screenSize)
{
    JGame::Get()->AddCamera(this);

    _GuessFractional(pos, fractionalPos);
    _GuessFractional(size, fractionalSize);
    _GuessFractional(screenPos, fractionalScreenPos);
    _GuessFractional(screenSize, fractionalScreenSize);
}
Camera::Camera(Vec2<int> pos, Vec2<int> size, Vec2<int> screenPos, Vec2<int> screenSize) : Camera(Vec2<float>((float)pos.x, (float)pos.y), Vec2<float>((float)size.x, (float)size.y), Vec2<float>((float)screenPos.x, (float)screenPos.y), Vec2<float>((float)screenSize.x, (float)screenSize.y))
{
}

Camera::~Camera()
{
    JGame::Get()->RemoveCamera(this);
}

SDL_Texture *Camera::Render(SDL_Renderer *renderer)
{
    SDL_Rect r;
    r.x = (int)_calcScreenPos.x;
    r.y = (int)_calcScreenPos.y;
    r.w = (int)_calcScreenSize.x;
    r.h = (int)_calcScreenSize.y;
    SDL_RenderSetViewport(renderer, &r);

    auto texSize = Vec2<int>(
        _calcSize.x,
        _calcSize.y);
    renderTex = GetRenderTexture(
        texSize,
        renderer, renderTex, true);

    JGame *game = JGame::Get();
    game->SetActiveCamera(this);
    for (PureActor *actor : game->GetActors())
    {
        VisualActor *visualActor = dynamic_cast<VisualActor *>(actor);
        if (visualActor && visualActor->IsVisible())
        {
            visualActor->Draw();
        }
    }

    return renderTex;
}

void Camera::_UpdateCoordinates()
{
    float screenWidth = JGame::Get()->GetScreenWidth(), renderWidth = JGame::Get()->GetRenderWidth();
    float screenHeight = JGame::Get()->GetScreenHeight(), renderHeight = JGame::Get()->GetRenderHeight();

    if (JGame::Get()->GetOptions().screenStretch)
    {
        _UpdateCoordinate(pos, _calcPos, fractionalPos, renderWidth, renderHeight);
        _UpdateCoordinate(size, _calcSize, fractionalSize, renderWidth, renderHeight);
    }
    else
    {
        _UpdateCoordinate(pos, _calcPos, fractionalPos, screenWidth, screenHeight);
        _UpdateCoordinate(size, _calcSize, fractionalSize, screenWidth, screenHeight);
    }
    _UpdateCoordinate(screenPos, _calcScreenPos, fractionalScreenPos, screenWidth, screenHeight);
    _UpdateCoordinate(screenSize, _calcScreenSize, fractionalScreenSize, screenWidth, screenHeight);
}

void Camera::_UpdateCoordinate(Vec2<float> &vec, Vec2<float> &outVec, Vec2<bool> &fractional, float w, float h)
{
    if (JGame::Get()->GetOptions().screenStretch)
    {
        if (!fractional.x)
            outVec.x = vec.x;
        else
            outVec.x = vec.x * w;
        if (!fractional.y)
            outVec.y = vec.y;
        else
            outVec.y = vec.y * h;
    }
    else
    {
        if (!fractional.x)
            outVec.x = vec.x;
        else
            outVec.x = vec.x * w;
        if (!fractional.y)
            outVec.y = vec.y;
        else
            outVec.y = vec.y * h;
    }
}

void Camera::_GuessFractional(Vec2<float> &vec, Vec2<bool> &outVec)
{
    outVec.x = (vec.x > 0.0f, vec.x <= 1.0f);
    outVec.y = (vec.y > 0.0f && vec.y <= 1.0f);
}