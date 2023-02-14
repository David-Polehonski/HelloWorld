
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include "../src/Vector.h"

int main(void)
{
	printf("=========================\nBeginning Test VectorTest\n=========================\n");

	vector_t *testVector = newVector(6, newVectorDirection(1, 0));

	assert(testVector != NULL);
	printf("Vector Created with Magnitude and Direction\n");

	destroyVector(testVector);

	return 0;
}