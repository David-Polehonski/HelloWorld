#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "Vector.h"
#include "Pieces.h"

#include "Square.h"
#include "Player.h"
#include "Move.h"

/**
 * LAN
 *	<LAN move descriptor piece moves> ::= <Piece symbol><from square>['-'|'x']<to square>
 *	<LAN move descriptor pawn moves>  ::= <from square>['-'|'x']<to square>[<promoted to>]
 *	<Piece symbol> ::= 'N' | 'B' | 'R' | 'Q' | 'K'
 *	Castling Short ::= 'O-O'
 *	Casltling Long ::= 'O-O-O'
 * */
struct move
{
	square_t origin;
	square_t destination;

	player_t playerOnMove;

	int promotionPiece;

	bool longCastling;
	bool shortCastling;
};
typedef struct move move_t;

move_t *newMove()
{
	move_t *newMove = malloc(sizeof(move_t));
	return newMove;
}

void destroyMove(move_t *move)
{
	if (!move)
		return;

	free(move);
}

void setMoveString(move_t *move, char *longAlgebraicExpression)
{
	return;
}

void setOriginSquare(move_t *move, square_t squareId)
{
	return;
}

void setDestinationSquare(move_t *move, square_t squareId)
{
	return;
}

void setCastlingLong(move_t *move)
{
	return;
}

void setCastlingShort(move_t *move)
{
	return;
}

void setPromotion(move_t *move, int pieceId)
{
	return;
}
