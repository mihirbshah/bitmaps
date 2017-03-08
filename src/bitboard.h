/*
 * bitboard.h
 *
 *  Created on: Feb 22, 2017
 *      Author: mihir
 */

#ifndef BITBOARD_H_
#define BITBOARD_H_

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

// macro to generate unsigned long long constants
#define C64(constantU64) constantU64##ULL

// 64 bit
typedef unsigned long long Bitboard;
typedef std::vector<short> BitPos;

void disp_bb(Bitboard b, const std::string& header = std::string())
{
	std::cout << header << ":\n\n";
	// mask to extract MSB
	Bitboard mask = C64(0x8000000000000000);
	std::string s;
	for (int i = 0; i < 65; ++i) //Run the loop til 65 to print the last batch
	{
		if (i % 8 == 0)
		{
			if (s.length())
			{
				// derive rank number
				s = std::to_string(9 - int(i/8)) + " | " + s;
				std::cout << s << "\n";
				s.clear();
			}
		}
		// prepend new bits since LSB <-> a1 and MSB <-> h8
		if (b & mask) // check if MSB is set
			s = "X  " + s;
		else
			s = "-  " + s;
		b = b << 1; // drop MSB
	}
	// unicode for continuous horizontal dash
	//const char* hdash  = "\u2500\u2500\u2500\u2500\u2500\u2500\u2500";
	//std::cout << hdash << hdash << hdash << hdash << std::endl;
	std::cout << "---------------------------\n";
	std::cout << "  | a  b  c  d  e  f  g  h  \n\n";
}


/*
 * 3 ways to shift -
 * 1. Using individual shift functions
 * 2. Using shift_one()
 * 3. Using rotate_one()
 */

/* Bitboard shifts
  northwest    north   northeast
  noWe         nort         noEa
          +7    +8    +9
              \  |  /
  west    -1 <-  0 -> +1    east
              /  |  \
          -9    -8    -7
  soWe         sout         soEa
  southwest    south   southeast
*/
Bitboard south_one(Bitboard b)
{
	return b >> 8;
}

Bitboard north_one(Bitboard b)
{
	return b << 8;
}

namespace
{
	const Bitboard not_a_file = C64(0xfefefefefefefefe); // ~0x0101010101010101
	const Bitboard not_h_file = C64(0x7f7f7f7f7f7f7f7f); // ~0x8080808080808080
};

Bitboard east_one(Bitboard b)
{
	return (b << 1) & not_a_file;
}

Bitboard west_one(Bitboard b)
{
	return (b >> 1) & not_h_file;
}

Bitboard ne_one(Bitboard b)
{
	return (b << 9) & not_a_file;
}

Bitboard nw_one(Bitboard b)
{
	return (b << 7) & not_h_file;
}

Bitboard se_one(Bitboard b)
{
	return (b >> 7) & not_a_file;
}

Bitboard sw_one(Bitboard b)
{
	return (b >> 9) & not_h_file;
}


enum ray_dir {ne, east, se, south, sw, west, nw, north};

namespace
{
//                ne e  se  s   sw  w   nw n
short shift[8] = {9, 1, -7, -8, -9, -1, 7, 8};

Bitboard shift_mask[8] =
{
	C64(0xfefefefefefefefe), // ne
	C64(0xfefefefefefefefe), // e
	C64(0xfefefefefefefefe), // se
	C64(0xffffffffffffffff), // s
	C64(0x7f7f7f7f7f7f7f7f), // sw
	C64(0x7f7f7f7f7f7f7f7f), // w
	C64(0x7f7f7f7f7f7f7f7f), // nw
	C64(0xffffffffffffffff)  // n
};

Bitboard rotate_mask[8] =
{
	C64(0xfefefefefefefe00), // ne
	C64(0xfefefefefefefefe), // e
	C64(0x00fefefefefefefe), // se
	C64(0x00ffffffffffffff), // s
	C64(0x007f7f7f7f7f7f7f), // sw
	C64(0x7f7f7f7f7f7f7f7f), // w
	C64(0x7f7f7f7f7f7f7f00), // nw
	C64(0xffffffffffffff00)  // n
};

};

Bitboard shift_one(Bitboard b, ray_dir dir)
{
	if (shift[dir] > 0)
		return (b << shift[dir]) & shift_mask[dir]; // n, e, ne, nw
	else
		return (b >> abs(shift[dir])) & shift_mask[dir]; // s, w, se, sw
}

