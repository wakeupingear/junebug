
#include "Rendering.h"
#include "Game.h"
#include "Sprite.h"
#include "Camera.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
namespace fs = std::filesystem;

namespace junebug
{
    void Game::AddSprite(std::string name, std::shared_ptr<class Sprite> sprite)
    {
        if (mSpriteCache.find(name) != mSpriteCache.end())
        {
            mSpriteCache[name].reset();
        }
        mSpriteCache[name] = sprite;
    }
    Vec2<float> GetDrawPosition(Vec2<float> pos)
    {
        Camera *camera = Game::Get()->GetActiveCamera();
        if (!camera)
            return Vec2<>::Zero;

        return (pos - camera->GetPosition()) * camera->GetZoom();
    }

    std::shared_ptr<Sprite> LoadSprite(std::string &imagePath)
    {
        if (imagePath.empty())
            return nullptr;
        auto it = Game::Get()->GetSpriteCache().find(imagePath);
        if (it != Game::Get()->GetSpriteCache().end())
            return it->second;

        std::shared_ptr<Sprite> sprite(new Sprite());

        std::error_code ec;
        if (fs::is_directory(imagePath, ec))
        {
            if (!sprite->LoadMetadataFile(imagePath))
            {
                sprite.reset();
                Game::Get()->AddSprite(imagePath, nullptr);
                return nullptr;
            }
        }
        else
        {
            if (ec)
            {
                PrintLog("Error for", imagePath, "in is_directory:", ec.message());
                sprite.reset();
                Game::Get()->AddSprite(imagePath, nullptr);
                return nullptr;
            }

            if (!sprite->LoadTextureFile(imagePath))
            {
                sprite.reset();
                Game::Get()->AddSprite(imagePath, nullptr);
                return nullptr;
            }
        }

        Game::Get()->AddSprite(imagePath, sprite);
        return sprite;
    }

    void DrawSprite(std::string &imagePath, int frame, const Vec2<float> &pos, const SpriteProperties &properties)
    {
        Game *game = Game::Get();
        if (!game)
            return;
        SDL_Renderer *renderer = game->GetRenderer();
        if (!renderer)
            return;
        std::shared_ptr<Sprite> sprite = LoadSprite(imagePath);
        if (!sprite)
            return;

        sprite->Draw(game->GetActiveCamera(), renderer, pos, Vec2<int>::Zero, sprite->GetTexSize(), frame, properties);
    }

    void DrawSpritePart(std::string &imagePath, int frame, const Vec2<float> &pos, const Vec2<int> &partPos, const Vec2<int> &partSize, const SpriteProperties &properties)
    {
        Game *game = Game::Get();
        if (!game)
            return;
        SDL_Renderer *renderer = game->GetRenderer();
        if (!renderer)
            return;
        std::shared_ptr<Sprite> sprite = LoadSprite(imagePath);
        if (!sprite)
            return;

        sprite->Draw(game->GetActiveCamera(), renderer, pos, partPos, partSize, frame, properties);
    }

    void DrawText(std::string text, const Vec2<float> &pos, const TextEffects effects)
    {
        Game *game = Game::Get();
        if (!game)
            return;
        FC_Font *font = game->GetCurrentFont();
        if (!font)
        {
            PrintLog("No font loaded");
            return;
        }
        SDL_Renderer *renderer = game->GetRenderer();
        if (!renderer)
            return;

        Vec2<float> dPos = GetDrawPosition(pos);
        TextEffects cEffects(effects);
        float zoom = game->GetActiveCamera()->GetZoom();
        cEffects.scale.x = zoom;
        cEffects.scale.y = zoom;
        FC_DrawEffect(font, renderer, dPos.x, dPos.y, cEffects, text.c_str());
    }

    void DrawRectangle(const Vec2<float> &topLeft, const Vec2<float> &bottomRight, const Color &color)
    {
        Game *game = Game::Get();
        if (!game)
            return;
        SDL_Renderer *renderer = game->GetRenderer();
        if (!renderer)
            return;

        Camera *camera = game->GetActiveCamera();
        const Vec2<float> &camPos = (camera) ? camera->GetPosition() : Vec2<>::Zero;

        SDL_Rect rect = {static_cast<int>(topLeft.x - camPos.x), static_cast<int>(topLeft.y - camPos.y), static_cast<int>(bottomRight.x - topLeft.x), static_cast<int>(bottomRight.y - topLeft.y)};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    void DrawLine(const Vec2<float> &start, const Vec2<float> &end, const Color &color, const float thickness)
    {
        Game *game = Game::Get();
        if (!game)
            return;
        SDL_Renderer *renderer = game->GetRenderer();
        if (!renderer)
            return;

        Camera *camera = game->GetActiveCamera();
        const Vec2<float> &camPos = (camera) ? camera->GetPosition() : Vec2<>::Zero;

        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
        bool shouldChange = (r != color.r || g != color.g || b != color.b || a != color.a);

        if (shouldChange)
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        SDL_RenderDrawLine(renderer, static_cast<int>(start.x - camPos.x), static_cast<int>(start.y - camPos.y), static_cast<int>(end.x - camPos.x), static_cast<int>(end.y - camPos.y));

        if (shouldChange)
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }

    void DrawPolygonOutline(const Vertices &vertices, const Color &color, const Vec2<float> offset, const float thickness)
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            DrawLine(offset + vertices[i], offset + vertices[(i + 1) % vertices.size()], color);
        }
    }

    void DrawTexture(SDL_Texture *texture, const Vec2<float> &pos, const Vec2<int> &size)
    {
        Game *game = Game::Get();
        if (!game)
            return;
        SDL_Renderer *renderer = game->GetRenderer();
        if (!renderer)
            return;

        Vec2<float> dPos = GetDrawPosition(pos);
        SDL_Rect rect = {static_cast<int>(dPos.x), static_cast<int>(dPos.y), size.x, size.y};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }

    SDL_Texture *CopyTexture(SDL_Texture *texture)
    {
        if (!texture)
            return nullptr;
        Game *game = Game::Get();
        if (!game)
            return nullptr;
        SDL_Renderer *renderer = game->GetRenderer();
        if (!renderer)
            return nullptr;

        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Texture *newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h), *oldTarget = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, newTexture);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_SetRenderTarget(renderer, oldTarget);
        return newTexture;
    }
}