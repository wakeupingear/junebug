#include "JGame.h"

#include <iostream>
#include <map>

using namespace junebug;

void JGame::DebugFormatConsole(std::string header)
{
    if (!mDebugAlreadyCleared)
    {
        system("clear");
        mDebugAlreadyCleared = true;
    }
    if (!mDebugSectionHeader)
    {
        if (!header.empty())
            print(header);
        mDebugSectionHeader = true;
    }
}

void JGame::DebugCheckpoint(std::string name, bool condition)
{
    if (!isDebug || !condition)
        return;

    JGame::Get()->mDebugCheckpoints.push_back({name, std::chrono::high_resolution_clock::now(), 0});
}
void JGame::DebugCheckpointStop(std::string name)
{
    if (!isDebug)
        return;

    JGame::Get()->mDebugCheckpoints.push_back({name, std::chrono::high_resolution_clock::now(), 1});
}

void JGame::DebugPrintCheckpoints()
{
    if (!isDebug || mDebugCheckpoints.empty())
        return;
    mDebugSectionHeader = false;

    std::vector<std::pair<std::string, std::chrono::_V2::system_clock::time_point>> stack;
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

            float num = std::chrono::duration_cast<std::chrono::microseconds>(
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

void JGame::DebugResetCheckpoints()
{
    mDebugCheckpoints.clear();
    mDebugCheckpointStart = std::chrono::high_resolution_clock::now();
}

void JGame::DebugPrintInfo()
{
    if (!isDebug || !mShowDebugInfo)
        return;
    mDebugSectionHeader = false;
    DebugFormatConsole("---Debug Info---");
    PrintNoSpaces(DEBUG_INDENT, "Actors: ", mActors.size());
    PrintNoSpaces(DEBUG_INDENT, "Loaded Sprites: ", mSpriteCache.size());
}