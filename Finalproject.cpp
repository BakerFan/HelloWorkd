#include "stdafx.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define BLACK 1                                 //these three definitions are for the chess type
#define WHITE 2
#define EMPTY 0

//this declaration is for the recurse 
int min(int board[16][16], int deepth, int computer, int competitor, int alpha, int beta);

//this data type is used to record the position of the board
typedef struct
{
	int x;
	int y;
}Position;

//this 2D array is used to evaluate the score at first and the middle procedure
int _Score_Board[16][16] =
{
	{ 2000,-600,10,10,10,10,10,10,10,10,10,10,10,10,-600,2000 },
	{ -600,-800,5,5,5,5,5,5,5,5,5,5,5,5,-800,-600 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,1,1,1,1,1,1,1,1,5,10 },
	{ -600,-800,5,5,5,5,5,5,5,5,5,5,5,5,-800,-600 },
	{ 2000,-600,10,10,10,10,10,10,10,10,10,10,10,10,-600,2000 },
};

//this array is to mark the 8 directions
int direct[8][2] =
{
	{ 1,0 },{ 1,-1 },{ 0,-1 },{ -1,-1 },{ -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },
};

//this function is to count the empty as the depth
int check_empty(int board[16][16])
{
	int count = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (board[i][j] == EMPTY)
				count++;
		}
	}
	return count;
}

//the assign of the board
void array_assign(int arr1[16][16], int arr2[16][16])
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			arr2[i][j] = arr1[i][j];
		}
	}
}

//to output the board
void print(int arr[16][16])
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}

//to flip the chesses if necessary
void setboard(int TestBoard[16][16], int computer, int competitor, Position P)
{
	int X_position = P.x;
	int Y_position = P.y;
	TestBoard[X_position][Y_position] = computer;
	for (int index = 0; index < 8; index++)         //this loop is use to show the operations on one direction
	{
		int line = X_position;
		int col = Y_position;
		line += direct[index][0];
		col += direct[index][1];
		int row_flips = 0;                          //row_flips: to count how many chess should be flipped
		int can_flip = 0;                           //can_flip: to judge whether it can do the flipping operations(if there is a computer chess and the chess inside are all the other)
		while (line >= 0 && col >= 0 && line < 16 && col < 16)
		{
			if (TestBoard[line][col] == computer)
			{
				can_flip = 1;
				break;
			}
			else if (TestBoard[line][col] == EMPTY)
			{
				break;
			}
			else
			{
				row_flips++;
			}
			line += direct[index][0];
			col += direct[index][1];
		}
		if (can_flip && row_flips > 0)            //if the row_flips is 0 ; there is no chess to be flipped
		{
			line = X_position;
			col = Y_position;
			while (row_flips > 0)                 //if the row_flips is 0 ; the flipping is over
			{
				line += direct[index][0];
				col += direct[index][1];
				if (TestBoard[line][col] == competitor)
					TestBoard[line][col] = computer;
				row_flips--;
			}
		}
	}
}

//to count the chesses of two players 
void chess_count(int board[16][16], int *C_computer, int *C_competitor, int computer, int competitor)
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (board[i][j] == computer)
				(*C_computer)++;
			else if (board[i][j] == competitor)
				(*C_competitor)++;
		}
	}
}

