#ifndef _IO_H_
#define _IO_H_

#include "chess.h"

void display_board();
Bool parse_board(char *board);

/* Parses the input move. 
 * This function repeatedly prompts for a move until a valid move is typed in. 
 * This function only validates that:
 *	 1) from and to are valid squares on the board.
 *	 2) from and to are separated by a '-' 
 * This function does not check the legality of the move.
 */ 
Move parse_move();

/* This function accepts a move from the user. The move is not validated */
void prompt_move(Move *m, unsigned int move_number);

/* This function prints a move on to the console */
void print_move(Move *m);

void clear_board();

char* move_to_string(Move* m);

#endif