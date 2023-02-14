#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ctype.h>
#include <string.h>

#include "Position.h"
#include "Pieces.h"
#include "Move.h"

const char files[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const char ranks[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

/**	Static Zone;
 * 	appendCharacter => Used for appendCharactering to char *
 */
static void appendCharacter(char **strPtr, char chr)
{
	**strPtr = chr; //	Pointer to the write position(pointer) of the string
	(*strPtr)++;		//	Order precedence matters, dereference the write position pointer, then increment.
}

struct position
{
	unsigned char board[64];
	struct detail
	{
		unsigned canWhiteQSCastle : 1;
		unsigned canWhiteKSCastle : 1;
		unsigned canBlackQSCastle : 1;
		unsigned canBlackKSCastle : 1;

		unsigned onMove : 1;

		unsigned canEnPassent : 1; // Can en passent be performed in this position

		unsigned char enPassentMove; // The numerical position of the en passent target square
		unsigned char halfMoveCount; // Number of moves since last pawn advance
		unsigned char fullMoveCount; // Number of full move cycles, incremented after black moves.

		float evaluation; // Evalutation of the position, + values for white - values for black
	} detail;
};

position_t *newPosition()
{

	position_t *newPosition = malloc(sizeof(struct position));

	int i;
	for (i = 0; i < 64; ++i)
	{
		newPosition->board[i] = 0x00;
	}

	struct detail *d = &newPosition->detail; // Copy a refrence to the detail for shorthand usage below;

	d->onMove = 0; // White on move

	d->canWhiteQSCastle = 1;
	d->canWhiteKSCastle = 1;
	d->canBlackQSCastle = 1;
	d->canBlackKSCastle = 1;

	d->canEnPassent = 0;
	d->enPassentMove = 0x00;
	d->halfMoveCount = 0;
	d->fullMoveCount = 1;
	d->evaluation = 0;

	return newPosition;
}

void destroyPosition(position_t *position)
{
	if (!position)
		return;

	free(position);
}

void setupPosition(position_t *position)
{
	position->board[(int)a1] = WHITE_ROOK;
	position->board[(int)b1] = WHITE_KNIGHT;
	position->board[(int)c1] = WHITE_BISHOP;
	position->board[(int)d1] = WHITE_QUEEN;
	position->board[(int)e1] = WHITE_KING;
	position->board[(int)f1] = WHITE_BISHOP;
	position->board[(int)g1] = WHITE_KNIGHT;
	position->board[(int)h1] = WHITE_ROOK;
	position->board[(int)a2] = WHITE_PAWN;
	position->board[(int)b2] = WHITE_PAWN;
	position->board[(int)c2] = WHITE_PAWN;
	position->board[(int)d2] = WHITE_PAWN;
	position->board[(int)e2] = WHITE_PAWN;
	position->board[(int)f2] = WHITE_PAWN;
	position->board[(int)g2] = WHITE_PAWN;
	position->board[(int)h2] = WHITE_PAWN;

	position->board[(int)a8] = BLACK_ROOK;
	position->board[(int)b8] = BLACK_KNIGHT;
	position->board[(int)c8] = BLACK_BISHOP;
	position->board[(int)d8] = BLACK_QUEEN;
	position->board[(int)e8] = BLACK_KING;
	position->board[(int)f8] = BLACK_BISHOP;
	position->board[(int)g8] = BLACK_KNIGHT;
	position->board[(int)h8] = BLACK_ROOK;
	position->board[(int)a7] = BLACK_PAWN;
	position->board[(int)b7] = BLACK_PAWN;
	position->board[(int)c7] = BLACK_PAWN;
	position->board[(int)d7] = BLACK_PAWN;
	position->board[(int)e7] = BLACK_PAWN;
	position->board[(int)f7] = BLACK_PAWN;
	position->board[(int)g7] = BLACK_PAWN;
	position->board[(int)h7] = BLACK_PAWN;
}

void printPosition(position_t *p)
{
	int i;
	int j;
	char row[8];

	for (i = 0; i < 7; i++)
	{
		row[i] = '-';
	}

	row[8] = '\0';

	for (i = 63; i >= 0; i--)
	{
		j = i % 8;
		switch (p->board[i])
		{
		case _WHITE_PAWN:
			row[j] = 'P';
			break;
		case _WHITE_KNIGHT:
			row[j] = 'N';
			break;
		case _WHITE_BISHOP:
			row[j] = 'B';
			break;
		case _WHITE_ROOK:
			row[j] = 'R';
			break;
		case _WHITE_QUEEN:
			row[j] = 'Q';
			break;
		case _WHITE_KING:
			row[j] = 'K';
			break;
		case _BLACK_PAWN:
			row[j] = 'p';
			break;
		case _BLACK_KNIGHT:
			row[j] = 'n';
			break;
		case _BLACK_BISHOP:
			row[j] = 'b';
			break;
		case _BLACK_ROOK:
			row[j] = 'r';
			break;
		case _BLACK_QUEEN:
			row[j] = 'q';
			break;
		case _BLACK_KING:
			row[j] = 'k';
			break;
		default:
			row[j] = '-';
			break;
		}

		if ((i) % 8 == 0)
		{
			row[8] = '\0';
			// puts(row);
			printf("|%s|\n", row);
		}
	}

	printf("|========|\n");
	printf("|Move.   |\n");

	if (p->detail.onMove == 0)
	{
		printf("|%s%3d|\n", "White", p->detail.fullMoveCount);
	}
	if (p->detail.onMove == 1)
	{
		printf("|%s%3d|\n", "Black", p->detail.fullMoveCount);
	}

	// Now Meta data:
	char castling[] = "O-O     ";

	if (p->detail.canWhiteKSCastle == 1)
	{
		castling[4] = 'K';
	}
	if (p->detail.canWhiteQSCastle == 1)
	{
		castling[5] = 'Q';
	}
	if (p->detail.canBlackKSCastle == 1)
	{
		castling[6] = 'k';
	}
	if (p->detail.canBlackQSCastle == 1)
	{
		castling[7] = 'q';
	}

	printf("|%s|\n", castling);
}

char *getFENFromPosition(struct position *p)
{
	int i;
	int j;
	int s;
	int blankCount = 0;
	char *fen = malloc(84 * sizeof(char));
	char *fen_writeHead = fen;

	for (i = 84; i >= 0; i--)
		fen[i] = 0x0;

	//	Walk backward from the black side to the white
	for (i = 7; i >= 0; i--)
	{
		for (j = 0; j < 8; j++)
		{
			s = (8 * i) + j;

			if (p->board[s] != 0 && blankCount > 0)
			{
				fen_writeHead += sprintf(fen_writeHead, "%d", blankCount);
				blankCount = 0;
			}

			switch (p->board[s])
			{
			case _WHITE_PAWN:
				*fen_writeHead = 'P';
				fen_writeHead++;
				break;
			case _WHITE_KNIGHT:
				*fen_writeHead = 'N';
				fen_writeHead++;
				break;
			case _WHITE_BISHOP:
				*fen_writeHead = 'B';
				fen_writeHead++;
				break;
			case _WHITE_ROOK:
				*fen_writeHead = 'R';
				fen_writeHead++;
				break;
			case _WHITE_QUEEN:
				*fen_writeHead = 'Q';
				fen_writeHead++;
				break;
			case _WHITE_KING:
				*fen_writeHead = 'K';
				fen_writeHead++;
				break;
			case _BLACK_PAWN:
				*fen_writeHead = 'p';
				fen_writeHead++;
				break;
			case _BLACK_KNIGHT:
				*fen_writeHead = 'n';
				fen_writeHead++;
				break;
			case _BLACK_BISHOP:
				*fen_writeHead = 'b';
				fen_writeHead++;
				break;
			case _BLACK_ROOK:
				*fen_writeHead = 'r';
				fen_writeHead++;
				break;
			case _BLACK_QUEEN:
				*fen_writeHead = 'q';
				fen_writeHead++;
				break;
			case _BLACK_KING:
				*fen_writeHead = 'k';
				fen_writeHead++;
				break;
			default:
				blankCount++;
				break;
			}
		}
		if (blankCount > 0)
		{
			fen_writeHead += sprintf(fen_writeHead, "%d", blankCount);
			blankCount = 0;
		}
		if (i != 0)
			appendCharacter(&fen_writeHead, '/');
	}

	//	Now to write all the other details, such move counter etc...
	// w KQkq - 0 1
	appendCharacter(&fen_writeHead, ' ');
	if (p->detail.onMove == 0)
	{
		appendCharacter(&fen_writeHead, 'w');
	}
	else
	{
		appendCharacter(&fen_writeHead, 'b');
	}

	appendCharacter(&fen_writeHead, ' ');
	if (
			(p->detail.canWhiteQSCastle == 0) &&
			(p->detail.canWhiteKSCastle == 0) &&
			(p->detail.canBlackKSCastle == 0) &&
			(p->detail.canBlackQSCastle == 0))
		appendCharacter(&fen_writeHead, '-');

	if (p->detail.canWhiteKSCastle == 1)
		appendCharacter(&fen_writeHead, 'K');

	if (p->detail.canWhiteQSCastle == 1)
		appendCharacter(&fen_writeHead, 'Q');

	if (p->detail.canBlackKSCastle == 1)
		appendCharacter(&fen_writeHead, 'k');

	if (p->detail.canBlackQSCastle == 1)
		appendCharacter(&fen_writeHead, 'q');

	appendCharacter(&fen_writeHead, ' ');
	if (p->detail.canEnPassent == 0)
	{
		appendCharacter(&fen_writeHead, '-');
	}
	else
	{
		char *squareNotation = getSquareNotation(p->detail.enPassentMove);
		for (char *c = squareNotation; *c != 0; c++)
		{
			appendCharacter(&fen_writeHead, *c);
		}
		free(squareNotation);
	}

	appendCharacter(&fen_writeHead, ' ');
	char halfMove[3];
	int halfMoveLen = sprintf(halfMove, "%d", p->detail.halfMoveCount);
	for (char *c = halfMove; *c != 0; c++)
	{
		appendCharacter(&fen_writeHead, *c);
	}

	appendCharacter(&fen_writeHead, ' ');
	char fullMove[4];
	int fullMoveLen = sprintf(fullMove, "%d", p->detail.fullMoveCount);
	for (char *c = fullMove; *c != 0; c++)
	{
		appendCharacter(&fen_writeHead, *c);
	}

	char *fenString = malloc(strlen(fen) + 1);
	strcpy(fenString, fen);
	free(fen);

	return fenString;
}

position_t *getPositionFromFEN(char *argString)
{

	char fenString[strlen(argString)];
	strcpy(fenString, argString);

	position_t *position = newPosition();

	int b;
	int i;

	char *board;
	char *onMove;
	char *castling;
	char *enPassent;
	char *halfMove;
	char *fullMove;

	char *delim = " ";

	board = strtok(fenString, delim);
	onMove = strtok(NULL, delim);
	castling = strtok(NULL, delim);
	enPassent = strtok(NULL, delim);
	halfMove = strtok(NULL, delim);
	fullMove = strtok(NULL, delim);

	char *symbol;

	int rank = 7;
	int file = 0;
	int square;

	char *FENRow;
	char *ROWDelim = "/";

	FENRow = strtok(board, ROWDelim);

	// printf("%s - %i\n", FENRow, rank);

	while (FENRow != NULL)
	{
		for (char *p = FENRow; *p != '\0'; p++)
		{
			symbol = p;
			// printf("Symbol %c\n", *symbol);
			if (isdigit(*symbol))
			{
				file += atoi(symbol);
			}
			else if (isalpha(*symbol))
			{
				square = (rank * 8) + file;
				// printf("Setting Square %i to %c\n", square, *symbol);
				switch (*symbol)
				{
				case 'P':
					position->board[square] = WHITE_PAWN;
					break;
				case 'N':
					position->board[square] = WHITE_KNIGHT;
					break;
				case 'B':
					position->board[square] = WHITE_BISHOP;
					break;
				case 'R':
					position->board[square] = WHITE_ROOK;
					break;
				case 'Q':
					position->board[square] = WHITE_QUEEN;
					break;
				case 'K':
					position->board[square] = WHITE_KING;
					break;
				case 'p':
					position->board[square] = BLACK_PAWN;
					break;
				case 'n':
					position->board[square] = BLACK_KNIGHT;
					break;
				case 'b':
					position->board[square] = BLACK_BISHOP;
					break;
				case 'r':
					position->board[square] = BLACK_ROOK;
					break;
				case 'q':
					position->board[square] = BLACK_QUEEN;
					break;
				case 'k':
					position->board[square] = BLACK_KING;
					break;
				}
				file += 1;
			}
		}

		rank -= 1;
		file = 0;

		FENRow = strtok(NULL, ROWDelim);

		// printf("%s - %i\n", FENRow, rank);
	}

	if (strcmp(onMove, "w") == 0)
	{
		position->detail.onMove = 0;
	}
	if (strcmp(onMove, "b") == 0)
	{
		position->detail.onMove = 1;
	}

	char noop[2] = "-";

	position->detail.canWhiteKSCastle = 0;
	position->detail.canWhiteQSCastle = 0;
	position->detail.canBlackKSCastle = 0;
	position->detail.canBlackQSCastle = 0;

	if (strcmp(castling, noop) != 0)
	{
		for (char *c = castling; *c != 0; c++)
		{
			switch (*c)
			{
			case 'K':
				position->detail.canWhiteKSCastle = 1;
				break;
			case 'Q':
				position->detail.canWhiteQSCastle = 1;
				break;
			case 'k':
				position->detail.canBlackKSCastle = 1;
				break;
			case 'q':
				position->detail.canBlackQSCastle = 1;
				break;
			}
		}
	}

	if (strcmp(enPassent, noop) != 0)
	{
		position->detail.canEnPassent = 1;
		position->detail.enPassentMove = parseSquareNotation(enPassent);
	}

	position->detail.halfMoveCount = atoi(halfMove);
	position->detail.fullMoveCount = atoi(fullMove);

	// printPosition(position);

	return position;
}

position_t *applyMove(position_t *position, move_t *move)
{
	return position;
}

char *getSquareNotation(unsigned int square)
{
	//	Converts an unsigned integer into a 2 char array notation
	char *notation = malloc(sizeof(char[3]));
	char rank = ranks[square / 8];
	char file = files[square % 8];
	notation[0] = file;
	notation[1] = rank;
	notation[2] = '\0';

	return notation;
}

unsigned int parseSquareNotation(char *notation)
{
	//	Converts an unsigned integer into a 2 char array notation
	char file = notation[0];
	char rank = notation[1];
	int i;
	int square = 0;
	for (i = 0; i < 8; i++)
	{
		if (files[i] == file)
			square += i;
		if (ranks[i] == rank)
			square += (i * 8);
	}

	return square;
}

float evaluatePosition(position_t *p)
{
	int pieceCount = 0;
	for (int i = 0; i < 64; i++)
	{
		if (p->board[i] > 0)
		{
			piece_t *piece = newPiece(p->board[i]); // free me!
			pieceCount += getPieceValue(piece);
			destroyPiece(piece); // free'd
		}
	}

	return (float)pieceCount;
}
