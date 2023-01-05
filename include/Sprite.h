#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Color.h"
#include "MathLib.h"
#include "Rendering.h"

#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace junebug
{
    enum SpriteOrigin
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Center
    };

    class Sprite
    {
    public:
        Sprite();
        ~Sprite();

        // Draw this sprite
        void Draw(class Camera *cam, SDL_Renderer *renderer, const Vec2<float> &pos, const Vec2<int> &partPos, const Vec2<int> &partSize, int frame, const SpriteProperties &properties);
        // Add a texture for this psirte
        virtual void AddTexture(SDL_Texture *texture);

        // Get the height of the texture
        Vec2<int> GetTexSize() const { return mTexSize; }

        // Set the origin of the sprite
        void SetOrigin(SpriteOrigin origin, const Vec2<int> &offset = Vec2<int>::Zero);
        // Get the origin of the sprite
        Vec2<int> GetOrigin() { return mOrigin; }

        // Load a texture from a file
        SDL_Texture *LoadTextureFile(std::string &fileName);
        // Load metadata from a file
        bool LoadMetadataFile(std::string &folder);

        // Set the textures for this sprite
        void SetTextures(const std::vector<SDL_Texture *> &textures) { mTextures = textures; }
        // Get the textures for this sprite
        const std::vector<SDL_Texture *> &GetTextures() const { return mTextures; }

        // Add an animation of the corresponding name to the animation map
        void AddAnimation(const std::string &name,
                          const std::vector<SDL_Texture *> &images);
        // Set the current active animation
        void SetAnimation(const std::string &name) { mAnimName = name; }
        // Get the name of the currently-playing animation
        const std::string &GetAnimName() const { return mAnimName; }
        // Reset the animation back to frame/time 0

        void ResetAnimTimer() { mAnimTimer = 0.0f; }
        // Use to pause/unpause the animation
        void SetIsPaused(bool pause) { mIsPaused = pause; }
        // Use to change the FPS of the animation
        void SetAnimFPS(float fps) { mAnimFPS = fps; }
        // Use to get the current FPS of the animation
        float GetAnimFPS() const { return mAnimFPS; }
        // Use to get the total duration of the animation of he specified name
        float GetAnimDuration(const std::string &name) { return mAnims[name].size() / mAnimFPS; }

    protected:
        // Texture to draw
        std::vector<SDL_Texture *> mTextures;
        // Width/height
        Vec2<int> mTexSize = Vec2<int>(0, 0);
        // Origin
        Vec2<int> mOrigin = Vec2<int>(0, 0);

        // Map of animation name to vector of textures corresponding to the animation
        std::unordered_map<std::string, std::vector<SDL_Texture *>> mAnims;

        // Name of current animation
        std::string mAnimName;
        // Whether or not the animation is paused (defaults to false)
        bool mIsPaused = false;
        // Tracks current elapsed time in animation
        float mAnimTimer = 0.0f;
        // The frames per second the animation should run at
        float mAnimFPS = 10.0f;

        // The color to tint the sprite
        Color mColor = Color::White;

    private:
        inline bool __IsTempSprite__();
    };
}