#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"
#include "Color.h"
#include "Sprite.h"
#include "components/CollisionComponent.h"

#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>

namespace junebug
{
// Helper macro, see below
#define __REGISTER_ACTOR__(T) Game::mActorConstructors[#T] = []() -> T * {T* inst = dynamic_cast<T *>(PureActor::__createInstance__<T>()); return inst; };
// A macro to register an arbitrary number of custom Actor classes for serialization
// This allows the engine to do some basic reflection on the classes
// (basically, to be able to create them from a string, like from a JSON Scene file)
// You'll want to invoke this macro before you start loading scenes
// Ideally, put it in Game::LoadData() so the mappings are set before any default scenes are loaded
#define JB_REGISTER_ACTORS(...) MAP(__REGISTER_ACTOR__, __VA_ARGS__)

    /// @brief Possible Actor states
    enum class ActorState
    {
        Started,
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

        // ToString
        virtual void ToString(std::ostream &out) const
        {
            out << "Depth(" << mDepth << ")";
        }
        // Printable
        [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const PureActor &v)
        {
            v.ToString(os);
            return os;
        }

        // Get the actor's state
        ActorState GetState() const { return mState; }
        // Set the actor's state
        void SetState(ActorState state) { mState = state; }
        // Mark the actor as paused. This will prevent it from being updated.
        void Pause() { mState = ActorState::Paused; }
        // Mark the actor as active (default). This will allow it to be updated
        void Unpause() { mState = ActorState::Active; }
        // Mark the actor as destroyed. This will remove it from the game.
        void Destroy() { mState = ActorState::Destroy; }
        // Set whether the actor should persist between scenes
        void SetPersistent(bool persistent) { mPersistent = persistent; }

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

        // Internal function to create an instance of a class
        template <typename T>
        static T *__createInstance__() { return new T(); };

        // Set the actor's depth
        void SetDepth(int newDepth) { mDepth = newDepth; };
        // Get the actor's depth
        int GetDepth() { return mDepth; };

        // Get the actor's id
        std::string GetId() const { return mId; }

    protected:
        friend class Component;
        friend class Game;
        // Add a component to the actor
        void AddComponent(class Component *c);
        // User-defined function to every frame when the actor updates
        virtual void InternalUpdate(float dt){};
        virtual void Update(float dt){};
        // User-defined function to run on the first frame update of an actor
        virtual void FirstUpdate(float dt){};
        virtual void InternalFirstUpdate(float dt){};

        // Actor state
        ActorState mState = ActorState::Started;

        // Actor persistence
        bool mPersistent = false;

        // Actor depth
        int mDepth = 0;

        // Actor ID
        std::string mId;
    };

    /// @brief A VisualActor is an actor that has a visual representation, including a texture, position, rotation, scale, and color.
    class VisualActor : public PureActor
    {
    public:
        struct Animation
        {
            std::string sprAnimName;
            std::weak_ptr<Sprite> sprite;
            float frame, fps;
            bool loop = true, finished = false;

            Animation(std::string name, std::weak_ptr<Sprite> spr, float fps, bool loop = true)
                : sprAnimName(name), sprite(spr), frame(0), fps(fps), loop(loop), finished(false){};

            void Update(std::string name, float dt, std::string &nextAnim);
        };

        VisualActor() : PureActor(){};
        // Position constructor
        VisualActor(Vec2<float> pos);
        VisualActor(Vec2<int> pos);
        // Position and image constructor
        VisualActor(Vec2<float> pos, std::string imagePath);
        VisualActor(Vec2<int> pos, std::string imagePath);

        void InternalFirstUpdate(float dt) override;
        void InternalUpdate(float dt) override;

        // Actor visibility
        void SetVisible(bool visible) { mVisible = visible; }
        bool Visible() const { return mVisible; }

        // Set the color of the actor
        /// @param color The new color of the actor
        void SetColor(const Color &color);
        // Get the color of the actor
        /// @returns const Color
        Color GetColor() const;

        // ToString
        virtual void ToString(std::ostream &out) const override
        {
            out << "Pos" << mPosition << " Scale" << mScale << " ";
            PureActor::ToString(out);
        };

        // Set the sprite of the actor
        /// @param path The path to the new sprite
        void SetSprite(std::string path);
        // Get a safe pointer to the sprite of the actor.
        // This does NOT use shared_ptr. Instaed, it will return a pointer to a temp sprite stored in the Game instance. The temp sprite has no actual impact of the game, but it lets you safely access this actor's sprite without worrying about causing a segfault if an invalid sprite path is used.
        class Sprite *GetSprite();
        // Get the sprite of the actor
        class Sprite *GetRawSprite();

        static Sprite __tempSprite__;

        void SetDefaultSpriteAnimation(std::string nickname);

        void AddSpriteAnimation(std::string nickname, std::string spriteName, std::string spriteAnimName, float fps, bool loop = true);

        int GetAnimationFrame(std::string nickname);

        // Get the sprite name of the actor.
        /// @returns std::string
        std::string GetSpriteName();
        // Get the sprite size
        /// @returns Vec2<int>
        Vec2<int> GetSpriteSize();
        Vec2<float> GetActorSize();

