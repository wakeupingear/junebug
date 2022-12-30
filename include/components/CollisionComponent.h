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

        void SetLayer(std::string layer);
        std::string GetLayer() { return mLayer; };

        void SetType(CollType type) { mType = type; };
        CollType GetType() { return mType; };

        virtual bool Intersects(CollisionComponent *other) = 0;
        virtual CollSide GetMinOverlap(CollisionComponent *other, Vec2<float> &offset) = 0;

    protected:
        VisualActor *mOwner;

        std::string mLayer;
        CollType mType;

    private:
        void UpdateCollEntry(bool initial = false);
    };
};