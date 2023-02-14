
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include "../src/Pieces.h"

int main(void) {
	printf("=========================\nBeginning Test PiecesTest\n=========================\n");
	
	struct piece * testPiece = newPiece( WHITE_KNIGHT );
	assert(getPieceId(testPiece) == WHITE_KNIGHT);

	printf("Piece Created with _WHITE_KNIGHT Constant\n");
	
	assert(getPieceColour(testPiece) == WHITE_PIECE);
	printf("Piece Colour Confirmed as WHITE_PIECE\n");

	testPiece = newPiece( 0xf ); // Invalid piece ID
	assert(testPiece == NULL);
	assert(errno == EINVAL);
	
	printf("Piece Created with invalid Id returns null and sets error number\n");
	
	return 0;
}