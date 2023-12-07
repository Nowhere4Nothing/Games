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
inline T clamp(T min, T val, T max);

void draw_rect_in_pixel(int, int, int, int, unsigned int);
 void drawRect(float x, float y, float halfSizeX, float halfSizeY, u32 i);
 void  drawNumber(int, float, float, float, u32);
void drawText (const char *text, float, float, float, u32);
void drawArenaBorder(float, float, u32 );

#endif //UNTITLED_RENDER_H