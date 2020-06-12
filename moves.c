#include "moves.h"
#include <stdlib.h>

/* Given a color, this function returns a singly linked list of all legal Moves with the head at *m. 
 * The function returns TRUE if at least 1 legal move is available. 
 * The total number of moves found is stored in the address pointed to by pcount. */

extern Pos ep_square; 
extern PlayerColor CurrentPlayer;

extern PlayerColor solvingFor;

void clear_list(Move* move)
{
	Move* next;
	while (move != 0)
	{
		next = move->next_move;
		free(move);
		move = next;
	}
}

Bool no_moves();
Bool is_check(PlayerColor c);
Bool isControllingCheck = 0;

PlayerState player_memory[2];
Pos ep_square_mem;
void saveState()
{
	player_memory[BLACK] = player[BLACK];
	player_memory[WHITE] = player[WHITE];
	ep_square_mem = ep_square;
}

void restoreState()
{
	player[BLACK] = player_memory[BLACK];
	player[WHITE] = player_memory[WHITE];
	ep_square = ep_square_mem;
}

PlayerState player_memory_castle[2];
Pos ep_square_mem_castle;
void saveState_castle()
{
	player_memory_castle[BLACK] = player[BLACK];
	player_memory_castle[WHITE] = player[WHITE];
	ep_square_mem_castle = ep_square;
}

void restoreState_castle()
{
	player[BLACK] = player_memory_castle[BLACK];
	player[WHITE] = player_memory_castle[WHITE];
	ep_square = ep_square_mem_castle;
}

int clockwise_of(Pos sq, int angle) /*chess.h'daki makrolari kullanimini kolaylastirmak icin*/
{
	switch (angle)
	{
	case 0: return NORTH_OF(sq);
	case 1: return NE_OF(sq);
	case 2: return EAST_OF(sq);
	case 3: return SE_OF(sq);
	case 4: return SOUTH_OF(sq);
	case 5: return SW_OF(sq);
	case 6: return WEST_OF(sq);
	case 7: return NW_OF(sq);
	default: return UNKNOWN_POS;
	}
}

