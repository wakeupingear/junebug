#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Utils.h"
#include "MathLib.h"

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

            int bufferCol[4]{0, 0, 0, 255};

            std::string title{"Junebug Game"};

            // Whether the game should close itself on a SDL_QUIT event
            bool autoCloseOnQuit{true};

            // Whether the game should automatically create a camera
            bool createDefaultCamera{true};

            // The game's target framerate in milliseconds
            Uint32 fpsTarget{1000 / 60};
            // The minimum framerate to maintain if the game is running slowly
            Uint32 fpsMin{1000 / 30};
            // Whether the game should automatically target the display's refresh rate
            bool detectFps{true};
        };
        // Initialize the game
        // MUST be called before RunLoop
        /// @returns true if successful, false otherwise
        bool Initialize(int screenWidth, int screenHeight, GameOptions options);

        // Get a reference to the game's options
        GameOptions &GetOptions();

        // Get the game's screen width
        int GetScreenWidth();
        // Get the game's screen height
        int GetScreenHeight();

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
        /// @returns Vec2 with the mouse position in screen coordinates
        Vec2<int> GetMousePos();

#define JB_INPUT_QUIT "__quit__"
#define JB_INPUT_FULLSCREEN "__fullscreen__"

        // Overridable callback when inputs are processed
        /// @param state The current SDL input state
        virtual void OnInputsProcessed(const Uint8 *state){};
        // Overridable callback before the game is updated
        /// @param deltaTime The time since the last update
        virtual void OnUpdateStart(float dt){};
        // Overridable callback after the game is updated
        /// @param deltaTime The time since the last update
        virtual void OnUpdateEnd(float dt){};
        // Overridable callback before the game is rendered
        virtual void OnRenderStart(){};
        // Overridable callback after the game is rendered
        virtual void OnRenderEnd(){};

        // Add an actor to the game
        /// @param actor The actor to add
        void AddActor(class PureActor *actor);
        // Remove an actor from the game
        /// @param actor The actor to remove
        void RemoveActor(class PureActor *actor);
        // Get a const reference to the list of actors
        /// @returns A const reference to the list of actors
        const std::vector<class PureActor *> &GetActors() const;

        // Add a camera to the game
        /// @param camera The camera to add
        void AddCamera(class Camera *camera);
        // Remove a camera from the game
        /// @param camera The camera to remove
        void RemoveCamera(class Camera *camera);

        // Add a sprite instance to the game
        /// @param sprite The sprite to add
        void AddSprite(class Sprite *sprite);
        // Remove a sprite instance from the game
        /// @param sprite The sprite to remove
        void RemoveSprite(class Sprite *sprite);
        // Load a texture from a file
        /// @param path The path to the texture file
        /// @returns A pointer to the loaded texture
        SDL_Texture *GetTexture(std::string fileName);
        // Get a const reference to the list of sprites
        /// @returns A const reference to the list of sprites
        const std::vector<class Sprite *> &GetSprites() const;

    protected:
        // The global game instance
        inline static JGame *firstGame = nullptr;

        // The game's options
        GameOptions options;
        // Internal function to set the game's options
        void ProcessOptions(GameOptions options);
        // Tracks whether the options have been changed
        // If true, ProcessOptions() will be called before the next update
        bool mOptionsUpdated = false;

        // The game's window
        SDL_Window *mWindow = nullptr;
        // The game's renderer
        SDL_Renderer *mRenderer = nullptr;

        // Internal boolean to track whether the game is running
        bool mGameIsRunning = false;
        // Current time between frames
        float mDeltaTime = 0;
        // The timestamp of the last frame
        Uint32 mPrevTime = 0;

        // The game's screen size
        int mScreenWidth, mScreenHeight;

        // Whether the game is currently in fullscreen mode
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
        // Flush all poll events
        // Useful for events like window resizing
        void FlushPollEvents();
        Vec2<int> mMousePos = Vec2<int>::Zero;

        // Actor list
        std::vector<class PureActor *> mActors;

        // Camera list
        std::vector<class Camera *> mCameras;
        // Game render target
        SDL_Texture *mRenderTarget = nullptr;

        // Texture map
        std::unordered_map<std::string, SDL_Texture *> mTextures;
        // Sprite list
        std::vector<class Sprite *> mSprites;
    };
}