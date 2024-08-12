//
// Created by Will&Carm on 11/12/2023.
//

#include <algorithm>
#include <cmath>
#include <iostream>
//#include <iostream>
#include <vector>
#include "AsteriodGameScreen.h"
#include "ScreenRender.h"
#include "Gameplay .h"



void ClearBackground(u32 color) {
    draw_rect_in_pixel(0, 0, renderState.bufferWidth, renderState.bufferHeight, color);
}

void drawRect(f32 x, f32 y, f32 halfSizeX, f32 halfSizeY, u32 color) {
    // this is to make the rect independent so the rect will resize if the gui is resized

    x *= static_cast<f32>(renderState.bufferHeight) * renderScale;
    y *= static_cast<f32>(renderState.bufferHeight) * renderScale;
    halfSizeX *= static_cast<f32>(renderState.bufferHeight) * renderScale;
    halfSizeY *= static_cast<f32>(renderState.bufferHeight) * renderScale;
    // convert, * the rect with the screen width and height so tha the rect stays the same even if the gui size is changed

    x += static_cast<f32>(renderState.bufferWidth) /2.f;
    y += static_cast<f32>(renderState.bufferHeight) /2.f;
    // offsetting to the center of the screen can pass 0, 0 to center it

    // change to pixels
    s32 x0 = static_cast<s32>(x - halfSizeX);
    s32 x1 = static_cast<s32>(x + halfSizeX);
    s32 y0 = static_cast<s32>(y - halfSizeY);
    s32 y1 = static_cast<s32>(y + halfSizeY);
    // find the min max points

    //centre
    draw_rect_in_pixel(x0, y0, x1, y1, color);

//    // Assuming renderScale is applied uniformly to x, y, halfSizeX, and halfSizeY
//    x *= renderScale;
//    y *= renderScale;
//    halfSizeX *= renderScale;
//    halfSizeY *= renderScale;
//
//    // Assuming renderScale is applied uniformly to bufferWidth and bufferHeight
//    x += static_cast<f32>(renderState.bufferWidth) / 2.f;
//    y += static_cast<f32>(renderState.bufferHeight) / 2.f;
//
//    // Drawing rectangle without scaling
//    draw_rect_in_pixel(static_cast<int>(x - halfSizeX), static_cast<int>(y - halfSizeY),
//                       static_cast<int>(x + halfSizeX), static_cast<int>(y + halfSizeY), color);
}

void draw_rect_in_pixel(int x0, int y0, int x1, int y1, u32 color) {

    x0 = clamp(0, x0, renderState.bufferWidth);
    x1 = clamp(0, x1, renderState.bufferWidth);
    y0 = clamp(0, y0, renderState.bufferHeight);
    y1 = clamp(0, y1, renderState.bufferHeight);
    // it needs to be clamped or the program will crash because the inner rect will become bigger than the gui if
    // the user resizes the gui smaller


    for (int i = y0; i < y1; i++) {
        s32* pixel = (s32*) renderState.bufferMemory + x0 + i*renderState.bufferWidth;
        // only colouring the pixels that are needed this is the y axis
        for (int u = x0; u < x1; u++) {
            *pixel++ = color;
            // this is the colour for the x axis
        }
    }
}

template <typename T>
inline T clamp(T min, T val, T max){
    if (val < min) return min;
    if (val > max) return max;
    return val;
    // function used to make the rect smaller if the screen is smaller, basic min max return function
}

void drawArenaBorder(f32 arenaX, f32 arenaY, u32 colour ) {
    arenaX *= static_cast<f32>(renderState.bufferHeight) * renderScale;
    arenaY *= static_cast<f32>(renderState.bufferHeight) * renderScale;

    s32 x0 = (s32)((f32)renderState.bufferWidth * .5f - arenaX);
    s32 x1 = (s32)((f32)renderState.bufferWidth * .5f + arenaX);
    s32 y0 = (s32)((f32)renderState.bufferHeight * .5f - arenaY);
    s32 y1 = (s32)((f32)renderState.bufferHeight * .5f + arenaY);

    draw_rect_in_pixel(0,0, renderState.bufferWidth, y0, colour); // the bottom border
    draw_rect_in_pixel(0,y1, x1, renderState.bufferHeight, colour); // the top border
    draw_rect_in_pixel(0, y0, x0, y1, colour); // the left side border
    draw_rect_in_pixel(x1,y0, renderState.bufferWidth, renderState.bufferHeight, colour); // the right side border

}

