#pragma once

#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "mathLib.h"
#include "color.h"

#include <functional>

namespace junebug
{

    /// @brief Possible Actor states
    enum class ActorState
    {
        Active,
        Paused,
        Destroy
    };

    /// @brief A PureActor is the base class for all actors. It contains no private data.
    class PureActor
    {
    public:
        // Default constructor
        PureActor();
        // Default destructor
        ~PureActor();

        ActorState GetState() const { return mState; }
        void SetState(ActorState state) { mState = state; }

        // Update the actor in the game loop
        /// @param dt The time since the last update
        void Update(float dt);

        // User-defined callback
        virtual void OnUpdate(float dt){};

    private:
        ActorState mState = ActorState::Active;
    };

    /// @brief A VisualActor is an actor that has a visual representation, including a texture, position, rotation, scale, and color.
    class VisualActor : public PureActor
    {
    public:
        // Position constructor
        VisualActor(Vector2 pos);

        // Set the position of the actor
        /// @param pos The new position of the actor
        void SetPosition(const Vector2 &pos);
        // Set the position of the actor
        /// @returns const Vector2
        Vector2 GetPosition() const;

        // Set the rotation of the actor
        /// @param rot The new rotation of the actor
        void SetRotation(float rot);
        // Get the rotation of the actor
        /// @returns float rotation
        float GetRotation() const;

        // Set the scale of the actor
        /// @param scale The new scale of the actor
        void SetScale(const Vector2 &scale);
        // Get the scale of the actor
        /// @returns const Vector2
        Vector2 GetScale() const;

        // Set the color of the actor
        /// @param color The new color of the actor
        void SetColor(const Color &color);
        // Get the color of the actor
        /// @returns const Color
        Color GetColor() const;

    private:
        Vector2 mPosition{0, 0};
        float mRotation{0};
        Vector2 mScale{1, 1};
        Color mColor{1, 1, 1};
    };

    /// @brief A PhysicalActor is an actor that has a physical representation, including a position, velocity, acceleration, collider, and mass.
    class PhysicalActor : public VisualActor
    {
    };
}