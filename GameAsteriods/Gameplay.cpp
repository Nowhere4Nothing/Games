//
// Created by Will&Carm on 11/12/2023.
//

#include <cmath>
#include <random>
#include <iostream>
#include <algorithm>
#include "ButtonsAsteriods.h"
#include "Gameplay .h"

f32 player_1_p_x, player_1_p_y, player_1_Dp_x, player_1_dp_y, playerAngle, centerX, centerY, asAngle;
//player_1_p_x = 630.0f, player_1_p_y = 330.0f,
f32 arenaHalfSizex = 95.0f, arenaHalfSizeY = 45.0f;


f32 astX, astY, astDpX = 100, astDpY = 100, astSize = 50.0f;
f32 powerX, powerY, powerDx = 100.0f, powerDy = 10.0f, powerSize = 40.0f, powerType = 'B';
Colour powerCol;


std::vector<Asteroid> vecAsteroid;
std::vector<Bullets> bulletsVec;
std::vector<Player> players;
std::vector<PowerUps> power;
std::vector<Asteroid> newAst;

GameMode currentGameMode;

Asteroid astOne(astX, astY, astDpX, astDpY, asAngle, astSize);
Player playerOne(player_1_p_x = 630.0f, player_1_p_y = 330.0f, playerAngle, player_1_Dp_x, player_1_dp_y,
                 10, centerX, centerY);
PowerUps powerBox(powerX, powerY, powerDx, powerDy, powerCol, powerSize, powerType);
std::random_device rd;
std::mt19937 gen(rd());

bool astAlive = true;

