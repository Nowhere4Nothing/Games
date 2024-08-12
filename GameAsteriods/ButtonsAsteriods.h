//
// Created by Will&Carm on 11/12/2023.
//

#ifndef UNTITLED_BUTTONSASTERIODS_H
#define UNTITLED_BUTTONSASTERIODS_H

struct ButtonState {
    bool isDown;
    bool change;
};

enum {
    BUTTON_Up,
    BUTTON_LEFT,
    BUTTON_RIGHT, // the arrow buttons


    BUTTON_ENTER,
    BUTTON_ESC,
    BUTTON_SPACE,
    BUTTON_COUNT,
};

struct Input {
    ButtonState buttons[BUTTON_COUNT];
};

#endif //UNTITLED_BUTTONSASTERIODS_H
