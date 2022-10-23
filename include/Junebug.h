#pragma once

#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "utils.h"
#include "mathLib.h"

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
        // Game constructor
        JGame();

        // Options for initializing the game
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
        // Initialize the game
        // MUST be called before RunLoop
        // \returns true if successful, false otherwise
        bool Initialize(int screenWidth, int screenHeight, InitializeOptions options);

        // Clean up any resources used by the game
        // Should be called after RunLoop() has finished and before the program exits
        void Shutdown();

        // Initiates the self-contained game loop
        // Run this after initializing the game
        void RunLoop();

        // Check a given input name
        // \param key The name of the input to check
        // \returns number of frames the input has been held for
        int Input(std::string key);
        // Set the input mapping for a given input name
        // \param key The name of the input
        // \param inputs A vector of SDL keycodes to map to the input
        void SetInputMapping(std::string key, std::vector<Uint8> inputs);
        // Set a list of input mappings
        // \param inputMapping A list of input mappings, where the each element is a pair with the input name and a vector of SDL keycodes
        void SetInputMappings(
            std::vector<std::pair<std::string, std::vector<Uint8>>> inputMappings);
        // Get the current mouse position
        // \returns Vector2 with the mouse position in screen coordinates
        Vector2 GetMousePos();

#define JB_INPUT_QUIT "__quit__"
#define JB_INPUT_FULLSCREEN "__fullscreen__"

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

        // Main input handling event
        void ProcessInput();
        // Main update event
        void UpdateGame();
        // Main draw event
        void GenerateOutput();

        // Overridable load funtions
        void LoadData();
        void UnloadData();

        // Inputs
        std::unordered_map<std::string, std::pair<std::vector<Uint8>, int>> mInputMapping;
        std::unordered_map<Uint8, int> mInputs;
        bool mAutoCloseOnQuit;
        // Flush all poll events
        // Useful for events like window resizing
        void FlushPollEvents();
        Vector2 mMousePos = Vector2::Zero;
    };
}