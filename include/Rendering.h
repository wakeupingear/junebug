#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"
#include "Color.h"

#include "SDL_FontCache.h"
#include <string>

namespace junebug
{
    // Draw a sprite
    void SpriteDraw(std::string imagePath, const Vec2<float> &pos, const Vec2<float> &scale = Vec2<float>::One, const float rotation = 0.0f, const Color color = Color::White);

    struct TextEffects : FC_Effect
    {
        TextEffects() : FC_Effect()
        {
            color = Color::White;
            scale.x = 1.0f;
            scale.y = 1.0f;
            alignment = FC_ALIGN_CENTER;
        }
        TextEffects(const Color &color) : TextEffects()
        {
            this->color = color;
        }
        TextEffects(const Vec2<float> &scale) : TextEffects()
        {
            this->scale.x = scale.x;
            this->scale.y = scale.y;
        }
        TextEffects(const FC_AlignEnum  alignment) : TextEffects()
        {
            this->alignment = alignment;
        }
        TextEffects(const Color &color, const Vec2<float> &scale) : TextEffects(color)
        {
            this->color = color;
            this->scale.x = scale.x;
            this->scale.y = scale.y;
        }
        TextEffects(const Color &color, const FC_AlignEnum  alignment) : TextEffects(color)
        {
            this->color = color;
            this->alignment = alignment;
        }
        TextEffects(const Vec2<float> &scale, const FC_AlignEnum  alignment) : TextEffects(scale)
        {
            this->scale.x = scale.x;
            this->scale.y = scale.y;
            this->alignment = alignment;
        }
        TextEffects(const Color &color, const Vec2<float> &scale, const FC_AlignEnum alignment) : FC_Effect()
        {
            this->color = color;
            this->scale.x = scale.x;
            this->scale.y = scale.y;
            this->alignment = alignment;
        }
    };
    // Draw text
    void TextDraw(std::string text, const Vec2<float> &pos, const TextEffects effects = {});
};