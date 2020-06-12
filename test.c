#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "ref.h"

#define BIT(n) 			(1UL << (n))
#define SET_BIT(x, n)	((x) |= BIT((n)))
#define RESET_BIT(x, n) ((x) &= ~BIT(n))
#define IS_SET(x, n) 	((x) & BIT((n)))

int main(int argc, char* argv[])
{
	ParseBoardOut* ret;
	Move* moves, * t;
	if (argc != 2) return  -1;

	ret = ref_parse_board(argv[1]);
	if (ret == NULL) {
		printf("Ret is null.\n");
		return -1;
	}
	ref_display_board(ret);

	moves = ref_legal_moves(argv[1], ret->current_player);
	while (moves) {
		ref_print_move(moves);
		printf("\n");
		t = moves->next_move;
		free(moves);
		moves = t;
	}
	free(ret);

	if (ref_is_checkmate(argv[1], WHITE)) {
		printf("White is under checkmate\n");
	}
	if (ref_is_checkmate(argv[1], BLACK)) {
		printf("Black is under checkmate\n");
	}


	return 0;
}
