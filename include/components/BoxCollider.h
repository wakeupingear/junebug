#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "components/CollisionComponent.h"

namespace junebug
{
    class BoxCollider : public CollisionComponent
    {
    public:
        BoxCollider(class VisualActor *owner, bool useSprite = true, std::string layer = "");

        void Update(float dt) override;

        bool Intersects(CollisionComponent *other) override;
        CollSide GetMinOverlap(CollisionComponent *other, Vec2<float> &offset) override;

        void SetSize(Vec2<float> size) { mSize = size; };
        Vec2<float> GetSize();
        void SetOffset(Vec2<float> offset) { mOffset = offset; };
        Vec2<float> GetOffset();

        Vec2<float> GetMin();
        Vec2<float> GetMax();

    protected:
        Vec2<float> mSize{0.0f, 0.0f};
        Vec2<float> mOffset{0.0f, 0.0f};

        bool mUseSprite = true;
        std::string mParentSprite{""};
    };
}