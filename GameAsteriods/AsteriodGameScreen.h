//
// Created by Will&Carm on 11/12/2023.
//

#ifndef UNTITLED_ASTERIODGAMESCREEN_H
#define UNTITLED_ASTERIODGAMESCREEN_H

#include <Windows.h>

#pragma comment(lib, "gdi32.lib")

#define global_variable static
extern float renderScale;

struct RenderState {
    void * bufferMemory;
    int bufferWidth;
    int bufferHeight;
    BITMAPINFO bufferBitMapInfo;
};
// using a struct to load the game better

extern RenderState renderState;






#endif //UNTITLED_ASTERIODGAMESCREEN_H
