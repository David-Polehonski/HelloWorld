#ifndef MOVE_H
#define MOVE_H

#include "stdbool.h"
/**
 * A Move is defined as a translation from one board position to another
 * Therefore a move should be initialised with a position_t * and either manipulated
 * to perform the translation or parse a long algebraic expression into a move
 * and once 'applied' a `complete` flag locks the move from furhter updates and a new
 * position_t * is generated representing the position after the move.
 *
 * A Move is primary concerned with expressing the translation from position_t A to B
 * */

struct move;
typedef struct move move_t;

move_t *newMove();
void destroyMove(move_t *move);

void setMoveString(move_t *move, char *longAlgebraicExpression);
char *getMoveString(move_t *move);

void setOriginSquare(move_t *move, square_t squareId);
void setDestinationSquare(move_t *move, square_t squareId);

square_t getOriginSquare(move_t *move);
square_t getDestinationSquare(move_t *move);

void setCastlingLong(move_t *move);
void setCastlingShort(move_t *move);

bool isCastlingLong(move_t *move);
bool isCastlingShort(move_t *move);

void setPromotion(move_t *move, int pieceId);
bool isPromotion(move_t *move);
int getPromotion(move_t *move, int pieceId);

#endif