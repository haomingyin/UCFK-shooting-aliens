/** @file view.c
    @author: Haoming Y, Liam C
    @date: 3/10/2016
    @brief: A module takes an array of bit mask, uses tinygl module to light up
  			pixels to represent the game board.
 */

#include "system.h"
#include "tinygl.h"
#include "led.h"
#include "../../fonts/font3x5_1.h"
#include "view.h"

static int8_t *board;
static int8_t row, col;
static int8_t refresh_rate;
static bool led_state;


/** initilize tinygl to display
	@param refresh rate in HZ. */
void init_board(uint16_t rate, int8_t *board_address, int8_t x, int8_t y) {
    refresh_rate = rate;
    tinygl_init(refresh_rate);
    board = board_address;
    led_init();
    indicator_off();
    col = x;
    row = y;
}

/** iterates the bitmask array to set up pixels. */
void display_board(void) {
    int8_t pixel_value;
    int8_t x, y;
    y = row;
    // iterate each element in the array
    while (y--) {
        x = col;
        while (x--) {
            // to check if a pixel should be set 0 or 1
            pixel_value = ((*(board + y) & (1 << x)) != 0);
            tinygl_pixel_set(tinygl_point(x, y), pixel_value);
        }
    }
    tinygl_update();
}

/** a mssage can be passed in and display
	@param the string to be displayed. */
void display_message(char *msg) {
    tinygl_init(refresh_rate);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(TEXT_SCROLLING_SPEED);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(msg);
}

/** update the text to make it scrolling on the led matrix */
void display_message_update(void) {
    tinygl_update();
}

/** clear the content of the board (led matrix) */
void board_clear(void) {
    tinygl_clear();
}

/** turn on the blue led */
void indicator_on(void) {
    led_state = true;
    led_set(LED1, led_state);
}

/** flash the blue led */
void indicator_flash(void) {
    led_state = !led_state;
    led_set(LED1, led_state);
}

/** turn off the blue led */
void indicator_off(void) {
    led_state = false;
    led_set(LED1, led_state);
}
