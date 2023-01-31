# https://github.com/suikki/simpleSDL/

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(LIBNAME junebug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(sdlDir "${junebugDir}/lib/SDL")
set(SDL2MAIN_LIBRARY SDL2main)
set(SDL2_LIBRARY SDL2)


# Joysticks were causing trouble on android.
add_definitions(-DSDL_JOYSTICK=0)

include(GNUInstallDirs)

file(GLOB_RECURSE LIB_SRC_C "lib/*.c")
set_source_files_properties(
    ${LIB_SRC_CPP}
    ${LIB_PLUGINS_CPP}
    ${LIB_SRC_C}
    PROPERTIES
    COMPILE_FLAGS "-w"
)
add_library(${LIBNAME} STATIC
    src/core/Game.cpp
    src/core/coreDefaultCallbacks.cpp
    src/core/coreLoadActor.cpp
    src/core/coreCollision.cpp
    src/core/coreInputs.cpp
    src/core/coreSprites.cpp
    src/core/coreScenes.cpp
    src/core/coreFonts.cpp
    src/core/coreDebug.cpp

    src/MathLib.cpp
    src/RandLib.cpp
    src/Color.cpp
    
    src/Utils/Utils.cpp
    src/Utils/Twerp.cpp
    
    src/Rendering.cpp
    
    src/Camera.cpp

    src/actors/PureActor.cpp
    src/actors/VisualActor.cpp
    src/actors/PhysicalActor.cpp
    src/actors/Background.cpp
    src/actors/Tileset.cpp
    src/actors/Transitions.cpp
    
    src/Component.cpp
    src/components/Rigidbody.cpp
    src/components/Collider.cpp
    src/components/PolygonCollider.cpp
    src/components/TileCollider.cpp

    src/Sprite.cpp

    src/Files.cpp

    ${LIB_SRC_CPP}
    ${LIB_SRC_C}
    ${LIB_PLUGINS_CPP}
)


set(junebugIncludeDirs ${junebugIncludeDirs}
    "${junebugDir}/include"
    "${junebugDir}/lib/SDL/include"
    "${junebugDir}/lib/GLEW/include"
    "${junebugDir}/lib/rapidjson/include"
    "${junebugDir}/lib/SDL_FontCache"
)

target_include_directories(${LIBNAME} PRIVATE include)
target_include_directories(${LIBNAME} PRIVATE lib/SDL_FontCache)
target_include_directories(${LIBNAME} PRIVATE lib/rapidjson/include)
target_include_directories(${LIBNAME} PRIVATE src)

target_compile_features(${LIBNAME} PRIVATE cxx_std_17)

#
# Platform dependent stuff.
#

if(MINGW)
  # -Link standard libs statically to reduce dll clutter.
  # -lmingw32 is needed to make WinMain not disappear when linking.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -static-libgcc -lmingw32")

  # Only show the console in debug builds and also strip unused dependencies on MinSizeRel.
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-mconsole")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-mwindows")
  set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "-mwindows -s")

  link_directories(lib/GLEW/lib/win/x64)
  link_directories(lib/SDL/lib/win/x64)
endif()

if(EMSCRIPTEN)
  # This makes emscripten build a html page in addition to the js code.
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  # Embedding and mapping files at from_path@to_path
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ../../assets@/")

  # Emscripten requires static linking.
  set(SDL2_LIBRARY SDL2-static)
elseif(UNIX)
    include(FindPkgConfig)

    if (NOT EMSCRIPTEN)
        PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2) 
        PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
        PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer)
        PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf)
    endif()

    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

    TARGET_LINK_LIBRARIES(
        ${LIBNAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2IMAGE_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
        ${SDL2TTF_LIBRARIES}
    )

    if (NOT EMSCRIPTEN)
        include_directories(lib/discord_game_sdk)
        link_directories(lib/discord_game_sdk/lib/x86_64)

        target_link_libraries(${LIBNAME} discord_game_sdk)
    endif()
endif()

target_include_directories(${GAMENAME}
  PRIVATE ${junebugIncludeDirs}
)