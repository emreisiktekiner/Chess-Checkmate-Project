#include <string.h>
#include "io.h"
#include "moves.h"
#include <stdlib.h>

/*
 * Implement each function in this file.
 */
 
char* pieceNames[]={"UNKNOWN","KING", "QUEEN", "ROOK", "BISHOP", "NIGHT", "PAWN"};
char pieceChars[]={' ','K', 'Q', 'R', 'B', 'N', 'P'};

extern PlayerColor CurrentPlayer; /*chess.h'da extern deklarasyonu var*/

extern Pos ep_square;

char get_piece_char(Pos pos)
{
	if (IS_SET(player[WHITE].r, pos)) return 'R';
	if (IS_SET(player[WHITE].b, pos)) return 'B';
	if (IS_SET(player[WHITE].q, pos)) return 'Q';
	if (IS_SET(player[WHITE].k, pos)) return 'K';
	if (IS_SET(player[WHITE].p, pos)) return 'P';
	if (IS_SET(player[WHITE].n, pos)) return 'N';

	if (IS_SET(player[BLACK].r, pos)) return 'r';
	if (IS_SET(player[BLACK].b, pos)) return 'b';
	if (IS_SET(player[BLACK].q, pos)) return 'q';
	if (IS_SET(player[BLACK].k, pos)) return 'k';
	if (IS_SET(player[BLACK].p, pos)) return 'p';
	if (IS_SET(player[BLACK].n, pos)) return 'n';
	return ' ';
}

void display_board()
{
	unsigned int i = 0, j = 0;
	printf("\n ---------------------------------\n");
	for (i = 0; i < 8; i++)
	{
		printf("%d |", 8 - i);
		for (j = 0; j < 8; j++)
		{
			printf(" %c |", get_piece_char(i * 8 + j));
		}
		printf("\n ---------------------------------\n");
	}
	printf("    a   b   c   d   e   f   g   h\n");
	printf("\n");
	printf("Current move : %s \n", CurrentPlayer == WHITE ? "White" : "Black");
}


Bool parse_board(char *board) { 
	int i, j;
	for (i = 0, j=0; board[i] != '\0'; i++, j++)
	{
		if (board[i] == '/')
		{
			j--;
			continue;
		}
		if ('1' <= board[i] && board[i] <= '8')
		{
			j += board[i] - '0' -1; /*-1 continue'yu ekarte etmek icin*/
			continue;
		}

		if (board[i] == 'R') SET_BIT(player[WHITE].r, j);
		if (board[i] == 'B') SET_BIT(player[WHITE].b, j);
		if (board[i] == 'Q') SET_BIT(player[WHITE].q, j);
		if (board[i] == 'K') SET_BIT(player[WHITE].k, j);
		if (board[i] == 'P') SET_BIT(player[WHITE].p, j);
		if (board[i] == 'N') SET_BIT(player[WHITE].n, j);

		if (board[i] == 'r') SET_BIT(player[BLACK].r, j);
		if (board[i] == 'b') SET_BIT(player[BLACK].b, j);
		if (board[i] == 'q') SET_BIT(player[BLACK].q, j);
		if (board[i] == 'k') SET_BIT(player[BLACK].k, j);
		if (board[i] == 'p') SET_BIT(player[BLACK].p, j);
		if (board[i] == 'n') SET_BIT(player[BLACK].n, j);

		if (board[i] == ' ') break;
	}

	i++;
	if (board[i] == 'w') CurrentPlayer = WHITE;
	else if (board[i] == 'b') CurrentPlayer = BLACK;
	else
	{
		printf("expected current player, got %c\n", board[i]);
		return 0;
	}
	player[BLACK].castle_flags = 0;
	player[WHITE].castle_flags = 0;

	for (i++; board[i] != '\0'; i++) /*flag degisimleri*/
	{
		if (board[i] == 'k') SET_BIT(player[BLACK].castle_flags, 0);
		if (board[i] == 'q') SET_BIT(player[BLACK].castle_flags, 1);
		if (board[i] == 'K') SET_BIT(player[WHITE].castle_flags, 0);
		if (board[i] == 'Q') SET_BIT(player[WHITE].castle_flags, 1);

	}
	return 1;
}

