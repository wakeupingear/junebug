#include "Camera.h"
#include "Game.h"
#include "Actors.h"
#include "Sprite.h"
#include "MathLib.h"

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
    Game::Get()->AddCamera(this);

    _GuessFractional(screenPos, fractionalScreenPos);
    _GuessFractional(screenSize, fractionalScreenSize);
}
Camera::Camera(Vec2<int> pos, Vec2<int> size, Vec2<int> screenPos, Vec2<int> screenSize) : Camera(Vec2<float>((float)pos.x, (float)pos.y), Vec2<float>((float)size.x, (float)size.y), Vec2<float>((float)screenPos.x, (float)screenPos.y), Vec2<float>((float)screenSize.x, (float)screenSize.y))
{
}

Camera::~Camera()
{
    Game::Get()->RemoveCamera(this);
}

SDL_Texture *Camera::Render(SDL_Renderer *renderer)
{
    Game *game = Game::Get();

    // Stay in bounds
    if (mStayInBounds)
    {
        pos.x = Clamp(pos.x, mBoundsStartOffset.x, (float)game->GetSceneSize().x - size.x + mBoundsStartOffset.x);
        pos.y = Clamp(pos.y, mBoundsStartOffset.y, (float)game->GetSceneSize().y - size.y + mBoundsStartOffset.y);
    }

    SDL_Rect r;
    r.x = (int)_calcScreenPos.x;
    r.y = (int)_calcScreenPos.y;
    r.w = (int)_calcScreenSize.x;
    r.h = (int)_calcScreenSize.y;
    SDL_RenderSetViewport(renderer, &r);

    auto texSize = Vec2<int>(size.x, size.y);
    renderTex = GetRenderTexture(
        texSize,
        renderer, renderTex, true);

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
    float screenWidth = Game::Get()->GetScreenWidth(), renderWidth = Game::Get()->GetRenderWidth();
    float screenHeight = Game::Get()->GetScreenHeight(), renderHeight = Game::Get()->GetRenderHeight();

    _UpdateCoordinate(screenPos, _calcScreenPos, fractionalScreenPos, screenWidth, screenHeight);
    _UpdateCoordinate(screenSize, _calcScreenSize, fractionalScreenSize, screenWidth, screenHeight);
}

void Camera::_UpdateCoordinate(Vec2<float> &vec, Vec2<float> &outVec, Vec2<bool> &fractional, float w, float h)
{
    if (Game::Get()->GetOptions().screenStretch)
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

const Vec2<float> Camera::GetCenter()
{
    return GetPosition() + GetSize() * 0.5f;
}
const Vec2<float> Camera::GetBottomRight()
{
    return GetPosition() + GetSize();
}

const Vec2<float> Camera::GetScreenCenter()
{
    return GetScreenPos() + GetScreenSize() * 0.5f;
}
const Vec2<float> Camera::GetScreenBottomRight()
{
    return GetScreenPos() + GetScreenSize();
}