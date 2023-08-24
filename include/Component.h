#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <SDL2/SDL_stdinc.h>

namespace junebug
{
    // CRTP base class for singletons
    template <typename T = class Actor>
    class Component
    {
    public:
        // Constructor
        Component(T *owner, int updateOrder = 100) : mOwner(owner), mUpdateOrder(updateOrder)
        {
            mOwner->AddComponent((Component<class Actor> *)this);
        }
        // Destructor
        virtual ~Component(){};
        // Update this component by delta time
        virtual void Update(float dt){};

        // Return the update order of this component
        int GetUpdateOrder() const { return mUpdateOrder; }

    protected:
        // Owning actor
        T *mOwner;
        // Update order
        int mUpdateOrder;
    };
}