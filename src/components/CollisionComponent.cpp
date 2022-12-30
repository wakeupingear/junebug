#include "components/CollisionComponent.h"
#include "Actors.h"
#include "Game.h"

using namespace junebug;

CollisionComponent::CollisionComponent(VisualActor *owner, std::string layer) : Component(owner), mOwner(owner), mLayer(layer)
{
    UpdateCollEntry(true);
}

CollisionComponent::~CollisionComponent()
{
    Game::Get()->RemoveCollision(this);
}

void CollisionComponent::SetCollLayer(std::string layer)
{
    bool shouldUpdate = mLayer != layer;
    mLayer = layer;
    if (shouldUpdate)
        UpdateCollEntry(false);
}

void CollisionComponent::SetType(CollType type)
{
    bool shouldUpdate = mType != type;
    mType = type;
    if (shouldUpdate)
        UpdateCollEntry(false);
}

void CollisionComponent::UpdateCollEntry(bool initial)
{
    if (!initial)
        Game::Get()->RemoveCollision(this);
    Game::Get()->AddCollision(this);
}