#include "components/PolygonCollisionComponent.h"
#include "Actors.h"
#include "Utils.h"

using namespace junebug;

PolygonCollisionComponent::PolygonCollisionComponent(VisualActor *owner, std::string layer) : CollisionComponent(owner, layer)
{
    SetType(CollType::Polygon);
}

void PolygonCollisionComponent::Update(float dt)
{
    Sprite *sprite = mOwner->GetSprite();
    if (mParentSprite != mOwner->GetSpriteName())
    {
        mParentSprite = mOwner->GetSpriteName();
        if (sprite)
        {
            mVertices = sprite->GetVertices();

            mAxes.clear();
            for (int i = 0; i < mVertices.size(); i++)
            {
                Vec2<double> v1 = mVertices[i];
                Vec2<double> v2 = mVertices[(i + 1) % mVertices.size()];
                Vec2<double> edge = v2 - v1;
                Vec2<double> perp = Vec2<double>(-edge.y, edge.x);
                perp.Normalize();
                mAxes.push_back(perp);
            }
        }
    }

    UpdateCollPositions();
}

bool PolygonCollisionComponent::Intersects(CollisionComponent *_other)
{
    Vec2<float> offset;
    return Intersects(_other, offset);
}

bool PolygonCollisionComponent::Intersects(CollisionComponent *_other, Vec2<float> &offset)
{
    if (_other->GetType() == CollType::Polygon)
    {
        UpdateCollPositions();
        PolygonCollisionComponent *other = dynamic_cast<PolygonCollisionComponent *>(_other);

        double overlap = 1000000000;
        Vec2<double> minAxis = Vec2<double>::Zero;
        if (!CheckAxes(mAxes, other, overlap, minAxis) ||
            !CheckAxes(other->mAxes, other, overlap, minAxis))
            return false;

        offset = Vec2<float>(minAxis) * -overlap;

        return true;
    }
    return false;
}

CollSide PolygonCollisionComponent::GetMinOverlap(CollisionComponent *other, Vec2<float> &offset)
{
    if (other->GetType() == CollType::Polygon)
    {
        if (!Intersects(other, offset))
        {
            return CollSide::None;
        }

        return FlipCollSide(VecCollSide(offset));
    }
    return CollSide::None;
}

Vec2<double> PolygonCollisionComponent::Project(Vec2<double> &axis)
{
    if (mVertices.empty())
        return Vec2<double>(0, 0);

    double min = Vec2<double>::Dot(axis, mWorldVertices[0]);
    double max = min;
    for (int i = 1; i < mWorldVertices.size(); i++)
    {
        double p = Vec2<double>::Dot(axis, mWorldVertices[i]);
        if (p < min)
            min = p;
        else if (p > max)
            max = p;
    }

    return Vec2(min, max);
}

bool PolygonCollisionComponent::CheckAxes(std::vector<Vec2<double>> &axes, PolygonCollisionComponent *other, double &overlap, Vec2<double> &minAxis)
{
    for (int i = 0; i < mVertices.size(); i++)
    {
        Vec2<double> proj = Project(axes[i]), otherProj = other->Project(axes[i]);
        if (proj.x > otherProj.y || proj.y < otherProj.x)
            return false;
        double o = std::min(proj.y, otherProj.y) - std::max(proj.x, otherProj.x);
        if (o < overlap)
        {
            overlap = o;
            minAxis = axes[i];
        }
    }
    return true;
}

void PolygonCollisionComponent::UpdateCollPositions()
{
    Vec2<double> origin = Vec2<double>(mOwner->GetSprite()->GetOrigin()), scale = Vec2<double>(mOwner->GetScale());
    mWorldVertices.clear();
    float ang = ToRadians(mOwner->GetRotation());
    for (int i = 0; i < mVertices.size(); i++)
    {
        Vec2<double> v = mVertices[i];
        Vec2<double> newV(
            origin.x + (cos(ang) * (v.x - origin.x) + sin(ang) * (v.y - origin.y)),
            origin.y + (-sin(ang) * (v.x - origin.x) + cos(ang) * (v.y - origin.y)));
        newV = ((newV - origin) * scale) + Vec2<double>(mOwner->GetPosition());
        mWorldVertices.push_back(newV);
    }
}

void PolygonCollisionComponent::Draw()
{
    DrawPolygonOutline(mWorldVertices, Color::Red);
}