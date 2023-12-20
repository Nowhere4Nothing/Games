cmake_minimum_required(VERSION 3.26)
project(untitled)

set(CMAKE_CXX_STANDARD 17)



add_executable(NewGame WIN32 Game/Game2.cpp
        Game/Render.cpp
        Game/Game2.h
        Game/Render.h
        Game/PlatformCommon.h
        Game/SimulateTheGame.cpp
        Game/stimulateTheGame.h)

