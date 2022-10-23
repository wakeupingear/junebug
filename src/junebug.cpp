#include "junebug.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

using namespace Junebug;

JGame::JGame()
{
}

bool JGame::Initialize(int screenWidth, int screenHeight, InitializeOptions options = {})
{
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;
    mResolutionWidth = options.resolutionX > 0 ? options.resolutionX : screenWidth;
    mResolutionHeight = options.resolutionY > 0 ? options.resolutionY : screenHeight;

    if (SDL_Init(options.initFlags) != 0)
        return false;
    mWindow = SDL_CreateWindow(
        options.title.c_str(), options.windowX, options.windowY, screenWidth, screenHeight, options.windowFlags);
    mRenderer = SDL_CreateRenderer(
        mWindow, -1, options.windowFlags);

    std::copy(options.bufferCol, options.bufferCol + 4, mBufferCol);

    mAutoCloseOnQuit = options.autoCloseOnQuit;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    LoadData();

    return true;
}

void JGame::Shutdown()
{
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
    } while (timeDiff < mFPSTarget);
    if (timeDiff > mFPSMin)
        timeDiff = mFPSMin;
    mPrevTime = SDL_GetTicks();
    mDeltaTime = timeDiff / 1000.0f;

    // User-defined callback
    if (OnUpdateStart)
        OnUpdateStart(mDeltaTime);

    // Update actors

    // User-defined callback
    if (OnUpdateEnd)
        OnUpdateEnd(mDeltaTime);
}

void JGame::GenerateOutput()
{
    SDL_SetRenderDrawColor(
        mRenderer,
        mBufferCol[0],
        mBufferCol[1],
        mBufferCol[2],
        mBufferCol[3]);

    SDL_RenderClear(mRenderer);

    if (OnGenerateOutputBegin)
        OnGenerateOutputBegin();

    if (OnGenerateOutputEnd)
        OnGenerateOutputEnd();

    SDL_RenderPresent(mRenderer);
}