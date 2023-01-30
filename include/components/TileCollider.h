#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "components/PolygonCollider.h"

#include <unordered_map>

namespace junebug
{
    class TileCollider : public CollisionComponent
    {
    public:
        TileCollider(class VisualActor *owner, std::vector<std::vector<Vec2<double>>> &collisionBounds, std::string layer = "");

        void Update(float dt) override;

        bool Intersects(CollisionComponent *other);
        CollSide Intersects(CollisionComponent *other, Vec2<float> &offset) override;

        void UpdateCollPositions(Vec2<float> offset = Vec2<float>::Zero) override;

        void Draw() override;

    private:
        class Tileset *mOwner;
        std::string mParentSprite{""};

        std::vector<PolygonCollisionBounds> mColliders;
    };
}