void playGame(Input *input, float dt) {

    ClearBackground(BLACK);

    static float asteroidSpawnTimer = 0.0f;
    static float powerSpawn = 0.0f;
    static bool playerSpawned = false;

//    drawRect(0, 0, arenaHalfSizex, arenaHalfSizeY, BLACK); // the border
//    drawArenaBorder(arenaHalfSizex, arenaHalfSizeY, BLUE); // drawing the border

//    vecModelAsteroid.clear();

    if (players.empty()) {
        players.push_back(playerOne);
    }

//    astOne.update(dt);
//    astOne.draw(astOne);
//
    if (vecAsteroid.empty()) {
        vecAsteroid.push_back(astOne);
    }

    for (Player &Noplayers: players) {
        Noplayers.draw(input, dt, Noplayers);
        Noplayers.update(dt);
    }

    powerBox.update(dt);
    powerBox.draw();

    // Specify the range of indices you want (adjust these according to your needs)
    std::uniform_int_distribution<int> xDistribution((int) -renderState.bufferWidth, (int) renderState.bufferWidth);
    std::uniform_int_distribution<int> yDistribution((int) -renderState.bufferHeight, (int) renderState.bufferHeight);


    for (Asteroid &ast: vecAsteroid) {

        ast.draw(ast);
        ast.update(dt);
    }

    for (Bullets &b: bulletsVec) {

        b.update(dt);
        b.draw();
    }

//    for (u32 i= 0; i < vecAsteroid.size(); i ++ ) {
//        std::cout << "Ast size = " << i << std::endl;
//// for seeing if the vec bullets delete
//    }

    for (PowerUps &p: power) {
        p.update(dt);
        p.draw();
    }

    asteroidSpawnTimer += dt;
    if (asteroidSpawnTimer >= 10.0f) {
        asteroidSpawnTimer = 0.0f;
        // Add a new asteroid with random position and velocity
        Asteroid newAsteroid(
                static_cast<float>(xDistribution(gen)),
                static_cast<float>(yDistribution(gen)),
                static_cast<float>(rand() % 200 - 100),  // Random x velocity between -10 and 10
                static_cast<float>(rand() % 200 - 100),  // Random y velocity between -10 and 10
                asAngle,
                astSize
        );
        vecAsteroid.emplace_back(newAsteroid);
    }

    powerSpawn += dt;
    if (powerSpawn >= 100.0f) {
        powerSpawn = 0.0f;
        std::random_device rdPower;
        std::mt19937 genower(rdPower());

        s8 powerTypeList[5] = {'S', 'B', 'D', 'A', 'T'};
        std::uniform_int_distribution<int> pDistribution(0, 4);
        s32 selectedIndex = pDistribution(genower);

        PowerUps p(
                static_cast<float>(xDistribution(gen)),
                static_cast<float>(yDistribution(gen)),
                static_cast<float>(rand() % -400 - 200),  // Random x velocity between -10 and 10
                static_cast<float>(rand() % -400 - 200),  // Random y velocity between -10 and 10
                powerCol, powerSize, powerTypeList[selectedIndex]
        );
        power.push_back(p);
    }

    for (Asteroid &ast: vecAsteroid) {
        for (Bullets &b: bulletsVec) {
            if (collision(ast.x, ast.y, ast.size, b.x, b.y, b.size)) {

                b.x = -100;



                if ( ast.size > 10) {

                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

                    float angle1 = dis(gen) * 6.283185f;
                    float x = 100.0f * sinf(angle1);
                    float y = 50.0f * cosf(angle1);

                    Asteroid a(ast.x, ast.y, x, y, 0, ast.size / 2);
                    newAst.emplace_back(a);

                    Asteroid bb (ast.x, ast.y, 10.0f, 50.0f, 0, ast.size / 2);
                    newAst.emplace_back(bb);

                }

                ast.x = -100;
            }

        }

        for (Asteroid &hhTwo: vecAsteroid) {
            if (&ast != &hhTwo && collision(ast.x, ast.y, ast.size, hhTwo.x, hhTwo.y, hhTwo.size)) {
                ast.dx = -ast.dx;
                ast.dy = -ast.dy;
                hhTwo.dx = -hhTwo.dx;
                hhTwo.dy = -hhTwo.dy;
            }
        }

    }

    for ( Asteroid& newAstA :  newAst) {
        vecAsteroid.push_back(newAstA);
        newAst.clear();
    }

    if (!vecAsteroid.empty()) {
        auto i = remove_if(vecAsteroid.begin(), vecAsteroid.end(), [&](Asteroid& a) {
            return (a.x < 0);
            /*
             * Cannot delete, it moves the elements that dont satisfy the condition to the start of the container.
             *  Then returns the new iterator pointing to the new logical end of the vector, this indicates the range of elements
             *  that are considered valid after the removal
             */
        });
        if (i != vecAsteroid.end()) vecAsteroid.erase(i);
        // removing the bullets
    }

    if (!bulletsVec.empty()) {
        auto i = remove_if(bulletsVec.begin(), bulletsVec.end(), [&](Bullets b) {
            return (b.x < 1 || b.y < 1 || b.x >= (f32)renderState.bufferWidth - 1 || b.y >= (f32)renderState.bufferHeight - 1);
            /*
             * Cannot delete, it moves the elements that dont satisfy the condition to the start of the container.
             *  Then returns the new iterator pointing to the new logical end of the vector, this indicates the range of elements
             *  that are considered valid after the removal
             */
        });
        if (i != bulletsVec.end()) bulletsVec.erase(i);
        // removing the bullets
    }
//
////    for (u32 i= 0; i < bulletsVec.size(); i ++ ) {
//////        std::cout << "Bullets size = " << i << std::endl;
//// for seeing if the vec bullets delete
////    }
}



void Player::update(f32 dt) {
    x += dx * dt;
    y += dy * dt;

//    std::cout << "Player Position: (" << x << ", " << y << ")\n";

    wrapCord(x, y, x, y);

}

void Asteroid::update(f32 dt) {
    float rotationSpeed = 0.1f;  // Adjust this value to control the rotation speed

    angle += rotationSpeed * dt;

    x += dx * dt;
    y += dy * dt;
//    std::cout << "Ast Position: (" << x << ", " << y << ")\n";
    wrapCord(x, y, x, y);

}

