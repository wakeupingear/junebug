#https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake

set(LIBNAME junebug)

cmake_minimum_required(VERSION 3.9)
project(${LIBNAME} VERSION 0.1.0 DESCRIPTION "A lightweight C++ game engine")
include(GNUInstallDirs)

add_library(${LIBNAME} STATIC
    src/core/JGame.cpp
    src/core/coreDataLoading.cpp
    src/core/coreInputs.cpp
    src/core/coreSprites.cpp

    src/MathLib.cpp
    src/Color.cpp
    src/Utils.cpp
    
    src/Camera.cpp

    src/actors/PureActor.cpp
    src/actors/VisualActor.cpp
    src/Component.cpp

    src/Sprite.cpp
)

set_target_properties(${LIBNAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/${LIBNAME}.h
)

if (WIN32)
    include_directories(lib/GLEW/include)
    include_directories(lib/SDL/include)
    link_directories(lib/GLEW/lib/win/x64)
    link_directories(lib/SDL/lib/win/x64)

	target_link_libraries(${LIBNAME} SDL2 SDL2_image SDL2_mixer SDL2_ttf glew32 opengl32)

    configure_file(${LIBNAME}.pc.in ${LIBNAME}.pc @ONLY)
    target_include_directories(${LIBNAME} PRIVATE include)
    target_include_directories(${LIBNAME} PRIVATE src)
    target_compile_features(${LIBNAME} PRIVATE cxx_std_17)

    TARGET_LINK_LIBRARIES(
        ${LIBNAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2IMAGE_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
    )

    install(TARGETS ${LIBNAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(FILES ${CMAKE_BINARY_DIR}/${LIBNAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
elseif (APPLE)
    include_directories(lib/GLEW/include)
    include_directories(lib/SDL/include)
    link_directories(lib/GLEW/lib/mac)
    link_directories(lib/SDL/lib/mac)

	target_link_libraries(${LIBNAME} SDL2 SDL2_image SDL2_mixer SDL2_ttf)

    configure_file(${LIBNAME}.pc.in ${LIBNAME}.pc @ONLY)
    target_include_directories(${LIBNAME} PRIVATE include)
    target_include_directories(${LIBNAME} PRIVATE src)
    target_compile_features(${LIBNAME} PRIVATE cxx_std_17)

    TARGET_LINK_LIBRARIES(
        ${LIBNAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2IMAGE_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
    )

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

    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

    TARGET_LINK_LIBRARIES(
        ${LIBNAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2IMAGE_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
    )

    install(TARGETS ${LIBNAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(FILES ${CMAKE_BINARY_DIR}/${LIBNAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
endif()