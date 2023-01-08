# Junebug

## C++ game engine for high performance game prototyping

C++ is performant, but it's not easy to use. `Junebug` takes that performance and wraps it in various abstractions to make it easy to use. The goal is to make a stand in replacement for Game Maker Studio 2 that can scale to the needs of a production game.

It also bundles a bunch of useful game functions and implementations that Game Maker ideally should include out of the box but doesn't.

## Dead Simple Sample

```cpp
#define SDL_MAIN_HANDLED
#include <junebug.h>
using namespace junebug;

int main()
{
    Game game;
    game.Options().title = "My First Game";

    game.Run(384, 216);

    return 0;
}
```

# Features

## Physics

-   2D box collision
-   Separated physics and collision components
-   Per-scene and per-actor gravity

## Tilesets

-   Runtime tileset loading
-   Actor collision
-   Built in editor

## Coroutine Animations

-   Interpolate any variable in the background
-   Apply animation curves to interpolation

## Cameras

-   Muli-camera support
-   Camera shake

## Scenes

-   Actor management across multiple Scenes
-   Scene transition effects

## Serialisation

-   Automatic reflection for base member variables
-   JSON saving/loading of Scenes and Actors

## Debug

-   CLI profiler
-   Automatic error logging

## Platform Support

-   Windows
-   Linux
-   Mac

## Compiler Support

-   GCC (recommended)
-   Clang

# Setup

`Junebug` uses the CMake build system, NOT Visual Studio solutions. It's recommended that you use [Visual Studio Code](https://code.visualstudio.com/) as your IDE, along with the [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension. This will natively support CMake and allow you to build and run your game directly from the IDE, using the 'Play' and 'Debug' icons in the bottom toolbar.

The recommended structure of a `Junebug` project is as follows:

```sh
MyGame
├── CMakeLists.txt
├── lib
│   ├── junebug
├── assets
│   ├── fonts
│   ├── sprites
│   ├── scenes
├── include
│   ├── Your Header Files
├── src
│   ├── Your Source Files
```

See the `examples` folder for a template `CMakeLists.txt` file, as well as several example projects.

NOTE: This engine is in active development and subject to breaking changes, so consider whether you want to clone the repository directly or use a submodule pinned to a specific commit.

## Windows

Windows users should have an installed copy of the [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019) and its 'Game Development with C++' workload. This will install the necessary C++ compiler and libraries. All other dependencies are included in the repository as `.dll` files.

## Linux

Linux users should have an installed copy of the `g++` compiler and the `libSDL2-dev` library. These can be installed via your package manager. An example for Apt is as follows:

```sh
sudo apt-get install g++
sudo apt-get install libsdl2-dev
```

Unlike other platforms, binaries for Linux are not included in the repository. CMake should be able to automatically find the SDL2 library on your system once you run the above installation. If it throws an error, you can manually specify the path to the SDL2 library by editing your `CMakeLists.txt` file.

## Mac

