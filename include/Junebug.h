#pragma once

#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "utils.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

namespace Junebug
{
    class JGame
    {
    public:
        JGame();

        struct InitializeOptions
        {
            Uint32 initFlags{SDL_INIT_AUDIO | SDL_INIT_VIDEO};
            Uint32 windowFlags{0};
            Uint32 renderFlags{SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};

            int windowX{SDL_WINDOWPOS_CENTERED}, windowY{SDL_WINDOWPOS_CENTERED};

            uint32_t resolutionX{0}, resolutionY{0};

            int bufferCol[4]{0, 0, 0, 255};

            std::string title{"Junebug Game"};

            bool autoCloseOnQuit{true};
        };
        bool Initialize(int screenWidth, int screenHeight, InitializeOptions options);

        void Shutdown();
        void RunLoop();

        int Input(std::string key);
        void SetInputMapping(std::string key, std::vector<Uint8> inputs);
        void SetInputMappings(
            std::vector<std::pair<std::string, std::vector<Uint8>>> inputMapping);

        std::function<void(const Uint8 *state)> OnInputsProcessed;
        std::function<void(float deltaTime)> OnUpdateStart;
        std::function<void(float deltaTime)> OnUpdateEnd;
        std::function<void()> OnGenerateOutputBegin;
        std::function<void()> OnGenerateOutputEnd;

    private:
        SDL_Window *mWindow = nullptr;
        SDL_Renderer *mRenderer = nullptr;

        float mFPSTarget = 1000.0f / 60.0f;
        float mFPSMin = 1000.0f / 30.0f;

        bool mGameIsRunning = false;
        float mDeltaTime = 0;
        Uint32 mPrevTime = 0;

        int mScreenWidth, mScreenHeight;
        uint32_t mResolutionWidth, mResolutionHeight;

        int mBufferCol[4];

        bool mFullscreen = false;

        // Core loop
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();

        // Overridable load funtions
        void LoadData();
        void UnloadData();

        // Inputs
        std::unordered_map<std::string, std::pair<std::vector<Uint8>, int>> mInputMapping;
        std::unordered_map<Uint8, int> mInputs;
        bool mAutoCloseOnQuit;
        void FlushInputs();
    };
}