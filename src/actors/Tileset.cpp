#include "Tileset.h"
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
}

void Tileset::InternalFirstUpdate(float dt)
{
    // Set the offset to the sprite's origin
    // This is so that the sprite is drawn from the top left corner regardless of the sprite's actual origin
    if (mCenterTopLeft)
    {
        Vec2<int> origin = GetSprite()->GetOrigin();
        MovePosition(Vec2((float)origin.x, (float)origin.y) * mScale);
    }

    mRoundToCamera = true;

    Game *game = Game::Get();
    if (game)
    {
        game->SetInputMappings({mDrawInput,
                                mEraseInput,
                                mRotateCWInput,
                                mRotateCCWInput,
                                mFlipXInput,
                                mFlipYInput});
    }

    Sprite *sprite = GetSprite();
    if (sprite)
    {
        Vec2 sprSize = sprite->GetTexSize();
        mNumTiles = (int)(sprSize.x / mTileSize.x) * (int)(sprSize.y / mTileSize.y);
    }
}

void Tileset::InternalUpdate(float dt)
{
    bool changed = false;
    if (mEditMode != TilesetEditMode::None)
    {
        mDrawAngle = (mDrawAngle + InputsPressedDir(mRotateCWInput, mRotateCCWInput) * 90) % 360;
        if (mDrawAngle < 0)
            mDrawAngle += 360;
        if (InputPressed(mFlipXInput))
            mDrawFlip.x *= -1;
        if (InputPressed(mFlipYInput))
            mDrawFlip.y *= -1;

        if (Input(mDrawInput))
            changed = SetWorldTile(Game::Get()->GetMousePos(), TransformTile(mDrawTile, mDrawAngle, mDrawFlip));
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

    int angle = 0;
    Vec2<int> flip;

    for (std::vector<int> &r : mTiles)
    {
        for (int tile : r)
        {
            if (tile < 0)
            {
                pos.x += tileGameWidth;
                continue;
            }

            int baseTile = tile % mNumTiles;
            partPos.x = (baseTile % (sprSize.x / mTileSize.x)) * mTileSize.x;
            partPos.y = (baseTile / (sprSize.y / mTileSize.y)) * mTileSize.y;

            GetTileTransform(tile, angle, flip);

            DrawSpritePart(
                mSpritePath, 0, pos, partPos, mTileSize, {mScale * flip, (float)angle, mColor});
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
        DrawSpritePart(mSpritePath, 0, TileToWorld(tile), partPos, mTileSize, {mScale * mDrawFlip, (float)mDrawAngle, Color(100, 100, 255, 100)});
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
    if (mNumTiles == 0) {
        printLog("numTiles is 0");
        return false;
    }
    if (tile.x < 0 || tile.y < 0 || tile.y >= mTiles.size() || tile.x >= mTiles[tile.y].size())
        return false;
    int tileNum = mTiles[tile.y][tile.x], baseTile = tileNum % mNumTiles;
    if (tileNum < 0 || baseTile >= mColliders.size())
        return false;
    return mColliders[baseTile];
}

void Tileset::SetCollLayer(std::string layer)
{
    mCollLayer = layer;
    if (!mColl)
        EnableCollision();
    else
        mColl->SetCollLayer(layer);
}

int Tileset::TransformTile(int tile, int angle, Vec2<int> flipped)
{
    int newTile = tile + (angle / 90) * mNumTiles + (flipped.x == -1 ? 1 : 0) * mNumTiles * 4 + (flipped.y == -1 ? 1 : 0) * mNumTiles * 8;
    return newTile;
}

void Tileset::GetTileTransform(int tile, int &angle, Vec2<int> &flipped)
{
    int baseTile = tile % mNumTiles;
    angle = (tile / mNumTiles) % 4 * 90;
    flipped.x = (tile / mNumTiles / 4) % 2 == 1 ? -1 : 1;
    flipped.y = (tile / mNumTiles / 8) % 2 == 1 ? -1 : 1;
}