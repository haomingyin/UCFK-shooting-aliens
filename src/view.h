/** @file view.h
    @author: Haoming Y, Liam C
    @date: 3/10/2016
    @brief: A module takes an array of bit mask, uses tinygl module to light up
  			pixels to represent the game board.
 */

#ifndef VIEW_H
#define VIEW_H

#define TEXT_SCROLLING_SPEED 100

#include "system.h"
#include "../../fonts/font3x5_1.h"

/** initilize tinygl to display 
	@param refresh rate in HZ.
    @param pointer of the board bit mask array
    @param the number of columns in the above array
    @param the number of rows in the above array*/
void init_board(uint16_t rate, int8_t *board_address, int8_t x, int8_t y);

/** iterates the bitmask array to set up pixels.  */
void display_board(void);

/** a mssage can be passed in and display
	@param the string to be displayed. */
void display_message(char *msg);

/** update the text to make it scrolling on the led matrix */
void display_message_update(void);

/** clear the content of the board (led matrix) */
void board_clear(void);

/** turn on the blue led */
void indicator_on(void);

/** flash the blue led */
void indicator_flash(void);

/** turn off the blue led */
void indicator_off(void);

#endif
