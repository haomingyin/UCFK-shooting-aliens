/** @file controller.c
 * 	@author Haoming Y, Liam C
 * 	@date 4/10/2016
 * 	@brief the controller of the shooting game, by controlling the bitmask array of the led matrix
 */

#include "system.h"
#include "navswitch.h"
#include "button.h"
#include "ir_uart.h"
#include "controller.h"

// the bullets bit mask from enemy
static int8_t enemy_board[BOARD_ROW_SIZE];
// the bullets bit mask from us
static int8_t host_board[BOARD_ROW_SIZE];
// current host gun position, range from 0 to 4
static int8_t ship_position;

// a bullet row needed to send to enemy, the first five bits show the bullet position,
// the 6th and 7th bit is used for the game status
static int8_t bullet_to_send;
// a bullet row received from enemy
static int8_t bullet_received;


/** initilize the controller */
void controller_init(void) {
    int i;
    for (i = 0; i < BOARD_ROW_SIZE; i++) {
        enemy_board[i] = 0;
        host_board[i] = 0;
        board[i] = 0;
    }
    ship_position = INIT_HOST_POSITION;
    bullet_received = 0;
    bullet_to_send = 0;

    navswitch_init();
    button_init();
    ir_uart_init();
}

/** to check if enemy's bullets hit our ship, if so we lost */
static void check_status(void) {
    if ((enemy_board[BOARD_ROW_SIZE - 1] & (1 << ship_position)) != 0) {
        controller_init();
        game_status = STATUS_LOST;
        bullet_to_send = (STATUS_LOST << 5);
    }
}

/** instead of update the whole board, this function should be called when only
	our ship location changes */
static void update_ship(void) {
    board[BOARD_ROW_SIZE - 1] =
            (enemy_board[BOARD_ROW_SIZE - 1] | (1 << ship_position));
}

/** merge host and enemy board into board */
static void update_board(void) {
    int8_t temp = 0;
    int i;
    // host bullets can cancel incoming bullets sent from enemy
    for (i = 0; i < BOARD_ROW_SIZE; i++) {
        temp = host_board[i] & enemy_board[i];
        host_board[i] ^= temp;
        enemy_board[i] ^= temp;
        board[i] = host_board[i] | enemy_board[i];
    }
    update_ship();
    check_status();
}

/** retrieve data from UART */
void retrieve_data(void) {
    int8_t temp = 0;

    if (ir_uart_read_ready_p()) {

        temp = ir_uart_getc();
        if (temp >> 5 == STATUS_PLAYING) {
            bullet_received = temp;
        }

        if (temp >> 5 == STATUS_LOST) {
            game_status = STATUS_WON;
        }
    }
}

/** send out data via UART */
static void transmit_data(void) {
    if (game_status == STATUS_PLAYING) {
        ir_uart_putc(bullet_to_send);
    } else if (game_status == STATUS_LOST) {
        // send a message to tell enemy they have won
        ir_uart_putc(STATUS_LOST << 5);
    }
}

/** move host bullets one row away, and enemy bullets one row close */
void step_board_ahead(void) {
    // shift enemy board bullet one row forward
    int i;
    for (i = 6; i > 0; i--) {
        enemy_board[i] = enemy_board[i - 1];
    }
    enemy_board[0] = bullet_received;

    update_board();

    // shift host board bullet one row away
    // bullet_to_send contains bullets' position and game_status as a parity/checksum
    bullet_to_send = host_board[0] | (game_status << 5);
    for (i = 1; i < BOARD_ROW_SIZE; i++) {
        host_board[i - 1] = host_board[i];
    }
    host_board[BOARD_ROW_SIZE - 1] = 0;

    update_board();
    transmit_data();
}

/** shoot a bullet according to the our ship current position */
static void shoot_bullet(void) {
    host_board[BOARD_ROW_SIZE - 2] |= (1 << ship_position);
    update_board();
}

/** everytime the S2 has been pressed, the game status should be changed according to this function */
void button_task(void) {
    if (game_status == STATUS_PLAYING) {

        navswitch_update();

        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            shoot_bullet();
        }

        if (navswitch_push_event_p(NAVSWITCH_WEST)) {
            ship_position = (ship_position + BOARD_COL_SIZE - 1) % BOARD_COL_SIZE;
        }

        if (navswitch_push_event_p(NAVSWITCH_EAST)) {
            ship_position = (ship_position + BOARD_COL_SIZE + 1) % BOARD_COL_SIZE;
        }
    }

    button_update();

    if (button_push_event_p(BUTTON1)) {
        if (game_status == STATUS_SLEEP) {
            game_status = STATUS_READY;
        } else if (game_status == STATUS_PLAYING) {
            game_status = STATUS_LOST;
        } else {
            game_status = STATUS_SLEEP;
        }
    }

    update_ship();
}
