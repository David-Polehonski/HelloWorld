#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>
/*
	MACRO Definitions for specific pieces, to
		a) ensure globally unique Ids for each piece and
		b) to enable colour detection by checking the `first` bit:
			0xxx = White piece
			1xxx = Black piece
*/
#define _WHITE_PIECE 0x0 // 0000
#define _BLACK_PIECE 0x8 // 1000

#define _WHITE_PAWN 0x1		// 0001
#define _WHITE_KNIGHT 0x2 // 0010
#define _WHITE_BISHOP 0x3 // 0011
#define _WHITE_ROOK 0x4		// 0100
#define _WHITE_QUEEN 0x5	// 0101
#define _WHITE_KING 0x6		// 0110
#define _BLACK_PAWN 0x9		// 1001
#define _BLACK_KNIGHT 0xa // 1010
#define _BLACK_BISHOP 0xb // 1011
#define _BLACK_ROOK 0xc		// 1100
#define _BLACK_QUEEN 0xd	// 1101
#define _BLACK_KING 0xe		// 1110

extern const unsigned char WHITE_PIECE;
extern const unsigned char BLACK_PIECE;

extern const unsigned char WHITE_PAWN;
extern const unsigned char WHITE_KNIGHT;
extern const unsigned char WHITE_BISHOP;
extern const unsigned char WHITE_ROOK;
extern const unsigned char WHITE_QUEEN;
extern const unsigned char WHITE_KING;

extern const unsigned char BLACK_PAWN;
extern const unsigned char BLACK_KNIGHT;
extern const unsigned char BLACK_BISHOP;
extern const unsigned char BLACK_ROOK;
extern const unsigned char BLACK_QUEEN;
extern const unsigned char BLACK_KING;

struct piece;
typedef struct piece piece_t;

piece_t *newPiece(unsigned char definition);
void destroyPiece(piece_t *p);

unsigned char getPieceId(piece_t *p);
signed int getPieceValue(piece_t *p);
unsigned char getPieceColour(piece_t *p);

#endif