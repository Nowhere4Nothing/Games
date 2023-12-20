//
// Created by Will&Carm on 27/11/2023.
//

#ifndef UNTITLED_RENDER_H
#define UNTITLED_RENDER_H



typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef unsigned long long u64;
typedef long long s64;

template <typename T>
T clamp(T min, T val, T max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
    // function used to make the rect smaller if the screen is smaller, basic min max return function
}

enum Colour {
    BLACK       = 0x000000,
    WHITE       = 0xFFFFFF,
    RED         = 0xFF0000,
    GREEN       = 0x00FF00,
    GRASS_GREEN = 0x4CAF50,
    BLUE        = 0x0000FF,
    YELLOW      = 0xFFFF00,
    CYAN        = 0x00FFFF,
    MAGENTA     = 0xFF00FF,
    GRAY        = 0x808080,
    LIGHT_GRAY  = 0xC0C0C0,
    DARK_RED    = 0x800000,
    DARK_GREEN  = 0x008000,
    DARK_BLUE   = 0x000080,
    DARK_CYAN   = 0x008080,
    DARK_MAGENTA= 0x800080,
    DARK_YELLOW = 0x808000,
    BROWN       = 0xA52A2A,
    ORANGE      = 0xFFA500,
    PINK        = 0xFF1493,
    LAVENDER    = 0x9370DB,
    LIGHT_RED        = 0xFFB6C1,
    LIGHT_GREEN      = 0x90EE90,
    LIGHT_BLUE       = 0xADD8E6,
    LIGHT_YELLOW     = 0xFFFFE0,
    LIGHT_MAGENTA    = 0xFFE4E1,
    LIGHT_CYAN       = 0xE0FFFF,
    LIGHT_ORANGE     = 0xFFD700,
    LIGHT_PINK       = 0xFFC0CB,
    LIGHT_LAVENDER   = 0xE6E6FA,
    LIGHT_PEACH      = 0xFFDAB9,
    LIGHT_AQUA       = 0x00FFFF,
    LIGHT_MINT       = 0xF5FFFA,
    LIGHT_LIME       = 0x32CD32,
    LIGHT_LILAC      = 0xC8A2C8,
    LIGHT_GOLD       = 0xFFD700,
    LIGHT_CORAL      = 0xF08080,
    LIGHT_SKY_BLUE   = 0x87CEFA,
    LIGHT_TURQUOISE  = 0xAFEEEE
};

void draw_rect_in_pixel(int, int, int, int, unsigned int);
 void drawRect(float x, float y, float halfSizeX, float halfSizeY, u32 i);
 void  drawNumber(int, float, float, float, u32);
void drawText (const char *text, float, float, float, u32);
void drawArenaBorder(float, float, u32 );

#endif //UNTITLED_RENDER_H