#include "JGame.h"
#include "Actors.h"
#include "Camera.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <thread>

using namespace junebug;
using namespace std::chrono;

#ifndef NDEBUG
bool JGame::isDebug = true;
#else
bool JGame::isDebug = false;
#endif

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

const GameOptions &JGame::GetOptions() const
{
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

    // Only create a default camera if there are no cameras after LoadData() has run
    if (mGameIsRunning && options.createDefaultCamera && mCameras.empty())
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
            options.fpsTarget = displayMode.refresh_rate;
            Log("Targeting display refresh rate of " + std::to_string(displayMode.refresh_rate) + " fps");
        }
    }
    mInvTargetFps = round<system_clock::duration>(dsec{1. / options.fpsTarget});
    mSleepMargin = round<system_clock::duration>(dsec{options.sleepMargin / 1000.});

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

int JGame::GetScreenWidth() { return mScreenWidth; }
int JGame::GetScreenHeight() { return mScreenHeight; }
int JGame::GetRenderWidth() { return mRenderWidth; }
int JGame::GetRenderHeight() { return mRenderHeight; }

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
    mScreenWidth = screenWidth, mScreenHeight = screenHeight;
    mRenderWidth = screenWidth, mRenderHeight = screenHeight;

#ifdef linux
    putenv((char *)"SDL_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR=0");
    putenv((char *)"SDL_VIDEO_X11_FORCE_EGL=1");
    putenv((char *)"SDL_HINT_RENDER_SCALE_QUALITY=1");
#endif

    if (SDL_Init(options.initFlags) != 0)
        return false;

    if (options.resizable && !(options.windowFlags & SDL_WINDOW_RESIZABLE))
        options.windowFlags |= SDL_WINDOW_RESIZABLE;
    options.resizable = (options.windowFlags & SDL_WINDOW_RESIZABLE);

    mWindow = SDL_CreateWindow(
        options.title.c_str(), options.windowX, options.windowY, screenWidth, screenHeight, options.windowFlags);
    if (!mWindow)
    {
        Print("Failed to create window: " + std::string(SDL_GetError()));
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow, options.rendererIndex, options.renderFlags);
    if (!mRenderer)
    {
        mRenderer = SDL_CreateRenderer(
            mWindow, 2, options.renderFlags);
    }
    if (!mRenderer)
    {
        Print("Failed to create renderer: " + std::string(SDL_GetError()));
        return false;
    }

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
    // Process any options that were changed in LoadData()
    ProcessOptions(options);

    mBeginFrame = system_clock::now();
    mEndFrame = mBeginFrame + mInvTargetFps;
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
    // Calculate the FPS
    auto time_in_seconds = time_point_cast<seconds>(system_clock::now());
    ++mFramesThisSecond;
    if (time_in_seconds > mPrevSecond)
    {
        mFps = Clamp(mFramesThisSecond, 0, 10000);
        mFramesThisSecond = 0;
        mPrevSecond = time_in_seconds;
    }

    // Sleep the thread until slightly before the next frame
    if (options.shouldThreadSleep)
        std::this_thread::sleep_until(mEndFrame - mSleepMargin);

    // Manually block the thread until the exact time of the next frame
    auto mWorkTime = system_clock::now() - mBeginFrame;
    while (mWorkTime < mInvTargetFps)
    {
        mEndFrame = system_clock::now();
        mWorkTime = mEndFrame - mBeginFrame;
    }
    mBeginFrame = mEndFrame;
    mEndFrame = mBeginFrame + mInvTargetFps;
    mDeltaTime = (float)duration_cast<dsec>(mEndFrame - mBeginFrame).count();

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
        camera->_UpdateCoordinates();

        SDL_Texture *tex = camera->Render(mRenderer);
        SDL_SetRenderTarget(mRenderer, mRenderTarget);
        SDL_Rect destR;
        destR.x = (int)camera->_calcScreenPos.x;
        destR.y = (int)camera->_calcScreenPos.y;
        destR.w = (int)camera->_calcScreenSize.x;
        destR.h = (int)camera->_calcScreenSize.y;
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

Vec2<int> JGame::GetDisplaySize() const
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    return Vec2<int>(DM.w, DM.h);
}