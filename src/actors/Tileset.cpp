#include "Tileset.h"
#include "Game.h"
#include "Camera.h"
#include "Sprite.h"
#include "components/TileCollider.h"
#include "GlobalGame.h"

using namespace junebug;

Tileset::Tileset(std::string sprite, Vec2<int> tileSize, Vec2<float> pos) : VisualActor(pos, sprite)
{
    if (tileSize.x <= 0 || tileSize.y <= 0)
        mTileSize = GetSpriteSize();
    else
        mTileSize = tileSize;

    mDrawInput = JB_INPUT_LEFT_CLICK;
    mEraseInput = JB_INPUT_RIGHT_CLICK;
}

void Tileset::FirstUpdate(float dt)
{
    // Set the offset to the sprite's origin
    // This is so that the sprite is drawn from the top left corner regardless of the sprite's actual origin
    if (mCenterTopLeft)
    {
        Vec2<int> origin = GetSprite()->GetOrigin();
        MovePosition(Vec2((float)origin.x, (float)origin.y) * mScale);
    }

    mRoundToCamera = true;
}

void Tileset::InternalUpdate(float dt)
{
    bool changed = false;

    if (mEditMode != TilesetEditMode::None)
    {
        if (Input(mDrawInput))
            changed = SetWorldTile(Game::Get()->GetMousePos(), mDrawTile);
        else if (Input(mEraseInput))
            changed = SetWorldTile(Game::Get()->GetMousePos(), -1);
    }

    if (changed)
    {
        Json *json = Game::Get()->GetSceneJSON();
        if (json)
        {
            auto actor = json->GetActor(mId);
            if (actor)
            {
                auto &allocator = json->GetDoc()->GetAllocator();
                Value tiles(kArrayType);
                for (std::vector<int> &r : mTiles)
                {
                    Value row(kArrayType);
                    for (int tile : r)
                        row.PushBack(tile, allocator);
                    tiles.PushBack(row, allocator);
                }
                
                actor->RemoveMember("tiles");
                actor->AddMember("tiles", tiles, allocator);
                json->Save();
            }
        }
    }
}

void Tileset::Draw()
{
    Vec2<int> sprSize = GetSpriteSize(), partPos;
    if (sprSize.x <= 0 || sprSize.y <= 0)
        return;

    float tileGameWidth = GetTileWidth(), tileGameHeight = GetTileHeight();
    Vec2<float> pos = GetPosition().Round(mSpacingRoundDir);
    float startX = pos.x;
    for (std::vector<int> &r : mTiles)
    {
        size_t x = 0;
        for (int tile : r)
        {
            if (tile < -1)
            {
                x++;
                continue;
            }

            partPos.x = (tile % (sprSize.x / mTileSize.x)) * mTileSize.x;
            partPos.y = (tile / (sprSize.y / mTileSize.y)) * mTileSize.y;

            DrawSpritePart(
                mSpritePath, 0, pos, partPos, mTileSize, {mScale, mRotation, mColor});
            pos.x += tileGameWidth;
        }
        pos.y += tileGameHeight;
        pos.x = startX;
    }

    if (mEditMode != TilesetEditMode::None)
    {
        Vec2<int> tile = WorldToTile(Game::Get()->GetMousePos());
        partPos.x = (mDrawTile % (sprSize.x / mTileSize.x)) * mTileSize.x;
        partPos.y = (mDrawTile / (sprSize.y / mTileSize.y)) * mTileSize.y;
        DrawSpritePart(mSpritePath, 0, TileToWorld(tile), partPos, mTileSize, {mScale, mRotation, Color(100, 100, 255, 100)});
    }
}

Vec2<int> Tileset::WorldToTile(Vec2<float> pos)
{
    if (mTileSize.x <= 0 || mTileSize.y <= 0)
        return Vec2<int>::Zero;
    pos -= GetPosition();
    if (mCenterTopLeft)
        pos += Vec2<float>(GetSprite()->GetOrigin()) * mScale;

    pos.x /= GetTileWidth();
    pos.y /= GetTileHeight();
    return Vec2<int>(pos.x, pos.y);
}

Vec2<float> Tileset::TileToWorld(Vec2<int> pos)
{
    Vec2<float> ret = GetPosition();
    if (mCenterTopLeft)
        ret -= Vec2<float>(GetSprite()->GetOrigin()) * mScale;
    ret.x += (float)pos.x * (float)GetTileWidth();
    ret.y += (float)pos.y * (float)GetTileHeight();
    return ret;
}

bool Tileset::SetTile(Vec2<int> tilePos, int tile)
{
    if (tilePos.x < 0 || tilePos.y < 0)
        return false;

    if (tile == -1)
    {
        if (mTiles.size() <= tilePos.y || mTiles[tilePos.y].size() <= tilePos.x)
            return false;

        mTiles[tilePos.y][tilePos.x] = -1;
        if (tilePos.x == mTiles[tilePos.y].size() - 1)
        {
            while (mTiles[tilePos.y].size() > 0 && mTiles[tilePos.y].back() == -1)
                mTiles[tilePos.y].pop_back();
            while (mTiles.size() > 0 && mTiles.back().size() == 0)
                mTiles.pop_back();
        }

        return true;
    }
    else
    {
        while (mTiles.size() <= tilePos.y)
            mTiles.push_back(std::vector<int>());
        while (mTiles[tilePos.y].size() <= tilePos.x)
            mTiles[tilePos.y].push_back(-1);
        if (mTiles[tilePos.y][tilePos.x] == tile)
            return false;

        mTiles[tilePos.y][tilePos.x] = tile;
        return true;
    }
}

int Tileset::GetTile(Vec2<int> tilePos)
{
    if (tilePos.x < 0 || tilePos.y < 0 || tilePos.y >= mTiles.size() || tilePos.x >= mTiles[tilePos.y].size())
        return -1;
    return mTiles[tilePos.y][tilePos.x];
}

float Tileset::GetTileWidth()
{
    return Round(mTileSize.x * mScale.x, mSpacingRoundDir);
}
float Tileset::GetTileHeight()
{
    return Round(mTileSize.y * mScale.y, mSpacingRoundDir);
}

void Tileset::SetColliders(std::vector<bool> colliders)
{
    mColliders = colliders;
    if (!mColl)
        EnableCollision();
}

void Tileset::EnableCollision()
{
    if (!mColl)
        mColl = new TileCollider(this, mCollLayer);
    else
        mColl->SetType(CollType::Tileset);
}
void Tileset::DisableCollision()
{
    if (mColl)
        mColl->SetType(CollType::None);
}

bool Tileset::TilePosHasCollider(Vec2<int> tile)
{
    if (tile.x < 0 || tile.y < 0 || tile.y >= mTiles.size() || tile.x >= mTiles[tile.y].size())
        return false;
    int tileNum = mTiles[tile.y][tile.x];
    if (tileNum < 0 || tileNum >= mColliders.size())
        return false;
    return mColliders[tileNum];
}

void Tileset::SetCollLayer(std::string layer)
{
    mCollLayer = layer;
    if (!mColl)
        EnableCollision();
    else
        mColl->SetCollLayer(layer);
}