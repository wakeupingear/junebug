#include "Actors.h"
#include "JGame.h"

using namespace junebug;

PureActor::PureActor()
{
    JGame::Get()->AddActor(this);
}

PureActor::~PureActor()
{
    JGame::Get()->RemoveActor(this);
}

void PureActor::Update(float dt)
{
    OnUpdate(dt);
}