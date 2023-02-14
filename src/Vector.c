#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "Vector.h"

struct direction
{
	signed int i;
	signed int j;
};

struct vector
{
	signed int magnitude;
	direction_t direction;
};

direction_t *newVectorDirection(int i, int j)
{
	direction_t *newDirection = malloc(sizeof(direction_t));
	newDirection->i = i;
	newDirection->j = j;

	return newDirection;
}

vector_t *newVector(int magnitude, direction_t *direction)
{

	vector_t *newVector = malloc(sizeof(vector_t));
	newVector->magnitude = magnitude;
	newVector->direction = *direction;

	return newVector;
}

void destroyVector(vector_t *vector)
{
	if (vector != NULL)
	{
		free(vector);
	}
}