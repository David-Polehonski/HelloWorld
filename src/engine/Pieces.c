#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "Vector.h"
#include "Pieces.h"

struct piece
{
	unsigned char id;
	signed int value;
};

const unsigned char WHITE_PIECE = _WHITE_PIECE;
const unsigned char BLACK_PIECE = _BLACK_PIECE;

const unsigned char WHITE_PAWN = _WHITE_PAWN;
const unsigned char WHITE_KNIGHT = _WHITE_KNIGHT;
const unsigned char WHITE_BISHOP = _WHITE_BISHOP;
const unsigned char WHITE_ROOK = _WHITE_ROOK;
const unsigned char WHITE_QUEEN = _WHITE_QUEEN;
const unsigned char WHITE_KING = _WHITE_KING;

const unsigned char BLACK_PAWN = _BLACK_PAWN;
const unsigned char BLACK_KNIGHT = _BLACK_KNIGHT;
const unsigned char BLACK_BISHOP = _BLACK_BISHOP;
const unsigned char BLACK_ROOK = _BLACK_ROOK;
const unsigned char BLACK_QUEEN = _BLACK_QUEEN;
const unsigned char BLACK_KING = _BLACK_KING;

unsigned char getPieceId(piece_t *p)
{
	return p->id;
}

//	Returns the raw value of the piece, unmodified by it's activity in the current position.
signed int getPieceValue(piece_t *p)
{
	return p->value;
}

//	Compare results to Constances WHITE_PIECE and BLACK_PIECE
unsigned char getPieceColour(piece_t *p)
{
	char pieceMask = 0x8; // 00001000;
	return pieceMask & getPieceId(p);
}

piece_t *newPiece(unsigned char definition)
{
	piece_t *newPiece = malloc(sizeof(piece_t));

	newPiece->id = definition;

	switch (definition)
	{
	case _WHITE_PAWN:
		newPiece->value = 1;
		break;
	case _WHITE_KNIGHT:
		newPiece->value = 3;
		break;
	case _WHITE_BISHOP:
		newPiece->value = 3;
		break;
	case _WHITE_ROOK:
		newPiece->value = 5;
		break;
	case _WHITE_QUEEN:
		newPiece->value = 9;
		break;
	case _WHITE_KING:
		newPiece->value = 100;
		break;
	case _BLACK_PAWN:
		newPiece->value = -1;
		break;
	case _BLACK_KNIGHT:
		newPiece->value = -3;
		break;
	case _BLACK_BISHOP:
		newPiece->value = -3;
		break;
	case _BLACK_ROOK:
		newPiece->value = -5;
		break;
	case _BLACK_QUEEN:
		newPiece->value = -9;
		break;
	case _BLACK_KING:
		newPiece->value = -100;
		break;
	default:
		destroyPiece(newPiece);
		errno = EINVAL;
		perror("Invalid piece ID");
		return NULL;
	}

	return newPiece;
}

void destroyPiece(piece_t *p)
{
	if (!p)
		return;

	free(p);
}