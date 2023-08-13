#include "Game.h"
#include "Camera.h"

using namespace junebug;

void Game::ProcessInput()
{
    DebugCheckpoint("Inputs", options.showDefaultDebugCheckpoints);

    // Store the current screen size in case fullscreen is toggled
    Vec2<int> oldScreenSize(mScreenWidth, mScreenHeight), oldWindowPos;
    SDL_GetWindowSize(mWindow, &mScreenWidth, &mScreenHeight);

    SDL_Event event = {0};
    std::unordered_map<Uint8, std::pair<int, float>> newInputs;

    // Read keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // Read poll events
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            if (options.autoCloseOnQuit)
                mGameIsRunning = false;
            break;
        case SDL_MOUSEMOTION:
        {
            int w, h;
            SDL_GetWindowSize(mWindow, &w, &h);
            w = std::max(w, 1);
            h = std::max(h, 1);
            mMouseScreenPos.x = event.motion.x * mScreenWidth / w;
            mMouseScreenPos.y = event.motion.y * mScreenHeight / h;
            if (options.screenStretch)
            {
                mMouseScreenPos.x /= ((float)w / (float)mRenderWidth);
                mMouseScreenPos.y /= ((float)h / (float)mRenderHeight);
            }

            // Get the mouse coordinates relative to a screen camera
            mMouseCamera = nullptr;
            float minDist = 10000000.0f;
            for (Camera *cam : mCameras)
            {
                if (!cam->IsScreenCamera())
                    continue;
                float dist = (mMouseScreenPos - cam->GetScreenCenter()).Length();
                if (dist < minDist)
                {
                    mMouseCamera = cam;
                    minDist = dist;
                }
            }

            mMousePos = mMouseScreenPos;
            if (mMouseCamera)
            {
                mMousePos += Vec2(
                    (int)(mMouseCamera->GetPosition().x - mMouseCamera->GetScreenPos().x),
                    (int)(mMouseCamera->GetPosition().y - mMouseCamera->GetScreenPos().y));
                mMouseOffset = mMousePos - mMouseCamera->GetPosition();
            }

            break;
        }
        case SDL_MOUSEBUTTONDOWN:
            mExtraStates[event.button.button + MOUSE_LEFT - 1] = 1;
            break;
        case SDL_MOUSEBUTTONUP:
            mExtraStates[event.button.button + MOUSE_LEFT - 1] = 0;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(mWindow))
                mGameIsRunning = false;
            else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                mScreenWidth = event.window.data1;
                mScreenHeight = event.window.data2;
            }
            break;
        default:
            break;
        }
    }

    // Loop through all inputs
    for (auto &playerMapping : mInputMappings)
    {
        for (auto &[key, inputs] : playerMapping)
        {
            inputs.second.first = 0;
            inputs.second.second = 0.0f;
            for (auto input : inputs.first)
            {
                if (state[input] || mExtraStates[input])
                {
                    auto loc = mInputs.find(input);
                    int frames = loc != mInputs.end() ? loc->second.first + 1 : 1;
                    float val = loc != mInputs.end() ? loc->second.second + mDeltaTime : mDeltaTime;
                    newInputs[input] = std::make_pair(frames, val);
                    inputs.second.first = std::max(inputs.second.first, frames);
                    inputs.second.second = std::max(inputs.second.second, val);
                }
            }
        }
    }
    mInputs = newInputs;

    // Default Events
    float quitTime = Input(JB_INPUT_QUIT, -1);
    if (!NearZero(quitTime) && quitTime > options.quitCloseTime)
    {
        mGameIsRunning = false;
    }
    if (InputPressed(JB_INPUT_FULLSCREEN, -1))
    {
        if (!mFullscreen)
        {
            SDL_GetWindowPosition(mWindow, &mPrevWindowPos.x, &mPrevWindowPos.y);
        }

        mFullscreen = !mFullscreen;
        SDL_SetWindowFullscreen(mWindow, mFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
        FlushPollEvents();

        if (!mFullscreen)
        {
            SDL_SetWindowSize(mWindow, mPrevScreenSize.x, mPrevScreenSize.y);
            SDL_SetWindowPosition(mWindow, mPrevWindowPos.x, mPrevWindowPos.y);
        }
        else
        {
            SDL_SetWindowPosition(mWindow, mPrevWindowPos.x, mPrevWindowPos.y);
            if (options.resizable)
            {
                auto displaySize = GetDisplaySize();
                mScreenWidth = displaySize.x;
                mScreenHeight = displaySize.y;
            }
        }
    }

    // Store the previous screen size if it changed
    // This happens after the checks since it can change in many places
    if (oldScreenSize.x != mScreenWidth || oldScreenSize.y != mScreenHeight)
    {
        // Update the screen cameras
        if (!Game::Get()->GetOptions().screenStretch)
        {
            for (Camera *cam : mCameras)
            {
                if (cam->IsScreenCamera())
                {
                    Vec2<float> ratio = Vec2<float>(mScreenWidth, mScreenHeight) / Vec2<float>(oldScreenSize);
                    cam->SetSize(cam->GetSize() * ratio);
                }
            }
        }

        mPrevScreenSize = oldScreenSize;
    }

    // User-defined callback
    InputsProcessed(state);

    DebugCheckpointStop("Inputs");
}