//This function is used to generate the active position
void generate(int board[16][16], int computer, int competitor, int *length, Position arr[256])
{
	int index_arr = 0;
	int TestBoard[16][16];
	array_assign(board, TestBoard);
	int is_break = 0;
	int is_in = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (TestBoard[i][j] == computer)
			{
				/*check the X+ direction*/
				for (int k = i + 2; k < 16; k++)
				{
					if (TestBoard[k][j] == EMPTY)
					{
						for (int m = i + 1; m < k; m++)
						{
							if (TestBoard[m][j] == EMPTY || TestBoard[m][j] == computer)
							{
								is_break = 1;
								break;
							}
						}
						if (is_break == 1)
						{
							is_break = 0;
							break;
						}
						for (int i = 0; i < index_arr; i++)
						{
							if (arr[i].x == k && arr[i].y == j)
							{
								is_in = 1;
								break;
							}
						}
						if (is_in == 0)
						{
							arr[index_arr].x = k;
							arr[index_arr].y = j;
							index_arr++;
						}
						is_in = 0;
						break;
					}
				}
				//check the X- direction
				for (int k = i - 2; k >= 0; k--)
				{
					if (TestBoard[k][j] == EMPTY)
					{
						for (int m = i - 1; m > k; m--)
						{
							if (TestBoard[m][j] == EMPTY || TestBoard[m][j] == computer)
							{
								is_break = 1;
								break;
							}
						}
						if (is_break == 1)
						{
							is_break = 0;
							break;
						}
						for (int i = 0; i < index_arr; i++)
						{
							if (arr[i].x == k && arr[i].y == j)
							{
								is_in = 1;
								break;
							}
						}
						if (is_in == 0)
						{
							arr[index_arr].x = k;
							arr[index_arr].y = j;
							index_arr++;
						}
						is_in = 0;
						break;
					}
				}
				//check the Y+ direction
				for (int k = j + 2; k < 16; k++)
				{
					if (TestBoard[i][k] == EMPTY)
					{
						for (int m = j + 1; m < k; m++)
						{
							if (TestBoard[i][m] == EMPTY || TestBoard[i][m] == computer)
							{
								is_break = 1;
								break;
							}
						}
						if (is_break == 1)
						{
							is_break = 0;
							break;
						}
						for (int q = 0; q < index_arr; q++)
						{
							if (arr[q].x == k && arr[q].y == j)
							{
								is_in = 1;
								break;
							}
						}
						if (is_in == 0)
						{
							arr[index_arr].x = i;
							arr[index_arr].y = k;
							index_arr++;
						}
						is_in = 0;
						break;
					}
				}
				//check the Y- directioon
				for (int k = j - 2; k >= 0; k--)
				{
					if (TestBoard[i][k] == EMPTY)
					{
						for (int m = j - 1; m > k; m--)
						{
							if (TestBoard[i][m] == EMPTY || TestBoard[i][m] == computer)
							{
								is_break = 1;
								break;
							}
						}
						if (is_break == 1)
						{
							is_break = 0;
							break;
						}
						for (int q = 0; q < index_arr; q++)
						{
							if (arr[q].x == k && arr[q].y == j)
							{
								is_in = 1;
								break;
							}
						}
						if (is_in == 0)
						{
							arr[index_arr].x = i;
							arr[index_arr].y = k;
							index_arr++;
						}
						is_in = 0;
						break;
					}
				}
				//check the y=x -direction
				for (int k = -2;; k--)
				{
					if (i + k < 0 || j + k < 0)
						break;
					if (TestBoard[i + k][j + k] == EMPTY)
					{
						for (int m = -1; m > k; m--)
						{
							if (TestBoard[i + m][j + m] == EMPTY || TestBoard[i + m][j + m] == computer)
							{
								is_break = 1;
								break;
							}
						}
						if (is_break == 1)
						{
							is_break = 0;
							break;
						}
						for (int q = 0; q < index_arr; q++)
						{
							if (arr[q].x == k + i && arr[q].y == j + k)
							{
								is_in = 1;
								break;
							}
						}
						if (is_in == 0)
						{
							arr[index_arr].x = i + k;
							arr[index_arr].y = j + k;
							index_arr++;
						}
						is_in = 0;
						break;
					}
				}
				//check the y=x + direction
				for (int k = 2;; k++)
				{
					if (i + k > 15 || j + k > 15)
						break;
					if (TestBoard[i + k][j + k] == EMPTY)
					{
						for (int m = 1; m < k; m++)
						{
							if (TestBoard[i + m][j + m] == EMPTY || TestBoard[i + m][j + m] == computer)
							{
								is_break = 1;
								break;
							}
						}
						if (is_break == 1)
						{
							is_break = 0;
							break;
						}
						for (int q = 0; q < index_arr; q++)
						{
							if (arr[q].x == k + i && arr[q].y == j + k)
							{
								is_in = 1;
								break;
							}
						}
						if (is_in == 0)
						{
							arr[index_arr].x = i + k;
							arr[index_arr].y = j + k;
							index_arr++;
						}
						is_in = 0;
						break;
					}
				}
				//check the y=-x -direction
				for (int k = 2;; k++)
				{
					if (i + k > 15 || j - k < 0)
						break;
					if (TestBoard[i + k][j - k] == EMPTY)
					{
						for (int m = 1; m < k; m++)
						{
							if (TestBoard[i + m][j - m] == EMPTY || TestBoard[i + m][j - m] == computer)
							{
								is_break = 1;
								break;
							}
						}
						if (is_break == 1)
						{
							is_break = 0;
							break;
						}
						for (int q = 0; q < index_arr; q++)
						{
							if (arr[q].x == k + i && arr[q].y == j - k)
							{
								is_in = 1;
								break;
							}
						}
						if (is_in == 0)
						{
							arr[index_arr].x = i + k;
							arr[index_arr].y = j - k;
							index_arr++;
						}
						is_in = 0;
						break;
					}
				}
				//check the y=-x +direction
				for (int k = 2;; k++)
				{
					if (i - k < 0 || j + k >15)
						break;
					if (TestBoard[i - k][j + k] == EMPTY)
					{
						for (int m = 1; m < k; m++)
						{
							if (TestBoard[i - m][j + m] == EMPTY || TestBoard[i - m][j + m] == computer)
							{
								is_break = 1;
								break;
							}
						}
						if (is_break == 1)
						{
							is_break = 0;
							break;
						}
						for (int q = 0; q < index_arr; q++)
						{
							if (arr[q].x == i - k && arr[q].y == j + k)
							{
								is_in = 1;
								break;
							}
						}
						if (is_in == 0)
						{
							arr[index_arr].x = i - k;
							arr[index_arr].y = j + k;
							index_arr++;
						}
						is_in = 0;
						break;
					}
				}
			}
		}
	}
	*length = index_arr;
}