Bool legal_moves(Move **m, PlayerColor c, unsigned int *pcount) {
	int i,j,k;
	*pcount = 0;
	Move* decoyMove = (Move*)malloc(sizeof(Move)); /*en sonda cikartilacak*/
	Move* curr = decoyMove;
	decoyMove->next_move = 0;
	*m = decoyMove;
	for (i=0;i<64;i++) /*tahtadaki her kare icin*/
	{
		if (IS_SET(player[c].k, i)) /*sah icin*/
		{
			for (j = 0; j < 8; j++)
			{
				if (!OCCUPIED_COLOR(c, clockwise_of(i, j)) && clockwise_of(i, j)!=-1) /*clockwise etraftaki tum 8 kareyi kontrol ediyor, i tas, j rotasyon*/
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = clockwise_of(i, j);
					curr->piece = KING;
					(*pcount)++;
				}
			}
		}

		if (IS_SET(player[c].p, i)) /*piyon icin*/
		{
			if (c == WHITE)
			{
				if (RANK_OF(i) == '2' && UNOCCUPIED(NORTH_OF(i)) && UNOCCUPIED(NORTH_OF(NORTH_OF(i))))
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = NORTH_OF(NORTH_OF(i));
					curr->piece = PAWN;
					curr->promotion_choice = 0;
					(*pcount)++;
				}
				if (UNOCCUPIED(NORTH_OF(i)) && RANK_OF(NORTH_OF(i)) != '8')
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = NORTH_OF(i);
					curr->piece = PAWN;
					curr->promotion_choice = 0;
					(*pcount)++;
				}
				else if (UNOCCUPIED(NORTH_OF(i)) && RANK_OF(NORTH_OF(i)) == '8') /*terfi*/
				{
					for (k = 2; k <= 5; k++)
					{
						curr->next_move = (Move*)malloc(sizeof(Move));
						curr = curr->next_move;
						curr->next_move = 0;
						curr->from = i;
						curr->to = NORTH_OF(i);
						curr->piece = PAWN;
						curr->promotion_choice = k; /*tasin enum degeri*/
						(*pcount)++;
					}
				}

				if ((OCCUPIED_COLOR(1-c,NW_OF(i)) && RANK_OF(NW_OF(i)) != '8')
					&& NW_OF(i) !=-1)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = NW_OF(i);
					curr->piece = PAWN;
					curr->promotion_choice = 0;
					(*pcount)++;
				}
				else if ((OCCUPIED_COLOR(1 - c, NW_OF(i)) && RANK_OF(NW_OF(i)) == '8')
					&& NW_OF(i) != -1) /*terfi*/
				{
					for (k = 2; k <= 5; k++)
					{
						curr->next_move = (Move*)malloc(sizeof(Move));
						curr = curr->next_move;
						curr->next_move = 0;
						curr->from = i;
						curr->to = NW_OF(i);
						curr->piece = PAWN;
						curr->promotion_choice = k; /*tasin enum degeri*/
						(*pcount)++;
					}
				}

				if ((OCCUPIED_COLOR(1 - c, NE_OF(i)) && RANK_OF(NE_OF(i)) != '8')
					&& NE_OF(i) != -1)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = NE_OF(i);
					curr->piece = PAWN;
					curr->promotion_choice = 0;
					(*pcount)++;
				}
				else if ((OCCUPIED_COLOR(1 - c, NE_OF(i)) && RANK_OF(NE_OF(i)) == '8')
					&& NE_OF(i) != -1) /*terfi*/
				{
					for (k = 2; k <= 5; k++)
					{
						curr->next_move = (Move*)malloc(sizeof(Move));
						curr = curr->next_move;
						curr->next_move = 0;
						curr->from = i;
						curr->to = NE_OF(i);
						curr->piece = PAWN;
						curr->promotion_choice = k; /*tasin enum degeri*/
						(*pcount)++;
					}
				}
				if (NE_OF(i) == ep_square)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = NE_OF(i);
					curr->piece = PAWN;
					(*pcount)++;
				}
				if (NW_OF(i) == ep_square)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = NW_OF(i);
					curr->piece = PAWN;
					(*pcount)++;
				}
			}
			else
			{
				if (RANK_OF(i) == '7' && UNOCCUPIED(SOUTH_OF(i)) && UNOCCUPIED(SOUTH_OF(SOUTH_OF(i))))
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = SOUTH_OF(SOUTH_OF(i));
					curr->piece = PAWN;
					curr->promotion_choice = 0;
					(*pcount)++;
				}
				if (UNOCCUPIED(SOUTH_OF(i)) && RANK_OF(SOUTH_OF(i)) != '1')
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = SOUTH_OF(i);
					curr->piece = PAWN;
					curr->promotion_choice = 0;
					(*pcount)++;
				}
				else if (UNOCCUPIED(SOUTH_OF(i)) && RANK_OF(SOUTH_OF(i)) == '1') /*terfi*/
				{
					for (k = 2; k <= 5; k++)
					{
						curr->next_move = (Move*)malloc(sizeof(Move));
						curr = curr->next_move;
						curr->next_move = 0;
						curr->from = i;
						curr->to = SOUTH_OF(i);
						curr->piece = PAWN;
						curr->promotion_choice = k; /*tasin enum degeri*/
						(*pcount)++;
					}
				}

				if ((OCCUPIED_COLOR(1 - c, SW_OF(i)) && RANK_OF(SW_OF(i)) != '1')
					&& SW_OF(i) != -1)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = SW_OF(i);
					curr->piece = PAWN;
					curr->promotion_choice = 0;
					(*pcount)++;
				}
				else if ((OCCUPIED_COLOR(1 - c, SW_OF(i)) && RANK_OF(SW_OF(i)) == '1')
					&& SW_OF(i) != -1) /*terfi*/
				{
				for (k = 2; k <= 5; k++)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = SW_OF(i);
					curr->piece = PAWN;
					curr->promotion_choice = k; /*tasin enum degeri*/
					(*pcount)++;
				}
				}

				if ((OCCUPIED_COLOR(1 - c, SE_OF(i)) && RANK_OF(SE_OF(i)) != '1')
					&& SE_OF(i) != -1)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = SE_OF(i);
					curr->piece = PAWN;
					curr->promotion_choice = 0;
					(*pcount)++;
				}
				else if ((OCCUPIED_COLOR(1 - c, SE_OF(i)) && RANK_OF(SE_OF(i)) == '1')
					&& SE_OF(i) != -1) /*terfi*/
				{
					for (k = 2; k <= 5; k++)
					{
						curr->next_move = (Move*)malloc(sizeof(Move));
						curr = curr->next_move;
						curr->next_move = 0;
						curr->from = i;
						curr->to = SE_OF(i);
						curr->piece = PAWN;
						curr->promotion_choice = k; /*tasin enum degeri*/
						(*pcount)++;
					}
				}
				if (SE_OF(i) == ep_square)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = SE_OF(i);
					curr->piece = PAWN;
					(*pcount)++;
				}
				if (SW_OF(i) == ep_square)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = SW_OF(i);
					curr->piece = PAWN;
					(*pcount)++;
				}
			}
		}
		if (IS_SET(player[c].r, i)) /*kale icin*/
		{
			for (j = 0; j < 8; j += 2)
			{
				k = i;
				while ((UNOCCUPIED(clockwise_of(k, j)) || OCCUPIED_COLOR(1 - c, clockwise_of(k, j)))
					&& clockwise_of(k, j) != -1)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = clockwise_of(k, j);
					curr->piece = ROOK;
					(*pcount)++;

					k = clockwise_of(k, j);
					if (OCCUPIED_COLOR(1-c,k)) break;
				}
			}
		}

		if (IS_SET(player[c].b, i)) /*fil icin (kale ile birlestirilebilir ama gerek yok)*/
		{
			for (j = 1; j < 8; j += 2)
			{
				k = i;
				while ((UNOCCUPIED(clockwise_of(k, j)) || OCCUPIED_COLOR(1 - c, clockwise_of(k, j)))
					&& clockwise_of(k, j) != -1)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = clockwise_of(k, j);
					curr->piece = BISHOP;
					(*pcount)++;

					k = clockwise_of(k, j);
					if (OCCUPIED_COLOR(1-c,k)) break;
				}
			}
		}

		if (IS_SET(player[c].q, i)) /*vezir icin (kale ile birlestirilebilir ama gerek yok)*/
		{
			for (j = 0; j < 8; j++)
			{
				k = i;
				while ((UNOCCUPIED(clockwise_of(k, j)) || OCCUPIED_COLOR(1 - c, clockwise_of(k, j)))
					&& clockwise_of(k, j) != -1)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = clockwise_of(k, j);
					curr->piece = QUEEN;
					(*pcount)++;

					k = clockwise_of(k, j);
					if (OCCUPIED_COLOR(1-c,k)) break;
				}
			}
		}

		if (IS_SET(player[c].n, i)) /*at icin*/
		{
			for (j = 0; j < 8; j++)
			{
				k = i;
				if ((UNOCCUPIED(clockwise_of(clockwise_of(k, j), (j+1<8? j+1: 0))) ||
					OCCUPIED_COLOR(1 - c, clockwise_of(clockwise_of(k, j), (j +1 < 8 ? j + 1 : 0))))
					&& clockwise_of(k, j) != -1)
				{
					curr->next_move = (Move*)malloc(sizeof(Move));
					curr = curr->next_move;
					curr->next_move = 0;
					curr->from = i;
					curr->to = clockwise_of(clockwise_of(k, j), (j +1 < 8 ? j + 1 : 0));
					curr->piece = NIGHT;
					(*pcount)++;
				}
			}
		}
	}
	if (IS_SET(player[c].castle_flags, 1)) /*castling*/
	{
		if (UNOCCUPIED(1 + (c == BLACK ? 0 : 56)) &&
			UNOCCUPIED(2 + (c == BLACK ? 0 : 56)) &&
			UNOCCUPIED(3 + (c == BLACK ? 0 : 56)))
		{
			curr->next_move = (Move*)malloc(sizeof(Move));
			curr = curr->next_move;
			curr->next_move = 0;
			curr->from = c == WHITE ? WKING_START_POS : BKING_START_POS;
			curr->to = WEST_OF(WEST_OF(curr->from));
			curr->piece = KING;
			(*pcount)++;
		}
	}
	if (IS_SET(player[c].castle_flags, 0))
	{
		if (UNOCCUPIED(5 + (c == BLACK ? 0 : 56)) &&
			UNOCCUPIED(6 + (c == BLACK ? 0 : 56)))
		{
			curr->next_move = (Move*)malloc(sizeof(Move));
			curr = curr->next_move;
			curr->next_move = 0;
			curr->from = c == WHITE ? WKING_START_POS : BKING_START_POS;
			curr->to = EAST_OF(EAST_OF(curr->from));
			curr->piece = KING;
			(*pcount)++;
		}
	}


	*m = decoyMove->next_move; /*ilk eleman cikarilis*/
	free(decoyMove);
	return 1;
}

