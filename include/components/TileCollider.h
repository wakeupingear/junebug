#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "components/CollisionComponent.h"

namespace junebug
{
    class TileCollider : public CollisionComponent
    {
    public:
        TileCollider(class VisualActor *owner, std::string layer = "");

        bool Intersects(CollisionComponent *other);
        CollSide GetMinOverlap(CollisionComponent *other, Vec2<float> &offset);
    
    private:
        class Tileset *mOwner;
    };
}