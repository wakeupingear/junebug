#include "Actors.h"
#include "Component.h"
#include "Sprite.h"
#include "JGame.h"

#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

using namespace junebug;

VisualActor::VisualActor(Vec2 pos) : PureActor()
{
    SetPosition(pos);
}

VisualActor::VisualActor(Vec2 pos, std::string imagePath, int drawOrder) : VisualActor(pos)
{
    Sprite *spr = new Sprite(this, drawOrder);

    print("Loading image: " + imagePath);
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

void VisualActor::SetPosition(const Vec2 &pos)
{
    mPosition = pos;
}

Vec2 VisualActor::GetPosition() const
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

void VisualActor::SetScale(const Vec2 &scale)
{
    mScale = scale;
}

Vec2 VisualActor::GetScale() const
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