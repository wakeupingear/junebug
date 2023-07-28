#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Component.h"
#include "MathLib.h"
#include "Collisions.h"

namespace junebug
{
    const Vertices squareCollider = {
        Vec2<double>(0.0, 0.0),
        Vec2<double>(1.0, 0.0),
        Vec2<double>(1.0, 1.0),
        Vec2<double>(0.0, 1.0)};

    class Collider : public Component
    {
    public:
        Collider(class VisualActor *owner, std::string layer);
        ~Collider();

        void SetCollLayer(std::string layer);
        std::string GetCollLayer() { return mLayer; };

        void SetType(CollType type);
        CollType GetType() { return mType; };

        virtual bool Intersects(Collider *other) = 0;
        virtual CollSide Intersects(Collider *other, Vec2<float> &offset) = 0;

        virtual void UpdateCollPositions(Vec2<float> offset = Vec2<float>::Zero){};

        virtual void Draw(){};

    protected:
        friend class TileCollider;
        VisualActor *mOwner;

        std::string mLayer;
        CollType mType = CollType::None;

        void UpdateCollEntry(bool initial);
    };
};