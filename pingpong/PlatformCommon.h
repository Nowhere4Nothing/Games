//
// Created by Will&Carm on 1/12/2023.
//

#ifndef UNTITLED_PLATFORMCOMMON_H
#define UNTITLED_PLATFORMCOMMON_H

struct ButtonState {
    bool isDown;
    bool change;
};

enum {
    BUTTON_Up,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT, // the arrow buttons

    BUTTON_W,
    BUTTON_S,

    BUTTON_ENTER,
    BUTTON_ESC,
    BUTTON_COUNT,
};

struct Input {
    ButtonState buttons[BUTTON_COUNT];
};

#endif //UNTITLED_PLATFORMCOMMON_H