/* Bitboard rotation */
Bitboard rotl64(Bitboard b, short pos)
{
	return (b << pos) | (b >> (64 - pos));
}

Bitboard rotr64(Bitboard b, short pos)
{
	return (b >> pos) | (b << (64 - pos));
}

Bitboard rotate_one(Bitboard b, ray_dir dir)
{
	return rotl64(b, shift[dir]) & rotate_mask[dir];
}


Bitboard lookup[64];
//TODO: Check if we can do this compile time  by template metaprogramming
void setup_lookup_table()
{
	for (int i = 0; i < 64; ++i)
		lookup[i] = C64(1) << i;
}

bool test(Bitboard x, short pos)
{
	return x & lookup[pos];
}

Bitboard set(Bitboard x, short pos)
{
	return x | lookup[pos];
}

Bitboard toggle(Bitboard x, short pos)
{
	return x ^ lookup[pos];
}

Bitboard reset(Bitboard x, short pos)
{
	//slower way
	//return x & ~lookup[pos];
	//faster way
	return toggle(set(x, pos), pos);
}

// http://stackoverflow.com/questions/14086854/get-an-array-of-the-bit-positions-within-a-64-bit-integer
// Time Complexity O(set bits)
BitPos get_set_bits(Bitboard b)
{
	BitPos res;
	res.reserve(64);
	for (int n = 0; b; ++n, b &= (b - 1))
	{
		res.push_back(log2(b & ~(b - 1)));
	}
	return res;
}

struct Move;

class ChessBoard
{
 public:
	enum Position
	{
		a1,b1,c1,d1,e1,f1,g1,h1,
		a2,b2,c2,d2,e2,f2,g2,h2,
		a3,b3,c3,d3,e3,f3,g3,h3,
		a4,b4,c4,d4,e4,f4,g4,h4,
		a5,b5,c5,d5,e5,f5,g5,h5,
		a6,b6,c6,d6,e6,f6,g6,h6,
		a7,b7,c7,d7,e7,f7,g7,h7,
		a8,b8,c8,d8,e8,f8,g8,h8,
		InvalidPos
	};

	enum Piece
	{
		nWhite,  // all white pieces
		nBlack,  // all black pieces
		nPawn,   // all pawns
		nKnight, // all knights
		nBishop, // all bishops
		nRook,   // all rooks
		nQueen,  // all queens
		nKing,   // all kings
		InvalidPiece
	};

	ChessBoard();

	void disp_cboard(const std::string& header = "Board");

	void quite_move(Move m);
	void capture_move(Move m);

 private:
	Bitboard pieceBB[8];
	Bitboard occupied_sq;
	Bitboard empty_sq;
};

typedef ChessBoard::Piece Piece;
typedef ChessBoard::Position Position;

ChessBoard::ChessBoard()
{
	pieceBB[nWhite]  = C64(0x000000000000ffff);
	pieceBB[nBlack]  = C64(0xffff000000000000);
	pieceBB[nPawn]   = C64(0x00ff00000000ff00);
	pieceBB[nKnight] = C64(0x4200000000000042);
	pieceBB[nBishop] = C64(0x2400000000000024);
	pieceBB[nRook]   = C64(0x8100000000000081);
	pieceBB[nQueen]  = C64(0x0800000000000008);
	pieceBB[nKing]   = C64(0x1000000000000010);

	occupied_sq = pieceBB[nWhite] | pieceBB[nBlack];
	empty_sq = ~occupied_sq;
}

