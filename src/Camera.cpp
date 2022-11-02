#include "Camera.h"
#include "JGame.h"
#include "Actors.h"
#include "Sprite.h"

using namespace junebug;

Camera::Camera() : Camera(Vec2::Zero, Vec2((float)JGame::Get()->GetScreenWidth(), (float)JGame::Get()->GetScreenHeight()))
{
}

Camera::Camera(Vec2 pos, Vec2 size) : Camera(pos, size, Vec2::Zero, size)
{
}

Camera::Camera(Vec2 pos, Vec2 size, Vec2 screenPos, Vec2 screenSize) : pos(pos), size(size), screenPos(screenPos), screenSize(screenSize)
{
    JGame::Get()->AddCamera(this);
}

Camera::~Camera()
{
    JGame::Get()->RemoveCamera(this);
}

SDL_Texture *Camera::Render(SDL_Renderer *renderer)
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
        {
            sprite->Draw(this, renderer);
        }
    }

    return renderTex;
}