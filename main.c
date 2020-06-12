#include "moves.h"
#include "chess.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PlayerState player[2];
Pos ep_square=-1;
PlayerColor CurrentPlayer;
PlayerColor solvingFor;

int isDebug = 0;

PlayerState player_memory_main[2];
Pos ep_square_mem_main;
void saveStateMain()
{
	player_memory_main[BLACK] = player[BLACK];
	player_memory_main[WHITE] = player[WHITE];
	ep_square_mem_main = ep_square;
}

void restoreStateMain()
{
	player[BLACK] = player_memory_main[BLACK];
	player[WHITE] = player_memory_main[WHITE];
	ep_square = ep_square_mem_main;
}

PlayerState player_memory_main2[2]; /*very cheap but too tired to implement this better*/
Pos ep_square_mem_main2;
void saveStateMain2()
{
	player_memory_main2[BLACK] = player[BLACK];
	player_memory_main2[WHITE] = player[WHITE];
	ep_square_mem_main2 = ep_square;
}

void restoreStateMain2()
{
	player[BLACK] = player_memory_main2[BLACK];
	player[WHITE] = player_memory_main2[WHITE];
	ep_square = ep_square_mem_main2;
}

PlayerState player_memory_main3[2]; /*should have made this into a stack but its too late now*/
Pos ep_square_mem_main3;
void saveStateMain3()
{
	player_memory_main3[BLACK] = player[BLACK];
	player_memory_main3[WHITE] = player[WHITE];
	ep_square_mem_main3 = ep_square;
}

void restoreStateMain3()
{
	player[BLACK] = player_memory_main3[BLACK];
	player[WHITE] = player_memory_main3[WHITE];
	ep_square = ep_square_mem_main3;
}

/* Given a position, find a mate in 1. */
Bool run_mate1(Move* soln)
{
	Move* m;
	Move* debugM; /*for debugging*/
	Move* mHead;
	unsigned int pCount;

	legal_moves(&m, CurrentPlayer, &pCount);
	legal_moves_cleaner(&m, CurrentPlayer, &pCount,ep_square);
	mHead = m;

	if (isDebug)
	{
		while (m != 0)
		{
			print_move(m);
			m = m->next_move;
		}
		m = mHead;
	}

	for (; m != 0;)
	{
		saveStateMain();
		validate_and_move(m, 0x0, CurrentPlayer, &ep_square);
		CurrentPlayer = 1 - CurrentPlayer;
		if (is_checkmate()) break;
		if (isDebug )
		{
			printf("not checkmate= ");
			print_move(m);
			legal_moves(&debugM, CurrentPlayer, &pCount);
			legal_moves_cleaner(&debugM, CurrentPlayer, &pCount, ep_square);
			printf("legal move count: %u\n\n",pCount);
			if (pCount) print_move(debugM);
		}
		CurrentPlayer = 1 - CurrentPlayer;
		restoreStateMain();
		m = m->next_move;
	}
	if (m != 0)
	{
		if (isDebug)
		{
			print_move(m);
		}
		*soln = *m;
		clear_list(mHead);
		return 1;
	}
	soln->from = 0xff;
	clear_list(mHead);
	return 0;
}
/* Given a position, find a mate in 2. */
Bool run_mate2(Move* soln)
{

	Move* m, * enemyM, *sol1= (Move*)malloc(sizeof(Move));
	unsigned int pCount;
	int nonMatePos = 0;

	legal_moves(&m, CurrentPlayer, &pCount);
	legal_moves_cleaner(&m, CurrentPlayer, &pCount, ep_square);
	Move* mHead=m;
	Move* enemyMHead;

	for (; m != 0; m = m->next_move)
	{
		saveStateMain2();
		validate_and_move(m, 0x0, CurrentPlayer, &ep_square);
		CurrentPlayer = 1 - CurrentPlayer;

		legal_moves(&enemyM, CurrentPlayer, &pCount); /*for opponent*/
		legal_moves_cleaner(&enemyM, CurrentPlayer, &pCount, ep_square);
		enemyMHead = enemyM;

		if (pCount == 0) {
			restoreStateMain2();
			CurrentPlayer = 1 - CurrentPlayer;
			continue; /*to avoid ties*/
		}

		while (enemyM!=0) /*for all enemy moves*/
		{
			saveStateMain3();
			validate_and_move(enemyM, 0x0, CurrentPlayer, &ep_square);
			CurrentPlayer = 1 - CurrentPlayer;
			if (isDebug)
			{
				printf("____________________\n");
				print_move(m);
				print_move(enemyM);
				printf("____________________\n");
			}
			if (!run_mate1(sol1))
			{
				nonMatePos = 1;
				break;
			}
			enemyM = enemyM->next_move;
			restoreStateMain3();
		}
		clear_list(enemyMHead);
		if (!nonMatePos) break;
		nonMatePos = 0;

		restoreStateMain2();
	}
	if (m != 0)
	{
		*soln = *m;
		clear_list(mHead);
		free(sol1);
		return 1;
	}
	soln->from = 0xff;
	clear_list(mHead);
	free(sol1);
	return 0;
}


