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
            mColl = new BoxCollider(this, true, mCollLayer);
            break;
        default:
            break;
        }
    }
}

void PhysicalActor::AddForce(const Vec2<float> &force)
{
    if (!mPhys)
        InitializePhysComponent();
    mPhys->AddForce(force);
}

void PhysicalActor::SetGravityOffset(Vec2<float> gravity)
{
    if (!mPhys)
        InitializePhysComponent();
    mPhys->SetGravityOffset(gravity);
}
Vec2<float> PhysicalActor::GetGravityOffset()
{
    if (!mPhys)
        InitializePhysComponent();
    return mPhys->GetGravityOffset();
}

void PhysicalActor::SetStatic(bool isStatic)
{
    if (!mPhys)
        InitializePhysComponent();
    mPhys->SetStatic(isStatic);
}
bool PhysicalActor::IsStatic()
{
    if (!mPhys)
        InitializePhysComponent();
    return mPhys->IsStatic();
}

void PhysicalActor::SetBounce(float bounce)
{
    if (!mPhys)
        InitializePhysComponent();
    mPhys->SetBounce(bounce);
}
float PhysicalActor::GetBounce()
{
    if (!mPhys)
        InitializePhysComponent();
    return mPhys->GetBounce();
}

void PhysicalActor::SetMass(float mass)
{
    if (!mPhys)
        InitializePhysComponent();
    mPhys->SetMass(mass);
}
float PhysicalActor::GetMass()
{
    if (!mPhys)
        InitializePhysComponent();
    return mPhys->GetMass();
}

void PhysicalActor::AddPhysLayer(std::string layer)
{
    if (!mPhys)
        InitializePhysComponent();
    mPhys->AddPhysLayer(layer);
}
void PhysicalActor::RemovePhysLayer(std::string layer)
{
    if (!mPhys)
        InitializePhysComponent();
    mPhys->RemovePhysLayer(layer);
}
void PhysicalActor::ClearPhysLayers()
{
    if (!mPhys)
        InitializePhysComponent();
    mPhys->ClearPhysLayers();
}
std::vector<std::string> &PhysicalActor::GetPhysLayers()
{
    if (!mPhys)
        InitializePhysComponent();
    return mPhys->GetPhysLayers();
}

void PhysicalActor::SetCollLayer(std::string layer)
{
    mCollLayer = layer;
    if (mColl)
        mColl->SetCollLayer(layer);
}

void PhysicalActor::SetCollType(CollType type)
{
    mCollType = type;
    if (mColl)
        mColl->SetType(type);
}

void PhysicalActor::InitializePhysComponent()
{
    if (!mPhys)
        mPhys = new PhysicsComponent(this, mColl);
    else
        mPhys->SetCollComponent(mColl);
}