## Junebug

# C++ game engine for high performance game prototyping

C++ is performant, but it's not easy to use. Junebug takes that performance and wraps it in various abstractions to make it easy to use. The goal is to make a stand in replacement for Game Maker Studio 2 that can scale to the needs of a production game.

It also bundles a bunch of useful game functions and implementations that Game Make should have had out of the box but didn't.

SDL2 v2.24.1

Progress on [Trello](https://trello.com/b/jWgkCUmD/junebug)

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

## Credits

Based heavily on [Game Programming in C++](https://books.google.com/books/about/Game_Programming_in_C++.html?id=VfxNDwAAQBAJ&source=kp_book_description) by Sanjay Madhav and the curriculum of USC's ITP 380.

Also inspired by @NoelFB's [blah](https://github.com/NoelFB/blah)

External Libraries:
[SDL_FontCache](https://github.com/grimfang4/SDL_FontCache)
