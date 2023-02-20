#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "components/PolygonCollider.h"

#include <unordered_map>

namespace junebug
{
    class TileIndividualCollider : public Collider
    {
    public:
        TileIndividualCollider(class VisualActor *owner, std::vector<Vertices> &collisionBounds, std::string layer = "");

        void Update(float dt) override;

        bool Intersects(Collider *other);
        CollSide Intersects(Collider *other, Vec2<float> &offset) override;

        void UpdateCollPositions(Vec2<float> offset = Vec2<float>::Zero) override;

        void Draw() override;

    private:
        class Tileset *mOwner;
        std::string mParentSprite{""};

        std::vector<PolygonCollisionBounds> mColliders;
    };
}