void Asteroid::draw(Asteroid &a) {
//        for (s32 i = 0; i < 12; i++) {
//            for (s32 j = 0; j < 12; j++) {
//                drawRect(i + static_cast<float>(a.x), j + static_cast<float>(a.y), 0.2, 0.1, WHITE);
//            }
//        }
//    drawAsteroidWithWrap(a.x, a.y, (f32)size, WHITE);

    drawWireFrame(
            vecModelAsteroid,
            a.x,
            a.y,
            a.angle,
            a.size,
            PIXEL_SOLID,
            WHITE
    );


}



void Player::draw(Input *input, f32 dt, Player &one) {
    one.vecModelShip =
            {
                    {12.5f,  -10.5f},
                    {-1.5f,  +12.5f},
                    {+15.5f, +15.5f}
            }; // A simple Isoceles Triangle

    one.vecModelShip =
            drawWireFrame(one.vecModelShip, one.x, one.y, one.angle, 1.0f, PIXEL_SOLID, WHITE);

//    std::cout << "vector " << one.vecModelShip[1].first << std::endl;

    f32 point1_x = one.vecModelShip[0].first;
    f32 point1_y = one.vecModelShip[0].second;

    f32 point2_x = one.vecModelShip[1].first;
    f32 point2_y = one.vecModelShip[1].second;

    f32 point3_x = vecModelShip[2].first;
    f32 point3_y = vecModelShip[2].second;

//    std::cout << "pointOne " << point2_x << std::endl;


    // getting the center of the player even when spinning
    centerX = (point1_x + point2_x + point3_x) / 3.0f;
    centerY = (point1_y + point2_y + point3_y) / 3.0f;

    if (is_down(BUTTON_LEFT)) {
        one.angle += .03f;

        // drawing fire when turning
        int lineStartX = (int) point3_x;
        int lineStartY = (int) point3_y;
        int lineEndX1 = lineStartX + static_cast<int>(18.0f * cosf(angle - 12));
        int lineEndY1 = lineStartY + static_cast<int>(18.0f * sinf(angle - 12));
        int lineEndX2 = lineEndX1 + static_cast<int>(12.0f * cosf(angle - 2));
        int lineEndY2 = lineEndY1 + static_cast<int>(12.0f * sinf(angle - 2));
        int lineEndX3 = lineEndX2 + static_cast<int>(10.0f * cosf(angle - 14));
        int lineEndY3 = lineEndY2 + static_cast<int>(10.0f * sinf(angle - 14));


        drawLine(lineStartX, lineStartY, lineEndX1, lineEndY1, PIXEL_SOLID, ORANGE);
        drawLine(lineEndX1, lineEndY1, lineEndX2, lineEndY2, PIXEL_SOLID, ORANGE);
        drawLine(lineEndX2, lineEndY2, lineEndX3, lineEndY3, PIXEL_SOLID, ORANGE);
        drawLine(lineEndX3, lineEndY3, lineStartX, lineStartY, PIXEL_SOLID, ORANGE);

    }
    if (is_down(BUTTON_RIGHT)) {
        one.angle -= 0.03f;

        //  drawing fire when turning
        int lineStartX = (int) point2_x;
        int lineStartY = (int) point2_y;
        int lineEndX1 = lineStartX + static_cast<int>(18.0f * cosf(angle + 15));
        int lineEndY1 = lineStartY + static_cast<int>(18.0f * sinf(angle + 15));
        int lineEndX2 = lineEndX1 + static_cast<int>(12.0f * cosf(angle + 5));
        int lineEndY2 = lineEndY1 + static_cast<int>(12.0f * sinf(angle + 5));
        int lineEndX3 = lineEndX2 + static_cast<int>(10.0f * cosf(angle + 10));
        int lineEndY3 = lineEndY2 + static_cast<int>(10.0f * sinf(angle + 10));

        drawLine(lineStartX, lineStartY, lineEndX1, lineEndY1, PIXEL_SOLID, ORANGE);
        drawLine(lineEndX1, lineEndY1, lineEndX2, lineEndY2, PIXEL_SOLID, ORANGE);
        drawLine(lineEndX2, lineEndY2, lineEndX3, lineEndY3, PIXEL_SOLID, ORANGE);
        drawLine(lineEndX3, lineEndY3, lineStartX, lineStartY, PIXEL_SOLID, ORANGE);
    }
    // speed, velocity and buttons for the player

    if (is_down(BUTTON_Up)) {
        one.dx += (float) sin(one.angle) * 70.0f * dt;
        one.dy += -(float) cos(one.angle) * 70.0f * dt;

// for the fire behind the spaceship
        {
            int lineStartX = (int) point2_x;
            int lineStartY = (int) point2_y;
            int lineEndX = (int) point3_x;
            int lineEndY = (int) point3_y;

            int lineEndX1 = lineStartX + static_cast<int>(12.0f * cosf(angle + 15));
            int lineEndY1 = lineStartY + static_cast<int>(12.0f * sinf(angle + 15));

            int lineEndX2 = lineEndX1 + static_cast<int>(12.0f * cosf(angle));
            int lineEndY2 = lineEndY1 + static_cast<int>(12.0f * sinf(angle));

            int lineEndX3 = lineEndX2 + static_cast<int>(18.0f * cosf(angle + 26.6f));
            int lineEndY3 = lineEndY2 + static_cast<int>(18.0f * sinf(angle + 26.6f));


            int lineEndX4 = lineEndX3 + static_cast<int>(18.0f * cosf(angle - 26.1f));
            int lineEndY4 = lineEndY3 + static_cast<int>(18.0f * sinf(angle - 26.1f));

            int lineEndX5 = lineEndX4 + static_cast<int>(12.0f * cosf(angle));
            int lineEndY5 = lineEndY4 + static_cast<int>(12.0f * sinf(angle));

            int lineEndX6 = lineEndX + static_cast<int>(3.0f * cosf(angle - 15));
            int lineEndY6 = lineEndY + static_cast<int>(3.0f * sinf(angle - 15));

            drawLine(lineStartX, lineStartY, lineEndX1, lineEndY1, PIXEL_SOLID, RED);
            drawLine(lineEndX1, lineEndY1, lineEndX2, lineEndY2, PIXEL_SOLID, RED);
            drawLine(lineEndX2, lineEndY2, lineEndX3, lineEndY3, PIXEL_SOLID, RED);
            drawLine(lineEndX3, lineEndY3, lineEndX4, lineEndY4, PIXEL_SOLID, RED);
            drawLine(lineEndX4, lineEndY4, lineEndX5, lineEndY5, PIXEL_SOLID, RED);
            drawLine(lineEndX5, lineEndY5, lineEndX6, lineEndY6, PIXEL_SOLID, RED);
        }
    }

    if (pressed(BUTTON_SPACE)) {
        Bullets newBullet(
                point1_y, point1_x, 350.0f * sinf(one.angle), -350.f * cosf(one.angle), RED, 5.0f, 0
        );
        bulletsVec.push_back(newBullet);
    }
}

