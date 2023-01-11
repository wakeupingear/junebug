#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "components/CollisionComponent.h"

#include <vector>

namespace junebug
{
    class PolygonCollisionComponent : public CollisionComponent
    {
    public:
        PolygonCollisionComponent(class VisualActor *owner, std::string layer = "");

        void Update(float dt) override;

        virtual bool Intersects(CollisionComponent *other) override;
        virtual CollSide GetMinOverlap(CollisionComponent *other, Vec2<float> &offset) override;

        void UpdateCollPositions() override;

        void Draw() override;

    protected:
        std::vector<Vec2<double>> mVertices, mWorldVertices, mAxes;
        std::string mParentSprite{""};

        CollType mType = CollType::Polygon;

        Vec2<double> Project(Vec2<double> &axis);
        bool CheckAxes(std::vector<Vec2<double>> &axes, PolygonCollisionComponent *other, double &overlap, Vec2<double> &minAxis);

        virtual bool Intersects(CollisionComponent *other, Vec2<float> &offset);
    };
};