cmake_minimum_required(VERSION 3.15)
project(DirectX11_Engine C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE _UNICODE)

add_executable(DirectX11_Engine WIN32
    src/Main.cpp
    src/Window.h
    src/Window.cpp
    src/Graphics.h
    src/Graphics.cpp
    src/Cube.h
    src/Cube.cpp
    src/MathUtils.h
)

target_link_libraries(DirectX11_Engine PRIVATE
    d3d11
    d3dcompiler
    dxgi
)

if(MSVC)
    target_compile_options(DirectX11_Engine PRIVATE /W4)
else()
    target_compile_options(DirectX11_Engine PRIVATE -Wall -Wextra)
endif()
