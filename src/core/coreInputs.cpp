#include "Game.h"
#include "Camera.h"

using namespace junebug;

void Game::ProcessInput()
{
    DebugCheckpoint("Inputs", mShowDefaultDebugCheckpoints);

    // Store the current screen size in case fullscreen is toggled
    Vec2<int> oldScreenSize(mScreenWidth, mScreenHeight), oldWindowPos;
    SDL_GetWindowSize(mWindow, &mScreenWidth, &mScreenHeight);

    SDL_Event event;
    std::unordered_map<Uint8, int> newInputs;

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
    for (auto &[key, inputs] : mInputMapping)
    {
        inputs.second = 0;
        for (auto input : inputs.first)
        {
            if (state[input] || mExtraStates[input])
            {
                auto loc = mInputs.find(input);
                int val = loc != mInputs.end() ? loc->second + 1 : 1;
                newInputs[input] = val;
                inputs.second = std::max(inputs.second, val);
            }
        }
    }
    mInputs = newInputs;

    // Default Events
    if (Input(JB_INPUT_QUIT))
    {
        mGameIsRunning = false;
    }
    if (Input(JB_INPUT_FULLSCREEN) == 1)
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
        if (!Game::Get()->Options().screenStretch)
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

int Game::Input(std::string key)
{
    auto it = mInputMapping.find(key);
    if (it == mInputMapping.end())
        return 0;
    return it->second.second;
}

bool Game::InputPressed(std::string key)
{
    return Input(key) == 1;
}

int Game::InputsDir(std::string negKey, std::string posKey)
{
    return ((bool)Input(posKey)) - ((bool)Input(negKey));
}
int Game::InputsPressedDir(std::string negKey, std::string posKey)
{
    return ((bool)InputPressed(posKey)) - ((bool)InputPressed(negKey));
}

void Game::SetInputMapping(std::string key, std::vector<Uint8> inputs)
{
    mInputMapping[key] = make_pair(inputs, 0);
}

void Game::SetInputMappings(
    std::vector<std::pair<std::string, std::vector<Uint8>>> inputMappings)
{
    for (auto &[name, inputs] : inputMappings)
    {
        Game::SetInputMapping(name, inputs);
    }
}

void Game::FlushPollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
    }
}