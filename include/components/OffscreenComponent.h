#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Component.h"
#include "MathLib.h"
#include "Actors.h"
#include "Game.h"
#include "Camera.h"

namespace junebug
{
    bool IsOutsideCamera(Vec2<float> pos, Vec2<float> mPadding = Vec2<>::Zero)
    {
        for (Camera *cam : Game::Get()->GetCameras())
        {
            if (pos.x < cam->GetPosition().x - mPadding.x || pos.x > cam->GetPosition().x + cam->GetSize().x + mPadding.x ||
                pos.y < cam->GetPosition().y - mPadding.y || pos.y > cam->GetPosition().y + cam->GetSize().y + mPadding.y)
                return true;
        }
        return false;
    }

    bool IsOutsideScene(Vec2<float> pos, Vec2<float> mPadding = Vec2<>::Zero)
    {
        Scene scene = Game::Get()->GetCurrentScene();
        return (pos.x < -mPadding.x || pos.x > scene.size.x + mPadding.x ||
                pos.y < -mPadding.y || pos.y > scene.size.y + mPadding.y);
    }

    // An abstract component that checks if an actor is offscreen and calls the appropriate callback functions for entering and exiting the screen.
    // The component can be used with a camera or the scene bounds.
    class OffscreenComponent : public Component<VisualActor>
    {
    public:
        OffscreenComponent(
            class VisualActor *owner, bool useCamera, std::function<void()> onExit = []() {},
            std::function<void()> onEnter = []() {}, Vec2<float> padding = Vec2<>::Zero, int updateOrder = 100) : Component(owner, updateOrder)
        {
            mOwner = owner;
            mUseCamera = useCamera;
            mOnExit = onExit;
            mOnEnter = onEnter;
            mPadding = padding;

            if (padding.IsZero())
            {
                mPadding = Vec2<float>(mOwner->GetSprite()->GetTexSize());
            }

            mOffscreen = IsOffscreen();
        };

        void Update(float dt) override
        {
            if (mOffscreen)
            {
                if (!IsOffscreen())
                {
                    mOffscreen = false;
                    mOnEnter();
                }
            }
            else if (IsOffscreen())
            {
                mOffscreen = true;
                mOnExit();
            }
        }

        bool IsOffscreen()
        {
            Vec2<float> pos = mOwner->GetPosition();
            if (mUseCamera)
            {
                // Return true if no cameras to handle situations where cameras haven't yet been initialized
                if (Game::Get()->GetCameras().empty())
                    return true;

                return IsOutsideCamera(pos, mPadding);
            }
            else
                return IsOutsideScene(pos, mPadding);
        }

        void SetOnEnter(std::function<void()> onEnter) { mOnEnter = onEnter; };
        void SetOnExit(std::function<void()> onExit) { mOnExit = onExit; };

        Vec2<float> GetPadding() { return mPadding; }
        void SetPadding(Vec2<float> padding) { mPadding = padding; }
        inline void SetPadding(float x, float y) { SetPadding(Vec2<float>(x, y)); }

    protected:
        VisualActor *mOwner;

        Vec2<float> mPadding = Vec2<>::Zero;

        bool mOffscreen = false;
        bool mUseCamera = false;

        // Lambda functions to be called when the actor enters or exits the screen
        std::function<void()> mOnEnter = []() {};
        std::function<void()> mOnExit = []() {};
    };

    class OffscreenDestroyComponent : public OffscreenComponent
    {
    public:
        OffscreenDestroyComponent(
            class VisualActor *owner, bool useCamera,
            Vec2<float> padding = Vec2<>::Zero,
            int updateOrder = 100)
            : OffscreenComponent(
                  owner, useCamera, [this]()
                  { mOwner->Destroy(); },
                  []() {}, padding, updateOrder)
        {
            mOnExit = [this]()
            { mOwner->Destroy(); };
        };
    };
}