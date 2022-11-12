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
    {
        SDL_SetMainReady();
        firstGame = this;
    }
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

GameOptions &JGame::Options()
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
        if (displayMode.refresh_rate > 0 && displayMode.refresh_rate <= 10000)
        {
            options.fpsTarget = 1000 / displayMode.refresh_rate;
            Log("Targeting display refresh rate of " + std::to_string(displayMode.refresh_rate) + " fps");
        }
    }

    if (options.randomSeed == -1)
        srand((unsigned int)time(NULL));
    else
        srand((unsigned int)options.randomSeed);

    SDL_ShowCursor(options.showCursor);

    if (options.windowIcon != "" && mWindow)
    {
        SDL_Surface *icon = IMG_Load((GetAssetPaths().sprites + options.windowIcon).c_str());
        if (icon)
        {
            SDL_SetWindowIcon(mWindow, icon);
            SDL_FreeSurface(icon);
        }
        else
        {
            PrintLog("Failed to load window icon: " + std::string(IMG_GetError()));
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

    while (!mFonts.empty())
    {
        FC_Font *font = mFonts.begin()->second;
        if (font)
            FC_FreeFont(font);
        mFonts.erase(mFonts.begin());
    }

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

bool JGame::Run(int screenWidth, int screenHeight)
{
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    if (SDL_Init(options.initFlags) != 0)
        return false;
    mWindow = SDL_CreateWindow(
        options.title.c_str(), options.windowX, options.windowY, screenWidth, screenHeight, options.windowFlags);
    mRenderer = SDL_CreateRenderer(
        mWindow, -1, options.windowFlags);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    if (mOptionsUpdated)
    {
        ProcessOptions(options);
        mOptionsUpdated = false;
    }

    JB_REGISTER_ACTORS(VisualActor);

    LoadData();

    if (options.startingScene != "")
    {
        LoadScene(options.startingScene);
        LoadQueuedScenes();
    }

    mGameIsRunning = true;

    while (mGameIsRunning)
    {
        ProcessInput();
        if (mGameIsRunning)
        {
            PreUpdate();
            UpdateGame();
            PostUpdate();
        }
        if (mGameIsRunning)
            GenerateOutput();
        LoadQueuedScenes();
    }

    Shutdown();

    return true;
}

void JGame::PreUpdate() {}
void JGame::PostUpdate() {}

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
    UpdateStart(mDeltaTime);

    // Update actors
    auto tempActors = mActors;
    for (PureActor *actor : tempActors)
    {
        if (actor->GetState() == ActorState::Active)
            actor->Update(mDeltaTime);
        else if (actor->GetState() == ActorState::Started)
        {
            actor->SetState(ActorState::Active);
            actor->FirstUpdate(mDeltaTime);
            actor->InternalUpdate(mDeltaTime);
        }
    }

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
    UpdateEnd(mDeltaTime);

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
    RenderStart();

    // Render cameras
    mRenderTarget = GetRenderTexture(Vec2<int>(mScreenWidth, mScreenHeight), mRenderer, mRenderTarget);

    for (Camera *camera : mCameras)
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
    SetActiveCamera(nullptr);

    SDL_SetRenderTarget(mRenderer, NULL);
    SDL_RenderSetViewport(mRenderer, &windowR);
    SDL_RenderClear(mRenderer);
    SDL_RenderCopy(mRenderer, mRenderTarget, NULL, &windowR);

    // User-defined callback
    RenderEnd();

    SDL_RenderPresent(mRenderer);
}

Vec2<int> JGame::GetMousePos()
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