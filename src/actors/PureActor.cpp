#include "Actors.h"
#include "Game.h"
#include "Component.h"

using namespace junebug;

PureActor::PureActor()
{
    Game::Get()->AddActor(this);
}

PureActor::PureActor(Vec2<float> pos) : PureActor()
{
}

PureActor::~PureActor()
{
    for (Component *comp : mComponents)
        delete comp;
    mComponents.clear();
    Game::Get()->RemoveActor(this);
}

void PureActor::InternalUpdate(float dt)
{
    Update(dt);
}

void PureActor::AddComponent(Component *c)
{
    mComponents.emplace_back(c);
    std::sort(mComponents.begin(), mComponents.end(), [](Component *a, Component *b)
              { return a->GetUpdateOrder() < b->GetUpdateOrder(); });
}