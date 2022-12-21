#include "Background.h"
#include "Game.h"
#include "Camera.h"

using namespace junebug;

Background::Background(Vec2<float> pos) : Background()
{
}
Background::Background(std::string sprite, Vec2<float> rate, Vec2<float> offset) : VisualActor(Vec2<float>::Zero, sprite)
{
    mRate = rate;
    mOffset = offset;
}
Background::Background(std::string sprite, float rate, Vec2<float> offset) : Background(sprite, Vec2<float>(rate, rate), offset)
{
}

void Background::Draw()
{
    PositionBackground();
    VisualActor::Draw();
}

void Background::PositionBackground()
{
    Camera *camera = Game::Get()->GetActiveCamera();

    Vec2<float> size = GetActorSize();
    if (size.x < 0.001f || size.y < 0.001f)
        return;

    Vec2<float> pos = mOffset + camera->GetPosition() * mRate;
    if (mTile.x)
    {
        while (pos.x + size.x < camera->GetPosition().x)
            pos.x += size.x;
        pos.x -= mCullPaddingStart.x * size.x;
    }
    if (mTile.y)
    {
        while (pos.y + size.y < camera->GetPosition().y)
            pos.y += size.y;
        pos.y -= mCullPaddingStart.y * size.y;
    }

    float startY = pos.y;
    Vec2<float>
        edge = camera->GetBottomRight();

    do
    {
        do
        {
            SetPosition(pos);
            VisualActor::Draw();
            pos.y += size.y;
        } while (size.y > 0 && mTile.y && pos.y < edge.y + mCullPaddingEnd.y * size.y);
        pos.y = startY;
        pos.x += size.x;
    } while (size.x > 0 && mTile.x && pos.x < edge.x + mCullPaddingEnd.x * size.x);
}