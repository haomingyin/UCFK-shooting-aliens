# UCFK Shooting Aliens

The project was created for ence260 course assignment. To play the game, you need to own an UCFunKit and load the program on it. Two players can connect each other by the infrared module. By controlling the nav buttons on UCFK, users can shoot or dodge the bullets.

Mady by: Haoming Yin, Liam Crum
Date: 13 Oct. 2016

## Structure

### Game is made up of following files

|-game.c

|-controller.c

|-controller.h

|-view.c

|-view.h

|-Makefile

### Game logic dependencies

    |-game

        |-controller

            |-ir_uart

            |-button

            |-navswitch

        |-view

            |-led

            |-tinygl

            |-font3x5_1

## Instructions:
1. Run command 'make program' within terminal to load the game on UCFunKit;
2. Press button(s3) to start the game;
3. Move ship by moving navswitch left or right. To shoot, press navswitch;
4. Press button(s3) will terminate the game (the first palyer terminated the game lose);
5. If bullets hit player's ship, the player lose the game. Also, game terminates.
6. Press button(s3) when prompt message showing up will turn UCFunKit to sleep, press it again to restart the game.
