#include "components/Rigidbody.h"
#include "components/Collider.h"
#include "Actors.h"
#include "Game.h"

using namespace junebug;

Rigidbody::Rigidbody(VisualActor *owner, Collider *coll) : Component(owner), mOwner(owner), mColl(coll)
{
}

void Rigidbody::Update(float dt)
{
    AddForce(Game::Get()->GetGravity() + mGravityOffset);

    PhysicsUpdate(dt);
}

void Rigidbody::AddForce(Vec2<float> force)
{
    if (!mStatic)
        mPendingForces += force;
}

void Rigidbody::PhysicsUpdate(float dt)
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

void Rigidbody::CheckCollisions()
{
    if (!mColl || mColl->GetType() == CollType::None)
        return;

    mColl->UpdateCollPositions();
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

void Rigidbody::CheckCollisionList(const std::vector<Collider *> &collisions)
{
    Vec2<float> fixVec = Vec2<float>::Zero;
    CollSide fixSide = CollSide::None;
    for (Collider *other : collisions)
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

void Rigidbody::OnCollide(Collider *other, CollSide side, Vec2<float> offset)
{
    // print("moving", mOwner->GetPosition(), offset, Game::Get()->GetFrameCount());
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

void Rigidbody::AddPhysLayer(std::string layer)
{
    auto loc = std::find(mPhysLayers.begin(), mPhysLayers.end(), layer);
    if (loc == mPhysLayers.end())
        mPhysLayers.push_back(layer);
}
void Rigidbody::RemovePhysLayer(std::string layer)
{
    auto loc = std::find(mPhysLayers.begin(), mPhysLayers.end(), layer);
    if (loc != mPhysLayers.end())
        mPhysLayers.erase(loc);
}
void Rigidbody::ClearPhysLayers()
{
    mPhysLayers.clear();
}