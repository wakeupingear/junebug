#include "components/TileCollider.h"
#include "components/PolygonCollider.h"
#include "Game.h"
#include "Sprite.h"
#include "Tileset.h"
#include "Camera.h"

#include <map>

using namespace junebug;

TileCollider::TileCollider(class VisualActor *owner, std::vector<VerticesPtr> &collisionBounds, std::string layer) : Collider(owner, layer)
{
    mType = CollType::TilesetIndividual;
    UpdateCollEntry(true);
    mOwner = dynamic_cast<Tileset *>(owner);
    if (!owner)
    {
        PrintLog("TileCollider: Owner is not a Tileset");
        delete this;
    }

    for (int i = 0; i < mOwner->GetNumTiles(); i++)
    {
        mColliders.push_back(PolygonCollisionBounds());
        mColliders[i].LoadVertices(collisionBounds[i]);
    }
}

void TileCollider::Update(float dt)
{
    UpdateCollPositions();
}

bool TileCollider::Intersects(Collider *_other)
{
    Vec2<float> offset;
    return Intersects(_other, offset) != CollSide::None;
}

CollSide TileCollider::Intersects(Collider *_other, Vec2<float> &offset)
{
    offset = Vec2<float>::Zero;

    if (_other->GetType() == CollType::Polygon)
    {
        PolygonCollider *other = static_cast<PolygonCollider *>(_other);
        auto otherBounds = other->GetCollBounds();
        Vec2<int> min, max;
        Vec2<float> otherOffset = Vec2<float>::Zero;
        Vec2<float> oppositeDir = Vec2<float>(_other->mOwner->GetPosition() - _other->mOwner->GetPrevPosition());
        oppositeDir.Normalize();

        auto &squareColliders = mOwner->GetSquareColliders();

        std::map<Vec2<float>, std::pair<float, float>> axisCount;
        for (int i = 0; i < 8; i++)
        {
            axisCount.clear();
            mOwner->GetCullBounds(otherBounds.topLeft + otherOffset, otherBounds.bottomRight + otherOffset, min, max);

            bool collided = false;
            for (Vec2<int> tile = min; tile.y <= max.y; tile.y++)
            {
                for (tile.x = min.x; tile.x <= max.x; tile.x++)
                {
                    int tileIndex = mOwner->GetTile(tile);
                    if (tileIndex == -1)
                        continue;

                    //if (tileIndex < squareColliders.size() && squareColliders[tileIndex])
                    //    continue;

                    auto &collBounds = mColliders[tileIndex];
                    Vec2<float> tilePos = Vec2<float>(mOwner->TileToWorld(tile));

                    float overlap = FLT_MAX;
                    Vec2<float> minAxis = Vec2<float>::Zero;
                    bool thisIntersects = collBounds.CheckAxes(other->mCollBounds, overlap, minAxis, tilePos, Vec2<float>::Zero);
                    if (!thisIntersects)
                        continue;
                    float currentOverlap = overlap;
                    bool otherIntersects = other->mCollBounds.CheckAxes(collBounds, overlap, minAxis, Vec2<float>::Zero, tilePos);
                    if (!otherIntersects)
                        continue;

                    if (overlap < currentOverlap)
                        overlap = -overlap;
                    if (overlap < 0)
                    {
                        overlap = -overlap;
                        minAxis = -1 * minAxis;
                    }

                    float val = Vec2<float>::Dot(oppositeDir, minAxis) + (oppositeDir == minAxis ? 0.5 : 0);

                    auto it = axisCount.find(minAxis);
                    if (it == axisCount.end())
                        axisCount[minAxis] = std::pair<int, int>(val, overlap);
                    else
                    {
                        axisCount[minAxis].first += val;
                        axisCount[minAxis].second = Max(axisCount[minAxis].second, overlap);
                    }
                }
            }

            if (!axisCount.empty())
            {
                Vec2<float> minAxis = Vec2<float>::Zero;
                float maxCount = FLT_MIN;
                float minOverlap = FLT_MAX;
                for (auto &axis : axisCount)
                {
                    if (axis.second.first > maxCount)
                    {
                        maxCount = axis.second.first;
                        minAxis = axis.first;
                        minOverlap = axis.second.second;
                    }
                }

                otherOffset -= Vec2<float>(minAxis * minOverlap);
                other->UpdateCollPositions(otherOffset);
            }
            else
                break;
        }

        offset = Vec2<float>(otherOffset);
        return otherOffset != Vec2<float>::Zero ? FlipCollSide(VecCollSide(offset)) : CollSide::None;
    }
    else if (_other->GetType() == CollType::TilesetIndividual)
        return _other->Intersects(this, offset);

    return CollSide::None;
}

