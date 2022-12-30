#include "Game.h"
#include "components/CollisionComponent.h"

using namespace junebug;

void Game::AddCollision(CollisionComponent *coll)
{
    mCollLayers[coll->GetCollLayer()].push_back(coll);
}

void Game::RemoveCollision(CollisionComponent *coll)
{
    auto &layer = mCollLayers[coll->GetCollLayer()];
    auto it = std::find(layer.begin(), layer.end(), coll);
    if (it != layer.end())
        layer.erase(it);
}

const Game::collision_layers &Game::GetCollLayers() const
{
    return mCollLayers;
}