#include "JGame.h"
#include "Actors.h"
#include "Camera.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

using namespace junebug;

JGame::JGame()
{
    if (!firstGame)
        firstGame = this;
}

JGame::~JGame()
{
    if (firstGame == this)
        firstGame = nullptr;
}

JGame *JGame::Get()
{
    return firstGame;
}

bool JGame::Initialize(int screenWidth, int screenHeight, GameOptions newOptions = {})
{
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    ProcessOptions(newOptions);

    if (SDL_Init(options.initFlags) != 0)
        return false;
    mWindow = SDL_CreateWindow(
        options.title.c_str(), options.windowX, options.windowY, screenWidth, screenHeight, options.windowFlags);
    mRenderer = SDL_CreateRenderer(
        mWindow, -1, options.windowFlags);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    LoadData();

    return true;
}

JGame::GameOptions &JGame::GetOptions()
{
    mOptionsUpdated = true;
    return options;
}

void JGame::ProcessOptions(GameOptions newOptions)
{
    options = newOptions;

    if (mWindow)
    {
        SDL_SetWindowTitle(mWindow, options.title.c_str());
        SDL_SetWindowPosition(mWindow, options.windowX, options.windowY);
        SDL_SetWindowSize(mWindow, mScreenWidth, mScreenHeight);
    }

    if (options.createDefaultCamera && mCameras.empty())
    {
        new Camera();
    }

    if (options.detectFps)
    {
        int displayIndex = SDL_GetWindowDisplayIndex(mWindow);
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(displayIndex, &displayMode);
        if (displayMode.refresh_rate>0) {
            options.fpsTarget = 1000 / displayMode.refresh_rate;
            print("Targeting display refresh rate of " + std::to_string(displayMode.refresh_rate) + " fps");
        }
    }
}

int JGame::GetScreenWidth()
{
    return mScreenWidth;
}

int JGame::GetScreenHeight()
{
    return mScreenHeight;
}

void JGame::Shutdown()
{
    UnloadData();

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void JGame::RunLoop()
{
    mGameIsRunning = true;

    while (mGameIsRunning)
    {
        ProcessInput();
        if (mGameIsRunning)
            UpdateGame();
        if (mGameIsRunning)
            GenerateOutput();
    }
}

void JGame::UpdateGame()
{
    // Delta time
    Uint32 timeDiff = 0;
    do
    {
        timeDiff = SDL_GetTicks() - mPrevTime;
    } while (timeDiff < options.fpsTarget);
    if (timeDiff > options.fpsMin)
        timeDiff = options.fpsMin;
    mPrevTime = SDL_GetTicks();
    mDeltaTime = (float)(timeDiff) / 1000.0f;

    // User-defined callback
    OnUpdateStart(mDeltaTime);

    // Update actors
    auto tempActors = mActors;
    for (PureActor *actor : tempActors)
        actor->Update(mDeltaTime);

    std::vector<PureActor *> destroyActors;
    for (PureActor *actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
            destroyActors.push_back(actor);
    }

    // Remove destroyed actors
    for (PureActor *actor : destroyActors)
        delete actor;

    // User-defined callback
    OnUpdateEnd(mDeltaTime);

    // Update options
    if (mOptionsUpdated)
    {
        ProcessOptions(options);
        mOptionsUpdated = false;
    }
}

void JGame::GenerateOutput()
{
    SDL_Rect windowR;
    windowR.x = 0;
    windowR.y = 0;
    SDL_GetWindowSize(mWindow, &windowR.w, &windowR.h);
    // SDL_RenderSetViewport(mRenderer, &windowR);

    SDL_SetRenderDrawColor(
        mRenderer,
        options.bufferCol[0],
        options.bufferCol[1],
        options.bufferCol[2],
        options.bufferCol[3]);
    SDL_RenderClear(mRenderer);

    // User-defined callback
    OnRenderStart();

    // Render cameras
    mRenderTarget = GetRenderTexture(Vec2((float)mScreenWidth, (float)mScreenHeight), mRenderer, mRenderTarget);

    for (Camera *camera : mCameras)
    {
        if (true)
        {
            SDL_Texture *tex = camera->Render(mRenderer);
            SDL_SetRenderTarget(mRenderer, mRenderTarget);
            SDL_Rect destR;
            destR.x = (int)camera->screenPos.x;
            destR.y = (int)camera->screenPos.y;
            destR.w = (int)camera->screenSize.x;
            destR.h = (int)camera->screenSize.y;
            SDL_RenderCopy(mRenderer, tex, NULL, &destR);
        }
    }

    SDL_SetRenderTarget(mRenderer, NULL);
    SDL_RenderSetViewport(mRenderer, &windowR);
    SDL_RenderClear(mRenderer);
    SDL_RenderCopy(mRenderer, mRenderTarget, NULL, &windowR);

    // User-defined callback
    OnRenderEnd();

    SDL_RenderPresent(mRenderer);
}

Vec2 JGame::GetMousePos()
{
    return mMousePos;
}

void JGame::AddActor(PureActor *actor)
{
    mActors.push_back(actor);
}

void JGame::RemoveActor(PureActor *actor)
{
    mActors.erase(std::remove(mActors.begin(), mActors.end(), actor), mActors.end());
}

const std::vector<PureActor *> &JGame::GetActors() const
{
    return mActors;
}

void JGame::AddCamera(Camera *camera)
{
    mCameras.push_back(camera);
}

void JGame::RemoveCamera(Camera *camera)
{
    mCameras.erase(std::remove(mCameras.begin(), mCameras.end(), camera), mCameras.end());
}