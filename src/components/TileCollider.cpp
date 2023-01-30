#include "components/TileCollider.h"
#include "components/PolygonCollider.h"
#include "Game.h"
#include "Sprite.h"
#include "Tileset.h"
#include "Camera.h"

#include <map>

using namespace junebug;

TileCollider::TileCollider(class VisualActor *owner, std::vector<Vertices> &collisionBounds, std::string layer) : Collider(owner, layer)
{
    mType = CollType::Tileset;
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
        Vec2<double> oppositeDir = Vec2<double>(_other->mOwner->GetPosition() - _other->mOwner->GetPrevPosition());
        oppositeDir.Normalize();

        std::map<Vec2<double>, std::pair<float, double>> axisCount;
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

                    auto &collBounds = mColliders[tileIndex];
                    Vec2<double> tilePos = Vec2<double>(mOwner->TileToWorld(tile));

                    double overlap = 1000000000;
                    Vec2<double> minAxis = Vec2<double>::Zero;
                    bool thisIntersects = collBounds.CheckAxes(other->mCollBounds, overlap, minAxis, tilePos, Vec2<double>::Zero);
                    if (!thisIntersects)
                        continue;
                    double currentOverlap = overlap;
                    bool otherIntersects = other->mCollBounds.CheckAxes(collBounds, overlap, minAxis, Vec2<double>::Zero, tilePos);
                    if (!otherIntersects)
                        continue;

                    if (overlap < currentOverlap)
                        overlap = -overlap;
                    if (overlap < 0)
                    {
                        overlap = -overlap;
                        minAxis = -1 * minAxis;
                    }

                    float val = Vec2<double>::Dot(oppositeDir, minAxis) + (oppositeDir == minAxis ? 0.5 : 0);

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
                Vec2<double> minAxis = Vec2<double>::Zero;
                float maxCount = -100000.0f;
                double minOverlap = 1000000000;
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
    else if (_other->GetType() == CollType::Tileset)
        return _other->Intersects(this, offset);

    return CollSide::None;
}

void TileCollider::UpdateCollPositions(Vec2<float> offset)
{
    for (auto &collBounds : mColliders)
    {
        collBounds.UpdateWorldVertices(offset, 0, Vec2<float>::One, Vec2<int>::Zero);
    }
}

void TileCollider::Draw()
{
    Camera *cam = Game::Get()->GetActiveCamera();
    if (!cam)
        return;

    Vec2<int> min, max;
    mOwner->GetCullBounds(cam->GetPosition(), cam->GetBottomRight(), min, max);

    Vec2<double> pos;
    for (Vec2<int> tile = min; tile.y <= max.y; tile.y++)
    {
        for (tile.x = min.x; tile.x <= max.x; tile.x++)
        {
            int tileIndex = mOwner->GetTile(tile);
            if (tileIndex == -1)
                continue;

            DrawPolygonOutline(*mColliders[tileIndex].vertices, Color::Red, mOwner->TileToWorld(tile));
        }
    }
}