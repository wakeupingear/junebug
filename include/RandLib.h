// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <random>
#include "MathLib.h"

class Random
{
public:
    static void Init();

    // Seed the generator with the specified int
    // NOTE: You should generally not need to manually use this
    static void Seed(unsigned int seed);

    // Get a float between 0.0f and 1.0f
    static float GetFloat();

    // Get a float from the specified range
    static float GetFloatRange(float min, float max);

    // Get an int from the specified range
    static int GetIntRange(int min, int max);

    // Get a random vector given the min/max bounds
    template <typename T>
    static Vec2<T> GetVec(const Vec2<T> &min, const Vec2<T> &max);
    template <typename T>
    static Vec3<T> GetVec(const Vec3<T> &min, const Vec3<T> &max);

    // EXTRAS

    // Get a random boolean
    static bool GetBool();

    // Get a random sign
    static int GetSign();

    // Get a random sign, but float!
    static float GetSignf();

    // Create a random vector pointing in a direction
    static Vec2<float> GetDirection(float ang1 = 0.0f, float ang2 = 360.0f, float offset = 0.0f);

private:
    static std::mt19937 sGenerator;
};