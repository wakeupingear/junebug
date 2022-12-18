#include "Rendering.h"
#include "JGame.h"
#include "Sprite.h"
#include "Camera.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
namespace fs = std::filesystem;

namespace junebug
{
    void JGame::AddSprite(std::string name, Sprite *sprite)
    {
        if (sprite == nullptr)
        {
            return;
        }
        if (mSpriteCache.find(name) != mSpriteCache.end())
        {
            delete mSpriteCache[name];
        }
        mSpriteCache[name] = sprite;
    }

    void SpriteDraw(std::string imagePath, const Vec2<float> &pos, const SpriteProperties &properties)
    {
        {
            JGame *game = JGame::Get();
            if (!game)
                return;
            SDL_Renderer *renderer = game->GetRenderer();
            if (!renderer)
                return;
            Camera *camera = game->GetActiveCamera();

            auto it = game->GetSpriteCache().find(imagePath);
            Sprite *sprite = nullptr;
            if (it == game->GetSpriteCache().end())
            {
                sprite = new Sprite();

                const fs::path path(imagePath);
                std::error_code ec;
                if (fs::is_directory(path, ec))
                {
                    sprite->SetAnimation(imagePath);
                }
                if (ec)
                {
                    std::cerr << "Error in is_directory: " << ec.message() << std::endl;
                    delete sprite;
                    return;
                }

                if (fs::is_regular_file(path, ec))
                {
                    sprite->SetTexture(JGame::Get()->GetTexture(imagePath));
                }

                if (ec)
                {
                    std::cerr << "Error in is_regular_file: " << ec.message() << std::endl;
                    delete sprite;
                    return;
                }

                game->AddSprite(imagePath, sprite);
            }
            else
                sprite = it->second;

            if (!sprite)
                return;

            sprite->Draw(camera, renderer, pos, properties);
        }
    }

    void TextDraw(std::string text, const Vec2<float> &pos, const TextEffects effects)
    {
        JGame *game = JGame::Get();
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

        Camera *camera = game->GetActiveCamera();
        const Vec2<float> &camPos = (camera) ? camera->_calcPos : Vec2<>::Zero;

        FC_DrawEffect(font, renderer, pos.x - camPos.x, pos.y - camPos.y, effects, text.c_str());
    }
}