int main(int argc, char const* argv[])
{
	if (argc == 3 && (argv[2][0] == '1' || argv[2][0] == '2')) /*single move debug mode*/
	{
		char* inputStr = (char*)argv[1];
		parse_board(inputStr);
		Move* soln = (Move*)malloc(sizeof(Move));
		char* output;

		if (argv[2][0] == '1')
		{
			display_board();
			solvingFor = CurrentPlayer;
			run_mate1(soln);
			output = move_to_string(soln);
			printf("%s \n", output);
			free(output);
			clear_board();
		}
		else if (argv[2][0] == '2')
		{
			display_board();
			solvingFor = CurrentPlayer;
			run_mate2(soln);
			output = move_to_string(soln);
			printf("%s \n", output);
			free(output);
			clear_board();
		}
		else
		{
			printf("mode can either be '1' or '2'");
			return -1;
		}
		return 0;
	}
	if (argc == 2 && (argv[1][0] == 'c')) /*not a part of project*/
	{
		parse_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		while (1)
		{
			display_board();
			
			Move m=parse_move();

			print_move(&m);

			validate_and_move(&m, 0x0, CurrentPlayer, &ep_square);

			CurrentPlayer = 1 - CurrentPlayer;

			if (is_draw())
			{
				printf("is draw\n");
				return 0;
			}
			if (is_checkmate())
			{
				printf("is checkmate %s wins\n",CurrentPlayer==WHITE?"BLACK":"WHITE");
				return 0;
			}

		}
	}
	else if (argc != 4)
	{
		printf("usage= ./chess <input boards> <output file> <mode('1' or '2')>");
		return -1;
	}

	char input[256];
	char* output;
	FILE* fp;
	FILE* outputFile;
	fp = fopen(argv[1], "r");
	outputFile = fopen(argv[2], "w");
	Move* soln= (Move*)malloc(sizeof(Move));

	if (atoi(argv[3])==2)
	{
		while (fgets(input, 256, fp))
		{
			if (strlen(input) > 6) /*to dodge solutions in sample file*/
			{
				parse_board(input);
				solvingFor = CurrentPlayer;
				run_mate2(soln);
				output = move_to_string(soln);
				fprintf(outputFile, "%s \n", output);
				free(output);
				clear_board();
			}
		}
	}
	else if (atoi(argv[3]) == 1)
	{
		while (fgets(input, 256, fp))
		{
			if (strlen(input) > 6) /*to dodge solutions in sample file*/
			{
				parse_board(input);
				solvingFor = CurrentPlayer;
				run_mate1(soln);
				output = move_to_string(soln);
				fprintf(outputFile, "%s \n", output);
				free(output);
				clear_board();
			}
		}
	}
	free(soln);
	fclose(fp);
	fclose(outputFile);
	return 0;
}

/***************************************************************************************
									CUSTOM TEST BOARDS
****************************************************************************************
"r3k2K/pppp4/8/8/8/8/8/8 b q -"  test case for castling
"r2qk2K/pppp4/8/8/8/8/8/8 b q -"  test case for castling when its full inbetween
"r2k3K/pppp4/8/8/8/8/8/8 b q -"
"kQK5/8/8/8/8/8/8/8 b -" test case for is_checkmate method
"k1K5/2R5/8/8/8/8/8/8 b -" test case for is_draw method
"k1K5/2Q5/8/8/8/8/8/8 w -" mate in 1
"7k/5Q2/8/4N3/8/8/8/1K6 w -" mate in 1
"7k/8/8/8/8/8/QK6/2Q5 w -" mate in 1
"7k/8/8/8/8/8/RK6/2R5 w -" mate in 2
***************************************************************************************/
