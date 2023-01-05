# Junebug

## C++ game engine for high performance game prototyping

C++ is performant, but it's not easy to use. `Junebug` takes that performance and wraps it in various abstractions to make it easy to use. The goal is to make a stand in replacement for Game Maker Studio 2 that can scale to the needs of a production game.

It also bundles a bunch of useful game functions and implementations that Game Maker should have had out of the box but didn't.

SDL2 v2.24.1

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
- 2D box collision
- Separated physics and collision components
- Per-scene and per-actor gravity

## Tilesets
- Runtime tileset loading
- Actor collision

## Cameras
- Muli-camera support
- Camera shake

## Serialisation
- Automatic reflection for base member variables
- JSON saving/loading of Scenes and Actors

## Debug
- CLI profiler
- Automatic error logging

## Platform Support
- Windows
- Linux
- Mac

## Compiler Support
- GCC (recommended)
- Clang

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
void DrawSprite(string imagePath, int frame, Vec2<float> pos, SpriteProperties properties);

void DrawSpritePart(string imagePath, int frame, Vec2<float> &pos, Vec2<int> partPos,  Vec2<int> partSize, SpriteProperties properties);

void DrawText(string text, Vec2<float> pos, TextEffects effects);

void DrawRectangle(Vec2<float> topLeft, Vec2<float> bottomRight, Color color);
```

By default, `VisualActor` will draw its default sprite every frame. It contains its own member variables for position, rotation, scale, and color, which can be change to alter the sprite's appearance. Accordingly, most of the parameters of these functions are optional and will default to the Actor's member variables if not specified.

## Math

Many useful game math functions are included out of the box.

The most useful of these is the `Vec2` class, which is a templated 2D vector with a variety of useful functions. It can be used to represent positions, velocities, and forces. It can also be implicitly cast to `SDL_Point` and `SDL_Rect` for use with SDL functions.

# Credits

Based heavily on [Game Programming in C++](https://books.google.com/books/about/Game_Programming_in_C++.html?id=VfxNDwAAQBAJ&source=kp_book_description) by Sanjay Madhav and the curriculum of USC's ITP 380.

Also inspired by @NoelFB's [blah](https://github.com/NoelFB/blah)

External Libraries:
[SDL_FontCache](https://github.com/grimfang4/SDL_FontCache)

Development tracked on [Trello](https://trello.com/b/jWgkCUmD/junebug)
