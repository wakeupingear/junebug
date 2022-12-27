#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "Utils.h"
#include "MathLib.h"
#include "RandLib.h"
#include "InputNames.h"
#include "Color.h"
#include "Files.h"
#include "Actors.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "SDL_FontCache.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <queue>
#include <chrono>

using namespace std::chrono;
using dsec = duration<double>;

/// @brief The Junebug namespace contains all of the Junebug classes and functions
namespace junebug
{
#pragma region Map Macros
#define __MAP_OUT__
#define __EVAL0__(...) __VA_ARGS__
#define __EVAL1__(...) __EVAL0__(__EVAL0__(__EVAL0__(__VA_ARGS__)))
#define __EVAL2__(...) __EVAL1__(__EVAL1__(__EVAL1__(__VA_ARGS__)))
#define __EVAL3__(...) __EVAL2__(__EVAL2__(__EVAL2__(__VA_ARGS__)))
#define __EVAL4__(...) __EVAL3__(__EVAL3__(__EVAL3__(__VA_ARGS__)))
#define __EVAL__(...) __EVAL4__(__EVAL4__(__EVAL4__(__VA_ARGS__)))
#define __MAP_END__(...)
#define __MAP_GET_END__() 0, __MAP_END__
#define ____MAP_NEXT__0__(item, next, ...) next __MAP_OUT__
#define ____MAP_NEXT__1__(item, next) ____MAP_NEXT__0__(item, next, 0)
#define __MAP_NEXT__(item, next) ____MAP_NEXT__1__(__MAP_GET_END__ item, next)
#define __MAP0__(f, x, peek, ...) f(x) __MAP_NEXT__(peek, __MAP1__)(f, peek, __VA_ARGS__)
#define __MAP1__(f, x, peek, ...) f(x) __MAP_NEXT__(peek, __MAP0__)(f, peek, __VA_ARGS__)
#define MAP(f, ...) __EVAL__(__MAP1__(f, __VA_ARGS__, (), 0))
#pragma endregion

        // Options for initializing the game
        struct GameOptions
        {
                Uint32 initFlags{SDL_INIT_AUDIO | SDL_INIT_VIDEO};
                Uint32 windowFlags{0};
                Uint32 renderFlags{SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};
                int rendererIndex{-1};

                int windowX{SDL_WINDOWPOS_CENTERED}, windowY{SDL_WINDOWPOS_CENTERED};
                bool resizable{false};
                bool screenStretch{true};

                int bufferCol[4]{0, 0, 0, 255};

                std::string title{"Junebug Game"};

                // Whether the game should close itself on a SDL_QUIT event
                bool autoCloseOnQuit{true};

                // Whether the game should automatically create a camera
                bool createDefaultCamera{true};
                Vec2<float> defaultCameraSize = Vec2(384.0f, 216.0f);
                Vec2<float> defaultCameraPos = Vec2(0.0f, 0.0f);

                // The game's target framerate
                int fpsTarget{120};
                // Whether the game should automatically target the display's refresh rate
                bool detectFps{true};
                // Whether the game should block the main thread until the target framerate is reached
                // True == Low CPU, occasional frame time spikes
                // False == High CPU, consistent frame time
                bool shouldThreadSleep{true};
                // The number of milliseconds to under-sleep the main thread by
                // This is to account for the fact that a sleeping thread will not wake up exactly when it is supposed to, and will likely overshoot the target time
                // A higher value will result in more consistent frame times, but will also result in more CPU usage
                // Only used if shouldThreadSleep is true
                double sleepMargin{9.};

                int randomSeed{-1};

                bool showCursor{true};

                std::string windowIcon{""};

                std::string startingScene{""};
        };

        struct Layer
        {
                std::string name{""};
                int depth{0};
                std::string id{""};
                bool visible{true};
        };

        struct Scene
        {
                std::string name{""};
                Vec2<int> size;
                std::unordered_map<std::string, Layer> layers;
        };

