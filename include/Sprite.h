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

        // Get the number of frames in this sprite
        int GetNumFrames() const { return mTextures.size(); }
        // Get the FPS of this sprite
        float GetFps() const { return mFps; }
        // Set the FPS of this sprite
        void SetFps(float fps) { mFps = fps; }

        // Set the textures for this sprite
        void SetTextures(const std::vector<SDL_Texture *> &textures) { mTextures = textures; }
        // Get the textures for this sprite
        const std::vector<SDL_Texture *> &GetTextures() const { return mTextures; }

        // Get a given animation
        const std::vector<int> &GetAnimation(const std::string &name = "_");
        // Add an animation of the corresponding name to the animation map
        void AddAnimation(const std::string &name,
                          const std::vector<int> &frames);

        /// Get the name of this sprite
        const std::string &GetName() const { return mName; }

        // Set the vertices
        void SetVertices(std::vector<Vec2<double>> &vertices) { mVertices = vertices; }
        const std::vector<Vec2<double>> &GetVertices() const { return mVertices; }

    protected:
        // Texture to draw
        std::vector<SDL_Texture *> mTextures;
        // Width/height
        Vec2<int> mTexSize = Vec2<int>(0, 0);
        // Origin
        Vec2<int> mOrigin = Vec2<int>(0, 0);
        // Polygon vertices
        std::vector<Vec2<double>> mVertices;

        float mFps = 12.0f;

        std::string mName;

        // Map of animation name to vector of textures corresponding to the animation
        std::unordered_map<std::string, std::vector<int>> mAnims;

    private:
        inline bool __IsTempSprite__();
    };
}