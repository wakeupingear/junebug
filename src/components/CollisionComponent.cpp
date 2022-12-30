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

void CollisionComponent::SetLayer(std::string layer)
{
    mLayer = layer;
    UpdateCollEntry();
}

void CollisionComponent::UpdateCollEntry(bool initial)
{
    if (!initial)
        Game::Get()->RemoveCollision(this);
    Game::Get()->AddCollision(this);
}