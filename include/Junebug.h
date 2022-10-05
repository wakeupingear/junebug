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

            int windowX{SDL_WINDOWPOS_CENTERED};
            int windowY{SDL_WINDOWPOS_CENTERED};

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

        void OnInputsProcessed(const Uint8 *state);
        void OnUpdateStart(float deltaTime);
        void OnUpdateEnd(float deltaTime);

    private:
        SDL_Window *mWindow;
        SDL_Renderer *mRenderer;

        float mFPSTarget;
        float mFPSMin;

        bool mGameIsRunning;
        float mDeltaTime;
        Uint32 mPrevTime;

        int mScreenWidth;
        int mScreenHeight;

        int mBufferCol[4];

        // Core loop
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();

        // Overridable load funtions
        void LoadData();
        void UnloadData();

        // Inputs
        std::unordered_map<std::string, std::vector<Uint8>> mInputMapping;
        std::unordered_map<Uint8, int> mInputs;
        bool mAutoCloseOnQuit;
    };
}