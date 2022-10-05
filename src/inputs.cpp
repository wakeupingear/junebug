#include "Junebug.h"

using namespace Junebug;

void JGame::ProcessInput()
{
    SDL_Event event;
    std::unordered_map<Uint8, int> newInputs;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            if (mAutoCloseOnQuit)
                mGameIsRunning = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mWindow))
                mGameIsRunning = false;
            else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                mScreenWidth = event.window.data1;
                mScreenHeight = event.window.data2;
                SDL_SetWindowSize(mWindow, mScreenWidth, mScreenHeight);
                print("Window resized to", mScreenWidth, mScreenHeight);
            }
            break;
        default:
            break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    for (auto &[key, inputs] : mInputMapping)
    {
        for (auto input : inputs)
        {
            if (state[input])
            {
                auto loc = newInputs.find(input);
                newInputs[event.type] = loc != mInputs.end() ? loc->second + 1 : 1;
            }
        }
    }

    mInputs = newInputs;

    OnInputsProcessed(state);
}

int JGame::Input(std::string key)
{
    auto it = mInputMapping.find(key);
    if (it == mInputMapping.end())
        return 0;

    for (auto &i : it->second)
    {
        auto it2 = mInputs.find(i);
        if (it2 != mInputs.end())
            return it2->second;
    }
    return 0;
}

void JGame::SetInputMapping(std::string key, std::vector<Uint8> inputs)
{
    mInputMapping[key] = inputs;
}

void JGame::SetInputMappings(
    std::vector<std::pair<std::string, std::vector<Uint8>>> inputMapping)
{
    for (auto &[name, inputs] : inputMapping)
    {
        JGame::SetInputMapping(name, inputs);
    }
}