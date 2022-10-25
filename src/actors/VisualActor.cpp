#include "Actors.h"
#include "Component.h"

#include <algorithm>

using namespace junebug;

VisualActor::VisualActor(Vector2 pos) : PureActor()
{
    SetPosition(pos);
}

void VisualActor::SetPosition(const Vector2 &pos)
{
    mPosition = pos;
}

Vector2 VisualActor::GetPosition() const
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

void VisualActor::SetScale(const Vector2 &scale)
{
    mScale = scale;
}

Vector2 VisualActor::GetScale() const
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