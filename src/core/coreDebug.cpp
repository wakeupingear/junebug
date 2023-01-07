#include "Game.h"

#include <iostream>
#include <map>
#include <mutex>
#include <ctime>

using namespace junebug;

void Game::DebugFormatConsole(std::string header)
{
    if (!mDebugAlreadyCleared)
    {
#ifdef linux
        system("clear");
#elif __EMSCRIPTEN__
        EM_ASM(console.clear());
#endif
        mDebugAlreadyCleared = true;
    }
    if (!mDebugSectionHeader)
    {
        if (!header.empty())
            print(header);
        mDebugSectionHeader = true;
    }
}

void Game::DebugCheckpoint(std::string name, bool condition)
{
    if (!isDebug || !condition)
        return;

#ifndef __EMSCRIPTEN__
    Game::Get()->mDebugCheckpoints.push_back({name, high_resolution_clock::now(), 0});
#endif
}
void Game::DebugCheckpointStop(std::string name)
{
    if (!isDebug)
        return;

#ifndef __EMSCRIPTEN__
    Game::Get()->mDebugCheckpoints.push_back({name, high_resolution_clock::now(), 1});
#endif
}

void Game::DebugPrintCheckpoints()
{
    if (!isDebug || mDebugCheckpoints.empty())
        return;
    mDebugSectionHeader = false;

    std::vector<std::pair<std::string, time_point<system_clock, nanoseconds>>> stack;
    std::map<std::string, std::string> messages;
    std::string indent = DEBUG_INDENT;
    for (auto &checkpoint : mDebugCheckpoints)
    {
        std::string name = std::get<0>(checkpoint);
        auto time = std::get<1>(checkpoint);
        int type = std::get<2>(checkpoint);

        if (type == 0)
        {
            stack.push_back({name, time});
            messages[name] = indent;
            indent += DEBUG_INDENT;
        }
        else if (type == 1)
        {
            indent = indent.substr(0, indent.size() - DEBUG_INDENT.size());

            if (stack.empty())
            {
                continue;
            }

            auto &top = stack.back();
            if (top.first != name)
            {
                continue;
            }

            float num = duration_cast<microseconds>(
                            time - top.second)
                            .count() /
                        1000.0f;
            messages[top.first] += name + " took " + RoundDecStr(num, 3) + " ms";
            stack.pop_back();
        }
    }

    for (auto &pair : messages)
    {
        if (IsWhitespace(pair.second))
            continue;
        DebugFormatConsole("---Debug Checkpoints---");
        print(pair.second);
    }

    DebugResetCheckpoints();
}

void Game::DebugResetCheckpoints()
{
    mDebugCheckpoints.clear();

#ifndef __EMSCRIPTEN__
    mDebugCheckpointStart = high_resolution_clock::now();
#endif
}

void Game::DebugPrintInfo()
{
#ifdef __EMSCRIPTEN__
    return;
#endif

    if (!isDebug || !mShowDebugInfo)
        return;
    mDebugSectionHeader = false;
    DebugFormatConsole("---Debug Info---");
    PrintNoSpaces(DEBUG_INDENT, mFps, " FPS");
    PrintNoSpaces(DEBUG_INDENT, "Delta Time: ", RoundDec(mDeltaTime * 1000.0f, 3), "ms");
    PrintNoSpaces(DEBUG_INDENT, "Actors: ", mActors.size());

    int numCoroutines = CountTwerps(mTwerpCoroutinesFloat) + CountTwerps(mTwerpCoroutinesInt) + CountTwerps(mTwerpCoroutinesUint8);
    PrintNoSpaces(DEBUG_INDENT, "Coroutines: ", numCoroutines);

    PrintNoSpaces(DEBUG_INDENT, "Loaded Sprites: ", mSpriteCache.size());
}

void Game::__DebugSkipPrintThisFrame__() { mSkipDebugPrintThisFrame = true; };