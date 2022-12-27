#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"

#include "SDL2/SDL.h"

namespace junebug
{
    class Camera
    {
    public:
        Camera();
        Camera(Vec2<float> pos, Vec2<float> size);
        Camera(Vec2<float> pos, Vec2<int> size);
        Camera(Vec2<int> pos, Vec2<int> size);

        Camera(Vec2<float> pos, Vec2<float> size, Vec2<float> screenPos, Vec2<float> screenSize);
        Camera(Vec2<int> pos, Vec2<int> size, Vec2<int> screenPos, Vec2<int> screenSize);
        ~Camera();

        void SetPosition(Vec2<float> newPos);
        const Vec2<float> GetPosition() const { return pos; }
        void SetSize(Vec2<float> newSize);
        const Vec2<float> GetSize() const { return size * (1.0f / mZoom); }
        const Vec2<float> GetScreenPos() const { return _calcScreenPos; }
        const Vec2<float> GetScreenSize() const { return _calcScreenSize; }

        const Vec2<float> GetCenter();
        const Vec2<float> GetBottomRight();

        const Vec2<float> GetScreenCenter();
        const Vec2<float> GetScreenBottomRight();

        bool GetStayInBounds() const { return mStayInBounds; };
        void SetStayInBounds(bool stayInBounds) { mStayInBounds = stayInBounds; };

        Vec2<float> GetBoundsEndOffset() { return mBoundsEndOffset; };
        void SetBoundsStartOffset(Vec2<float> offset) { mBoundsStartOffset = offset; };
        Vec2<float> GetBoundsStartOffset() { return mBoundsStartOffset; };
        void SetBoundsEndOffset(Vec2<float> offset) { mBoundsEndOffset = offset; };

        bool IsScreenCamera() { return mIsScreenCamera; };
        void SetIsScreenCamera(bool isCam) { mIsScreenCamera = isCam; };

        float GetZoom() { return mZoom; };
        void SetZoom(float zoom);
        inline void MoveZoom(float addZoom) { SetZoom(GetZoom() + addZoom); };

    protected:
        friend class Game;
        void _UpdateCoordinates();

        Vec2<float> pos;
        Vec2<float> size;

        Vec2<float> screenPos, _calcScreenPos;
        Vec2<bool> fractionalScreenPos{false, false};
        Vec2<float> screenSize, _calcScreenSize;
        Vec2<bool> fractionalScreenSize{false, false};

    private:
        SDL_Texture *Render(SDL_Renderer *renderer);

        SDL_Texture *renderTex = nullptr;

        void _UpdateCoordinate(Vec2<float> &vec, Vec2<float> &outVec, Vec2<bool> &fractional, float sW, float sH);
        void _GuessFractional(Vec2<float> &vec, Vec2<bool> &outVec);

        bool mStayInBounds = true;
        Vec2<float> mBoundsStartOffset = Vec2<>::Zero, mBoundsEndOffset = Vec2<>::Zero;
        void CheckBounds();

        // Whether the camera renders directly to the main display
        // Used to in-universe camera surfaces
        bool mIsScreenCamera = true;

        // Zoom level
        float mZoom = 1.0f;
    };
}
