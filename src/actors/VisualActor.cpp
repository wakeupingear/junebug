#include "Actors.h"
#include "Sprite.h"
#include "Game.h"
#include "Rendering.h"

using namespace junebug;

Sprite VisualActor::__tempSprite__ = Sprite();

void VisualActor::Animation::Update(std::string name, float dt, std::string &nextAnim)
{
    if (finished)
        return;

    if (sprite.expired())
        return;

    auto &list = sprite.lock()->GetAnimation(sprAnimName);
    if (!list.size())
        return;

    frame += fps * dt;
    if (!loop)
    {
        if (fps > 0.0f && frame >= list.size())
        {
            finished = true;
            frame = list.size();
            return;
        }
        else if (fps < 0.0f && frame < list.size())
        {
            finished = true;
            frame = list.size();
            return;
        }
    }

    float newFrame = fmod(frame, list.size());
    if (newFrame != frame)
    {
        if (name == "_" && nextAnim != "")
        {
            sprAnimName = nextAnim;
            nextAnim = "";
        }
    }
    frame = newFrame;
}

VisualActor::VisualActor(Vec2<float> pos) : PureActor()
{
    SetPosition(pos);
}
VisualActor::VisualActor(Vec2<int> pos) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y))
{
}

VisualActor::VisualActor(Vec2<float> pos, std::string imagePath) : PureActor()
{
    SetPosition(pos);
    SetSprite(imagePath);
}
VisualActor::VisualActor(Vec2<int> pos, std::string imagePath) : VisualActor(Vec2<float>((float)pos.x, (float)pos.y), imagePath)
{
}

void VisualActor::SetSprite(std::string imagePath)
{
    if (imagePath == "")
    {
        mSpritePath = "";
        return;
    }

    if (imagePath == mSpritePath)
        return;

    Game *game = Game::Get();
    if (game)
        mSpritePath = game->GetAssetPaths().sprites + imagePath;

    Sprite *sprite = GetRawSprite();
    if (!sprite)
        return;
    mFrameAnimations.insert_or_assign("_", Animation("_", LoadSprite(mSpritePath), sprite->GetFps()));
}

Sprite *VisualActor::GetSprite()
{
    std::shared_ptr<Sprite> sprite = LoadSprite(mSpritePath);
    if (!sprite || !sprite.get())
        return &__tempSprite__;
    return sprite.get();
}
Sprite *VisualActor::GetRawSprite()
{
    return LoadSprite(mSpritePath).get();
}

std::string VisualActor::GetSpriteName()
{
    return mSpritePath;
}
Vec2<int> VisualActor::GetSpriteSize()
{
    Sprite *sprite = GetRawSprite();
    if (!sprite)
        return Vec2(1, 1);
    return sprite->GetTexSize();
}
Vec2<float> VisualActor::GetActorSize()
{
    Sprite *sprite = GetRawSprite();
    if (!sprite)
        return Vec2<float>::Zero;
    return mScale * sprite->GetTexSize();
}

void VisualActor::SetOrigin(SpriteOrigin origin, const Vec2<int> &offset)
{
    Sprite *sprite = GetRawSprite();
    if (!sprite)
        return;
    sprite->SetOrigin(origin, offset);
}

void VisualActor::SetPosition(const Vec2<float> &pos)
{
    mPosition = pos;
}
void VisualActor::MovePosition(const Vec2<float> &pos)
{
    mPosition += pos;
}
Vec2<float> VisualActor::GetPosition() const
{
    return mPosition;
}
Vec2<float> VisualActor::GetStartPosition() const
{
    return mStartPosition;
}

void VisualActor::ClampPosition(const Vec2<float> &min, const Vec2<float> &max)
{
    mPosition.x = std::max(min.x, std::min(mPosition.x, max.x));
    mPosition.y = std::max(min.y, std::min(mPosition.y, max.y));
}

void VisualActor::SetRotation(float rot)
{
    mRotation = rot;
}
float VisualActor::GetRotation() const
{
    return mRotation;
}

void VisualActor::SetAlpha(float alpha)
{
    mColor.a = alpha * 255;
}
void VisualActor::SetAlpha(int alpha)
{
    mColor.a = alpha;
}
int VisualActor::GetAlpha()
{
    return mColor.a;
}

void VisualActor::SetScale(const Vec2<float> &scale)
{
    mScale = scale;
}
void VisualActor::SetScale(float scale)
{
    mScale.x = scale;
    mScale.y = scale;
}
void VisualActor::SetScale(float scaleX, float scaleY)
{
    mScale.x = scaleX;
    mScale.y = scaleY;
}
Vec2<float> VisualActor::GetScale() const
{
    return mScale;
}

void VisualActor::SetFrame(int frame)
{
    auto it = mFrameAnimations.find("_");
    if (it != mFrameAnimations.end())
        it->second.frame = frame;
}
int VisualActor::GetFrame()
{
    auto it = mFrameAnimations.find("_");
    if (it != mFrameAnimations.end()) {
        Sprite *sprite = GetSprite();
        const std::vector<int> &frames = sprite->GetAnimation(it->second.sprAnimName);
        int frame = Clamp((int)it->second.frame, 0, (int)frames.size() - 1);
        return frames[frame];
    }
    return 0;
}

void VisualActor::SetFPS(float fps)
{
    auto it = mFrameAnimations.find("_");
    if (it != mFrameAnimations.end())
        it->second.fps = fps;
}
float VisualActor::GetFPS() const
{
    auto it = mFrameAnimations.find("_");
    if (it != mFrameAnimations.end())
        return it->second.fps;
    return 0.0f;
}

void VisualActor::SetColor(const Color &color)
{
    mColor = color;
}
Color VisualActor::GetColor() const
{
    return mColor;
}

void VisualActor::SetSpriteAnimation(std::string name)
{
    mNextSpriteAnimation = name;
}

void VisualActor::InternalFirstUpdate(float dt)
{
    mStartPosition = mPosition;
}

void VisualActor::InternalUpdate(float dt)
{
    Sprite *sprite = GetRawSprite();
    if (!sprite)
        return;

    for (auto &pair : mFrameAnimations)
    {
        Animation &anim = pair.second;
        anim.Update(pair.first, dt, mNextSpriteAnimation);
    }
}

void VisualActor::Draw()
{
    DrawSprite(mSpritePath, GetFrame(), mPosition, {mScale, mRotation, mColor, mRoundToCamera});
}