void legal_moves_cleaner(Move** legal,PlayerColor c, unsigned int* pcount,Pos ep_sq)
{
	char** msg=0x0;
	Move* prev=(Move*)malloc(sizeof(Move));
	Move* origPrev = prev;
	prev->next_move = legal[0];
	Move* curr = legal[0];
	Move* head = prev;
	Move temp = *prev;
	Move castleMove; /*to check the piece king travels through when castling*/
	while (curr != 0)
	{
		saveState();
		if (is_check(c) && detect_castle_move(*curr, c)) /*cant castle while checked*/
		{
			prev->next_move = curr->next_move;
			temp = *curr;
			free(curr);
			(*pcount)--;
			curr = temp.next_move;
			restoreState();
			continue;
		}
		if (RANK_OF(curr->to) > '8' ||
			RANK_OF(curr->to) < '1' ||
			FILE_OF(curr->to) > 'h' ||
			FILE_OF(curr->to) < 'a') /*night problem patch*/
		{
			prev->next_move = curr->next_move;
			temp = *curr;
			free(curr);
			(*pcount)--;
			curr = temp.next_move;
			restoreState();
			continue;
		}
		if (curr->promotion_choice && is_check(c)) /*promotion problem patch*/
		{
			saveState_castle();
			validate_and_move(curr, msg, c, &ep_square);
			if (is_check(c))
			{
				prev->next_move = curr->next_move;
				temp = *curr;
				free(curr);
				(*pcount)--;
				curr = temp.next_move;
				restoreState();
				continue;
			}
			restoreState_castle();
		}
		if (detect_castle_move(*curr, c))
		{
			saveState_castle();
			castleMove.to = (curr->to + curr->from) / 2; /*the square inbetween*/
			castleMove.from = curr->from;
			castleMove.piece = KING;
			validate_and_move(&castleMove, msg, c, &ep_square);
			if (is_check(c))
			{
				prev->next_move = curr->next_move;
				temp = *curr;
				free(curr);
				(*pcount)--;
				curr = temp.next_move;
				restoreState();
				continue;
			}
			restoreState_castle();
		}
		validate_and_move(curr, msg, c, &ep_square);
		if (is_check(c))
		{
			prev->next_move = curr->next_move;
			temp = *curr;
			free(curr);
			(*pcount)--;
			curr = temp.next_move;
		}
		else
		{
			prev = curr;
			curr = curr->next_move;
		}
		restoreState();
	}
	if (pcount == 0) (*legal) = 0;
	else (*legal) = head->next_move;
	ep_square = -1;
	free(origPrev);
}

