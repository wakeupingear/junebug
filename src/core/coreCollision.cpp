#include "Game.h"
#include "components/CollisionComponent.h"

using namespace junebug;

void Game::AddCollision(CollisionComponent *coll)
{
    mCollisionLayers[coll->GetLayer()].push_back(coll);
}

void Game::RemoveCollision(CollisionComponent *coll)
{
    auto &layer = mCollisionLayers[coll->GetLayer()];
    auto it = std::find(layer.begin(), layer.end(), coll);
    if (it != layer.end())
        layer.erase(it);
}

const Game::collision_layers &Game::GetCollisionLayers() const
{
    return mCollisionLayers;
}