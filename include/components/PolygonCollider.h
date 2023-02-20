#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "components/Collider.h"

#include <vector>

namespace junebug
{
    struct PolygonCollisionBounds
    {
        const Vertices *vertices = nullptr;
        Vertices worldVertices, axes;
        Vec2<float> topLeft, bottomRight;

        void LoadVertices(const Vertices &vertices);

        bool CheckAxes(const PolygonCollisionBounds &other, double &overlap, Vec2<double> &minAxis, Vec2<double> offset = Vec2<double>::Zero, Vec2<double> otherOffset = Vec2<double>::Zero);

        Vec2<double> Project(Vec2<double> &axis, Vec2<double> &offset) const;

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
        friend class TileIndividualCollider;

        PolygonCollisionBounds mCollBounds;
        std::string mParentSprite{""};
    };
};