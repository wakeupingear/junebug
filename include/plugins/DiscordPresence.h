#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#ifdef JUNEBUG_PLUGIN_DISCORD

#include "Actors.h"

#define _CRT_SECURE_NO_WARNINGS

#include <array>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include <discord.h>

namespace junebug
{
#if defined(_WIN32)
#pragma pack(push, 1)
    struct BitmapImageHeader
    {
        uint32_t const structSize{sizeof(BitmapImageHeader)};
        int32_t width{0};
        int32_t height{0};
        uint16_t const planes{1};
        uint16_t const bpp{32};
        uint32_t const pad0{0};
        uint32_t const pad1{0};
        uint32_t const hres{2835};
        uint32_t const vres{2835};
        uint32_t const pad4{0};
        uint32_t const pad5{0};

        BitmapImageHeader &operator=(BitmapImageHeader const &) = delete;
    };

    struct BitmapFileHeader
    {
        uint8_t const magic0{'B'};
        uint8_t const magic1{'M'};
        uint32_t size{0};
        uint32_t const pad{0};
        uint32_t const offset{sizeof(BitmapFileHeader) + sizeof(BitmapImageHeader)};

        BitmapFileHeader &operator=(BitmapFileHeader const &) = delete;
    };
#pragma pack(pop)
#endif

    struct DiscordState
    {
        discord::User currentUser;

        std::unique_ptr<discord::Core> core;
    };

    // A class to handle Discord Rich Presence
    class DiscordPresence : public PureActor
    {
    public:
        // Instantiate a new DiscordPresence object. Automatically connects to the supplied client ID.
        // @param clientID The Discord client ID to connect to
        DiscordPresence(long long clientId) : mClientId(clientId)
        {
            discord::Core *core{};
            auto result = discord::Core::Create(clientId, DiscordCreateFlags_Default, &core);
            mState.core.reset(core);
            if (!mState.core)
            {
                std::cout << "Failed to instantiate discord core! (err " << static_cast<int>(result)
                          << ")\n";
                return;
            }

            mActivity = new discord::Activity();
            SetType(discord::ActivityType::Playing);
            UpdateActivity();
        };

        ~DiscordPresence()
        {
            if (mState.core)
            {
                ClearActivity();
                if (mActivity)
                {
                    delete mActivity;
                }
            }
        };

        void SetDetails(std::string details)
        {
            if (mActivity)
            {
                mActivity->SetDetails(details.c_str());
                mActivityChanged = true;
            }
        };
        void SetState(std::string state)
        {
            if (mActivity)
            {
                mActivity->SetState(state.c_str());
                mActivityChanged = true;
            }
        };
        void SetType(discord::ActivityType type)
        {
            if (mActivity)
            {
                mActivity->SetType(type);
                mActivityChanged = true;
            }
        };
        void SetLargeImage(std::string image)
        {
            if (mActivity)
            {
                mActivity->GetAssets().SetLargeImage(image.c_str());
                mActivityChanged = true;
            }
        };
        void SetSmallImage(std::string image)
        {
            if (mActivity)
            {
                mActivity->GetAssets().SetSmallImage(image.c_str());
                mActivityChanged = true;
            }
        };

        void OnUpdate(float dt)
        {
            if (mActivityChanged)
            {
                UpdateActivity();
                mActivityChanged = false;
            }

            if (mState.core)
                mState.core->RunCallbacks();
        }

        discord::Activity *GetActivity() { return mActivity; }

        void ClearActivity()
        {
            mState.core->ActivityManager().ClearActivity([](discord::Result result)
                                                         { log(((result == discord::Result::Ok) ? "Succeeded" : "Failed"), "clearing activity!"); });
        };

    protected:
        void UpdateActivity()
        {
            mState.core->ActivityManager().UpdateActivity(*mActivity, [](discord::Result result)
                                                          { log(((result == discord::Result::Ok) ? "Succeeded" : "Failed"), "updating activity!"); });
        };

    private:
        long long mClientId;
        DiscordState mState{};

        discord::Activity *mActivity = nullptr;
        bool mActivityChanged = false;
    };
}

#endif