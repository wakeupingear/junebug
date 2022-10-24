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

/// @brief The Junebug namespace contains all of the Junebug classes and functions
namespace junebug
{
    /// @brief The JGame class is the main class for the Junebug engine. It contains the game loop and handles all of the SDL2 initialization and shutdown.
    class JGame
    {
    public:
        // Game constructor
        JGame();
        // Game destructor
        ~JGame();
        // Get the global game instance
        static JGame *Get();

        // Options for initializing the game
        struct GameOptions
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
        /// @returns true if successful, false otherwise
        bool Initialize(int screenWidth, int screenHeight, GameOptions options);

        // Get a reference to the game's options
        static GameOptions GetOptions();
        // Set the game's options
        static void SetOptions(GameOptions options);

        // Clean up any resources used by the game
        // Should be called after RunLoop() has finished and before the program exits
        void Shutdown();

        // Initiates the self-contained game loop
        // Run this after initializing the game
        void RunLoop();

        // Check a given input name
        /// @param key The name of the input to check
        /// @returns number of frames the input has been held for
        int Input(std::string key);
        // Set the input mapping for a given input name
        /// @param key The name of the input
        /// @param inputs A vector of SDL keycodes to map to the input
        void SetInputMapping(std::string key, std::vector<Uint8> inputs);
        // Set a list of input mappings
        /// @param inputMapping A list of input mappings, where the each element is a pair with the input name and a vector of SDL keycodes
        void SetInputMappings(
            std::vector<std::pair<std::string, std::vector<Uint8>>> inputMappings);
        // Get the current mouse position
        /// @returns Vector2 with the mouse position in screen coordinates
        Vector2 GetMousePos();

        // Add an actor to the game
        void AddActor(class PureActor *actor);
        // Remove an actor from the game
        void RemoveActor(class PureActor *actor);

#define JB_INPUT_QUIT "__quit__"
#define JB_INPUT_FULLSCREEN "__fullscreen__"

        // Overridable callback when inputs are processed
        virtual void OnInputsProcessed(const Uint8 *state){};
        // Overridable callback before the game is updated
        virtual void OnUpdateStart(float dt){};
        // Overridable callback after the game is updated
        virtual void OnUpdateEnd(float dt){};
        // Overridable callback before the game is rendered
        virtual void OnRenderStart(){};
        // Overridable callback after the game is rendered
        virtual void OnRenderEnd(){};

    protected:
        inline static JGame *firstGame = nullptr;

        GameOptions options;
        void ProcessOptions(GameOptions options);

        SDL_Window *mWindow = nullptr;
        SDL_Renderer *mRenderer = nullptr;

        Uint32 mFPSTarget = 1000 / 60;
        Uint32 mFPSMin = 1000 / 30;

        bool mGameIsRunning = false;
        float mDeltaTime = 0;
        Uint32 mPrevTime = 0;

        int mScreenWidth, mScreenHeight;
        uint32_t mResolutionWidth, mResolutionHeight;

        bool mFullscreen = false;

        // Main input handling event
        void ProcessInput();
        // Main update event
        void UpdateGame();
        // Main draw event
        void GenerateOutput();

        // Overridable function for loading game resources on startup
        virtual void LoadData();
        // Overridable function for unloading game resources on shutdown
        virtual void UnloadData();

        // Inputs
        std::unordered_map<std::string, std::pair<std::vector<Uint8>, int>> mInputMapping;
        std::unordered_map<Uint8, int> mInputs;
        bool mAutoCloseOnQuit;
        // Flush all poll events
        // Useful for events like window resizing
        void FlushPollEvents();
        Vector2 mMousePos = Vector2::Zero;

        // Actor list
        std::vector<class PureActor *> mActors;
    };
}