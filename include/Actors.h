#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"
#include "Color.h"

#include <functional>
#include <vector>

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
        virtual ~PureActor();

        ActorState GetState() const { return mState; }
        void SetState(ActorState state) { mState = state; }

        // Update the actor in the game loop
        /// @param dt The time since the last update
        void Update(float dt);

        // Vector of attached components
        std::vector<class Component *> mComponents;
        // Returns component of type T, or null if doesn't exist
        template <typename T>
        T *GetComponent() const
        {
            for (auto c : mComponents)
            {
                T *t = dynamic_cast<T *>(c);
                if (t != nullptr)
                {
                    return t;
                }
            }

            return nullptr;
        }

        // Check if the actor persists between scenes
        /// @return true if the actor persists between scenes, false otherwise
        inline bool IsPersistent() const { return mPersistent; }

    protected:
        // User-defined callback
        virtual void OnUpdate(float dt){};

        // Actor state
        ActorState mState = ActorState::Active;

        // Actor persistence
        bool mPersistent = false;
    };

    /// @brief A VisualActor is an actor that has a visual representation, including a texture, position, rotation, scale, and color.
    class VisualActor : public PureActor
    {
    public:
        // Position constructor
        VisualActor(Vec2<float> pos);
        VisualActor(Vec2<int> pos);
        // Position and image constructor
        VisualActor(Vec2<float> pos, std::string imagePath, int drawOrder = 100);
        VisualActor(Vec2<int> pos, std::string imagePath, int drawOrder = 100);

        // Set the position of the actor
        /// @param pos The new position of the actor
        void SetPosition(const Vec2<float> &pos);
        // Set the position of the actor
        /// @returns const Vec2
        Vec2<float> GetPosition() const;

        // Set the rotation of the actor
        /// @param rot The new rotation of the actor
        void SetRotation(float rot);
        // Get the rotation of the actor
        /// @returns float rotation
        float GetRotation() const;

        // Set the scale of the actor
        /// @param scale The new scale of the actor
        void SetScale(const Vec2<float> &scale);
        // Get the scale of the actor
        /// @returns const Vec2
        Vec2<float> GetScale() const;

        // Set the color of the actor
        /// @param color The new color of the actor
        void SetColor(const Color &color);
        // Get the color of the actor
        /// @returns const Color
        Color GetColor() const;

        // Get the sprite of the actor
        /// @returns Sprite*
        class Sprite *GetSprite() const { return mSprite; }

    private:
        friend class Component;
        // Add a component to the actor
        void AddComponent(class Component *c);

        Sprite *mSprite = nullptr;

        Vec2<float> mPosition{0, 0};
        float mRotation{0};
        Vec2<float> mScale{1, 1};
        Color mColor{1, 1, 1};
    };

    /// @brief A PhysicalActor is an actor that has a physical representation, including a position, velocity, acceleration, collider, and mass.
    class PhysicalActor : public VisualActor
    {
    };
}