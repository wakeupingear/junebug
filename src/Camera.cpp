#include "Camera.h"
#include "JGame.h"
#include "Actors.h"
#include "Sprite.h"

using namespace junebug;

Camera::Camera() : Camera(Vector2::Zero, Vector2((float)JGame::Get()->GetScreenWidth(), (float)JGame::Get()->GetScreenHeight()))
{
}

Camera::Camera(Vector2 pos, Vector2 size) : Camera(pos, size, pos, size)
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

void Camera::Render(SDL_Renderer *renderer)
{
    SDL_Rect r;
    r.x = (int)screenPos.x;
    r.y = (int)screenPos.y;
    r.w = (int)screenSize.x;
    r.h = (int)screenSize.y;
    SDL_RenderSetViewport(renderer, &r);

    renderTex = GetRenderTexture(screenSize, renderer, renderTex, true);

    JGame *game = JGame::Get();
    for (Sprite *sprite : game->GetSprites())
    {
        if (sprite->IsVisible())
            sprite->Draw(this, renderer);
    }

    SDL_SetRenderTarget(renderer, NULL);
}