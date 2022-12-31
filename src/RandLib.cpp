// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "RandLib.h"
#include "Utils.h"

void Random::Init()
{
    std::random_device rd;
    Random::Seed(rd());
}

void Random::Seed(unsigned int seed)
{
    sGenerator.seed(seed);
}

float Random::GetFloat()
{
    return GetFloatRange(0.0f, 1.0f);
}

float Random::GetFloatRange(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(sGenerator);
}

int Random::GetIntRange(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(sGenerator);
}

template <typename T>
Vec2<T> Random::GetVec(const Vec2<T> &min, const Vec2<T> &max)
{
    Vec2<T> r = Vec2<T>(GetFloat(), GetFloat());
    return min + (max - min) * r;
}

template <typename T>
Vec3<T> Random::GetVec(const Vec3<T> &min, const Vec3<T> &max)
{
    Vec3<T> r = Vec3<T>(GetFloat(), GetFloat(), GetFloat());
    return min + (max - min) * r;
}

bool Random::GetBool()
{
    return GetIntRange(0, 1) == 1;
}

int Random::GetSign()
{
    return GetIntRange(0, 1) == 1 ? 1 : -1;
}

float Random::GetSignf()
{
    return GetIntRange(0, 1) == 1 ? 1.0f : -1.0f;
}

Vec2<float> Random::GetDirection(float ang1, float ang2, float offset)
{
    float angle = ang1 <= ang2 ? GetFloatRange(ang1, ang2) + offset : GetFloatRange(ang2, ang1) + offset;
    return Vec2<float>(cosf(ToRadians(angle)), sinf(ToRadians(angle)));
}

std::mt19937 Random::sGenerator;