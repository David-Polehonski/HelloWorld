#ifndef VECTOR_H
#define VECTOR_H

	/*
		All Legal moves in can be expressed a integer vectors, [SCALAR,[I,J]] Where I and J are direction of squares
		on a board and the scalar is the number of squares available e.g
			[2,[1,0]] Is a move 2 squares up the board.
			[4,[1,-1]] Is a move diagonally up and left.
			All Moves apart from castling which requires moving two pieces can be expressed this way
	*/
	struct direction;
	typedef struct direction direction_t;
	
	struct vector;
	typedef struct vector vector_t;

	direction_t * newVectorDirection(int i, int j);
	vector_t * newVector(int magnitude, direction_t * direction);

	void destroyVector(vector_t * vector);
#endif