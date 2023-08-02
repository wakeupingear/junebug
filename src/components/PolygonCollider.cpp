#include "components/PolygonCollider.h"
#include "Actors.h"
#include "Utils.h"
#include "Game.h"

using namespace junebug;

void PolygonCollisionBounds::LoadVertices(const VerticesPtr vertices)
{
    this->vertices = vertices;
}

bool PolygonCollisionBounds::CheckAxes(const PolygonCollisionBounds &other, float &overlap, Vec2<float> &minAxis, Vec2<float> offset, Vec2<float> otherOffset)
{
    for (int i = 0; i < worldVertices.size(); i++)
    {
        Vec2<float> proj = Project(axes[i], offset), otherProj = other.Project(axes[i], otherOffset);
        if (proj.x >= otherProj.y || otherProj.x >= proj.y)
            return false;

        float o = otherProj.y - proj.x;
        if (o < overlap)
        {
            overlap = o;
            minAxis = axes[i];
        }
    }
    return true;
}

Vec2<float> PolygonCollisionBounds::Project(Vec2<float> &axis, Vec2<float> &offset) const
{
    if (worldVertices.empty())
        return Vec2<float>(0, 0);

    float min = Vec2<float>::Dot(axis, worldVertices[0] + offset);
    float max = min;
    for (int i = 1; i < worldVertices.size(); i++)
    {
        float p = Vec2<float>::Dot(axis, worldVertices[i] + offset);
        if (p < min)
            min = p;
        else if (p > max)
            max = p;
    }

    return Vec2(min, max);
}

void PolygonCollisionBounds::UpdateWorldVertices(const Vec2<float> &pos, float rot, const Vec2<float> &_scale, const Vec2<int> &_origin)
{
    if (!vertices)
        return;

    Vec2<float> origin = Vec2<float>(_origin), scale = Vec2<float>(_scale);
    worldVertices.clear();
    topLeft.x = topLeft.y = std::numeric_limits<float>::max();
    bottomRight.x = bottomRight.y = std::numeric_limits<float>::min();

    float ang = ToRadians(rot);
    for (int i = 0; i < vertices->size(); i++)
    {
        Vec2<float> v = vertices->at(i);
        Vec2<float> newV(
            origin.x + (cos(ang) * (v.x - origin.x) * scale.x + sin(ang) * (v.y - origin.y) * scale.y),
            origin.y + (-sin(ang) * (v.x - origin.x) * scale.x + cos(ang) * (v.y - origin.y) * scale.y));
        newV = ((newV - origin)) + Vec2<float>(pos);

        if (newV.x < topLeft.x)
            topLeft.x = (float)newV.x;
        if (newV.y < topLeft.y)
            topLeft.y = (float)newV.y;
        if (newV.x > bottomRight.x)
            bottomRight.x = (float)newV.x;
        if (newV.y > bottomRight.y)
            bottomRight.y = (float)newV.y;

        worldVertices.push_back(newV);
    }

    axes.clear();
    for (int i = 0; i < worldVertices.size(); i++)
    {
        Vec2<float> v1 = worldVertices[i];
        Vec2<float> v2 = worldVertices[(i + 1) % worldVertices.size()];
        Vec2<float> edge = v1 - v2;
        Vec2<float> perp = Vec2<float>(-edge.y, edge.x);
        perp.Normalize();
        axes.push_back(perp);
    }
}

PolygonCollider::PolygonCollider(VisualActor *owner, std::string layer) : Collider(owner, layer)
{
    mType = CollType::Polygon;
    UpdateCollEntry(true);
}

void PolygonCollider::Update(float dt)
{
    Sprite *sprite = mOwner->GetSprite();
    if (mParentSprite != mOwner->GetSpriteName())
    {
        mParentSprite = mOwner->GetSpriteName();
        if (sprite)
            mCollBounds.LoadVertices(sprite->GetVertices());
    }

    UpdateCollPositions();
}

bool PolygonCollider::Intersects(Collider *_other)
{
    Vec2<float> offset;
    return Intersects(_other, offset) != CollSide::None;
}

CollSide PolygonCollider::Intersects(Collider *_other, Vec2<float> &offset)
{
    offset = Vec2<float>::Zero;

    if (_other->GetType() == CollType::Polygon)
    {
        PolygonCollider *other = static_cast<PolygonCollider *>(_other);
        float overlap = 1000000000;
        Vec2<float> minAxis = Vec2<float>::Zero;

        bool thisIntersects = mCollBounds.CheckAxes(other->mCollBounds, overlap, minAxis);
        if (!thisIntersects)
            return CollSide::None;
        float currentOverlap = overlap;
        bool otherIntersects = other->mCollBounds.CheckAxes(mCollBounds, overlap, minAxis);
        if (!otherIntersects)
            return CollSide::None;

        if (overlap < currentOverlap)
            overlap = -overlap;
        offset = Vec2<float>(minAxis * overlap);
        return FlipCollSide(VecCollSide(offset));
    }
    else if (_other->GetType() == CollType::TilesetIndividual || _other->GetType() == CollType::TilesetMerged)
    {
        return _other->Intersects(this, offset);
    }

    return CollSide::None;
}

void PolygonCollider::UpdateCollPositions(Vec2<float> offset)
{
    mCollBounds.UpdateWorldVertices(mOwner->GetPosition() + offset, mOwner->GetRotation(), mOwner->GetScale(), mOwner->GetSprite()->GetOrigin());
}

void PolygonCollider::Draw()
{
    DrawPolygonOutline(mCollBounds.worldVertices, Color::Red);
}