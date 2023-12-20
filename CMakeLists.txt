cmake_minimum_required(VERSION 3.26)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp)
add_executable(WindowMain O&X/Program.cpp
        O&X/Window.h
        O&X/Window.cpp)
add_executable(test O&X/test.cpp)

add_executable(NewGame WIN32 Game/Game2.cpp
        Game/Render.cpp
        Game/Game2.h
        Game/Render.h
        Game/PlatformCommon.h
        Game/SimulateTheGame.cpp
        Game/stimulateTheGame.h)
add_executable(Asteriod GameAsteriods/AsteriodGameScreen.cpp
        GameAsteriods/AsteriodGameScreen.h
        GameAsteriods/ButtonsAsteriods.h
        GameAsteriods/Gameplay.cpp
        "GameAsteriods/Gameplay .h"
        GameAsteriods/ScreenRender.cpp
        GameAsteriods/ScreenRender.h
)
