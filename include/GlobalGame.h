#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <string>

// This file contains namespace-scoped functions that are used by the engine.
// They all map to functions that have to be called on the current Game instance.
// This is included purely to make it easier to use the engine, since you don't have to include

namespace junebug
{
#pragma region Input
    // Check a given input name.
    /// @param key The name of the input to check
    /// @returns number of frames the input has been held for
    inline float Input(std::string key, int player = 0)
    {
        return Game::Get()->Input(key, player);
    };
    // Check if a given input is pressed.
    /// @param key The name of the input to check
    /// @returns true if the input was first pressed this frame, false otherwise
    inline bool InputPressed(std::string key, int player = 0)
    {
        return Game::Get()->InputPressed(key, player);
    };
    // Get the int result of two opposite inputs.
    /// @param key1 The first input
    /// @param key2 The second input
    inline int InputsDir(std::string negKey, std::string posKey, int player = 0)
    {
        return Game::Get()->InputsDir(negKey, posKey, player);
    };
    // Get the int result of two opposite inputs being pressed this frame.
    /// @param key1 The first input
    /// @param key2 The second input
    inline int InputsPressedDir(std::string negKey, std::string posKey, int player = 0);
#pragma endregion

#pragma region Camera
    // Shake all screen cameras
    inline void ShakeCamera(Vec2<int> intensity, float duration)
    {
        Game::Get()->ShakeCamera(intensity, duration);
    };
    // Shake a given camera
    inline void ShakeCamera(class Camera *camera, Vec2<int> intensity, float duration);
#pragma endregion
};