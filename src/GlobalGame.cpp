#include "Inputs.h"
#include "Game.h"
#include "Camera.h"

namespace junebug
{
    int InputsDir(std::string negKey, std::string posKey, int player);

    int InputsPressedDir(std::string negKey, std::string posKey, int player)
    {
        return Game::Get()->InputsPressedDir(negKey, posKey, player);
    };

    void ShakeCamera(Vec2<int> intensity, float duration);
    void ShakeCamera(Camera *camera, Vec2<int> intensity, float duration)
    {
        Game::Get()->ShakeCamera(camera, intensity, duration);
    };
};