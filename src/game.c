/** @file game.c
    @author: Haoming Y, Liam C
    @date: 3/10/2016
    @brief: the runner of the whole game, which specifies how the game works.
 */

#include "system.h"
#include "navswitch.h"
#include "controller.h"
#include "view.h"
#include "task.h"
#include "button.h"

#define BUTTON_RATE 200
#define DISPLAY_RATE 200
#define DISPLAY_MSG_RATE 4000
#define CONTROLLER_RATE 3
#define INDICATOR_RATE 2
#define GAME_INIT_TASK 200
#define RETRIEVE_RATE 600

/** capture the change of game status when a player presses s2 */
static void button_update_task(__unused__ void *data) {
    button_task();
}

/** refresh the led screen according the board bit mask array */
static void display_task(__unused__ void *data) {
    if (game_status == STATUS_PLAYING) {
        display_board();
    } else if (game_status == STATUS_SLEEP) {
        board_clear();
        display_message_update();
    }
}

/** showing scrolling message when game over */
static void display_msg_task(__unused__ void *data) {
    if (game_status == STATUS_IDLE) {
        display_message_update();
    }
}

/** retrieve incoming bullets from enemy */
static void retrieve_task(__unused__ void *data) {
    if (game_status == STATUS_PLAYING) {
        retrieve_data();
    }
}

/** use blue LED to show current game status */
static void indicator_task(__unused__ void *data) {
    switch (game_status) {
        case STATUS_IDLE:
            indicator_flash();
            break;
        case STATUS_PLAYING:
            indicator_on();
            break;
        case STATUS_SLEEP:
            indicator_off();
            break;
    };
}

/** move host bullets one row away and enemy bullets one row forward in every specific sec. */
static void board_task(__unused__ void *data) {
    if (game_status == STATUS_PLAYING) {
        step_board_ahead();
    }
}

/** initilizer the board according to current game status */
static void game_init(__unused__ void *data) {
    switch (game_status) {
        case STATUS_READY:
            board_clear();
            controller_init();
            game_status = STATUS_PLAYING;
            break;
        case STATUS_LOST:
            board_clear();
            display_message("LOST");
            game_status = STATUS_IDLE;
            break;
        case STATUS_WON:
            board_clear();
            display_message("WIN");
            game_status = STATUS_IDLE;
            break;
    };
}

/** the entry point of the game */
int main(void) {
    system_init();

    init_board(DISPLAY_RATE, board, BOARD_COL_SIZE, BOARD_ROW_SIZE);
    controller_init();

    game_status = STATUS_SLEEP;

    task_t tasks[] =
            {
                    {.func = button_update_task, .period = TASK_RATE / BUTTON_RATE},
                    {.func = display_task, .period = TASK_RATE / DISPLAY_RATE},
                    {.func = display_msg_task, .period = TASK_RATE / DISPLAY_MSG_RATE},
                    {.func = retrieve_task, .period = TASK_RATE / RETRIEVE_RATE},
                    {.func = indicator_task, .period = TASK_RATE / INDICATOR_RATE},
                    {.func = board_task, .period = TASK_RATE / CONTROLLER_RATE},
                    {.func = game_init, .period = TASK_RATE / GAME_INIT_TASK}
            };

    task_schedule(tasks, ARRAY_SIZE(tasks));
    return 1;
}
