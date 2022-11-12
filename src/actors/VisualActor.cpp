#include "Actors.h"
#include "Sprite.h"
#include "JGame.h"
#include "Rendering.h"

using namespace junebug;

VisualActor::VisualActor(Vec2<float> pos) : PureActor()
{
    SetPosition(pos);
}
VisualActor::VisualActor(Vec2<int> pos) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y))
{
}

VisualActor::VisualActor(Vec2<float> pos, std::string imagePath) : VisualActor(pos)
{
    JGame *game = JGame::Get();
    if (game)
        mSpritePath = game->GetAssetPaths().sprites + imagePath;
}
VisualActor::VisualActor(Vec2<int> pos, std::string imagePath) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y), imagePath)
{
}

void VisualActor::SetPosition(const Vec2<float> &pos)
{
    mPosition = pos;
}

Vec2<float> VisualActor::GetPosition() const
{
    return mPosition;
}

void VisualActor::SetRotation(float rot)
{
    mRotation = rot;
}

float VisualActor::GetRotation() const
{
    return mRotation;
}

void VisualActor::SetScale(const Vec2<float> &scale)
{
    mScale = scale;
}

Vec2<float> VisualActor::GetScale() const
{
    return mScale;
}

void VisualActor::SetColor(const Color &color)
{
    mColor = color;
}

Color VisualActor::GetColor() const
{
    return mColor;
}

void VisualActor::Draw()
{
    SpriteDraw(mSpritePath, mPosition, mScale, mRotation, mColor);
}