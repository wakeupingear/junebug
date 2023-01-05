#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <SDL2/SDL_stdinc.h>

namespace junebug
{
    class Component
    {
    public:
        // Constructor
        Component(class PureActor *owner, int updateOrder = 100);
        // Destructor
        virtual ~Component();
        // Update this component by delta time
        virtual void Update(float dt);

        // Return the update order of this component
        int GetUpdateOrder() const { return mUpdateOrder; }

    protected:
        // Owning actor
        class PureActor *mOwner;
        // Update order
        int mUpdateOrder;
    };
}