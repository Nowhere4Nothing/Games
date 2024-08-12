//
// Created by Will&Carm on 1/12/2023.
//


#ifndef UNTITLED_STIMULATETHEGAME_H
#define UNTITLED_STIMULATETHEGAME_H

#include "PlatformCommon.h"
#include "Render.h"

#define is_down(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].change)
//#define is_released(b) (!input->buttons[b].isDown && input->buttons[b].change)

enum GameMode {
    GM_MENU,
    GM_GAMEPLAY,
    GM_DIFFICULTY
};

const float SPEED_SCALING_FACTOR = 0.1f; // Adjust this value based on your game's dynamics

void
simulateGame (Input* input, float dt);


#endif //UNTITLED_STIMULATETHEGAME_H