void PowerUps::update(f32 dt) {
    x += dx * dt;
    y += dy * dt;

//    std::cout << "Power Position: (" << x << ", " << y << ")\n";

    wrapCord(x, y, x, y);
}

void PowerUps::draw() const {

    if (type == 'S') drawSquare((s32) x, (s32) y, (int) size, PIXEL_SOLID, LIGHT_BLUE, true, "S");
    else if (type == 'B') drawSquare((s32) x, (s32) y, (int) size, PIXEL_SOLID, LIGHT_BLUE, true, "B");
    else if (type == 'D') drawSquare((s32) x, (s32) y, (int) size, PIXEL_SOLID, LIGHT_BLUE, true, "D");
    else if (type == 'A') drawSquare((s32) x, (s32) y, (int) size, PIXEL_SOLID, LIGHT_BLUE, true, "A");
    else if (type == 'T') drawSquare((s32) x, (s32) y, (int) size, PIXEL_SOLID, LIGHT_BLUE, true, "T");
}

void wrapCord(f32 ix, f32 iy, f32 &ox, f32 &oy) {

    {
        //        ox = ix;
//        oy = iy;
//
//        if (ix < -arenaHalfSizex) ox = ix +  arenaHalfSizex * 2;
//        else if (ix >= arenaHalfSizex) ox = ix - arenaHalfSizex * 2 ;
//
//        if (iy < -arenaHalfSizeY) oy = iy +  arenaHalfSizeY *2;
//        else if (iy >= arenaHalfSizeY) oy = iy - arenaHalfSizeY *2;
    }

    ox = ix;
    oy = iy;
    if (ix < 0.0f) ox = ix + (f32) renderState.bufferWidth;
    if (ix >= (f32) renderState.bufferWidth) ox = ix - (f32) renderState.bufferWidth;
    if (iy < 0.0f) oy = iy + (f32) renderState.bufferHeight;
    if (iy >= (f32) renderState.bufferHeight) oy = iy - (f32) renderState.bufferHeight;
}