//the evaluation of the board
int evaluate(int board[16][16], int score_board[16][16], int computer, int competitor)
{
	int score = 0;
	int board_score = 0;                                                 //the scoreboard score
	int active_score = 0;                                                // the active number as AI's - Human's
	int length = 0;
	int *active_len = &length;
	int *C_computer = (int *)malloc(sizeof(int));
	int *C_competitor = (int *)malloc(sizeof(int));
	int chess_score = 0;                                                 //the chess number
	int chess_deepth = 0;
	chess_deepth = check_empty(board);
	chess_count(board, C_computer, C_competitor, computer, competitor);
	chess_score = *C_computer - *C_competitor;
	Position arr[256] = {};
	generate(board, computer, competitor, active_len, arr);
	active_score += *active_len;
	*active_len = 0;
	generate(board, competitor, computer, active_len, arr);
	active_score -= *active_len;
	if (chess_deepth > 196 || (chess_deepth > 128 && chess_deepth <= 196)) 
	{
		Position point = {};
		for (int i = 0; i < 16; i++)
		{
			point.x = i;
			for (int j = 0; j < 16; j++)
			{
				point.y = j;
				if (board[i][j] != EMPTY && !((i == 7 && j == 7) || (i == 7 && j == 7) || (i == 8 && i == 7) || (i == 8 && i == 8)))
				{
					if (board[i][j] == computer)
						board_score += (int)score_board[i][j];
					else if (board[i][j] == competitor)
						board_score -= (int)score_board[i][j];
				}
			}
		}
		if (chess_deepth > 196)                                                       //the evaluation at first
			score = 2 * active_score + 4 * board_score + 2 * chess_score;
		else                                                                          //the middle evaluation
			score = 2 * active_score +3* board_score + 2 * chess_score;
	}
	if (chess_deepth > 64 && chess_deepth <= 128)                                     //the second middle evaluation
	{
		score = 2 * active_score + 3 * chess_score+ 2 * board_score;
	}
	else                                                                              //the evaluation at last
	{
		score = active_score + 4 * chess_score + board_score;
	}
	free(C_computer);
	free(C_competitor);
	return score;
}

//check if one player has won (return 1 if yes)
int win(int board[16][16])
{
	int empty_n = 0;
	int white_n = 0;
	int black_n = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (board[i][j] == EMPTY)
				empty_n++;
			else if (board[i][j] == WHITE)
				white_n++;
			else
				black_n++;
		}
	}
	if (empty_n != 0 && black_n != 0 && white_n != 0)
		return 0;
	return 1;
}


//the maxmin algorithm with the alpha-beta cut
int max(int board[16][16], int deepth, int computer, int competitor, int alpha, int beta)
{
	int len = 0;
	int *length = &len;
	int value = evaluate(board, _Score_Board, computer, competitor);                //evaluation
	if (deepth <= 0 || win(board) == 1)
	{
		return value;
	}
	int best = -100000;
	Position points[256] = {};
	generate(board, computer, competitor, length, points);
	for (int i = 0; i<*length; i++)
	{
		Position *p = &points[i];
		int Ori_Board[16][16] = {};
		array_assign(board, Ori_Board);
		setboard(board, computer, competitor, *p);
		value = min(board, deepth - 1, computer, competitor, alpha, best > beta ? best : beta);          //the recurse of last min function
		array_assign(Ori_Board, board);
		if (value > best)                                                                //find the best score
		{
			best = value;
		}
		if (value > alpha)                                                               //the alpha cut
			break;
	}
	return best;
}

