#include "Inputs.h"
#include "Game.h"
#include "Camera.h"

namespace junebug
{
    float Input(std::string key, int player)
    {
        return Game::Get()->Input(key, player);
    };

    bool InputPressed(std::string key, int player)
    {
        return Game::Get()->InputPressed(key, player);
    };

    int InputsDir(std::string negKey, std::string posKey, int player)
    {
        return Game::Get()->InputsDir(negKey, posKey, player);
    };

    int InputsPressedDir(std::string negKey, std::string posKey, int player)
    {
        return Game::Get()->InputsPressedDir(negKey, posKey, player);
    };

    void ShakeCamera(Vec2<int> intensity, float duration)
    {
        Game::Get()->ShakeCamera(intensity, duration);
    };
    void ShakeCamera(Camera *camera, Vec2<int> intensity, float duration)
    {
        Game::Get()->ShakeCamera(camera, intensity, duration);
    };
};