void Bullets::update(f32 dt) {
    x += dx * dt;
    y += dy * dt;
}

void Bullets::draw() const {
    drawSquare((s32) x, (s32) y, (s32) size, PIXEL_SOLID, WHITE, false, "A");


}

bool collision(f32 cx, f32 cy, f32 radius, f32 x, f32 y, f32 radius2){
    return sqrt((x - cx) * (x-cx) + (y - cy) * (y - cy)) < radius;

    // if(sqrt ((Ax- Bx)^2 + (Ay- By)^2) < the size
//
//    float dx = x - cx;
//    float dy = y - cy;
//    float distance = std::sqrt(dx * dx + dy * dy);
//    return distance < (radius + radius2);
}

//anything lower is a working progress

//void DrawWrapped(f32 x, f32 y, short c, Colour col)
//{
//    f32 wrappedX, wrappedY;
////    one.WrapCoordinates(static_cast<f32>(x), static_cast<f32>(y), wrappedX, wrappedY);
////    Draw(static_cast<int>(wrappedX), static_cast<int>(wrappedY), c, col);
//
////    one.WrapCoordinates(x, y, x, y);
//    Clamping(reinterpret_cast<int &>(x), reinterpret_cast<int &>(y), c, col);
//}

//void Asteroid::wrapCord(f32 ix, f32 iy, f32 &ox, f32 &oy) {
////    ox = ix;
////    oy = iy;
////    if (ix < 0.0f) ox = ix + (f32) renderState.bufferWidth;
////    if (ix >= (f32) renderState.bufferWidth) ox = ix - (f32) renderState.bufferWidth;
////    if (iy < 0.0f) oy = iy + (f32) renderState.bufferHeight;
////    if (iy >= (f32) renderState.bufferHeight) oy = iy - (f32) renderState.bufferHeight;
//
//
//}

//void Asteroid::drawAsteroidWithWrap(f32 x, f32 y, f32 radius, Colour color) {
//    // Check if the asteroid extends beyond the screen boundary in any direction
//    if (x - radius < 0) {
//        // Calculate the position of the asteroid on the opposite side of the screen
//        f32 wrapX = renderState.bufferWidth + (x - radius);
//        // Draw the asteroid partially on the opposite side of the screen
//        drawPartialAsteroid(x, y, radius, color, wrapX);
//    } else if (x + radius > renderState.bufferWidth) {
//        // Calculate the position of the asteroid on the opposite side of the screen
//        f32 wrapX = x + radius - renderState.bufferWidth;
//        // Draw the asteroid partially on the opposite side of the screen
//        drawPartialAsteroid(x, y, radius, color, wrapX);
//    } else {
//        // Draw the asteroid normally
//        draw(x, y, radius, color);
//    }
//}
//
//void Asteroid::drawPartialAsteroid(f32 x, f32 y, f32 radius, Colour color, f32 wrapX) {
//    // Draw the asteroid on the current side of the screen
//    draw(x, y, radius, color);
//    // Draw the remaining portion of the asteroid on the opposite side of the screen
//    draw(wrapX, y, radius, color);
//}

//void Asteroid::draw(f32 x, f32 y, f32 radius, Colour color) {
//    // Your draw function implementation
//    // Ensure that this function correctly draws a pixel at the specified coordinates
//    drawWireFrame(
//            vecModelAsteroid,
//            x,
//            y,
//            radius,
//            50,
//            PIXEL_SOLID,
//            WHITE
//    );
//
//}