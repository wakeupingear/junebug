#include "Component.h"
#include "Actors.h"

using namespace junebug;

Component::Component(VisualActor *owner, int updateOrder)
    : mOwner(owner), mUpdateOrder(updateOrder)
{
    mOwner->AddComponent(this);
}

Component::~Component()
{
}

void Component::Update(float deltaTime)
{
}

void Component::ProcessInput(const Uint8 *keyState)
{
}