Bool is_check(PlayerColor c) {
	if (player[c].k==0) return FALSE; /*if no KING - bugged*/
	if (!isControllingCheck)
	{
		isControllingCheck = 1;
		Pos kingPos = -1, i;
		for (i = 0; kingPos == -1; i++)
		{
			if (IS_SET(player[c].k, i))kingPos = i;
		}
		unsigned int count;
		Move* moves,*movesHead; /*movesHead for memory leaks*/
		legal_moves(&moves, 1 - c, &count);
		movesHead = moves;

		for (; moves != 0; moves = moves->next_move)
		{
			if (moves->to == kingPos) {
				isControllingCheck = 0;
				clear_list(movesHead);
				return TRUE;
			}
		}
		clear_list(movesHead);
		isControllingCheck = 0;
	}
	return FALSE;
}

/* Returns TRUE if the CurrentPlayer is under checkmate, FALSE otherwise. */
Bool is_checkmate() {
	if (is_check(CurrentPlayer) && no_moves()) return TRUE;
	return FALSE;
}

/* Validate a move and make it. Returns TRUE if successful, FALSE if not. 
 *  Error message if any, are stored in *msg. 
 * ep_square (if any) is stored in *ep_sq
 */
Bool validate_and_move(Move *move, char **msg, PlayerColor c, Pos *ep_sq) {
	Board* boards = &player[c].k; /*adress of first board*/
	Board* enemyBoards = &player[1-c].k; /*adress of first enemy board*/
	boards--; /*shift to count in UNKNOWN*/
	enemyBoards--; /*shift to count in UNKNOWN*/
	int isep = OCCUPIED(move->to)?0:1;
	RESET_BIT(boards[move->piece], move->from);
	SET_BIT(boards[move->piece], move->to);
	int i;
	for (i = 1; i <= 6; i++)
	{
		RESET_BIT(enemyBoards[i],move->to);
	}
	if (move->piece == PAWN) /*ep_square*/
	{
		if (((move->to == NE_OF(move->from)) ||
			(move->to == NW_OF(move->from)) ||
			(move->to == SE_OF(move->from)) ||
			(move->to == SW_OF(move->from))) &&
			isep
			) /*ep_square*/
		{
			if (c == WHITE) RESET_BIT(player[1 - c].p, move->to + 8);
			if (c == BLACK) RESET_BIT(player[1 - c].p, move->to - 8);
		}

		if (move->from - move->to == 16) *ep_sq = move->from - 8;
		else if (move->to - move->from == 16) *ep_sq = move->to - 8;

		if (c == WHITE && RANK_OF(move->to) == '8') /*promotion*/
		{
			RESET_BIT(boards[move->piece], move->to);
			SET_BIT(boards[move->promotion_choice], move->to);
		}
		if (c == BLACK && RANK_OF(move->to) == '1') /*promotion*/
		{
			RESET_BIT(boards[move->piece], move->to);
			SET_BIT(boards[move->promotion_choice], move->to);
		}
	}
	if (move->piece == KING) /*castling privilleges*/
	{
		player[c].castle_flags = 0;
	}
	if (move->piece == ROOK && (RANK_OF(move->from) == '1' || RANK_OF(move->from) == '8'))
	{
		if (FILE_OF(move->from)=='a') RESET_BIT(player[c].castle_flags,1);
		else if (FILE_OF(move->from) == 'h') RESET_BIT(player[c].castle_flags, 0);
	}

	/*should add something to change castling flags*/
	if (detect_castle_move(*move,c)) perform_castle(detect_castle_move(*move, c),c);
	return 1;
}