/* Parses the input move. 
 * This function repeatedly prompts for a move until a valid move is typed in. 
 * This function only validates that:
 *	 1) from and to are valid squares on the board.
 *	 2) from and to are separated by a '-' 
 * This function does not check the legality of the move.
 */ 
Move parse_move()
{
	char s[128];
	unsigned int error;
	
	Move* mov = (Move*)malloc(sizeof(Move));
	unsigned int pCount;

	legal_moves(&mov, CurrentPlayer, &pCount);
	legal_moves_cleaner(&mov, CurrentPlayer, &pCount, ep_square);
	Move* movHead = mov;


	Move m = {INVALID_INDEX, INVALID_INDEX};
	error=0;
	
	do {
		if (error==1) printf("Try again.\n");
		
		error=0;
		
		printf("Enter Move: ");
		
		fgets(s, 127, stdin);
		
		s[5] = '\0';
		
		printf("s[0] = %c, s[1] = %c, s[2] = %c , s[3] = %c , s[4] = %c \n"
		,s[0],s[1],s[2],s[3],s[4]);
		
		m.from = TO_POS(s[0], s[1]);
		if (m.from==INVALID_INDEX) 
		{
			printf("Invalid from. \n");
			error=1;
			continue;
		}
		
		if (s[2] != '-')
		{
			printf("A '-' expected after from square \n");
			error =1;
			continue;
		}
		
		m.to = TO_POS(s[3], s[4]);
		
		if (m.to == INVALID_INDEX)
		{
			printf("Invalid to.\n");
			error=1;
			continue;
		}

		error = 1;
		mov = movHead;
		while (mov != 0)
		{
			if (mov->to == m.to && mov->from == m.from)
			{
				error = 0;
				m.piece = get_piece_at(m.from,CurrentPlayer);
			}
			mov = mov->next_move;
		}
		if (error) printf("Move illegal!");
	} while (error==1);
	free(movHead);
	return m;
	
}

/* This function accepts a move from the user. The move is not validated */
void prompt_move(Move *m, unsigned int move_number) /*might not be required to be implemented*/
{
for (;;)
	{
		Move mObj=parse_move(); /*yukaridaki metod*/
		m=&mObj;
		if (-1)/*eger hareket yapilabiliyorsa  if (make_move(m) != 1) make move burada yok,  (DEGISTIRILECEK)*/
		{
			printf("Invalid move . Try again!\n");
		}
		else 
		{
			display_board();
		}
	}
}

void clear_board()
{
	player[BLACK].r = 0;
	player[BLACK].b = 0;
	player[BLACK].q = 0;
	player[BLACK].k = 0;
	player[BLACK].p = 0;
	player[BLACK].n = 0;

	player[WHITE].r = 0;
	player[WHITE].b = 0;
	player[WHITE].q = 0;
	player[WHITE].k = 0;
	player[WHITE].p = 0;
	player[WHITE].n = 0;

	player[BLACK].castle_flags = 0;
	player[WHITE].castle_flags = 0;

	ep_square = -1;
}

/* This function prints a move on to the console */
/*dont know how but I somehow broke this for main (dont use at main)*/
void print_move(Move* m) {
	printf("%s %c%c-%c%c %s\n",
		pieceNames[m->piece],
		FILE_OF(m->from),
		RANK_OF(m->from),
		FILE_OF(m->to),
		RANK_OF(m->to),
		m->promotion_choice ? pieceNames[m->promotion_choice] : ""
	);
}

char* move_to_string(Move* m) {
	if (m->from!=0xff)
	{
		char* retStr = (char*)malloc(sizeof(char) * 6);
		retStr[0] = FILE_OF(m->from);
		retStr[1] = RANK_OF(m->from);
		retStr[2] = '-';
		retStr[3] = FILE_OF(m->to);
		retStr[4] = RANK_OF(m->to);
		retStr[5] = '\n';
		return retStr;
	}
	else
	{
		char* retStr = (char*)malloc(sizeof(char) * 12);
		strcpy(retStr,"not found\n");
		return retStr;
	}
}