        void SetOrigin(SpriteOrigin origin, const Vec2<int> &offset = Vec2<int>::Zero);

        // Set the position of the actor
        /// @param pos The new position of the actor
        void SetPosition(const Vec2<float> &pos);
        // Move the actor by a given amount
        /// @param pos The amount to move the actor
        void MovePosition(const Vec2<float> &pos);
        // Get the position of the actor
        /// @returns const Vec2
        Vec2<float> GetPosition() const;
        // Get the starting position of the actor
        /// @returns const Vec2
        Vec2<float> GetStartPosition() const;
        // Get the previous position of the actor
        Vec2<float> GetPrevPosition() const;
        // Clamp the actor's position to the given bounds
        /// @param bounds The bounds to clamp the actor to
        void ClampPosition(const Vec2<float> &start, const Vec2<float> &end);

        // Set the rotation of the actor
        /// @param rot The new rotation of the actor
        void SetRotation(float rot);
        // Get the rotation of the actor
        /// @returns float rotation
        float GetRotation() const;
        // Get a reference to the actor's rotation
        float &GetRotationRef() { return mRotation; };

        // Set the alpha of the actor
        /// @param alpha The new alpha of the actor
        void SetAlpha(float alpha);
        void SetAlpha(int alpha);
        // Get the alpha of the actor
        /// @returns int alpha
        int GetAlpha();

        // Set the scale of the actor
        /// @param scale The new scale of the actor
        void SetScale(const Vec2<float> &scale);
        void SetScale(float scaleX, float scaleY);
        void SetScale(float scale);
        // Get the scale of the actor
        /// @returns const Vec2
        Vec2<float> GetScale() const;

        // Set the frame of the actor
        /// @param frame The new frame of the actor
        void SetFrame(int frame);
        // Get the frame of the actor
        /// @returns int frame
        int GetFrame();
        // Set the fps of the actor
        /// @param fps The new fps of the actor
        void SetFPS(float fps);
        // Get the fps of the actor
        /// @returns float fps
        float GetFPS() const;

        // Set the Camera Rounding boolean
        // This will round the actor's position to the nearest pixel when rendered onto a camera.
        // There are various cases where this is useful, where fractional pixels can cause visual artifacts.
        /// @param roundToCamera The new value of the Camera Rounding boolean
        void SetRoundToCamera(bool roundToCamera) { mRoundToCamera = roundToCamera; }
        // Get the Camera Rounding boolean
        /// @returns bool
        bool GetRoundToCamera() const { return mRoundToCamera; }

    protected:
        friend class Camera;
        friend class Game;
        // User-defined function to run every frame when the actor draws
        virtual void Draw();

        std::string mSpritePath;
        bool mVisible{true};
        Color mColor = Color::White;

        std::unordered_map<std::string, Animation> mFrameAnimations;

        Vec2<float> mPosition{0, 0}, mStartPosition{0, 0}, mPrevPosition{0, 0};
        float mRotation{0};
        Vec2<float> mScale{1, 1};
        bool mRoundToCamera{false};

    private:
        std::string mNextSpriteAnimation{""};
    };

    /// @brief A PhysicalActor is a VisualActor that has a physical representation, including velocity, acceleration, collider, and mass.
    class PhysicalActor : public VisualActor
    {
    public:
        PhysicalActor();
        PhysicalActor(Vec2<float> pos);
        PhysicalActor(Vec2<int> pos);
        PhysicalActor(Vec2<float> pos, std::string imagePath, bool isStatic = false);
        PhysicalActor(Vec2<int> pos, std::string imagePath, bool isStatic = false);

        void InternalFirstUpdate(float dt);

        void AddForce(const Vec2<float> &force);
        void SetGravityOffset(Vec2<float> gravity);
        Vec2<float> GetGravityOffset();
        void SetStatic(bool isStatic);
        bool IsStatic();
        class PhysicsComponent *GetPhysicsComponent() { return mPhys; }
        void SetBounce(float bounce);
        float GetBounce();
        void SetMass(float mass);
        float GetMass();
        void SetVelocity(Vec2<float> velocity);
        Vec2<float> GetVelocity();

        void AddPhysLayer(std::string layer);
        void RemovePhysLayer(std::string layer);
        void ClearPhysLayers();
        std::vector<std::string> &GetPhysLayers();

        void SetCollLayer(std::string layer);
        std::string GetCollLayer() { return mCollLayer; };
        void SetCollType(CollType type);
        CollType GetCollType() { return mCollType; };
        class CollisionComponent *GetCollComponent() { return mColl; }

    protected:
        friend class Game;

        class PhysicsComponent *mPhys{nullptr};
        class CollisionComponent *mColl{nullptr};

        // Member variables that mirror the CollisionComponent.
        // These are used to initialize the CollisionComponent when it is created to avoid unnecessary calls to Game::AddCollision()
        std::string mCollLayer = "";
        CollType mCollType = CollType::Polygon;

    private:
        void InitializeComponents();
        void InitializePhysComponent();
    };
};