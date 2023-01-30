#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Component.h"
#include "MathLib.h"
#include "Collisions.h"

namespace junebug
{
    class CollisionComponent : public Component
    {
    public:
        CollisionComponent(class VisualActor *owner, std::string layer);
        ~CollisionComponent();

        void SetCollLayer(std::string layer);
        std::string GetCollLayer() { return mLayer; };

        void SetType(CollType type);
        CollType GetType() { return mType; };

        virtual bool Intersects(CollisionComponent *other) = 0;
        virtual CollSide Intersects(CollisionComponent *other, Vec2<float> &offset) = 0;

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