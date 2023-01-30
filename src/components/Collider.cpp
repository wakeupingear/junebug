#include "components/Collider.h"
#include "Actors.h"
#include "Game.h"

using namespace junebug;

Collider::Collider(VisualActor *owner, std::string layer) : Component(owner), mOwner(owner), mLayer(layer)
{
}

Collider::~Collider()
{
    Game::Get()->RemoveCollision(this);
}

void Collider::SetCollLayer(std::string layer)
{
    bool shouldUpdate = mLayer != layer;
    mLayer = layer;
    if (shouldUpdate)
        UpdateCollEntry(false);
}

void Collider::SetType(CollType type)
{
    bool shouldUpdate = mType != type;
    mType = type;
    if (shouldUpdate)
        UpdateCollEntry(false);
}

void Collider::UpdateCollEntry(bool initial)
{
    if (!initial)
        Game::Get()->RemoveCollision(this);
    Game::Get()->AddCollision(this);
}