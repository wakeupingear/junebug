#include "Background.h"
#include "Game.h"
#include "Camera.h"
#include "Sprite.h"

using namespace junebug;

Background::Background(std::string sprite, Vec2<float> rate, Vec2<float> offset) : VisualActor(Vec2<float>::Zero, sprite)
{
    mRate = rate;
    mOffset = offset;
}
Background::Background(std::string sprite, float rate, Vec2<float> offset) : Background(sprite, Vec2<float>(rate, rate), offset)
{
}

// Set the mRoundToCamera bool here
// Using the FirstUpdate() function since it is called after member variables are set
void Background::FirstUpdate(float dt)
{
    mRoundToCamera = true;
}

void Background::Draw()
{
    if (mSpritePath != "")
        DrawSpriteBackground();
    else
        DrawColor();
}

void Background::DrawSpriteBackground()
{
    Camera *camera = Game::Get()->GetActiveCamera();
    if (!camera)
        return;

    Vec2<float> size = GetActorSize().Round(mSpacingRoundDir);
    SetScale(size / GetSprite()->GetTexSize());

    Vec2<float> pos = (mOffset * camera->GetZoom() + camera->GetPosition() * mRate);
    if (mCenterTopLeft)
        pos -= Vec2<float>(GetSprite()->GetOrigin() * GetScale());

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
    Vec2<float> edge = GetBackgroundEdge();
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

void Background::DrawColor()
{
    Camera *camera = Game::Get()->GetActiveCamera();
    if (!camera)
        return;

    DrawRectangle(camera->GetPosition(), GetBackgroundEdge(), mColor);
}

Vec2<float> Background::GetBackgroundEdge()
{
    Camera *camera = Game::Get()->GetActiveCamera();
    if (!camera)
        return Vec2<float>::Zero;

    Vec2<float>
        edge = camera->GetBottomRight();
    if (!mDrawOutsideScene)
    {
        edge = Vec2<float>::Min(edge, Vec2<float>(Game::Get()->GetSceneSize()));
    }
    return edge;
}