Mac users only need to have an installed C++ compiler. `g++/gcc` is generally recommended, although `clang` works with a few more warnings. Xcode may already include `clang` with any existing dev tools on your systetm, which should work without additional configuration. You can install any other compilers via [Homebrew](https://brew.sh/).

## HTML5 (Emscripten)

`Junebug` games can also be compiled directly to Webassembly, which can then be run in any modern browser. This is done using the Emscripten SDK, which can be downloaded from [here](https://emscripten.org/docs/getting_started/downloads.html).

Once Enscripten is installed and added to PATH (or aliased), you can compile a `Junebug` game to HTML5 by running the following commands from the project root:

```sh
cmake . -DCMAKE_TOOLCHAIN_FILE=~/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_BUILD_TYPE=Release -B embuild
cmake --build ./embuild --target all
```

This will compile to 4 files: a `.data` file with the game's assets, a `.wasm` file with the compiled code, a `.js` file with the glue code, and an `.html` file with the HTML5 boilerplate. To test these files, run the following command to run a local sever at [Port 8080](http://localhost:8080):

```sh
emrun --port 8080 embuild
```

# Structure

`Junebug` is a Hybrid Entity Component System game engine. It uses the Singleton pattern to consolidate global game functions and object management inside of one manager class, while delegating the actual rendering and updating of objects to the individual objects themselves.

To access member variables on `Junebug` objects, use the explicitly defined Setters and Getters. This enforces type safety and prevents the user from accidentally overwriting important data. Many of these Setters and Getters have internal logic to handle edge cases and prevent common errors. Accordingly, member variables are typically `protected` or `private` and are denoted with a leading `m`; for example, `mPosition`.

## Game Class

Every `Junebug` application starts with one instancee of a single global `Game`. This object controls all aspects of game management, from window creation and rendering to input and object management. It can also be inherited from to override the following Event Functions to easily customize its behavior:

```cpp
void LoadData();
void UnloadData();

void LoadActor(PureActor* actor, rapidjson::Value& actorRef, Scene &newScene);

void InputsProcessed(const Uint8 *state);

void UpdateStart(float deltaTime);
void UpdateEnd(float deltaTime);

void RenderStart();
void RenderEnd();
```

## Actor Classes

Entities in `Junebug` are called `Actors`. They are automatically managed by the `Game` class and can be directly created and destroyed at any time. There are 3 types of base `Actors` to choose from, each with their own overridable Event Functions:

```cpp
class PureActor {
    void FirstUpdate(float deltaTime);
    void Update(float deltaTime);
};

class VisualActor : public PureActor {
    void Draw();
};

class PhysicalActor : public VisualActor;
```

`PureActor` is the simplest class and should be used to represent any object that doesn't need to be drawn or interact with the physics engine, like manager obejcts for internal game logic.

`VisualActor` is the most common class and should be used for any object that needs to be drawn to the screen. It has a member variable `mSpritePath` to indicate the actor's default sprite, plus an optionally overrideable `Draw()` function. See the below section on `Rendering` for more information.

`PhysicalActor` doesn't add any new callbacks, but it automatically adds two new member variables: `mPhys` (`PhysicsComponent`) and `mColl` (`CollisionComponent`).

## Components

Although `Junebug`'s actor system is built around inheritance, it also supports the use of Components. Components are objects that can be attached to Actors to add common functionality, like the aforementioned `PhysicsComponent` which makes the Actor automatically obey the current scene's physics rules. Components are managed automatically by the `Game` class and can be directly created and destroyed at any time.

While Components aren't actors, they also have an overridable `Update` function that is called every frame BEFORE the Actor's `Update` function. This allows the Component to modify the Actor's state before it is updated.

## Rendering

Rendering is where `Junebug` differs from many engines like Unity and Unreal. Every draw call, whether drawing a Sprite or Text, must be explicitly issued in code every frame. This can be very useful for 2D sprite-based games specifically since it allows for complex and effects that involve many different sprites or transformations separate from a given Actor. In other words, an Actor's internal state doesn't strictly determine how it is drawn.

This is exactly how Game Maker Studio 2 works, and it's a very powerful paradigm when used responsibly. The following namespace-level functions are available for drawing:

```cpp
void DrawSprite(
    string imagePath, 
    int frame, 
    Vec2<float> pos, 
    SpriteProperties properties
);

void DrawSpritePart(
    string imagePath,
    int frame,
    Vec2<float> pos,
    Vec2<int> partPos,
    Vec2<int> partSize,
    SpriteProperties properties
);

void DrawText(
    string text,
    Vec2<float> pos,
    TextEffects effects
);

void DrawRectangle(
    Vec2<float> topLeft,
    Vec2<float> bottomRight,
    Color color
);

void DrawTexture(
    SDL_Texture *texture,
    Vec2<float> pos,
    Vec2<int> size
)
```

By default, `VisualActor` will draw its default sprite every frame. It contains its own member variables for position, rotation, scale, and color, which can be change to alter the sprite's appearance. Accordingly, most of the parameters of these functions are optional and will default to the Actor's member variables if not specified.

## Assets

All `Junebug` assets - meaning non-code files, like sprites and scene data - should be placed in an `assets` folder in the file root. When referencing asset paths in code, you can use paths that are either relative to the `assets` folder or, more conveniently, paths that are relative to an asset type subfolder, like `assets/sprites` or `assets/scenes`.

All engine-specific asset data is stored as JSON files. This is to allow for more human readable/writable unstructured data. It also lends itself well for reading file data in a web context, especially from network requests or JavaScript interopability.

## Sprites

Sprites are represented internally via the `Sprite` class, which is responsible for managing its associated textures, animations, and metadata. These objects are not referenced directly. Instead, the `Game` instance can be used to retrieve a pointer to a `Sprite` from a given asset path. This means that a sprite's metadata, like its origin, will be shared across all objects, and altering any of that data from one sprite will alter it for all other objects. While technically a limitation, this reduces ambiguity and encourages you to keep assets consistent across different objects.

A sprite can be imported through two ways:

1.  A direct path to an image file in thes `assets/sprites` folder, ex: `joe.png`
2.  A folder containing a series of images and a metadata JSON file, ex: `joe/joe.json`

The only required metadata in a JSON file is `frames`, an ordered array of the sprite's image frames within the folder. For a comprehensive list of available metadata, see the Pong demo in `examples/pong`.

## Animations

Sprites can also hold animations, which can be initially loaded from a metadata file or added later from code.

Animations are just a `vector` of sprite frames, along with an `mfps` member variable in a `Sprite` object to control the animation speed. Since `Sprite` objects contain all actual data instead of the `VisualActors` that use them, the actual animation processing is handled on a per-actor basis.

`VisualActors` contain a `vector` of `Animation` structs, which represent a named animation for a `Sprite` pointer. By default, an animation containing all frames is created and set. The following functions can be used to interact with actors' animations:

```cpp
class VisualActor: public PureActor {
    void AddSpriteAnimation(
        string nickname, 
        string spriteName, 
        string spriteAnimName, 
        float fps, 
        bool loop = true
    );
    int GetAnimationFrame(std::string nickname);

    void SetDefaultSpriteAnimation(string name);
    int GetFrame();
};
```

The 'default sprite animation' is just the animation that plays for the actor's main sprite. This gets used in the default Draw Event and its current frame can be retrieved with `GetFrame()`.

This whole system may seem needlessly complicated, but it is designed to be more useful beyond just the an object's main sprite. The `Junebug` Draw Event allows and encourages drawing multiple different sprites for one object where necessary, useful for things like segmented 2D animation. This system can automatically process many different animations in the background, which can then be referenced for consecutive `DrawSprite` calls in the Draw Event.

In other words, no more need to make frame and counter variables for every sub-animation within an object! It all just works under the hood.

## Math

Many useful game math functions are included out of the box.

The most useful of these is the `Vec2` class, which is a templated 2D vector with a variety of useful functions. It can be used to represent positions, velocities, and forces. It can also be implicitly cast to `SDL_Point` and `SDL_Rect` for use with SDL functions.

# Credits

Based heavily on [Game Programming in C++](https://books.google.com/books/about/Game_Programming_in_C++.html?id=VfxNDwAAQBAJ&source=kp_book_description) by Sanjay Madhav and the curriculum of USC's ITP 380.

Also inspired by @NoelFB's [blah](https://github.com/NoelFB/blah)

## Libraries

[SDL2 v2.24.1](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.1) (mostly forward compatible with later versions)

[SDL_FontCache](https://github.com/grimfang4/SDL_FontCache)

[rapidjson](https://github.com/Tencent/rapidjson)

Development tracked on [Trello](https://trello.com/b/jWgkCUmD/junebug)
