#include "components/TileCollider.h"
#include "components/PolygonCollider.h"
#include "Game.h"
#include "Sprite.h"
#include "Tileset.h"
#include "Camera.h"

using namespace junebug;

TileCollider::TileCollider(class VisualActor *owner, std::vector<std::vector<Vec2<double>>> &collisionBounds, std::string layer) : CollisionComponent(owner, layer)
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

bool TileCollider::Intersects(CollisionComponent *_other)
{
    Vec2<float> offset;
    return Intersects(_other, offset) != CollSide::None;
}

CollSide TileCollider::Intersects(CollisionComponent *_other, Vec2<float> &_offset)
{
    if (_other->GetType() == CollType::Polygon)
    {
        UpdateCollPositions();
        PolygonCollider *other = static_cast<PolygonCollider *>(_other);
        auto otherBounds = other->GetCollBounds();

        Vec2<int> min, max;
        Vec2<double> offset = Vec2<double>::Zero;
        for (int i = 0; i < 8; i++)
        {
            mOwner->GetCullBounds(otherBounds.topLeft, otherBounds.bottomRight, min, max);

            bool collided = false;
            for (Vec2<int> tile = min; tile.y <= max.y; tile.y++)
            {
                for (tile.x = min.x; tile.x <= max.x; tile.x++)
                {
                    int tileIndex = mOwner->GetTile(tile);
                    if (tileIndex == -1)
                        continue;

                    auto &collBounds = mColliders[tileIndex];
                    Vec2<double> tilePos = Vec2<double>(tile) * mOwner->GetTileSize() * mOwner->GetScale();

                    double overlap = 1000000000;
                    Vec2<double> minAxis = Vec2<double>::Zero;
                    bool thisIntersects = collBounds.CheckAxes(other->mCollBounds, overlap, minAxis, tilePos, offset);
                    if (!thisIntersects)
                        return CollSide::None;
                    double currentOverlap = overlap;
                    bool otherIntersects = other->mCollBounds.CheckAxes(collBounds, overlap, minAxis, offset, tilePos);
                    if (!otherIntersects)
                        return CollSide::None;

                    if (overlap < currentOverlap)
                        overlap = -overlap;
                    offset -= (minAxis * overlap);
                    collided = true;
                    break;
                }

                if (collided)
                    break;
            }

            if (!collided)
                return CollSide::None;
        }

        _offset = Vec2<float>(offset);
        return FlipCollSide(VecCollSide(_offset));
    }
    else if (_other->GetType() == CollType::Tileset)
        return _other->Intersects(this, _offset);

    return CollSide::None;
}

void TileCollider::UpdateCollPositions()
{
    for (auto &collBounds : mColliders)
    {
        collBounds.UpdateWorldVertices(mOwner->GetPosition(), 0, mOwner->GetScale(), mOwner->GetSprite()->GetOrigin());
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