int min(int board[16][16], int deepth, int computer, int competitor, int alpha, int beta)
{
	int len = 0;
	int *length = &len;
	int value = evaluate(board, _Score_Board, computer, competitor);                 //the evaluation
	if (deepth <= 0 || win(board) == 1)
	{
		return value;
	}

	int best = 100000;
	Position points[256] = {};
	generate(board, competitor, computer, length, points);

	for (int i = 0; i<*length; i++)
	{
		Position *p = &points[i];
		int Ori_Board[16][16] = {};
		array_assign(board, Ori_Board);
		setboard(board, competitor, computer, *p);
		value = max(board, deepth - 1, computer, competitor, best < alpha ? best : alpha, beta);      //the recurse of the last max node
		array_assign(Ori_Board, board);
		if (value < best)                                                       //find the best score
		{
			best = value;
		}
		if (value < beta)                                                       //the beta cut
			break;
	}
	return best;
}

//the result function
Position maxmin(int board[16][16], int deepth, int computer, int competitor)
{
	int best = -100000;
	int len = 0;
	int *length = &len;
	Position list[256] = {};
	Position bestpoint = {};
	Position* Best_Point = &bestpoint;
	generate(board, computer, competitor, length, list);
	Best_Point->x = list[0].x;
	Best_Point->y = list[0].y;
	int value = 0;
	Position *p = (Position*)malloc(sizeof(Position));
	for (int i = 0; i<*length; i++)
	{
		p->x = list[i].x;
		p->y = list[i].y;
		int Ori_Board[16][16] = {};
		array_assign(board, Ori_Board);
		setboard(board, computer, competitor, *p);
		value = min(board, deepth - 1, computer, competitor, best > -100000 ? best : -100000, 100000);
		if (value == best)
		{
			Best_Point->x = p->x;
			Best_Point->y = p->y;
		}
		if (value > best)
		{
			best = value;
			Best_Point->x = p->x;
			Best_Point->y = p->y;
		}
		array_assign(Ori_Board, board);
	}
	free(p);
	Position result;
	result.x = Best_Point->x;
	result.y = Best_Point->y;
	return result;                                             //the result will be in the root node
}

//the I/O operations (inside the main function)
int main()
{
	char order[6] = { 0 };
	char order_place[] = "PLACE";
	char order_start[] = "START";
	char order_end[] = "END";
	char order_turn[] = "TURN";
	int computer = 0;
	int competitor = 0;
	int winner = 0;
	int board[16][16] = { 0 };
	board[7][8] = board[8][7] = BLACK;                              //the initialization of the board
	board[7][7] = board[8][8] = WHITE;
	scanf("%s", order);
	if (strcmp(order, order_start) == 0)                            //the START operations
	{
		scanf("%d", &computer);
		competitor = (computer == WHITE) ? BLACK : WHITE;
		printf("OK\n");
		fflush(stdout);
	}
	Position *in = (Position*)malloc(sizeof(Position));
	Position*out = (Position*)malloc(sizeof(Position));
	while (scanf("%s", order))
	{
		if (strcmp(order, order_end) == 0)                        //the END operations
		{
			scanf("%d", &winner);
			break;
		}
		if (strcmp(order, order_place) == 0)                      //the PLACE operations
		{
			scanf("%d %d", &in->x, &in->y);
			board[in->x][in->y] = competitor;
			setboard(board, competitor, computer, *in);
			//print(board);
		}
		if (strcmp(order, order_turn) == 0)                       //the TURN operations
		{
			int deepth = 6;                                        //the depth of the search is 6
			if (check_empty(board) < 6)
				deepth = check_empty(board);
			*out = maxmin(board, deepth, computer, competitor);
			board[out->x][out->y] = computer;
			setboard(board, computer, competitor, *out);
			//print(board);
			printf("%d %d\n", out->x, out->y);
			fflush(stdout);
		}
		if (strcmp(order, "PRINT") == 0)                          //the PRINT and the ASSIGN are the new orders, just for debug
			print(board);
		if (strcmp(order, "ASSIGN") == 0)
		{
			int i = 0;
			int j = 0;
			int chess = 0;
			scanf("%d %d %d", &i, &j, &chess);
			board[i][j] = chess;
		}
	}
	free(in);                                                     //release the memory
	free(out);
	return 0;
}