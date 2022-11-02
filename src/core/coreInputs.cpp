#include "JGame.h"

using namespace junebug;

void JGame::ProcessInput()
{
    SDL_Event event;
    std::unordered_map<Uint8, int> newInputs;

    // Read poll events
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            if (options.autoCloseOnQuit)
                mGameIsRunning = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mWindow))
                mGameIsRunning = false;
            else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                mScreenWidth = event.window.data1;
                mScreenHeight = event.window.data2;
                //SDL_SetWindowSize(mWindow, mScreenWidth, mScreenHeight);
                print("Window resized to", mScreenWidth, mScreenHeight);
            }
            break;
        default:
            break;
        }
    }

    // Read keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    for (auto &[key, inputs] : mInputMapping)
    {
        inputs.second = 0;
        for (auto input : inputs.first)
        {
            if (state[input])
            {
                auto loc = mInputs.find(input);
                int val = loc != mInputs.end() ? loc->second + 1 : 1;
                newInputs[input] = val;
                inputs.second = std::max(inputs.second, val);
            }
        }
    }
    mInputs = newInputs;

    // Read mouse state
    int x, y;
    SDL_GetMouseState(&x, &y);
    mMousePos.x = (float)x;
    mMousePos.y = (float)y;

    // Default Events
    if (Input(JB_INPUT_QUIT))
    {
        mGameIsRunning = false;
    }
    if (Input(JB_INPUT_FULLSCREEN) == 1)
    {
        mFullscreen = !mFullscreen;
        SDL_SetWindowFullscreen(mWindow, mFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
        FlushPollEvents();
        SDL_ShowCursor(!mFullscreen);

        if (!mFullscreen)
        {
            SDL_SetWindowSize(mWindow, mScreenWidth, mScreenHeight);
            // SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        }
    }

    // User-defined callback
    OnInputsProcessed(state);
}

int JGame::Input(std::string key)
{
    auto it = mInputMapping.find(key);
    if (it == mInputMapping.end())
        return 0;
    return it->second.second;
}

void JGame::SetInputMapping(std::string key, std::vector<Uint8> inputs)
{
    mInputMapping[key] = make_pair(inputs, 0);
}

void JGame::SetInputMappings(
    std::vector<std::pair<std::string, std::vector<Uint8>>> inputMappings)
{
    for (auto &[name, inputs] : inputMappings)
    {
        JGame::SetInputMapping(name, inputs);
    }
}

void JGame::FlushPollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
    }
}