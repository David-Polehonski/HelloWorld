
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/Position.h"

int main(void)
{
	printf("=========================\nBeginning Test BoardTest\n=========================\n");

	position_t *position = newPosition();
	assert(position != NULL);
	printf("New Positions can be created.\n");

	setupPosition(position);
	printf("Default position after setup.\n");

	printPosition(position);

	char *fenString = getFENFromPosition(position);
	char startingPosition[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	printf("|\n| Pos : %s\n", fenString);
	printf("| Pos : %s\n", startingPosition);
	printf("|\n| ---------------------------------------------------------------------- |\n");

	assert(strcmp(fenString, startingPosition) == 0);
	free(fenString);
	fenString = NULL;

	char testPosition[] = "2kr3r/pp3ppp/5n2/qNpP4/Pb1p4/3Q4/1PPB1PPP/R4RK1 w - c6 0 14";
	position_t *loadedPosition = getPositionFromFEN(testPosition);

	char *loadedFenString = getFENFromPosition(loadedPosition);

	printf("|\n| Pos : %s (%i)\n", testPosition, strlen(testPosition));
	printf("| Pos : %s (%i)\n", loadedFenString, strlen(loadedFenString));
	printf("|\n| ---------------------------------------------------------------------- |\n");
	assert(strcmp(testPosition, loadedFenString) == 0);

	printf("| Pos : %s\n| Evl : %f \n", startingPosition, evaluatePosition(position));

	free(loadedFenString);

	destroyPosition(position);
	destroyPosition(loadedPosition);

	return 0;
}