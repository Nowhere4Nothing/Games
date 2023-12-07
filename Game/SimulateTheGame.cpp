
#include "stimulateTheGame.h"
#include <iostream>
// semantic compression.

f32 player_1_p, player_1_Dp, player_2_p, player_2_Dp;
f32 arenaHalfSizex = 85, arenaHalfSizeY = 45;
f32 playerHalfSixeX = 2.5, playerHalfSizeY = 12;
f32 ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ballHalfSize = 1;
f32 speedIncrementFact = 30.0f, paddleAcceleration = 1.05f, arenaAcceleration = 1.02f;

s32 playerScore1, playerScore2;

void simulatePlayer(f32 *p, f32 *dp, f32 ddp, f32 dt) {
    ddp -= *dp * 10.f; // this is the friction. higher it is the faster it stops

    *p = *p + *dp * dt + ddp * dt * dt * .5f;
    *dp = *dp + ddp * dt; // velocity for acceleration

    if (*p + playerHalfSizeY > arenaHalfSizeY) { // hitting the top border
        *p = arenaHalfSizeY - playerHalfSizeY; // stops the player and moves it back to the limit
        *dp *= -1; // change teh velocity to bounce it back
    } else if (*p - playerHalfSizeY < -arenaHalfSizeY) { // hitting the bottom border
        *p = -arenaHalfSizeY + playerHalfSizeY; // stops the player and moves it back to the limit
        *dp *= -1; // change velocity to bounce it back
    }
}

enum GameMode {
    GM_MENU,
    GM_GAMEPLAY
};

GameMode currentGameMode;
s32 hotButton;
bool enemyIsAI;

s32 count = 0;

