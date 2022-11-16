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
    class Sprite
    {
    public:
        Sprite();
        ~Sprite();

        // Draw this sprite
        void Draw(class Camera *cam, SDL_Renderer *renderer, const Vec2<float> &pos, const SpriteProperties &properties = {});
        // Set the texture to draw for this psirte
        virtual void SetTexture(SDL_Texture *texture);

        // Get the height of the texture
        int GetTexHeight() const { return mTexHeight; }
        // Get the height of the texture
        int GetTexWidth() const { return mTexWidth; }
        // Get the height of the texture

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
        SDL_Texture *mTexture = nullptr;
        // Width/height
        int mTexWidth;
        int mTexHeight;

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
    };
}