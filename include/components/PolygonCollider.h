#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "components/CollisionComponent.h"

#include <vector>

namespace junebug
{
    struct PolygonCollisionBounds
    {
        const std::vector<Vec2<double>> *vertices = nullptr;
        std::vector<Vec2<double>> worldVertices, axes;
        Vec2<float> topLeft, bottomRight;

        void LoadVertices(const std::vector<Vec2<double>> &vertices);

        bool CheckAxes(const PolygonCollisionBounds &other, double &overlap, Vec2<double> &minAxis, Vec2<double> offset = Vec2<double>::Zero, Vec2<double> otherOffset = Vec2<double>::Zero);

        Vec2<double> Project(Vec2<double> &axis, Vec2<double> &offset) const;

        void UpdateWorldVertices(const Vec2<float> &pos, float rot, const Vec2<float> &scale, const Vec2<int> &origin);
    };

    class PolygonCollider : public CollisionComponent
    {
    public:
        PolygonCollider(class VisualActor *owner, std::string layer = "");

        void Update(float dt) override;

        bool Intersects(CollisionComponent *other) override;
        CollSide Intersects(CollisionComponent *other, Vec2<float> &offset) override;

        void UpdateCollPositions(Vec2<float> offset = Vec2<float>::Zero) override;

        void Draw() override;

        const PolygonCollisionBounds &GetCollBounds() const { return mCollBounds; }

    protected:
        friend class TileCollider;

        PolygonCollisionBounds mCollBounds;
        std::string mParentSprite{""};
    };
};