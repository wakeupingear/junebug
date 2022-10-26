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
        Component(class VisualActor *owner, int updateOrder = 100);
        // Destructor
        virtual ~Component();
        // Update this component by delta time
        virtual void Update(float deltaTime);
        // Process input for this component (if needed)
        virtual void ProcessInput(const Uint8 *keyState);

        // Return the update order of this component
        int GetUpdateOrder() const { return mUpdateOrder; }

    protected:
        // Owning actor
        class VisualActor *mOwner;
        // Update order
        int mUpdateOrder;
    };
}