void ChessBoard::disp_cboard(const std::string& header)
{
	std::cout << header << ": \n\n";
	char board_str[65];
	BitPos onebits, zerobits;
	onebits = get_set_bits(pieceBB[nPawn] & pieceBB[nWhite]); for (short i : onebits) board_str[i] = 'P';
	onebits = get_set_bits(pieceBB[nKnight] & pieceBB[nWhite]); for (short i : onebits) board_str[i] = 'N';
	onebits = get_set_bits(pieceBB[nBishop] & pieceBB[nWhite]); for (short i : onebits) board_str[i] = 'B';
	onebits = get_set_bits(pieceBB[nRook] & pieceBB[nWhite]); for (short i : onebits) board_str[i] = 'R';
	onebits = get_set_bits(pieceBB[nQueen] & pieceBB[nWhite]); for (short i : onebits) board_str[i] = 'Q';
	onebits = get_set_bits(pieceBB[nKing] & pieceBB[nWhite]); for (short i : onebits) board_str[i] = 'K';
	onebits = get_set_bits(pieceBB[nPawn] & pieceBB[nBlack]); for (short i : onebits) board_str[i] = 'p';
	onebits = get_set_bits(pieceBB[nKnight] & pieceBB[nBlack]); for (short i : onebits) board_str[i] = 'n';
	onebits = get_set_bits(pieceBB[nBishop] & pieceBB[nBlack]); for (short i : onebits) board_str[i] = 'b';
	onebits = get_set_bits(pieceBB[nRook] & pieceBB[nBlack]); for (short i : onebits) board_str[i] = 'r';
	onebits = get_set_bits(pieceBB[nQueen] & pieceBB[nBlack]); for (short i : onebits) board_str[i] = 'q';
	onebits = get_set_bits(pieceBB[nKing] & pieceBB[nBlack]); for (short i : onebits) board_str[i] = 'k';
	zerobits = get_set_bits(empty_sq); for (short i : zerobits) board_str[i] = '-';
	board_str[64] = '\0'; //needed just incase you want to print board_str

	std::string s;
	for (int i = 63; i >= 0; --i)
	{
		if (((i + 1) % 8))
		{
			s += ' '; s += ' ';
		}
		s += board_str[i];
		if (i % 8 == 0)
		{
			if (s.length())
			{
				// derive rank number
				std::reverse(s.begin(), s.end());
				s = std::to_string(1 + int(i/8)) + " | " + s;
				std::cout << s << "\n";
				s.clear();
			}
		}
	}
	std::cout << "---------------------------\n";
	std::cout << "  | a  b  c  d  e  f  g  h  \n\n";
}

struct Move
{
	Position from, to;
	Piece piece, color, c_piece, c_color;

	Move()
	{
		from = to = ChessBoard::InvalidPos;
		piece = color = c_piece = c_color = ChessBoard::InvalidPiece;
	}

	Move(Position from_, Position to_, Piece piece_, Piece color_, Piece c_piece_ = ChessBoard::InvalidPiece, Piece c_color_ = ChessBoard::InvalidPiece)
	{
		from = from_;
		to = to_;
		piece = piece_;
		color = color_;
		c_piece = c_piece_;
		c_color = c_color_;
	}
};

void ChessBoard::quite_move(Move m)
{
 	Bitboard fromBB = lookup[m.from];
	Bitboard toBB = lookup[m.to];
	Bitboard fromtoBB = fromBB ^ toBB;
	pieceBB[m.piece] ^= fromtoBB;
	pieceBB[m.color] ^= fromtoBB;
	occupied_sq ^= fromtoBB;
	empty_sq ^= fromtoBB;
}

void ChessBoard::capture_move(Move m)
{
 	Bitboard fromBB = lookup[m.from];
	Bitboard toBB = lookup[m.to];
	Bitboard fromtoBB = fromBB ^ toBB;
	pieceBB[m.piece] ^= fromtoBB;
	pieceBB[m.color] ^= fromtoBB;
	pieceBB[m.c_piece] ^= toBB;
	pieceBB[m.c_color] ^= toBB;
	occupied_sq ^= fromBB;
	empty_sq ^= fromBB;
}

Bitboard upper_bits(Position pos)
{
	return C64(~1) << pos;
}

Bitboard lower_bits(Position pos)
{
	return (C64(1) << pos) - 1;
}

// http://chessprogramming.wikispaces.com/General+Setwise+Operations#Shifting Bitboards-Swapping Bits
/**
 * swap n non-overlapping bits from index "from" to index "to"
 * @param b any bitboard
 * @param from,to positions on bitboard
 * @param n number of consecutive bits to swap
 * @return bitboard with swapped bit-sequences
 */
Bitboard swap_n_bits(Bitboard b, Position from, Position to, int n)
{
	Bitboard m = (1 << n) - 1;
	Bitboard x = ((b >> from) ^ (b >> to)) & m;
	return ((x << from) ^ (x << to)) ^ b;
}

/**
 * swap any non-overlapping pairs of bits that are delta places apart
 * @param b any bitboard
 * @param mask has a 1 on the least significant position
 *        for each pair supposed to be swapped
 * @param delta of pairwise swapped bits
 * @return bitboard b with bits swapped
 */

Bitboard delta_swap(Bitboard b, Bitboard mask, int delta)
{
	Bitboard x = (b ^ (b >> delta)) & mask;
	return (x ^ (x << delta)) ^ b;
}

#endif /* BITBOARD_H_ */
