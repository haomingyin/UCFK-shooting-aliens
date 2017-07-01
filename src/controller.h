/** @file controller.h
 * 	@author Haoming Y, Liam C
 * 	@date 4/10/2016
 * 	@brief control the logic part of the game
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "system.h"
#include "controller.h"

#define INIT_HOST_POSITION 2
#define BOARD_COL_SIZE 5
#define BOARD_ROW_SIZE 7

/** enum of different game status */
enum {
    STATUS_SLEEP, STATUS_LOST, STATUS_READY, STATUS_PLAYING, STATUS_WON, STATUS_IDLE
};

// bit mask contains bullets from both host and enemy and host position
int8_t board[BOARD_ROW_SIZE];
// the value should be used from the above enum to show current game status
int8_t game_status;


/** initilize the controller */
void controller_init(void);

/** move host bullets one row away, and enemy bullets one row close */
void step_board_ahead(void);

/** retrieve data from UART */
void retrieve_data(void);

/** read navswitch button and excute command to control the game. */
void button_task(void);

#endif
