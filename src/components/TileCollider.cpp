#include "components/TileCollider.h"
#include "components/BoxCollider.h"
#include "Game.h"
#include "Sprite.h"
#include "Tileset.h"

using namespace junebug;

TileCollider::TileCollider(class VisualActor *owner, std::string layer) : CollisionComponent(owner, layer)
{
    mType = CollType::Tileset;
    mOwner = dynamic_cast<Tileset *>(owner);
    if (!owner)
    {
        PrintLog("TileCollider: Owner is not a Tileset");
        delete this;
    }
}

bool TileCollider::Intersects(CollisionComponent *_other)
{

    if (_other->GetType() == CollType::Box)
    {
        BoxCollider *other = dynamic_cast<BoxCollider *>(_other);
        Vec2<int> tileMin = mOwner->WorldToTile(other->GetMin()), tileMax = mOwner->WorldToTile(other->GetMax());
        for (Vec2<int> tile = tileMin; tile.x <= tileMax.x; tile.x++)
        {
            for (tile.y = tileMin.y; tile.y <= tileMax.y; tile.y++)
            {
                if (mOwner->TilePosHasCollider(tile))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

CollSide TileCollider::GetMinOverlap(CollisionComponent *_other, Vec2<float> &offset)
{
    if (!_other)
        return CollSide::None;

    if (_other->GetType() == CollType::Box)
    {
        if (!Intersects(_other))
            return CollSide::None;

        BoxCollider *other = static_cast<BoxCollider *>(_other);

        CollSide lastSide = CollSide::None;
        Vec2<float> otherMin = other->GetMin(), otherMax = other->GetMax();
        for (int i = 0; i < 8; i++)
        {
            Vec2<int> tileMin = mOwner->WorldToTile(otherMin), tileMax = mOwner->WorldToTile(otherMax);
            Vec2<int> startTile = Vec2(-1, -1), endTile = Vec2(-1, -1);
            for (Vec2<int> tile = tileMin; tile.x <= tileMax.x; tile.x++)
            {
                for (tile.y = tileMin.y; tile.y <= tileMax.y; tile.y++)
                {
                    if (mOwner->TilePosHasCollider(tile))
                    {
                        startTile = tile;
                        endTile = tile;
                        while (endTile.x + 1 <= tileMax.x && mOwner->TilePosHasCollider(Vec2<int>(endTile.x + 1, endTile.y)))
                            endTile.x++;
                        while (endTile.y + 1 <= tileMax.y && mOwner->TilePosHasCollider(Vec2<int>(endTile.x, endTile.y + 1)))
                            endTile.y++;
                        break;
                    }
                }
            }

            if (startTile.x == -1 || startTile.y == -1)
                break;

            Vec2<float> thisMin = mOwner->TileToWorld(startTile), thisMax = mOwner->TileToWorld(endTile + Vec2(1, 1)), tempOffset = Vec2<>::Zero;

            float xOverlap = std::min(thisMax.x, otherMax.x) - std::max(thisMin.x, otherMin.x);
            float yOverlap = std::min(thisMax.y, otherMax.y) - std::max(thisMin.y, otherMin.y);

            if (xOverlap < yOverlap && !NearZero(xOverlap))
            {
                if (thisMax.x > otherMax.x)
                {
                    tempOffset.x = otherMax.x - thisMin.x;
                    lastSide = CollSide::Left;
                }
                else
                {
                    tempOffset.x = otherMin.x - thisMax.x;
                    lastSide = CollSide::Right;
                }
            }
            else
            {
                if (thisMax.y > otherMax.y)
                {
                    tempOffset.y = otherMax.y - thisMin.y;
                    lastSide = CollSide::Top;
                }
                else
                {
                    tempOffset.y = otherMin.y - thisMax.y;
                    lastSide = CollSide::Bottom;
                }
            }

            otherMin -= tempOffset;
            otherMax -= tempOffset;
        }

        offset = otherMin - other->GetMin();
        return lastSide;
    }
    return CollSide::None;
}