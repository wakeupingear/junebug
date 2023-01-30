#include "Game.h"
#include "components/Collider.h"

using namespace junebug;

void Game::AddCollision(Collider *coll)
{
    mCollLayers[coll->GetCollLayer()].push_back(coll);
}

void Game::RemoveCollision(Collider *coll)
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