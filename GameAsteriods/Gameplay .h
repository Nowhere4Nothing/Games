//
// Created by Will&Carm on 11/12/2023.
//

#ifndef UNTITLED_GAMEPLAY_H
#define UNTITLED_GAMEPLAY_H

#include "ButtonsAsteriods.h"
#include "ScreenRender.h"
#include "AsteriodGameScreen.h"
#include <vector>
#include <cmath>

#define is_down(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].change)
//#define is_released(b) (!input->buttons[b].isDown && input->buttons[b].change)

enum GameMode {
    GM_MENU,
    GM_GAMEPLAY
};

void playGame(Input* input, float dt);
void wrapCord(float ix, float iy, float &ox, float &oy);
bool collision(f32 cx, f32 cy, f32 radius, f32 x, f32 y, f32 radius2);

class Asteroid {
private:

//        f32 angle;


public:
    f32 x;
    f32 y;
    f32 dx;
    f32 dy;
    f32 angle;
    f32 size;
    std::vector<Point> vecModelAsteroid;

    Asteroid(f32 yCord, f32 xCord, f32 xVel, f32 yVel, f32 an, f32 is) : y(yCord), x (xCord), dx(xVel), dy(yVel), angle(an), size(is) {
        onUserCreateAst();}
    Asteroid() : y(), x (), dx(), dy(), angle(), size() {}
    virtual void update(f32 dt);
    virtual void draw(Asteroid& a);
//    virtual void wrapCord(f32 ix, f32 iy, f32 &ox, f32 &oy);
//     void drawAsteroidWithWrap(f32 x, f32 y, f32 radius, Colour color);
//    void drawPartialAsteroid(f32 x, f32 y, f32 radius, Colour color, f32 wrapX);

     void onUserCreateAst() {
        int verts = 20;
        vecModelAsteroid.clear();
        for (int i = 0; i < verts; i++)
        {
            float noise = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
            vecModelAsteroid.push_back(std::make_pair(noise * sinf(((float)i / (float)verts) * 6.28318f),
                                                 noise * cosf(((float)i / (float)verts) * 6.28318f)));
        }
    }

//    void draw(f32 x, f32 y, f32 radius, Colour color);
};

class Player {
public:
    f32 x;
    f32 y;
    f32 dx;
    f32 dy;
    f32 angle;
    s32 size;
    f32 centerX;
    f32 centerY;
    std::vector<Point> vecModelShip;

    Player(f32 yCord, f32 xCord, f32 an, f32 xVel, f32 yVel, s32 i2, f32 x, f32 y) :
    y(yCord), x (xCord), angle(an), dx(xVel), dy(yVel), size(i2), centerX(x), centerY(y) {
//        OnUserCreatePlayer();
    }
    Player() : y(), x(), angle(), dx(), dy(), size(), centerX(), centerY() {}

//    void OnUserCreatePlayer() {
//        vecModelShip =
//                {
//                        { 12.5f, -10.5f},
//                        {-1.5f, +12.5f},
//                        {+15.5f, +15.5f}
//                }; // A simple Isoceles Triangle
////        vecModelShip = initialShape.empty() ? defaultShipShape() : initialShape;
//    }
// doesnt work yet as the flames from the ship dont work properly without a return

    void update(float dt);
    virtual void draw(Input* input, float dt, Player& one);

};

class PowerUps {
public:
    f32 x;
    f32 y;
    f32 dx;
    f32 dy;
    Colour col;
    f32 size;
    s8 type;

PowerUps(f32 yCord, f32 xCord, f32 xVel, f32 yVel, Colour c, f32 s, s8 t) :
        y(yCord), x(xCord),  dy(yVel), dx(xVel), col(c), size(s), type(t) {}

    void update(f32);
    void draw() const;
};

class Bullets {
public:
    f32 x;
    f32 y;
    f32 dx;
    f32 dy;


    Colour col;
    f32 size;
    s32 type;

    Bullets(f32 yCord, f32 xCord, f32 xVel, f32 yVel, Colour c, f32 s, s32 t) :
    y(yCord), x(xCord),  dy(yVel), dx(xVel),col(c), size(s), type(t) {}



    void update(f32);
    void draw() const;
};

// Define the grid cell structure


#endif //UNTITLED_GAMEPLAY_H
