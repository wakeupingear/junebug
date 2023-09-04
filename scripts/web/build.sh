cmake . -DCMAKE_TOOLCHAIN_FILE=${1-~/emsdk}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_BUILD_TYPE=Release -B embuild
cmake --build ./embuild --target all