        /// @brief The Game class is the main class for the Junebug engine. It contains the game loop and handles all of the SDL2 initialization and shutdown.
        class Game
        {
        public:
                // Game constructor
                Game();
                // Game destructorf
                ~Game();
                // Get the global game instance
                static Game *Get();

                // Whether the game is running in debug mode
                static bool isDebug;
                // Whether the editor is enabled
                static bool isEditor;

                // Get a reference to the game's options
                GameOptions &Options();
                // Get a const reference to the game's options without being able to modify them
                const GameOptions &GetOptions() const;

                // Get the game's screen width
                int GetScreenWidth();
                // Get the game's screen height
                int GetScreenHeight();
                // Get the game's internal rendering width
                int GetRenderWidth();
                // Get the game's internal rendering height
                int GetRenderHeight();

                // Get the renderer
                SDL_Renderer *GetRenderer() { return mRenderer; }
                // Get the window
                SDL_Window *GetWindow() { return mWindow; }
                // Raise window
                void RaiseWindow();

                // Get the FPS the game is currently running at
                int GetFPS() { return mFps; }

#pragma region Input
                // Check a given input name
                /// @param key The name of the input to check
                /// @returns number of frames the input has been held for
                int Input(std::string key);
                // Check if a given input is pressed
                /// @param key The name of the input to check
                /// @returns true if the input was first pressed this frame, false otherwise
                bool InputPressed(std::string key);
                // Get the int result of two opposite inputs
                /// @param key1 The first input
                /// @param key2 The second input
                int InputsDir(std::string negKey, std::string posKey);
                // Get the int result of two opposite inputs being pressed this frame
                /// @param key1 The first input
                /// @param key2 The second input
                int InputsPressedDir(std::string negKey, std::string posKey);
                // Set the input mapping for a given input name
                /// @param key The name of the input
                /// @param inputs A vector of SDL keycodes to map to the input
                void SetInputMapping(std::string key, std::vector<Uint8> inputs);
                // Set a list of input mappings
                /// @param inputMapping A list of input mappings, where the each element is a pair with the input name and a vector of SDL keycodes
                void SetInputMappings(
                    std::vector<std::pair<std::string, std::vector<Uint8>>> inputMappings);
                // Get the current mouse position
                /// @returns Vec2 with the mouse position in game coordinates, relative to a certain camera
                Vec2<int> GetMousePos();
                // Set the mouse position
                /// @param pos The position to set the mouse to
                void SetMousePos(Vec2<int> pos);
                // Get the current mouse position relative to the display
                /// @returns Vec2 with the mouse position in screen coordinates
                Vec2<int> GetMouseScreenPos();
                // Get the screen camera that the mouse is relative to
                /// @returns a pointer to a camera or nullptr if no valid camera is found
                class Camera *GetMouseCamera();
                // Get the mouse's offset from the mouse camera
                /// @returns a Vec2
                Vec2<int> GetMouseOffset();

#define JB_INPUT_QUIT "__quit__"
#define JB_INPUT_FULLSCREEN "__fullscreen__"
#pragma endregion

#pragma region Game Loop
                // Run the game
                /// @returns true if successful, false otherwise
                bool Run(int screenWidth, int screenHeight);

                // Clean up any resources used by the game
                // Should be called after RunLoop() has finished and before the program exits
                void Shutdown();

                // Overridable callback when inputs are processed
                /// @param state The current SDL input state
                virtual void InputsProcessed(const Uint8 *state){};
                // Overridable callback before the game is updated
                /// @param deltaTime The time since the last update
                virtual void UpdateStart(float dt){};
                // Overridable callback after the game is updated
                /// @param deltaTime The time since the last update
                virtual void UpdateEnd(float dt){};
                // Overridable callback before the game is rendered
                virtual void RenderStart(){};
                // Overridable callback after the game is rendered
                virtual void RenderEnd(){};
#pragma endregion

#pragma region Sprites
                // Add a sprite to the cache
                /// @param name The name of the sprite
                /// @param sprite The sprite to add
                void AddSprite(std::string name, class Sprite *sprite);
                // Get a const reference to the sprite cache
                const std::unordered_map<std::string, class Sprite *> &GetSpriteCache() { return mSpriteCache; }
#pragma endregion

#pragma region Actors
                // Add an actor to the game
                /// @param actor The actor to add
                void AddActor(class PureActor *actor);
                // Remove an actor from the game
                /// @param actor The actor to remove
                void RemoveActor(class PureActor *actor);