void TileCollider::UpdateCollPositions(Vec2<float> offset)
{
    for (auto &collBounds : mColliders)
    {
        collBounds.UpdateWorldVertices(offset, 0, Vec2<float>::One, Vec2<int>::Zero);
    }

    UpdateMergedColliders();
}

void TileCollider::Draw()
{
    Camera *cam = Game::Get()->GetActiveCamera();
    if (!cam)
        return;

    Vec2<int> min, max;
    mOwner->GetCullBounds(cam->GetPosition(), cam->GetBottomRight(), min, max);
    auto &squareColliders = mOwner->GetSquareColliders();

    // Draw all colliders except for squares
    Vec2<float> pos;
    for (Vec2<int> tile = min; tile.y <= max.y; tile.y++)
    {
        for (tile.x = min.x; tile.x <= max.x; tile.x++)
        {
            int tileIndex = mOwner->GetTile(tile);
            if (tileIndex == -1 || tileIndex >= mColliders.size() || !mColliders[tileIndex].vertices || (tileIndex < squareColliders.size() && squareColliders[tileIndex]))
                continue;

            DrawPolygonOutline(*mColliders[tileIndex].vertices, Color::Red, mOwner->TileToWorld(tile));
        }
    }

    // Draw merged colliders
    for (auto &collider : mMergedColliders)
    {
        DrawPolygonOutline(*collider.vertices, Color::Green);
    }
}

void TileCollider::UpdateMergedColliders()
{
    const auto &tiles = mOwner->GetTiles();
    const auto &squareColliders = mOwner->GetSquareColliders();
    Vec2<float> ownerSize = Vec2<float>(mOwner->GetTileSize() * mOwner->GetScale());

    mMergedColliders.clear();
    mMergedColliderVertices.clear();

    std::vector<std::vector<bool>> visited(tiles.size());
    for (size_t i = 0; i < visited.size(); i++)
    {
        visited[i].resize(tiles[i].size());
        visited[i].assign(tiles[i].size(), false);
    }

    for (Vec2<int> tile = Vec2<int>::Zero; tile.y < tiles.size(); tile.y++)
    {
        for (tile.x = 0; tile.x < tiles[tile.y].size(); tile.x++)
        {
            if (visited[tile.y][tile.x])
                continue;
            visited[tile.y][tile.x] = true;

            int tileIndex = mOwner->GetTile(tile);
            if (tileIndex == -1 || tileIndex >= squareColliders.size() || !squareColliders[tileIndex])
                continue;

            int count = 1;
            Vec2<float> offset = Vec2<float>(tile) * ownerSize;

            // Check if the adjacent x tiles are also squares
            int xTileIndex = mOwner->GetTile(tile + Vec2<int>(1, 0));
            if (xTileIndex != -1 && xTileIndex < squareColliders.size() && squareColliders[xTileIndex])
            {
                while (xTileIndex != -1 && xTileIndex < squareColliders.size() && squareColliders[xTileIndex])
                {
                    visited[tile.y][tile.x + count] = true;
                    count++;
                    xTileIndex = mOwner->GetTile(tile + Vec2<int>(count, 0));
                }

                VerticesPtr vertices = std::make_shared<Vertices>(Vertices{offset, offset + Vertex(count * ownerSize.x, 0), offset + Vertex(count * ownerSize.x, ownerSize.y), offset + Vertex(0, ownerSize.y)});

                PolygonCollisionBounds bounds;
                mMergedColliders.push_back(bounds);
                mMergedColliderVertices.push_back(vertices);
                mMergedColliders.back().LoadVertices(mMergedColliderVertices.back());

                continue;
            }
        }
    }
}