#ifndef POSITION_H
#define POSITION_H

#include "Pieces.h"
#include "Square.h"
#include "Player.h"
#include "Move.h"

struct position;
typedef struct position position_t;

position_t *newPosition();
void destroyPosition(position_t *position);

void setupPosition(position_t *position);

void printPosition(position_t *p);

char *getFENFromPosition(position_t *p);
position_t *getPositionFromFEN(char *fenString);

position_t *applyMove(position_t *position, move_t *move);

char *getSquareNotation(unsigned square);
unsigned int parseSquareNotation(char *square);

/**
 * Evaluates the position passed, both setting and returning a signed floating point number
 * indicating the strenth of the position for either player
 * + in whites favour
 * - in blacks favour
 * */
float evaluatePosition(position_t *p);
#endif
