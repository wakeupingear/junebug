#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <unordered_map>
#include <memory>

#include "components/PolygonCollider.h"

namespace junebug
{
    class TileCollider : public Collider
    {
    public:
        TileCollider(class VisualActor *owner, std::vector<VerticesPtr> &collisionBounds, std::string layer = "");

        void Update(float dt) override;

        bool Intersects(Collider *other);
        CollSide Intersects(Collider *other, Vec2<float> &offset) override;

        void UpdateCollPositions(Vec2<float> offset = Vec2<float>::Zero) override;

        void Draw() override;

    private:
        class Tileset *mOwner;

        std::string mParentSprite{""};
        std::vector<PolygonCollisionBounds> mColliders, mMergedColliders;
        std::vector<VerticesPtr> mMergedColliderVertices;

        void UpdateMergedColliders();
    };
}