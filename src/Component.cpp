#include "Component.h"
#include "Actors.h"

using namespace junebug;

Component::Component(PureActor *owner, int updateOrder)
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