/* Function to decide whether the current position is a draw */
/* Draw can be due to insufficient material, 3 move repetition or stalemate */
	/* 3 move repetition also covers perpetual check */
Bool is_draw() {
	if (no_moves() && !is_checkmate()) /*assumed not checkmate*/
	{
		return TRUE;
	}
	return FALSE;
}

Bool no_moves() {
	Move* decoy;
	unsigned int count;
	legal_moves(&decoy, CurrentPlayer, &count);
	legal_moves_cleaner(&decoy, CurrentPlayer, &count, ep_square);
	if (count == 0) /*assumed not checkmate*/
	{
		clear_list(decoy);
		return TRUE;
	}
	clear_list(decoy);
	return FALSE;
}

/* Returns the piece on a square belonging to player color c. 
 * If there is no piece with color c, UNKNOWN is returned. */
Piece get_piece_at(Board pos, PlayerColor c)
{
  if (OCCUPIED(pos))
  {
    int pieceOwner = (BIT(pos) & BOARD(WHITE)) ? WHITE : BLACK;
    
    if (player[pieceOwner].r & BIT(pos)) return ROOK;
    if (player[pieceOwner].n & BIT(pos)) return NIGHT;
    if (player[pieceOwner].b & BIT(pos)) return BISHOP;
    if (player[pieceOwner].q & BIT(pos)) return QUEEN;
    if (player[pieceOwner].k & BIT(pos)) return KING;
    if (player[pieceOwner].p & BIT(pos)) return PAWN;
  }
  return UNKNOWN;
}

/* Check if this move is trying to castle */
unsigned int detect_castle_move(Move move, PlayerColor c) {
	if (move.piece == KING)
	{
		if (move.from - move.to == 2)
		{
			return 2; /*queen side*/
		}
		if (move.to - move.from == 2)
		{
			return 1; /*king side*/
		}
	}
	return 0;
}

/* Perform castling. Moves king and rook and resets castle flags */
void perform_castle(unsigned int castle, PlayerColor c) {
	unsigned int rookPos = (castle == 2 ? 0 : 7) + (c == BLACK ? 0 : 56);
	unsigned int newRookPos= (castle == 2 ? 3 : 5) + (c == BLACK ? 0 : 56);
	RESET_BIT(player[c].r, rookPos);
	SET_BIT(player[c].r, newRookPos);
	player[c].castle_flags = 0;
}

