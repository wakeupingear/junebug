#include "Actors.h"
#include "Component.h"
#include "Sprite.h"
#include "JGame.h"

#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

using namespace junebug;

VisualActor::VisualActor(Vec2<float> pos) : PureActor()
{
    SetPosition(pos);
}
VisualActor::VisualActor(Vec2<int> pos) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y))
{
}

VisualActor::VisualActor(Vec2<float> pos, std::string imagePath, int drawOrder) : VisualActor(pos)
{
    Sprite *spr = new Sprite(this, drawOrder);

    const fs::path path(imagePath);
    std::error_code ec;
    if (fs::is_directory(path, ec))
    {
        spr->SetAnimation(imagePath);
    }
    if (ec)
        std::cerr << "Error in is_directory: " << ec.message() << std::endl;
    if (fs::is_regular_file(path, ec))
    {
        spr->SetTexture(JGame::Get()->GetTexture(imagePath));
    }
    if (ec)
        std::cerr << "Error in is_regular_file: " << ec.message() << std::endl;
}
VisualActor::VisualActor(Vec2<int> pos, std::string imagePath, int drawOrder) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y), imagePath, drawOrder)
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

void VisualActor::AddComponent(Component *c)
{
    mComponents.emplace_back(c);
    std::sort(mComponents.begin(), mComponents.end(), [](Component *a, Component *b)
              { return a->GetUpdateOrder() < b->GetUpdateOrder(); });
}