                // Get a const reference to the list of actors
                /// @returns A const reference to the list of actors
                const std::vector<class PureActor *> &GetActors() const;

                // Get an optionally typed actor by id
                /// @param id The id of the actor
                /// @returns A pointer to the actor or nullptr if no actor with the given id exists
                template <typename T = class PureActor>
                T *GetActor(std::string id)
                {
                        for (auto actor : mActors)
                        {
                                if (actor->GetId() == id)
                                {
                                        return dynamic_cast<T *>(actor);
                                }
                        }
                        return nullptr;
                }

                typedef std::unordered_map<std::string,
                                           std::function<class PureActor *()>>
                    factory_map;
                // Actor name to class map
                factory_map mActorConstructors;
#pragma endregion

#pragma region Cameras
                // Add a camera to the game
                /// @param camera The camera to add
                void AddCamera(class Camera *camera);
                // Remove a camera from the game
                /// @param camera The camera to remove
                void RemoveCamera(class Camera *camera);
                // Set the active camera
                /// @param camera The camera to set as active
                void SetActiveCamera(class Camera *camera) { mActiveCamera = camera; }
                // Get the active camera
                /// @returns The active camera
                class Camera *GetActiveCamera() { return mActiveCamera; }
#pragma endregion

#pragma region Textures
                // Load a texture from a file
                /// @param path The path to the texture file
                /// @returns A pointer to the loaded texture
                SDL_Texture *GetTexture(std::string fileName);
#pragma endregion

#pragma region Scenes
                // Load a scene from a file or JSON string
                /// @param scene The scene to load
                void LoadScene(std::string);
                // Reload the current scene
                void ReloadScene();
                // Check if a scene is loaded
                /// @returns true if a scene is loaded, false otherwise
                bool IsSceneLoaded();
                // Get the current scene
                /// @returns A const reference to the current scene
                const Scene &GetCurrentScene() const;

                // Get the current scene's size
                /// @returns A Vec2
                const Vec2<int> GetSceneSize();
                // Get the current scene's name
                /// @returns A string
                const std::string GetSceneName();
#pragma endregion

#pragma region Fonts
                // Get the current font
                /// @returns A pointer to the current font
                FC_Font *GetCurrentFont() { return mCurrentFont; }
                // Add a font to the game
                /// @param path The path to the font file
                /// @param size The size of the font
                /// @returns A pointer to the loaded font
                FC_Font *AddFont(std::string path, int size, int style = TTF_STYLE_NORMAL, Color color = Color::White);
                // Remove a font from the game
                /// @param name The name of the font to remove
                /// @returns true if the font was removed, false otherwise
                bool RemoveFont(std::string name);
                // Set the default font
                /// @param name The name of the font to set as default
                /// @returns true if font exists, false otherwise
                bool SetFont(std::string name);
#pragma endregion

#pragma region Asset Lookup
                // The default paths appended to various asset lookups by file path
                struct AssetPaths
                {
                        std::string sprites{"assets/sprites/"};
                        std::string fonts{"assets/fonts/"};
                        std::string scenes{"assets/scenes/"};
                };
                // Get the asset paths
                /// @returns A reference to the asset paths
                AssetPaths &GetAssetPaths() { return mAssetPaths; }
#pragma endregion

#pragma region Debug Tools
                // Start a debug checkpoint for the given step
                /// @param name The name of the checkpoint
                /// @param condition Whether to start the checkpoint; used for cleaner conditionals to keep debug code clean
                static void DebugCheckpoint(std::string name, bool condition = true);
                // Stop the current debug checkpoint
                /// @param name The name of the checkpoint
                static void DebugCheckpointStop(std::string name);
                // Pause the debug printout for a frame
                // Useful for temporarily halting the ouput
                // This really isn't a function that has a public purpose but it needs to be public to be accessible from namespace-scoped functions
                void __DebugSkipPrintThisFrame__();
#pragma endregion

