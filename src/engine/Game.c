#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Game.h"

const char translation[2] = {1,8};

struct game* newGame() {
	struct game* g = malloc(sizeof(struct game));
	g->startingPosition = setupPosition();
	return g;
}

void endGame(struct game* g) {
	if (g->startingPosition != 0) {
		free(g->startingPosition);
	}
	free(g);
}

struct move* newMove(struct position* p, unsigned char piece, unsigned char origin, char vector[2]) {
	//	do some math to generate the move and the new position
	struct move* move = malloc(sizeof(struct move));

	move->piece = piece;
	move->origin = origin;
	move->vector[0] = vector[0];
	move->vector[1] = vector[1];

	//	Check the piece exists in the correct location:
	if(p->board.array[move->origin] == move->piece) {
		// calculate new location:
		move->destination = move->origin + ( vector[0] * translation[0] + vector[1] * translation[1] );
		move->position = copyPosition(p);
		move->position->board.array[move->origin] = 0x00;
		move->position->board.array[move->destination] = move->piece;
	}

	return move;
}

/*@validateMove
 * This method only validates whether the move stays within the board bounds.
 * TODO: Extend the method to check for  other validation criteria, possibly through
 * sub methods.
 */
bool validateMove(struct position* currentPosition, unsigned char piece, unsigned char origin, char vector[2] ) {

	//	First check the piece hasn't run off the board:
	bool isValid = moveStaysOnBoard(origin, vector);
	return isValid;
}

bool moveStaysOnBoard (unsigned char origin, char vector[2]) {
	unsigned char a = origin % 8;
	unsigned char b = ( vector[0] * translation[0] + vector[1] * translation[1] ) % 8;

	if (vector[0] != 0) {
		if (vector[0] > 0 && (a > b)) {
			return false;
		}
		if (vector[0] < 0 && (a < b)) {
			return false;
		}
	}

	if (vector[1] != 0) {
		if ((a + b) > 63) {
			return false;
		}
	}

	return true;
}
