#include "components/PhysicsComponent.h"
#include "components/CollisionComponent.h"
#include "Actors.h"
#include "Game.h"

using namespace junebug;

PhysicsComponent::PhysicsComponent(VisualActor *owner, CollisionComponent *coll) : Component(owner), mOwner(owner), mColl(coll)
{
}

void PhysicsComponent::Update(float dt)
{
    AddForce(Game::Get()->GetGravity() + mGravityOffset);

    PhysicsUpdate(dt);
}

void PhysicsComponent::AddForce(Vec2<float> force)
{
    if (!mStatic)
        mPendingForces += force;
}

void PhysicsComponent::PhysicsUpdate(float dt)
{
    if (!mStatic)
    {
        mAcceleration = mPendingForces * (1.0f / mMass);
        mVelocity += mAcceleration * dt;

        mOwner->MovePosition(mVelocity * dt);
    }

    mPendingForces = Vec2<>::Zero;

    CheckCollisions();
}

void PhysicsComponent::CheckCollisions()
{
    if (mStatic || !mColl || mColl->GetType() == CollType::None)
        return;

    if (mColl->GetLayer().empty())
    {
        for (auto &layer : Game::Get()->GetCollisionLayers())
        {
            CheckCollisionList(layer.second);
        }
    }
    else
    {
        auto loc = Game::Get()->GetCollisionLayers().find(mColl->GetLayer());
        if (loc != Game::Get()->GetCollisionLayers().end())
        {
            CheckCollisionList(loc->second);
        }
    }
}

void PhysicsComponent::CheckCollisionList(const std::vector<CollisionComponent *> &collisions)
{
    Vec2<float> fixVec = Vec2<float>::Zero;
    CollSide fixSide = CollSide::None;
    for (CollisionComponent *other : collisions)
    {
        if (other == mColl)
            continue;
        fixSide = mColl->GetMinOverlap(other, fixVec);
        if (fixSide != CollSide::None)
        {
            OnCollide(other, fixSide, fixVec);
            break;
        }
    }
}

void PhysicsComponent::OnCollide(CollisionComponent *other, CollSide side, Vec2<float> offset)
{
    mOwner->MovePosition(offset);
    SetVelocity(GetVelocity() * -mBounce);
}