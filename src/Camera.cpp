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

void Camera::SetPosition(Vec2<float> newPos)
{
    pos = newPos;

    if (Game::Get()->GetMouseCamera() == this)
    {
        Game::Get()->SetMousePos(Vec2<int>((int)GetPosition().x, (int)GetPosition().y) + Game::Get()->GetMouseOffset());
    }
}

void Camera::SetSize(Vec2<float> newSize)
{
    size = newSize;
}

SDL_Texture *Camera::Render(SDL_Renderer *renderer, float dt)
{
    Game *game = Game::Get();

    CheckBounds();

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

    Vec2<float> oldPos = GetPosition();
    for (auto it = mShakeEntries.begin(); it != mShakeEntries.end();)
    {
        auto &shakeEntry = *it;
        SetPosition(
            GetPosition() +
            Vec2(
                Random::GetIntRange(-shakeEntry.first.x, shakeEntry.first.x),
                Random::GetIntRange(-shakeEntry.first.y, shakeEntry.first.y)));
        shakeEntry.second -= dt;
        if (shakeEntry.second <= 0.0f)
            it = mShakeEntries.erase(it);
        else
            ++it;
    }

    game->SetActiveCamera(this);
    for (PureActor *actor : game->GetActors())
    {
        VisualActor *visualActor = dynamic_cast<VisualActor *>(actor);
        if (visualActor)
        {
            if (visualActor->Visible())
                visualActor->Draw();
            visualActor->mPrevPosition = visualActor->GetPosition();
        }
    }

    if (game->Options().drawColliders)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (auto &layer : game->GetCollLayers())
        {
            for (auto &comp : layer.second)
            {
                comp->Draw();
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    SetPosition(oldPos);

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
    return GetPosition() + GetSize() * 0.5f * mZoom;
}
const Vec2<float> Camera::GetBottomRight()
{
    return GetPosition() + GetSize() * mZoom;
}

const Vec2<float> Camera::GetScreenCenter()
{
    return GetScreenPos() + GetScreenSize() * 0.5f;
}
const Vec2<float> Camera::GetScreenBottomRight()
{
    return GetScreenPos() + GetScreenSize();
}

void Camera::CheckBounds()
{
    if (mStayInBounds)
    {
        pos.x = Clamp(pos.x, mBoundsStartOffset.x, Max((float)Game::Get()->GetSceneSize().x - GetSize().x + mBoundsEndOffset.x, 0.0f));
        pos.y = Clamp(pos.y, mBoundsStartOffset.y, Max((float)Game::Get()->GetSceneSize().y - GetSize().y + mBoundsEndOffset.y, 0.0f));
    }
}

void Camera::SetZoom(float zoom)
{
    zoom = std::max(zoom, -mZoom + 0.01f);
    float zoomDiff = zoom - mZoom;
    mZoom = zoom;
    SetPosition(pos + size / 2.0f * zoomDiff);
    CheckBounds();
}

void Camera::Shake(Vec2<int> intensity, float duration)
{
    mShakeEntries.push_back(std::make_pair(intensity, duration));
}