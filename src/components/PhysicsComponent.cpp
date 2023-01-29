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

    if (mPhysLayers.empty())
    {
        for (auto &layer : Game::Get()->GetCollLayers())
        {
            CheckCollisionList(layer.second);
        }
    }
    else
    {
        for (auto &layer : mPhysLayers)
        {
            auto loc = Game::Get()->GetCollLayers().find(layer);
            if (loc != Game::Get()->GetCollLayers().end())
            {
                CheckCollisionList(loc->second);
            }
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
        fixSide = mColl->Intersects(other, fixVec);
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
    Vec2<float> newVel = GetVelocity();
    if (!NearZero(offset.x))
        newVel.x *= -mBounce;
    if (!NearZero(offset.y))
        newVel.y *= -mBounce;
    SetVelocity(newVel);

    if (mColl)
        mColl->UpdateCollPositions();
}

void PhysicsComponent::AddPhysLayer(std::string layer)
{
    auto loc = std::find(mPhysLayers.begin(), mPhysLayers.end(), layer);
    if (loc == mPhysLayers.end())
        mPhysLayers.push_back(layer);
}
void PhysicsComponent::RemovePhysLayer(std::string layer)
{
    auto loc = std::find(mPhysLayers.begin(), mPhysLayers.end(), layer);
    if (loc != mPhysLayers.end())
        mPhysLayers.erase(loc);
}
void PhysicsComponent::ClearPhysLayers()
{
    mPhysLayers.clear();
}