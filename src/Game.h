#ifndef GAME_H
#define GAME_H

#include "Position.h"

const char translation[2];

struct game {
	position_t * startingPosition;
	struct move* firstMove;
};

struct move {

	struct position* position;
	struct move* previous;
	struct move* next;

	unsigned char piece;
	unsigned char origin;
	unsigned char destination;
	char vector[2];

	//	Byte of flags
	unsigned isCapture 		: 1;
	unsigned isQSCastle 	: 1;
	unsigned isKSCastle 	: 1;
	unsigned isEnPassent 	: 1;
	unsigned isCheck 		: 1;
	unsigned isMate 		: 1;
	unsigned isFlagA 		: 1;
	unsigned isFlagB 		: 1;

	char* notation[];
};

struct game* newGame();
void endGame(struct game* g);

struct move* newMove(struct position* p, unsigned char piece, unsigned char origin, char vector[2]);

bool moveStaysOnBoard (unsigned char origin, char vector[2]);
bool validateMove(struct position* currentPosition, unsigned char piece, unsigned char origin, char vector[2] );

#endif
