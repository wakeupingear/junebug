#include "Sprite.h"
#include "Actors.h"
#include "Game.h"
#include "Camera.h"
#include "Files.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
namespace fs = std::filesystem;

using namespace junebug;

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::SetOrigin(SpriteOrigin origin, const Vec2<int> &offset)
{
    switch (origin)
    {
    case SpriteOrigin::TopLeft:
        mOrigin = Vec2<int>::Zero;
        break;
    case SpriteOrigin::TopRight:
        mOrigin.x = mTexSize.x;
        break;
    case SpriteOrigin::BottomLeft:
        mOrigin.y = mTexSize.y;
        break;
    case SpriteOrigin::BottomRight:
        mOrigin = mTexSize;
        break;
    case SpriteOrigin::Center:
        mOrigin = mTexSize / 2;
        break;
    }

    mOrigin += offset;
}

void Sprite::Draw(Camera *cam, SDL_Renderer *renderer, const Vec2<float> &pos, const Vec2<int> &partPos, const Vec2<int> &partSize, int frame, const SpriteProperties &properties)
{
    if (frame < 0 || frame >= mTextures.size())
        return;
    SDL_Texture *texture = mTextures[frame];
    if (texture && cam)
    {
        SDL_Rect src;
        src.w = static_cast<int>(partSize.x);
        src.h = static_cast<int>(partSize.y);
        src.x = static_cast<int>(partPos.x);
        src.y = static_cast<int>(partPos.y);

        SDL_Rect dest;
        dest.w = static_cast<int>(partSize.x * Abs(properties.scale.x) * cam->GetZoom());
        dest.h = static_cast<int>(partSize.y * Abs(properties.scale.y) * cam->GetZoom());

        Vec2<float> dPos = GetDrawPosition(pos - mOrigin * properties.scale);
        if (properties.roundToCamera)
        {
            dPos.x = round(dPos.x);
            dPos.y = round(dPos.y);
        }
        dest.x = static_cast<int>(dPos.x);
        dest.y = static_cast<int>(dPos.y);

        SDL_SetTextureColorMod(texture, properties.color.r, properties.color.g, properties.color.b);
        SDL_SetTextureAlphaMod(texture, properties.color.a);
        if (properties.scale.x < 0 && properties.scale.y < 0)
            SDL_RenderCopyEx(renderer,
                             texture,
                             &src,
                             &dest,
                             -properties.rotation + 180.0f,
                             nullptr,
                             SDL_FLIP_NONE);
        else
            SDL_RenderCopyEx(renderer,
                             texture,
                             &src,
                             &dest,
                             -properties.rotation,
                             nullptr,
                             (properties.scale.x < 0) ? SDL_FLIP_HORIZONTAL : (properties.scale.y < 0) ? SDL_FLIP_VERTICAL
                                                                                                       : SDL_FLIP_NONE);
    }
}

void Sprite::AddTexture(SDL_Texture *texture)
{
    if (!texture || __IsTempSprite__())
        return;
    mTextures.push_back(texture);
    if (mTextures.size() == 1)
        SDL_QueryTexture(texture, nullptr, nullptr, &mTexSize.x, &mTexSize.y);
}

SDL_Texture *Sprite::LoadTextureFile(std::string &fileName)
{
    if (__IsTempSprite__())
        return nullptr;

    std::error_code ec;
    if (fs::is_regular_file(fileName, ec))
    {
        SDL_Texture *tex = Game::Get()->GetTexture(fileName);
        if (!tex)
            return nullptr;

        AddTexture(tex);
        if (ec)
        {
            PrintLog("Error for", fileName, "in is_regular_file:", ec.message());
            return nullptr;
        }
        return tex;
    }
    return nullptr;
}

bool Sprite::LoadMetadataFile(std::string &folder)
{
    if (__IsTempSprite__())
        return false;

    mName = StringSplitEntry(folder, "/", -1);
    Json json(folder + "/" + mName + ".json");
    if (!json.IsValid())
    {
        print("Sprite metadata", folder, "is invalid");
        return false;
    }

    mFps = Json::GetNumber(&json, "fps", mFps);

    // Don't unload textures from the sprite level; just clear the sprite's list
    mTextures.clear();
    std::vector<std::string> frames = Json::GetStringArray(&json, "frames");
    for (auto &frame : frames)
    {
        std::string fileName = folder + "/" + frame;
        if (!LoadTextureFile(fileName))
        {
            PrintLog("Failed to load frame", fileName);
        }
    }

    // Add default animation
    std::vector<int> framesInt;
    for (int i = 0; i < frames.size(); i++)
        framesInt.push_back(i);
    AddAnimation("_", framesInt);

    // Load animations
    if (json.GetDoc()->HasMember("animations") && json.Get("Animations")->value.IsObject())
    {
        const auto &animsRef = json.Get("animations")->value.GetObject();
        for (auto &animRef : animsRef)
        {
            std::string name = animRef.name.GetString();
            std::vector<int> frames;
            if (animRef.value.IsArray())
            {
                for (auto &v : animRef.value.GetArray())
                {
                    frames.push_back(Json::GetNumber<int>(v));
                }
            }
            AddAnimation(name, frames);
        }
    }

    // Set origin
    Vec2<float> origin = Json::GetVec2<float>(&json, "origin");
    if (origin.x >= 1.0f)
        mOrigin.x = origin.x;
    else
        mOrigin.x = origin.x * mTexSize.x;
    if (origin.y >= 1.0f)
        mOrigin.y = origin.y;
    else
        mOrigin.y = origin.y * mTexSize.y;

    // Load vertices
    if (json.GetDoc()->HasMember("colliders"))
    {
        const auto &verticesRef = json.Get("colliders")->value.GetArray();
        bool isFractional = false;
        for (auto &v : verticesRef)
        {
            Vec2<double> vertex = Json::GetVec2<double>(v);
            mVertices.push_back(vertex);
            isFractional |=
                ((!NearZero(vertex.x) && !NearZero(vertex.x - 1.0) && vertex.x < 1.0) ||
                 (!NearZero(vertex.y) && !NearZero(vertex.y - 1.0) && vertex.y < 1.0));
        }

        if (isFractional)
        {
            for (auto &v : mVertices)
            {
                v.x *= mTexSize.x;
                v.y *= mTexSize.y;
            }
        }
    }
    else
    {
        // Default box vertices
        mVertices.push_back(Vec2<double>(0, 0));
        mVertices.push_back(Vec2<double>(mTexSize.x, 0));
        mVertices.push_back(Vec2<double>(mTexSize.x, mTexSize.y));
        mVertices.push_back(Vec2<double>(0, mTexSize.y));
    }

    return true;
}

const std::vector<int> &Sprite::GetAnimation(const std::string &name)
{
    auto it = mAnims.find(name);
    if (it != mAnims.end())
        return it->second;

    auto defIt = mAnims.find("_");
    if (defIt != mAnims.end())
        return defIt->second;

    AddAnimation(name, std::vector<int>());
    return mAnims.at(name);
}

void Sprite::AddAnimation(const std::string &name,
                          const std::vector<int> &frames)
{
    if (__IsTempSprite__())
        return;
    mAnims.emplace(name, frames);
}

bool Sprite::__IsTempSprite__()
{
    return (this == &VisualActor::__tempSprite__);
}