#include "Junebug.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

using namespace Junebug;

JGame::JGame()
{
    mWindow = nullptr;
    mRenderer = nullptr;

    mFPSTarget = 1000.0f / 60.0f;
    mFPSMin = 1000.0f / 30.0f;

    mGameIsRunning = false;
    mDeltaTime = 0;
    mPrevTime = 0;
}

bool JGame::Initialize(int screenWidth, int screenHeight, InitializeOptions options = {})
{
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

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
    OnUpdateStart(mDeltaTime);

    // Update actors

    // User-defined callback
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

    OnGenerateOutputBegin();

    OnGenerateOutputEnd();

    SDL_RenderPresent(mRenderer);
}

void JGame::OnInputsProcessed(const Uint8 *state) {}
void JGame::OnUpdateStart(float deltaTime) {}
void JGame::OnUpdateEnd(float deltaTime) {}
void JGame::OnGenerateOutputBegin() {}
void JGame::OnGenerateOutputEnd() {}