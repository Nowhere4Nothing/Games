
#include "stimulateTheGame.h"
#include "Render.h"
#include <iostream>
// semantic compression.

f32 player_1_p, player_1_Dp, player_2_p, player_2_Dp;
f32 arenaHalfSizex = 85, arenaHalfSizeY = 45;
f32 playerHalfSixeX = 2.5, playerHalfSizeY = 12;
f32 ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ballHalfSize = 1;
f32 paddleAcceleration = 1.02f, arenaAcceleration = 1.01f;

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

GameMode currentGameMode;
s32 hotButton = 0;
bool enemyIsAI;
bool difficulty = true;
s32 hotbuttonDif = 0;

s32 count = 0;

void simulateGame(Input *input, f32 dt) {

    drawRect(0, 0, arenaHalfSizex, arenaHalfSizeY, GRASS_GREEN); // the border
    drawArenaBorder(arenaHalfSizex, arenaHalfSizeY, BLUE); // drawing the border
    // use this to stop overdraw
    count++;

    if ((pressed(BUTTON_ENTER)) && currentGameMode == GM_MENU) {
        if (hotButton) {
            currentGameMode = GM_GAMEPLAY;
            enemyIsAI = false;
        } else {
            currentGameMode = GM_DIFFICULTY;
        }
        // swaps to gameplay if multiplayer and the difficulty menu if single player
    } else if ((pressed(BUTTON_ENTER)) && currentGameMode == GM_DIFFICULTY) {
        if (hotbuttonDif) {
            currentGameMode = GM_GAMEPLAY;
            enemyIsAI = true;
            difficulty = true;
        } else {
            currentGameMode = GM_GAMEPLAY;
            enemyIsAI = true;
            difficulty = false;
        }

        // swaps to gameplay
    }

    if ((pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) && currentGameMode == GM_MENU) {
        hotButton = !hotButton;
        // used to pick the type of game
    }

    if ((pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) && currentGameMode == GM_DIFFICULTY) {
        hotbuttonDif = !hotbuttonDif;
        // used to pick the type of game
    }

    if (currentGameMode == GM_MENU) {
        drawText("PING PONG", -76, 35, 3, ORANGE);
        if (!hotButton) {
            drawText("SINGLE PLAYER", -82, -20, 1, WHITE);
            drawText("MULTI PLAYER", 10, -20, 1, RED);
        } else {
            drawText("SINGLE PLAYER", -82, -20, 1, RED);
            drawText("MULTI PLAYER", 10, -20, 1, WHITE);
        }
    } else if (currentGameMode == GM_DIFFICULTY) {
        drawText("PICK DIFFICULTY", -76, 35, 1.7, ORANGE);

        if (!hotbuttonDif) {
            drawText("EASY", -82, -20, 1, WHITE);
            drawText("HARD", 10, -20, 1, RED);
        } else {
            drawText("EASY", -82, -20, 1, RED);
            drawText("HARD", 10, -20, 1, WHITE);
        }
    } else if (currentGameMode == GM_GAMEPLAY) {

        // stimulate player
        {
            f32 player_1_Ddp = 0.f; // units per second
            if (is_down(BUTTON_Up)) player_1_Ddp += 2000;
            if (is_down(BUTTON_DOWN)) player_1_Ddp -= 2000;
            // speed, velocity and buttons for the player

            f32 player_2_Ddp = 0.f; // units per second
            if ((!enemyIsAI) && difficulty) {
                if (is_down(BUTTON_W)) player_2_Ddp += 2000;
                if (is_down(BUTTON_S)) player_2_Ddp -= 2000;
                // the code for the player controls
            } else if ((enemyIsAI) && difficulty) {
                {
                    // this is the hard difficulty
                    f32 speedIncrementFact = 30.0f;

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

                    if (ball_dp_x <= 600) {
                        ball_dp_x += speedIncrementFact * dt;
                    } else {
                        ball_dp_x = 600;
                    }
//                    std::cout << ball_dp_y << std::endl;
//                    std::cout << ball_dp_x << std::endl;
                    // can be used for debugging the speed of the ball
                }
            } else if ((enemyIsAI) && !difficulty) {
                {
                    // this is the easy difficulty
                    f32 speedIncrementFact = 10.0f;
// Adjusting speed and acceleration
                    player_2_Ddp = (ball_p_y - player_2_p) * 40;  // Reduce scaling factor

                    if (player_2_Ddp > 1000) player_2_Ddp += 100;
                    if (player_2_Ddp < -1000) player_2_Ddp += -100;

                    if (ball_p_y > player_2_Ddp) player_2_Ddp += SPEED_SCALING_FACTOR * ball_dp_y;
                    if (ball_p_x < player_2_Ddp) player_2_Ddp -= SPEED_SCALING_FACTOR * ball_dp_x;
                    // hopefully it speeds the paddles up if teh ball velocity is going faster

                    f32 paddleTopY = player_2_p + playerHalfSizeY;
                    if (ball_p_y > paddleTopY) {
                        player_2_Ddp += SPEED_SCALING_FACTOR * (ball_p_y - paddleTopY);
                    } else if (ball_p_y < player_2_p - playerHalfSizeY) {
                        player_2_Ddp -= SPEED_SCALING_FACTOR * (player_2_p - playerHalfSizeY - ball_p_y);
                    }

                    // Introduce randomness
//                player_2_Ddp += getRandomValueInRange(-50, 50);
//
//                // Reaction delay
//                waitRandomTime(0, 500);  // Add a function to introduce a delay
//
//                // Limit movement range
                    player_2_Ddp = clamp(player_2_Ddp, -1000.f, 1000.f);

                    // AI initiation of ball movement with reduced impact
                    player_2_Dp -= 0.5f * ball_p_x / arenaHalfSizex;  // Reduce impact

                    if (ball_dp_x <= 200) {
                        ball_dp_x += speedIncrementFact * dt;
                    } else {
                        ball_dp_x = 200;
                    }
                }
                std::cout << "Its easy mode" << std::endl;
//                std::cout << ball_dp_y << std::endl;
//                std::cout << ball_dp_x << std::endl;
//                // can be used for debugging the speed of the ball
            }
            // AI code for the left hand player, changing the speed and velocity of the AI

            simulatePlayer(&player_1_p, &player_1_Dp, player_1_Ddp, dt);
            simulatePlayer(&player_2_p, &player_2_Dp, player_2_Ddp, dt);
            // method that gives the play movement and keeps them on the board

        }
        // simulate ball
        {
            ball_p_x += ball_dp_x * dt;
            ball_p_y += ball_dp_y * dt;
            // ball speed

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
                ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_Dp * 0.55f;
            } // if statement that focuses on the player and the ball hitting them and rebounding

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


        }




        //score
        drawNumber(playerScore1, -10, 40, 1.f, GRAY);
        drawNumber(playerScore2, 10, 40, 1.f, GRAY);

        drawRect(ball_p_x, ball_p_y, ballHalfSize, ballHalfSize, WHITE);
        drawRect(80, player_1_p, playerHalfSixeX, playerHalfSizeY, LAVENDER);
        drawRect(-80, player_2_p, playerHalfSixeX, playerHalfSizeY, PINK);
        // drawing the players

        if (pressed(BUTTON_ESC)) {
            currentGameMode = GM_MENU;
            playerScore1 = 0;
            playerScore2 = 0;
            // used to go back to the menu while in the game
        }



    }
}