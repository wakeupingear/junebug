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
    struct SpriteProperties
    {
        Vec2<float> scale{1.0f, 1.0f};
        float rotation{0.0f};
        Color color{Color::White};

        SpriteProperties() {}
        SpriteProperties(const Vec2<float> &scale)
            : scale(scale){};
        SpriteProperties(const float rotation) : rotation(rotation) {}
        SpriteProperties(const Color &color) : color(color) {}
        SpriteProperties(const Vec2<float> &scale, const float rotation)
            : scale(scale), rotation(rotation) {}
        SpriteProperties(const Vec2<float> &scale, const Color &color)
            : scale(scale), color(color) {}
        SpriteProperties(const float rotation, const Color &color)
            : rotation(rotation), color(color) {}
        SpriteProperties(const Vec2<float> &scale, const float rotation, const Color &color)
            : scale(scale), rotation(rotation), color(color){};
    };

    // Draw a sprite
    void SpriteDraw(std::string imagePath, const Vec2<float> &pos, const SpriteProperties &properties = {});

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
        TextEffects(const FC_AlignEnum alignment) : TextEffects()
        {
            this->alignment = alignment;
        }
        TextEffects(const Color &color, const Vec2<float> &scale) : TextEffects(color)
        {
            this->color = color;
            this->scale.x = scale.x;
            this->scale.y = scale.y;
        }
        TextEffects(const Color &color, const FC_AlignEnum alignment) : TextEffects(color)
        {
            this->color = color;
            this->alignment = alignment;
        }
        TextEffects(const Vec2<float> &scale, const FC_AlignEnum alignment) : TextEffects(scale)
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