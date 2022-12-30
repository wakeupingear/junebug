#include "Actors.h"
#include "components/PhysicsComponent.h"
#include "components/BoxCollider.h"
#include "Game.h"

using namespace junebug;

PhysicalActor::PhysicalActor() : VisualActor() {}
PhysicalActor::PhysicalActor(Vec2<float> pos) : VisualActor(pos) {}
PhysicalActor::PhysicalActor(Vec2<int> pos) : VisualActor(pos) {}
PhysicalActor::PhysicalActor(Vec2<float> pos, std::string spritePath) : VisualActor(pos, spritePath) {}
PhysicalActor::PhysicalActor(Vec2<int> pos, std::string spritePath) : VisualActor(pos, spritePath) {}

void PhysicalActor::Awake()
{
    InitializeComponents();
}

void PhysicalActor::InitializeComponents(CollType type)
{
    if (mColl && mColl->GetType() != type)
    {
        delete mColl;
        mColl = nullptr;
    }

    if (!mColl)
    {
        switch (type)
        {
        case CollType::Box:
            mColl = new BoxCollider(this, "");
            break;
        default:
            break;
        }
    }

    if (!mPhys)
        mPhys = new PhysicsComponent(this, mColl);
}

void PhysicalActor::AddForce(const Vec2<float> &force)
{
    if (mPhys)
        mPhys->AddForce(force);
}

void PhysicalActor::SetGravityOffset(Vec2<float> gravity)
{
    if (mPhys)
        mPhys->SetGravityOffset(gravity);
}
Vec2<float> PhysicalActor::GetGravityOffset()
{
    if (mPhys)
        return mPhys->GetGravityOffset();
    return Vec2<float>::Zero;
}

void PhysicalActor::SetStatic(bool isStatic)
{
    if (mPhys)
        mPhys->SetStatic(isStatic);
}
bool PhysicalActor::IsStatic()
{
    if (mPhys)
        return mPhys->IsStatic();
    return false;
}

void PhysicalActor::SetBounce(float bounce)
{
    if (mPhys)
        mPhys->SetBounce(bounce);
}
float PhysicalActor::GetBounce()
{
    if (mPhys)
        return mPhys->GetBounce();
    return 0.0f;
}

void PhysicalActor::SetMass(float mass)
{
    if (mPhys)
        mPhys->SetMass(mass);
}
float PhysicalActor::GetMass()
{
    if (mPhys)
        return mPhys->GetMass();
    return 1.0f;
}

void PhysicalActor::SetCollisionLayer(std::string layer)
{
    if (mColl)
        mColl->SetLayer(layer);
}
std::string PhysicalActor::GetCollisionLayer()
{
    if (mColl)
        return mColl->GetLayer();
    return "";
}

void PhysicalActor::SetCollisionType(CollType type)
{
    if (mColl && mColl->GetType() == type)
        return;

    InitializeComponents(type);
}
CollType PhysicalActor::GetCollisionType()
{
    if (mColl)
        return mColl->GetType();
    return CollType::None;
}