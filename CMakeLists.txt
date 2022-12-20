#https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake

set(LIBNAME junebug)

cmake_minimum_required(VERSION 3.9)
project(${LIBNAME} VERSION 0.1.0 DESCRIPTION "A lightweight C++ game engine")
include(GNUInstallDirs)

file(GLOB_RECURSE LIB_SRC_CPP "lib/*.cpp")
file(GLOB_RECURSE LIB_PLUGINS_CPP "plugins/*.cpp")
file(GLOB_RECURSE LIB_SRC_C "lib/*.c")
set_source_files_properties(
    ${LIB_SRC_CPP}
    ${LIB_SRC_C}
    PROPERTIES
    COMPILE_FLAGS "-w"
)
add_library(${LIBNAME} STATIC
    src/core/JGame.cpp
    src/core/coreDataLoading.cpp
    src/core/coreInputs.cpp
    src/core/coreSprites.cpp
    src/core/coreScenes.cpp
    src/core/coreFonts.cpp
    src/core/coreDebug.cpp

    src/MathLib.cpp
    src/RandLib.cpp
    src/Color.cpp
    
    src/Utils/Utils.cpp
    src/Utils/twerp.cpp
    
    src/Rendering.cpp
    
    src/Camera.cpp

    src/actors/PureActor.cpp
    src/actors/VisualActor.cpp
    src/Component.cpp

    src/Sprite.cpp

    src/Files.cpp

    ${LIB_SRC_CPP}
    ${LIB_SRC_C}
    ${LIB_PLUGINS_CPP}
)

set_target_properties(${LIBNAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/${LIBNAME}.h
)

if (WIN32)
    include_directories(lib/GLEW/include)
    include_directories(lib/SDL/include)
    include_directories(lib/rapidjson/include)
    include_directories(lib/SDL_FontCache)
    link_directories(lib/GLEW/lib/win/x64)
    link_directories(lib/SDL/lib/win/x64)

    include_directories(plugins/discord/discord_game_sdk)
    link_directories(plugins/discord/discord_game_sdk/lib/x86_64)

	target_link_libraries(${LIBNAME} SDL2 SDL2_image SDL2_mixer SDL2_ttf glew32 opengl32 discord_game_sdk)

    configure_file(${LIBNAME}.pc.in ${LIBNAME}.pc @ONLY)
    target_include_directories(${LIBNAME} PRIVATE include)
    target_include_directories(${LIBNAME} PRIVATE src)
    target_compile_features(${LIBNAME} PRIVATE cxx_std_17)

    install(TARGETS ${LIBNAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(FILES ${CMAKE_BINARY_DIR}/${LIBNAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
elseif (APPLE)
    include_directories(lib/GLEW/include)
    include_directories(lib/SDL/include)
    include_directories(lib/rapidjson/include)
    link_directories(lib/GLEW/lib/mac)
    link_directories(lib/SDL/lib/mac)

    include_directories(lib/discord_game_sdk)
    link_directories(lib/discord_game_sdk/lib/x86_64)

	target_link_libraries(${LIBNAME} SDL2 SDL2_image SDL2_mixer SDL2_ttf discord_game_sdk)

    configure_file(${LIBNAME}.pc.in ${LIBNAME}.pc @ONLY)
    target_include_directories(${LIBNAME} PRIVATE include)
    target_include_directories(${LIBNAME} PRIVATE src)
    target_compile_features(${LIBNAME} PRIVATE cxx_std_17)

    install(TARGETS ${LIBNAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(FILES ${CMAKE_BINARY_DIR}/${LIBNAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

else()
    configure_file(${LIBNAME}.pc.in ${LIBNAME}.pc @ONLY)
    target_include_directories(${LIBNAME} PRIVATE include)
    target_include_directories(${LIBNAME} PRIVATE src)
    target_compile_features(${LIBNAME} PRIVATE cxx_std_17)

    include(FindPkgConfig)

    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2) 
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer)
    PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf)

    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

    include_directories(lib/discord_game_sdk)
    link_directories(lib/discord_game_sdk/lib/x86_64)

    TARGET_LINK_LIBRARIES(
        ${LIBNAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2IMAGE_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
        ${SDL2TTF_LIBRARIES}
        discord_game_sdk
    )

    install(TARGETS ${LIBNAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(FILES ${CMAKE_BINARY_DIR}/${LIBNAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
endif()