//
// Created by Will&Carm on 26/11/2023.
//

#ifndef UNTITLED_GAME2_H
#define UNTITLED_GAME2_H

#include <Windows.h>

#pragma comment(lib, "gdi32.lib")

#define global_variable static
global_variable float renderScale = 0.01f;

#include "Render.h"
#include "stimulateTheGame.h"

struct RenderState {
    void * bufferMemory;
    int bufferWidth;
    int bufferHeight;
    BITMAPINFO bufferBitMapInfo;
};
// using a struct to load the game better

extern RenderState renderState;


#endif //UNTITLED_GAME2_H