        protected:
                // The global game instance
                inline static Game *firstGame = nullptr;

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
                // The inverse of the target framerate
                system_clock::duration mInvTargetFps = round<system_clock::duration>(dsec{1. / 60});
                system_clock::duration mSleepMargin = round<system_clock::duration>(dsec{1. / 1000});
                // The previous time in seconds
                time_point<system_clock, seconds> mPrevSecond;
                // The accumulated frames in the current second
                int mFramesThisSecond;
                time_point<system_clock, nanoseconds> mBeginFrame = system_clock::now();
                time_point<system_clock, nanoseconds> mEndFrame;
                // The FPS of the previous frame
                unsigned int mFps = 0;
                void HaltFrame();

                // Get the display size
                /// @returns A Vec2<int> containing the display size
                Vec2<int> GetDisplaySize() const;
                // The game's screen size
                int mScreenWidth, mScreenHeight;
                // the Previous screen size (for fullscreen toggling)
                Vec2<int> mPrevScreenSize;
                // The previous window position (for fullscreen toggling)
                Vec2<int> mPrevWindowPos;
                // The game's render size
                int mRenderWidth, mRenderHeight;

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

                // Overridable function for processing an actor loaded from a JSON structure
                virtual void LoadActor(PureActor *actor, rapidjson::Value &actorRef, Scene &newScene);

                // Overridable callback for before an update
                virtual void PreUpdate();
                // Overridable callback for after an update
                virtual void PostUpdate();

                // Inputs
                std::unordered_map<std::string, std::pair<std::vector<Uint8>, int>> mInputMapping;
                std::unordered_map<Uint8, int> mInputs;
                Uint8 mExtraStates[256] = {0};
                // Flush all poll events
                // Useful for events like window resizing
                void FlushPollEvents();
                Vec2<int> mMousePos = Vec2<int>::Zero;
                Vec2<int> mMouseScreenPos = Vec2<int>::Zero;
                // Which camera the mouse coordinates are relative to
                // nullptr if not relative to a camera
                class Camera *mMouseCamera = nullptr;
                // The mouse's offset from the camera
                Vec2<int> mMouseOffset = Vec2<int>::Zero;

                // Actor list
                std::vector<class PureActor *> mActors;

                // Camera list
                std::vector<class Camera *> mCameras;
                // Game render target
                SDL_Texture *mRenderTarget = nullptr;
                // Active camera
                class Camera *mActiveCamera = nullptr;

                // Texture map
                std::unordered_map<std::string, SDL_Texture *> mTextures;
                // Sprite cache
                std::unordered_map<std::string, class Sprite *> mSpriteCache;

                // Scene
                Scene mScene;
                // Queue of scenes to load
                std::queue<std::string> mSceneQueue;
                // Helper function to load queued scenes
                void LoadQueuedScenes();
                // Helper function to instantiate an actor from a secene JSON reference
                void LoadActor(rapidjson::Value &actorRef, Scene &newScene);

                // Current font
                FC_Font *mCurrentFont = nullptr;
                // Font map
                std::unordered_map<std::string, FC_Font *> mFonts;

                // Asset paths
                AssetPaths mAssetPaths;

                // Debug info
                bool mDebugAlreadyCleared = false;
                bool mDebugSectionHeader = false;
                void DebugFormatConsole(std::string header = "");
                const std::string DEBUG_INDENT = "  ";

                void DebugPrintInfo();
                bool mShowDebugInfo = true;
                bool mSkipDebugPrintThisFrame = false;

                std::vector<std::tuple<std::string, time_point<system_clock, nanoseconds>, bool>> mDebugCheckpoints;
                time_point<system_clock, nanoseconds> mDebugCheckpointStart;
                void DebugPrintCheckpoints();
                void DebugResetCheckpoints();
                bool mShowDefaultDebugCheckpoints = true;
        };
};