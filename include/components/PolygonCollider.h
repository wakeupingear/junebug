#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <vector>
#include <memory>

#include "components/Collider.h"

namespace junebug
{
    struct PolygonCollisionBounds
    {
        VerticesPtr vertices = nullptr;
        Vertices worldVertices, axes;
        Vec2<float> topLeft, bottomRight;

        void LoadVertices(const VerticesPtr vertices);

        bool CheckAxes(const PolygonCollisionBounds &other, float &overlap, Vec2<float> &minAxis, Vec2<float> offset = Vec2<float>::Zero, Vec2<float> otherOffset = Vec2<float>::Zero);

        Vec2<float> Project(Vec2<float> &axis, Vec2<float> &offset) const;

        void UpdateWorldVertices(const Vec2<float> &pos, float rot, const Vec2<float> &scale, const Vec2<int> &origin);
    };

    class PolygonCollider : public Collider
    {
    public:
        PolygonCollider(class VisualActor *owner, std::string layer = "");

        void Update(float dt) override;

        bool Intersects(Collider *other) override;
        CollSide Intersects(Collider *other, Vec2<float> &offset) override;

        void UpdateCollPositions(Vec2<float> offset = Vec2<float>::Zero) override;

        void Draw() override;

        const PolygonCollisionBounds &GetCollBounds() const { return mCollBounds; }

    protected:
        friend class TileCollider;

        PolygonCollisionBounds mCollBounds;
        std::string mParentSprite{""};
    };
};