float Game::Input(std::string key, int player)
{
    if (player < 0)
    {
        float maxVal = 0;
        for (int i = 0; i < mInputMappings.size(); i++)
        {
            maxVal = Max(maxVal, Input(key, i));
        }
        return maxVal;
    }
    else
    {
        if (player >= mInputMappings.size())
            return 0.0f;
        auto it = mInputMappings[player].find(key);
        if (it == mInputMappings[player].end())
            return 0;
        return it->second.second.second;
    }
}

bool Game::InputPressed(std::string key, int player)
{
    if (player < 0)
    {
        for (int i = 0; i < mInputMappings.size(); i++)
        {
            if (InputPressed(key, i))
                return true;
        }
        return false;
    }
    else
    {
        if (player >= mInputMappings.size())
            return 0;
        auto it = mInputMappings[player].find(key);
        if (it == mInputMappings[player].end())
            return 0;
        return it->second.second.first == 1;
    }
}

int Game::InputsDir(std::string negKey, std::string posKey, int player)
{
    return ((bool)Input(posKey, player)) - ((bool)Input(negKey, player));
}
int Game::InputsPressedDir(std::string negKey, std::string posKey, int player)
{
    return ((bool)InputPressed(posKey, player)) - ((bool)InputPressed(negKey, player));
}

void Game::SetInputMapping(std::string key, std::vector<Uint8> inputs, int player)
{
    if (player < 0)
    {
        for (int i = 0; i < mInputMappings.size(); i++)
            SetInputMapping(key, inputs, i);
    }
    else
    {
        while (mInputMappings.size() < player + 1)
            mInputMappings.push_back({});
        mInputMappings[player][key] = std::make_pair(inputs, std::make_pair(0, 0.0f));
    }
}

void Game::SetInputMappings(
    std::vector<std::pair<std::string, std::vector<Uint8>>> inputMappings, int player)
{
    for (auto &[name, inputs] : inputMappings)
        Game::SetInputMapping(name, inputs, player);
}

std::vector<Uint8> *Game::GetInputMapping(std::string key, int player)
{
    if (player < 0)
    {
        for (int i = 0; i < player; i++)
        {
            auto ptr = GetInputMapping(key, i);
            if (ptr)
                return ptr;
        }
    }
    else if (player < mInputMappings.size())
    {
        auto loc = mInputMappings[player].find(key);
        if (loc != mInputMappings[player].end())
            return &loc->second.first;
    }
    return nullptr;
}

bool Game::InputExists(std::string key, Uint8 input, int player)
{
    if (player < 0)
    {
        for (int i = 0; i < mInputMappings.size(); i++)
        {
            if (InputExists(key, i))
                return true;
        }
        return false;
    }

    auto ptr = GetInputMapping(key, player);
    if (!ptr)
        return false;

    auto loc = std::find(ptr->begin(), ptr->end(), input);
    return (loc != ptr->end());
}

void Game::FlushPollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
    }
}