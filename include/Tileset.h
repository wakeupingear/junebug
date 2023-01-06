#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Actors.h"

#include <vector>

namespace junebug
{
    class Tileset : public VisualActor
    {
    public:
        enum TilesetEditMode
        {
            None = 0,
            TileDraw = 1,
            TileErase = 2,
            TileDrawErase = 3
        };

        Tileset(std::string sprite = "", Vec2<int> tileSize = Vec2<int>::Zero, Vec2<float> pos = Vec2<>::Zero);

        void FirstUpdate(float dt) override;
        void InternalUpdate(float dt) override;
        void Draw() override;

        void SetTileSize(Vec2<int> tileSize) { mTileSize = tileSize; };
        Vec2<int> GetTileSize() const { return mTileSize; };
        void SetTiles(std::vector<std::vector<int>> tiles) { mTiles = tiles; };
        std::vector<std::vector<int>> GetTiles() const { return mTiles; };

        Vec2<int> WorldToTile(Vec2<float> pos);
        Vec2<int> WorldToTile(Vec2<int> pos) { return WorldToTile(Vec2<float>(pos)); }
        Vec2<float> TileToWorld(Vec2<int> pos);

        bool SetTile(Vec2<int> tilePos, int tile);
        inline bool SetWorldTile(Vec2<float> pos, int tile) { return SetTile(WorldToTile(pos), tile); }
        inline bool SetWorldTile(Vec2<int> pos, int tile) { return SetWorldTile(Vec2<float>(pos), tile); }

        int GetTile(Vec2<int> tilePos);
        inline int GetWorldTile(Vec2<float> pos) { return GetTile(WorldToTile(pos)); }
        inline int GetWorldTile(Vec2<int> pos) { return GetWorldTile(Vec2<float>(pos)); }

        void SetColliders(std::vector<bool> colliders);
        std::vector<bool> GetColliders() const { return mColliders; };
        void EnableCollision();
        void DisableCollision();
        void SetCollLayer(std::string layer);
        std::string GetCollLayer() { return mCollLayer; };

        bool TilePosHasCollider(Vec2<int> tile);
        bool WorldPosHasCollider(Vec2<float> pos) { return TilePosHasCollider(WorldToTile(pos)); }

        void SetEditMode(TilesetEditMode mode) { mEditMode = mode; };
        TilesetEditMode GetEditMode() const { return mEditMode; };

    private:
        Vec2<int> mTileSize;
        bool mCenterTopLeft{true};

        std::vector<std::vector<int>> mTiles;

        RoundDir mSpacingRoundDir{RoundDir::Down};

        inline float GetTileWidth();
        inline float GetTileHeight();

        class TileCollider *mColl{nullptr};
        std::vector<bool> mColliders;

        std::string mCollLayer{""};

        TilesetEditMode mEditMode{TilesetEditMode::None};
        std::string mDrawInput, mEraseInput;
        int mDrawTile{0};
    };
}