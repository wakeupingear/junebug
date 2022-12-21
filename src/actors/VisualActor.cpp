#include "Actors.h"
#include "Sprite.h"
#include "Game.h"
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
    SetSprite(imagePath);
}
VisualActor::VisualActor(Vec2<int> pos, std::string imagePath) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y), imagePath)
{
}

void VisualActor::SetSprite(std::string imagePath)
{
    Game *game = Game::Get();
    if (game)
        mSpritePath = game->GetAssetPaths().sprites + imagePath;
}
Sprite *VisualActor::GetSprite() const
{
    auto it = Game::Get()->GetSpriteCache().find(mSpritePath);
    if (it == Game::Get()->GetSpriteCache().end())
        return nullptr;
    return it->second;
}
std::string VisualActor::GetSpriteName() const
{
    return mSpritePath;
}
Vec2<int> VisualActor::GetSpriteSize() const
{
    Sprite *sprite = GetSprite();
    if (!sprite)
        return Vec2<int>::Zero;
    return Vec2(
        sprite->GetTexWidth(),
        sprite->GetTexHeight());
}
Vec2<float> VisualActor::GetActorSize() const
{
    Sprite *sprite = GetSprite();
    if (!sprite)
        return Vec2<float>::Zero;
    return Vec2(
        mScale.x * sprite->GetTexWidth(),
        mScale.y * sprite->GetTexHeight());
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

void VisualActor::SetAlpha(float alpha)
{
    mColor.a = alpha * 255;
}
void VisualActor::SetAlpha(int alpha)
{
    mColor.a = alpha;
}
int VisualActor::GetAlpha()
{
    return mColor.a;
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
    SpriteDraw(mSpritePath, mPosition, {mScale, mRotation, mColor});
}