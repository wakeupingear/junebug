#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Actors.h"

namespace junebug
{
    class Background : public VisualActor
    {
    public:
        Background(Vec2<float> pos);
        Background(std::string sprite = "", Vec2<float> rate = Vec2<float>::Zero, Vec2<float> offset = Vec2<float>::Zero);
        Background(std::string sprite, float rate, Vec2<float> offset = Vec2<float>::Zero);

        void Draw() override;

        void SetRate(Vec2<float> rate) { mRate = rate; }
        void SetRate(float rate) { mRate = Vec2<float>(rate, rate); }
        void SetOffset(Vec2<float> offset) { mOffset = offset; }

        void SetTile(Vec2<bool> tile) { mTile = tile; };
        void SetTile(bool tile)
        {
            mTile.x = tile;
            mTile.y = tile;
        };
        Vec2<bool> GetTileVec() { return mTile; };
        bool GetTile() { return mTile.x || mTile.y; };

    protected:
        Vec2<float> mRate{0.0f, 0.0f}, mOffset{0.0f, 0.0f};
        Vec2<bool> mTile{false, false};
        Vec2<int> mCullPaddingStart{1, 1}, mCullPaddingEnd{1, 1};

        void DrawSpriteBackground();
        void DrawColor();
    };
}