void simulateGame(Input *input, f32 dt) {

    drawRect(0, 0, arenaHalfSizex, arenaHalfSizeY, 0xaaa50); // the border
    drawArenaBorder(arenaHalfSizex, arenaHalfSizeY, 0x0000ff); // drawing the border
    // use this to stop overdraw
    count++;


    if (currentGameMode == GM_GAMEPLAY) {

        f32 player_1_Ddp = 0.f; // units per second
        if (is_down(BUTTON_Up)) player_1_Ddp += 2000;
        if (is_down(BUTTON_DOWN)) player_1_Ddp -= 2000;
        // speed, velocity and buttons for the player

        f32 player_2_Ddp = 0.f; // units per second
        if (!enemyIsAI) {
            if (is_down(BUTTON_W)) player_2_Ddp += 2000;
            if (is_down(BUTTON_S)) player_2_Ddp -= 2000;
            // the code for the player controls
        } else {
            player_2_Ddp = (ball_p_y - player_2_p) * 100;
            if (player_2_Ddp > 1300) player_2_Ddp = 1300;
            if (player_2_Ddp < -1300) player_2_Ddp = -1300;
            // makes the paddles go faster the further away they are from the ball

            if (ball_p_y > player_2_Ddp) player_2_Ddp += SPEED_SCALING_FACTOR * ball_dp_y;
            if (ball_p_x < player_2_Ddp) player_2_Ddp -= SPEED_SCALING_FACTOR * ball_dp_x;
            // hopefully it speeds the paddles up if teh ball velocity is going faster

            f32 paddleTopY = player_2_p + playerHalfSizeY;
            if (ball_p_y > paddleTopY) {
                player_2_Ddp += SPEED_SCALING_FACTOR * (ball_p_y - paddleTopY);
            } else if (ball_p_y < player_2_p - playerHalfSizeY) {
                player_2_Ddp -= SPEED_SCALING_FACTOR * (player_2_p - playerHalfSizeY - ball_p_y);
            }

            if (player_2_Dp == 0 && player_2_p > 0 && ball_p_x > 0) {
                player_2_Dp -= 1 * ball_p_x / arenaHalfSizex;
            } else if (player_2_Dp == 0 && player_2_p < 0 && ball_p_x > 0) {
                player_2_Dp += 1 * ball_p_x / arenaHalfSizex;
            }
            // used for the ai to initiate the ball moving if the player is still
            // code for the ai speed && acceleration

        }
            // AI code for the left hand player, changing the speed and velocity of the AI

        simulatePlayer(&player_1_p, &player_1_Dp, player_1_Ddp, dt);
        simulatePlayer(&player_2_p, &player_2_Dp, player_2_Ddp, dt);
        // method that gives the play movement and keeps them on the board

// simulate ball
        {
            ball_p_x += ball_dp_x * dt;
            ball_p_y += ball_dp_y * dt;
            // ball speed

            ball_dp_x += speedIncrementFact * dt;


            if (ball_p_x + ballHalfSize > 80 - playerHalfSixeX &&
                ball_p_x - ballHalfSize < 80 + playerHalfSixeX &&
                ball_p_y + ballHalfSize > player_1_p - playerHalfSizeY &&
                ball_p_y - ballHalfSize < player_1_p + playerHalfSizeY) {
                ball_p_x = 80 - playerHalfSixeX - ballHalfSize;
                ball_dp_x *= -paddleAcceleration; // using this to speed up the game when the paddle hits the ball
                ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_Dp * 0.75f;
                // the speed + angle the ball will hit off the player
            } else if (ball_p_x + ballHalfSize > -80 - playerHalfSixeX &&
                       ball_p_x - ballHalfSize < -80 + playerHalfSixeX &&
                       ball_p_y + ballHalfSize > player_2_p - playerHalfSizeY &&
                       ball_p_y - ballHalfSize < player_2_p + playerHalfSizeY) {
                ball_p_x = -80 + playerHalfSixeX + ballHalfSize;
                ball_dp_x *= -paddleAcceleration;
                ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_Dp * 0.75f;
            } // if statement that focuses on the player and the ball hitting them

            if (ball_p_y + ballHalfSize > arenaHalfSizeY) {
                ball_p_y = arenaHalfSizeY - ballHalfSize;
                ball_dp_y *= -arenaAcceleration; // speeding up the game when the ball hits the edges
            } else if (ball_p_y - ballHalfSize < -arenaHalfSizeY) {
                ball_p_y = -arenaHalfSizeY + ballHalfSize;
                ball_dp_y *= -arenaAcceleration;
            } // if statement that makes the ball bounce off he walls.

            if (ball_p_x + ballHalfSize > arenaHalfSizex) {

                ball_dp_x *= -1;
                ball_dp_y = 0;
                ball_p_x = 0;
                ball_p_y = 0;
                playerScore1++;

                if (playerScore1 > 0) {
                    ball_dp_x = -130;
                    ball_dp_y = 0;
                    // reset the speed of the ball everytime someone scores
                }
                // right side
            } else if (ball_p_x - ballHalfSize < -arenaHalfSizex) {
                ball_dp_x *= -1;
                ball_dp_y = 0;
                ball_p_x = 0;
                ball_p_y = 0;
                playerScore2++;

                if (playerScore2 > 0) {
                    ball_dp_x = 130;
                    ball_dp_y = 0;
                }
                // left side
            } // if statement that sets the ball after scoring

            std::cout << ball_dp_y << std::endl;
            std::cout << ball_dp_x << std::endl;
            // can be used for debugging the speed of the ball
        }

        //score
        drawNumber(playerScore1, -10, 40, 1.f, 0x808080);
        drawNumber(playerScore2, 10, 40, 1.f, 0x808080);

        drawRect(ball_p_x, ball_p_y, ballHalfSize, ballHalfSize, 0xff0000);
        drawRect(80, player_1_p, playerHalfSixeX, playerHalfSizeY, 0xff00ff);
        drawRect(-80, player_2_p, playerHalfSixeX, playerHalfSizeY, 0xff00ff);
        // drawing the players

        if (pressed(BUTTON_ESC)) {
            currentGameMode = GM_MENU;
            playerScore1 = 0;
            playerScore2 = 0;
            // used to go back to the menu while in the game
        }

        // drawing of the ball and the players
    } else {
        if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
            hotButton = !hotButton;
            // used to pick the type of game
        }

        if (pressed(BUTTON_ENTER)) {
            currentGameMode = GM_GAMEPLAY;
            enemyIsAI = hotButton == 0;
            // swaps to gameplay
        }
        if (hotButton == 0) {
            drawText("SINGLE PLAYER", -82, -20, 1, 0xff0000);
            drawText("MULTI PLAYER", 10, -20, 1, 0xaaaaaa);
        } else {
            drawText("SINGLE PLAYER", -82, -20, 1, 0xaaaaaa);
            drawText("MULTI PLAYER", 10, -20, 1, 0xff0000);
        }

        drawText("PING PONG", -76, 35, 3, 0xFFA500);
        // drawing on the menu
    }
}

