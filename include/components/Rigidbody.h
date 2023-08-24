#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Component.h"
#include "MathLib.h"
#include "Collider.h"

#include <vector>

namespace junebug
{
    class Rigidbody : public Component<VisualActor>
    {
    public:
        Rigidbody(class VisualActor *owner, class Collider *coll = nullptr);

        void Update(float dt) override;

        void AddForce(Vec2<float> force);

        void SetMass(float mass) { mMass = mass; };
        float GetMass() { return mMass; };
        void SetStatic(bool isStatic) { mStatic = isStatic; };
        bool IsStatic() { return mStatic; };

        void SetVelocity(Vec2<float> velocity) { mVelocity = velocity; };
        Vec2<float> GetVelocity() { return mVelocity; };
        void SetAcceleration(Vec2<float> acceleration) { mAcceleration = acceleration; };
        Vec2<float> GetAcceleration() { return mAcceleration; };
        void SetGravityOffset(Vec2<float> gravity) { mGravityOffset = gravity; };
        Vec2<float> GetGravityOffset() { return mGravityOffset; };

        void SetBounce(float bounce) { mBounce = bounce; };
        float GetBounce() { return mBounce; };

        void AddPhysLayer(std::string layer);
        void RemovePhysLayer(std::string layer);
        void ClearPhysLayers();
        std::vector<std::string> &GetPhysLayers() { return mPhysLayers; };

        void SetCollComponent(class Collider *coll) { mColl = coll; };
        class Collider *GetCollComponent() { return mColl; };

    protected:
        VisualActor *mOwner;
        class Collider *mColl{nullptr};

        float mMass = 1.0f;
        bool mStatic = false;
        float mBounce = 0.0f;

        virtual void OnCollide(class Collider *other, CollSide side, Vec2<float> offset);

        Vec2<float> mVelocity = Vec2<float>::Zero;
        Vec2<float> mAcceleration = Vec2<float>::Zero;
        Vec2<float> mGravityOffset = Vec2<float>::Zero;

        Vec2<float> mPendingForces = Vec2<float>::Zero;

        void PhysicsUpdate(float dt);
        void CheckCollisions();
        void CheckCollisionList(const std::vector<class Collider *> &collisions);

        std::vector<std::string> mPhysLayers;
    };
}