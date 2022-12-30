#include "components/BoxCollider.h"
#include "Game.h"
#include "Sprite.h"
#include "Collisions.h"

using namespace junebug;

BoxCollider::BoxCollider(class VisualActor *owner, bool useSprite, std::string layer) : CollisionComponent(owner, layer), mUseSprite(useSprite)
{
    mType = CollType::Box;
}

void BoxCollider::Update(float dt)
{
    if (mUseSprite)
    {
        if (mParentSprite != mOwner->GetSpriteName())
        {
            mParentSprite = mOwner->GetSpriteName();
            Sprite *sprite = mOwner->GetSprite();
            if (sprite)
            {
                mSize = Vec2<float>(sprite->GetTexSize());
                mOffset = Vec2<float>(sprite->GetOrigin());
            }
        }
    }
}

Vec2<float> BoxCollider::GetSize()
{
    return mSize * mOwner->GetScale();
}
Vec2<float> BoxCollider::GetOffset()
{
    return mOffset * mOwner->GetScale();
}

Vec2<float> BoxCollider::GetMin() { return mOwner->GetPosition() - GetOffset(); };
Vec2<float> BoxCollider::GetMax() { return mOwner->GetPosition() + GetSize() - GetOffset(); };

bool BoxCollider::Intersects(CollisionComponent *_other)
{
    if (_other->GetType() == CollType::Box)
    {
        BoxCollider *other = dynamic_cast<BoxCollider *>(_other);

        Vec2<float> thisMin = GetMin(), thisMax = GetMax();
        Vec2<float> otherMin = other->GetMin(), otherMax = other->GetMax();

        if (thisMax.x < otherMin.x || thisMax.y < otherMin.y || thisMin.x > otherMax.x || thisMin.y > otherMax.y)
            return false;
        return true;
    }
    else if (_other->GetType() == CollType::Tileset)
        return _other->Intersects(this);

    return false;
}

CollSide BoxCollider::GetMinOverlap(CollisionComponent *_other, Vec2<float> &offset)
{
    if (_other->GetType() == CollType::Box)
    {
        if (!Intersects(_other))
            return CollSide::None;

        BoxCollider *other = dynamic_cast<BoxCollider *>(_other);

        Vec2<float> thisMin = GetMin(), thisMax = GetMax();
        Vec2<float> otherMin = other->GetMin(), otherMax = other->GetMax();

        float xOverlap = std::min(thisMax.x, otherMax.x) - std::max(thisMin.x, otherMin.x);
        float yOverlap = std::min(thisMax.y, otherMax.y) - std::max(thisMin.y, otherMin.y);

        if (xOverlap < yOverlap)
        {
            if (thisMax.x > otherMax.x)
            {
                offset.x = otherMax.x - thisMin.x;
                return CollSide::Left;
            }
            else
            {
                offset.x = otherMin.x - thisMax.x;
                return CollSide::Right;
            }
        }
        else
        {
            if (thisMax.y > otherMax.y)
            {
                offset.y = otherMax.y - thisMin.y;
                return CollSide::Top;
            }
            else
            {
                offset.y = otherMin.y - thisMax.y;
                return CollSide::Bottom;
            }
        }
    }
    else if (_other->GetType() == CollType::Tileset)
    {
        CollSide side = _other->GetMinOverlap(this, offset);
        if (false && (offset.x != 0.0f || offset.y != 0.0f))
            print("Offset from tile:", offset);
        return FlipCollSide(side);
    }
    return CollSide::None;
}