void drawLine(int x1, int y1, int x2, int y2, short c, Colour col)
{

    s32 x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1; dy = y2 - y1;
    dx1 = abs(dx); dy1 = abs(dy);
    px = 2 * dy1 - dx1; py = 2 * dx1 - dy1;

    if (dy1 <= dx1) {
        if (dx >= 0) {
            x = x1; y = y1; xe = x2;
        } else {
            x = x2; y = y2; xe = x1;
        }

//        Clamping(x, y, c, col);
        Draw(x, y, c, col);
//        drawRect(x,y,x,y, WHITE);
//        DrawWrapped(x,y,c,col);

        for (i = 0; x < xe; i++) {
            x = x + 1;
            if (px < 0)
                px = px + 2 * dy1;
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1;
                else
                    y = y - 1;
                px = px + 2 * (dy1 - dx1);
            }
//            Clamping(x, y, c, col);
            Draw(x, y, c, col);
//            drawRect(x,y,x,y, WHITE);
//            DrawWrapped(x,y,c,col);
        }
    } else {
        if (dy >= 0) {
            x = x1; y = y1; ye = y2;
        } else {
            x = x2; y = y2; ye = y1;
        }
//        Clamping(x, y, c, col);
        Draw(x, y, c, col);
//        drawRect(x,y,x,y, WHITE);
//        DrawWrapped(x,y,c,col);

        for (i = 0; y < ye; i++) {
            y = y + 1;
            if (py <= 0)
                py = py + 2 * dx1;
            else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1;
                else
                    x = x - 1;
                py = py + 2 * (dx1 - dy1);
            }
//            Clamping(x, y, c, col);
//            drawRect(x,y,x,y, WHITE);
            Draw(x, y, c, col);
//            DrawWrapped(x,y,c,col);
        }
    }
}


std::vector<Point> drawWireFrame(const std::vector<Point> &vecModelCoordinates, f32 &x, f32 &y, f32 &r, f32 s, s16 p, Colour col) {
std::vector<Point> vecTransformedCoordinates;
    s32 verts = (s32)vecModelCoordinates.size();
    vecTransformedCoordinates.resize(verts);
// Rotate
    for (u32 i = 0; i < verts; i++)
    {
        vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
        vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);

    // Scale

        vecTransformedCoordinates[i].first *= s;
        vecTransformedCoordinates[i].second *= s;

    // Translate
        vecTransformedCoordinates[i].first += x;
        vecTransformedCoordinates[i].second += y;
    }

    // Draw Closed Polygon
    for (u32 i = 0; i < verts + 1; i++)
    {
        u32 j = (i + 1);
        drawLine((s32)vecTransformedCoordinates[i % verts].first, (s32)vecTransformedCoordinates[i % verts].second,
                 (s32)vecTransformedCoordinates[j % verts].first, (s32)vecTransformedCoordinates[j % verts].second,
                 PIXEL_SOLID, col);
    }

    return vecTransformedCoordinates;
}

void drawSquare(s32 x, s32 y, s32 size, s16 c, Colour col, bool powerUp,const s8* text)
{
    // the coordinates used to make a square
    int x0 = x - size / 2;
    int y0 = y - size / 2;
    int x1 = x + size / 2;
    int y1 = y - size / 2;
    int x2 = x + size / 2;
    int y2 = y + size / 2;
    int x3 = x - size / 2;
    int y3 = y + size / 2;
    /*
     * These lines calculate the coordinates of the four corners of the square. The square is centered at (x, y),
     * so the corners are calculated by adding or subtracting half of the size from the center coordinates.
     */

    // make random

    // draw letter

    if (powerUp) {
        drawLine(x0, y0, x1, y1, c, col);
        drawLine(x1, y1, x2, y2, c, col);
        drawLine(x2, y2, x3, y3, c, col);
        drawLine(x3, y3, x0, y0, c, col);
        // calculate the center of the square
        int centerX = (x0 + x2) / 2;
        int centerY = (y0 + y2) / 2;

        // calculate the text size
        f32 textSize = static_cast<f32>(size) * 0.12f;

        // draw the text centered inside the square
        drawText(text, static_cast<f32>(x3 + 10), static_cast<f32>(y3 - 5), textSize, col);
        // sending it off to the drawline function and drawing each line
    } else {
        drawLine(x0, y0, x1, y1, c, col);
        drawLine(x1, y1, x2, y2, c, col);
        drawLine(x2, y2, x3, y3, c, col);
        drawLine(x3, y3, x0, y0, c, col);
        // sending it off to the drawline function and drawing each line
    }
}

void Draw(s32 x, s32 y, s16 c, Colour col ) {

    x = clamp(0, x, renderState.bufferWidth - 1);
    y = clamp(0, y, renderState.bufferHeight - 1);

    if (x >= 0 && x < renderState.bufferWidth && y >= 0 && y < renderState.bufferHeight) {
        // used to get the regions og the screen, if it is on the screen it draws a line
        s32* pixel = static_cast<s32*>(renderState.bufferMemory) + x + y * renderState.bufferWidth;
        *pixel = col;
    }
}

const s8 *letters[][7] = {

        " 00",
        "0  0",
        "0  0",
        "0000",
        "0  0",
        "0  0",
        "0  0",

        "000",
        "0  0",
        "0  0",
        "000",
        "0  0",
        "0  0",
        "000",

        " 000",
        "0",
        "0",
        "0",
        "0",
        "0",
        " 000",

        "000",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "000",

        "0000",
        "0",
        "0",
        "000",
        "0",
        "0",
        "0000",

        "0000",
        "0",
        "0",
        "000",
        "0",
        "0",
        "0",

        " 000",
        "0",
        "0",
        "0 00",
        "0  0",
        "0  0",
        " 000",

        "0  0",
        "0  0",
        "0  0",
        "0000",
        "0  0",
        "0  0",
        "0  0",

        "000",
        " 0",
        " 0",
        " 0",
        " 0",
        " 0",
        "000",

        " 000",
        "   0",
        "   0",
        "   0",
        "0  0",
        "0  0",
        " 000",

        "0  0",
        "0  0",
        "0 0",
        "00",
        "0 0",
        "0  0",
        "0  0",

        "0",
        "0",
        "0",
        "0",
        "0",
        "0",
        "0000",

        "00 00",
        "0 0 0",
        "0 0 0",
        "0   0",
        "0   0",
        "0   0",
        "0   0",

        "00  0",
        "0 0 0",
        "0 0 0",
        "0 0 0",
        "0 0 0",
        "0 0 0",
        "0  00",

        "0000",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0000",

        " 000",
        "0  0",
        "0  0",
        "000",
        "0",
        "0",
        "0",

        " 000 ",
        "0   0",
        "0   0",
        "0   0",
        "0 0 0",
        "0  0 ",
        " 00 0",

        "000",
        "0  0",
        "0  0",
        "000",
        "0  0",
        "0  0",
        "0  0",

        " 000",
        "0",
        "0 ",
        " 00",
        "   0",
        "   0",
        "000 ",

        "000",
        " 0",
        " 0",
        " 0",
        " 0",
        " 0",
        " 0",

        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        " 00",

        "0   0",
        "0   0",
        "0   0",
        "0   0",
        "0   0",
        " 0 0",
        "  0",

        "0   0 ",
        "0   0",
        "0   0",
        "0 0 0",
        "0 0 0",
        "0 0 0",
        " 0 0 ",

        "0   0",
        "0   0",
        " 0 0",
        "  0",
        " 0 0",
        "0   0",
        "0   0",

        "0   0",
        "0   0",
        " 0 0",
        "  0",
        "  0",
        "  0",
        "  0",

        "0000",
        "   0",
        "  0",
        " 0",
        "0",
        "0",
        "0000",

        "",
        "",
        "",
        "",
        "",
        "",
        "0",

        "   0",
        "  0",
        "  0",
        " 0",
        " 0",
        "0",
        "0",
};

void drawText (const s8 *text, f32 x, f32 y, f32 size, Colour colour) {
    f32 halfSize = size * .5f;
    f32 originalY = y;

    while (*text) {
        if (*text != 32) {
            const s8 **letter;
            if (*text == 47) letter = letters[27];
            else if (*text == 46) letter = letters[26];
            else letter = letters[*text - 'A'];
            f32 originalX = x;

            for (u32 i = 0; i < 7; i++) {
                const s8 *row = letter[i];
                while (*row) {
                    if (*row == '0') {
//                        drawLine(static_cast<int>(x), static_cast<int>(y), static_cast<int>(x + size), static_cast<int>(y),
//                        PIXEL_SOLID, RED);
                        fillSquare(x, y, size, colour);
                    }
                    x += size;
                    row++;
                }
                y -= size;
                x = originalX;
            }
        }
        text++;
        x += size * 6.f;
        y = originalY;
    }
}

void fillSquare(s32 x, s32 y, s32 size, Colour col)
{
    // the coordinates used to make a square
    int x0 = x - size / 2;
    int y0 = y - size / 2;
    int x1 = x + size / 2;
    int y1 = y - size / 2;
    int x2 = x + size / 2;
    int y2 = y + size / 2;
    int x3 = x - size / 2;
    int y3 = y + size / 2;

    for (int i = y0; i <= y2; i++) {
        for (int j = x0; j <= x2; j++) {
            Draw(j, i, PIXEL_SOLID, col);
        }
    }
}

//
//void Clamping(int& x, int& y, short c, Colour col) {
//
//    f32 halfSizex = 85.0f, halfSizeY = 45.0f;
//
//    // Clamp to arena boundaries
//    x = clamp(static_cast<int>(-halfSizex), x, static_cast<int>(halfSizex));
//    y = clamp(static_cast<int>(-halfSizeY), y, static_cast<int>(halfSizeY));
//
//    // Convert to screen coordinates
//    int screenX = static_cast<int>(x * renderState.bufferHeight * renderScale + renderState.bufferWidth / 2.0f);
//    int screenY = static_cast<int>(y * renderState.bufferHeight * renderScale + renderState.bufferHeight / 2.0f);
//
//    // Ensure the adjusted coordinates are within the buffer boundaries
//    x = clamp(0, screenX, renderState.bufferWidth - 1);
//    y = clamp(0, screenY, renderState.bufferHeight - 1);
//
//    // Draw the clamped rectangle
//    Draw(x, y, c, col);
//
//}