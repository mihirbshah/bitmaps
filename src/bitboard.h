/*
 * bitboard.h
 *
 *  Created on: Feb 22, 2017
 *      Author: mihir
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <cassert>

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
	const Bitboard not_g_h_file = C64(0x3f3f3f3f3f3f3f3f);
	const Bitboard not_a_b_file = C64(0xfcfcfcfcfcfcfcfc);
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

// Get LS1B
Bitboard ls1b_of_x(Bitboard b)
{
	return b & (-b);
}

// Reset LS1B
Bitboard reset_ls1b_of_x(Bitboard b)
{
	return b & (b - 1);
}

// Get mask of all bits above LS1B set
Bitboard above_ls1b_mask(Bitboard b)
{
	return b ^ (-b);
}

// Get mask of all bits below (including) LS1B
Bitboard below_ls1b_mask_including(Bitboard b)
{
	return b ^ (b - 1);
}

// Get mask of all bits below LS1B
Bitboard below_ls1b_mask(Bitboard b)
{
	return (~b) & (b - 1);
}

// Paint all bits above LS1B with 1b
Bitboard smear_ls1b_up(Bitboard b)
{
	return b | (-b);
}

// Paint all bits below LS1B with 1b
Bitboard smear_ls1b_down(Bitboard b)
{
	return b | (b - 1);
}

// Get LS0B
Bitboard ls0b_of_x(Bitboard b)
{
	return b | ~(b + 1);
}

// Set LS0B
Bitboard set_ls0b_of_x(Bitboard b)
{
	return b | (b + 1);
}

// Get mask of all bits below (including) LS0B
Bitboard below_ls0b_mask_including(Bitboard b)
{
	return b ^ (b + 1);
}

// Get mask of all bits below LS0B
Bitboard below_ls0b_mask(Bitboard b)
{
	return b & ~(b + 1);
}

// Get mask of all bits above LS0B set
Bitboard above_ls0b_mask(Bitboard b)
{
	return ~below_ls0b_mask_including(b);
}

// Paint all bits above LS0B with 0b
Bitboard smear_ls0b_up(Bitboard b)
{
	return below_ls0b_mask(b);
}

// Paint all bits below LS0B with 0b
Bitboard smear_ls0b_down(Bitboard b)
{
	return b & (b + 1);
}

bool empty(Bitboard b) { return !b; }

// Bitboard is power of 2
bool popcnt_eq_1(Bitboard b)
{
	return (!empty(b) && !reset_ls1b_of_x(b));
}

// popcnt = 0 or 1
bool popcnt_le_1(Bitboard b)
{
	return !reset_ls1b_of_x(b);
}

// popcnt > 1
bool popcnt_gt_1(Bitboard b)
{
	return reset_ls1b_of_x(b);
}

// x % 2^n == x & (2^n - 1)
Bitboard mod(Bitboard x, Bitboard y)
{
	if (popcnt_eq_1(y)) return x & (y - 1);
	return x % y;
}

int pop_cnt_0(Bitboard b)
{
	int cnt = 0;
	for (int i = 0; i < 64; ++i, b >>= 1)
		cnt += (int)(b & 1);
	return cnt;
}

// population count - Kernighan way
int pop_cnt_1(Bitboard b)
{
	int cnt = 0;
	while (b)
	{
		++cnt;
		b = reset_ls1b_of_x(b);
	}
	return cnt;
}

// population count - lookup table
unsigned char pop_cnt_of_byte[256];
void init_pop_cnt_of_byte()
{
	pop_cnt_of_byte[0] = 0;
	for (int i = 1; i < 256; ++i)
		pop_cnt_of_byte[i] = pop_cnt_of_byte[i/2] + mod(i,2);
}

int pop_cnt_2(Bitboard b)
{
	return pop_cnt_of_byte[b         & 0xff] +
		   pop_cnt_of_byte[(b >> 8)  & 0xff] +
		   pop_cnt_of_byte[(b >> 16) & 0xff] +
		   pop_cnt_of_byte[(b >> 24) & 0xff] +
		   pop_cnt_of_byte[(b >> 32) & 0xff] +
		   pop_cnt_of_byte[(b >> 40) & 0xff] +
		   pop_cnt_of_byte[(b >> 48) & 0xff] +
		   pop_cnt_of_byte[(b >> 56) & 0xff];
}

// Return bit index of LS1B
int bit_scan_forward(Bitboard b)
{
	assert(b);
	// slow
	//return log2(ls1b_of_x(b));

	// fast (because it avoids negation)
	// return log2(below_ls1b_mask_including(b));

	// faster avoids log2
	return pop_cnt_1(below_ls1b_mask_including(b)) - 1;
}

int bit_scan_reverse(Bitboard b)
{
	return floor(log2(b));
}

int trailing_zero_cnt(Bitboard b)
{
	if (b) return bit_scan_forward(b);
	return 64;
}

int leading_zero_cnt(Bitboard b)
{
	if (b) return bit_scan_reverse(b) ^ 63; // xor with 63 because 63 is the largest value return by bit_scan_reverse
	return 64;
}

Bitboard sum(Bitboard a, Bitboard b)
{
	return (a ^ b) + 2 * (a & b);
}

Bitboard flip_vertical(Bitboard b)
{
  Bitboard sel_mask_1 = C64(0xff000000000000ff);
  Bitboard sel_mask_2 = C64(0x00ff00000000ff00);
  Bitboard sel_mask_3 = C64(0x0000ff0000ff0000);
  Bitboard sel_mask_4 = C64(0x000000ffff000000);
  Bitboard and_mask_1 = C64(0x00000000000000ff);
  Bitboard and_mask_2 = C64(0x000000000000ff00);
  Bitboard and_mask_3 = C64(0x0000000000ff0000);
  Bitboard and_mask_4 = C64(0x00000000ff000000);
  return delta_swap(b & sel_mask_1, and_mask_1, (ChessBoard::a8 - ChessBoard::a1)) |
         delta_swap(b & sel_mask_2, and_mask_2, (ChessBoard::a7 - ChessBoard::a2)) |
         delta_swap(b & sel_mask_3, and_mask_3, (ChessBoard::a6 - ChessBoard::a3)) |
         delta_swap(b & sel_mask_4, and_mask_4, (ChessBoard::a5 - ChessBoard::a4));
}

Bitboard mirror_horizontal(Bitboard b)
{
  Bitboard sel_mask_1 = C64(0x8181818181818181);
  Bitboard sel_mask_2 = C64(0x4242424242424242);
  Bitboard sel_mask_3 = C64(0x2424242424242424);
  Bitboard sel_mask_4 = C64(0x1818181818181818);
  Bitboard and_mask_1 = C64(0x0101010101010101);
  Bitboard and_mask_2 = C64(0x0202020202020202);
  Bitboard and_mask_3 = C64(0x0404040404040404);
  Bitboard and_mask_4 = C64(0x0808080808080808);
  return delta_swap(b & sel_mask_1, and_mask_1, (ChessBoard::h1 - ChessBoard::a1)) |
         delta_swap(b & sel_mask_2, and_mask_2, (ChessBoard::g1 - ChessBoard::b1)) |
         delta_swap(b & sel_mask_3, and_mask_3, (ChessBoard::f1 - ChessBoard::c1)) |
         delta_swap(b & sel_mask_4, and_mask_4, (ChessBoard::e1 - ChessBoard::d1));
}

Bitboard flip_diag_a1h8(Bitboard b) {
  Bitboard t;
  const Bitboard k1 = C64(0x5500550055005500);
  const Bitboard k2 = C64(0x3333000033330000);
  const Bitboard k4 = C64(0x0f0f0f0f00000000);
  t  = k4 & (b ^ (b << 28));
  b ^=       t ^ (t >> 28) ;
  t  = k2 & (b ^ (b << 14));
  b ^=       t ^ (t >> 14) ;
  t  = k1 & (b ^ (b <<  7));
  b ^=       t ^ (t >>  7) ;
  return b;
}

Bitboard flip_diag_a8h1(Bitboard b) {
  Bitboard t;
  const Bitboard k1 = C64(0xaa00aa00aa00aa00);
  const Bitboard k2 = C64(0xcccc0000cccc0000);
  const Bitboard k4 = C64(0xf0f0f0f00f0f0f0f);
  t  =       b ^ (b << 36) ;
  b ^= k4 & (t ^ (b >> 36));
  t  = k2 & (b ^ (b << 18));
  b ^=       t ^ (t >> 18) ;
  t  = k1 & (b ^ (b <<  9));
  b ^=       t ^ (t >>  9) ;
  return b;
}

Bitboard rotate180(Bitboard b)
{
	return flip_vertical(mirror_horizontal(b));
}

Bitboard rotate90_clockwise(Bitboard b)
{
	return flip_vertical(flip_diag_a1h8(b));
}

Bitboard rotate90_anticlockwise(Bitboard b)
{
	return flip_vertical(flip_diag_a8h1(b));
}

Bitboard pawn_north_fill(Bitboard b)
{
	b |= (b << 8);
	b |= (b << 16);
	b |= (b << 32);
	return b;
}

Bitboard pawn_south_fill(Bitboard b)
{
	b |= (b >> 8);
	b |= (b >> 16);
	b |= (b >> 32);
	return b;
}

Bitboard pawn_file_fill(Bitboard b)
{
	return pawn_north_fill(b) | pawn_south_fill(b);
}

Bitboard wpawn_front_fill(Bitboard b)
{
	return pawn_north_fill(b);
}

Bitboard wpawn_rear_fill(Bitboard b)
{
	return pawn_south_fill(b);
}

Bitboard bpawn_front_fill(Bitboard b)
{
	return pawn_south_fill(b);
}

Bitboard bpawn_rear_fill(Bitboard b)
{
	return pawn_north_fill(b);
}

Bitboard wpawn_front_span(Bitboard b)
{
	return north_one(pawn_north_fill(b));
}

Bitboard wpawn_rear_span(Bitboard b)
{
	return south_one(pawn_south_fill(b));
}

Bitboard bpawn_front_span(Bitboard b)
{
	return south_one(pawn_south_fill(b));
}

Bitboard bpawn_rear_span(Bitboard b)
{
	return north_one(pawn_north_fill(b));
}

Bitboard pawn_interspans(Bitboard wpawn, Bitboard bpawn)
{
	return wpawn_front_span(wpawn) & bpawn_front_span(bpawn);
}

Bitboard wpawn_stop(Bitboard b)
{
	return north_one(b);
}

Bitboard bpawn_stop(Bitboard b)
{
	return south_one(b);
}

Bitboard wpawn_telestops(Bitboard b)
{
	return wpawn_front_span(b) ^ wpawn_stop(b);
}

Bitboard bpawn_telestops(Bitboard b)
{
	return bpawn_front_span(b) ^ bpawn_stop(b);
}

Bitboard wpawn_east_attack_front_spans(Bitboard b)
{
	return east_one(wpawn_front_span(b));
}

Bitboard wpawn_west_attack_front_spans(Bitboard b)
{
	return west_one(wpawn_front_span(b));
}

Bitboard bpawn_east_attack_front_spans(Bitboard b)
{
	return east_one(bpawn_front_span(b));
}

Bitboard bpawn_west_attack_front_spans(Bitboard b)
{
	return west_one(bpawn_front_span(b));
}

Bitboard wpawn_east_attack_rear_spans(Bitboard b)
{
	return east_one(wpawn_rear_fill(b));
}

Bitboard wpawn_west_attack_rear_spans(Bitboard b)
{
	return west_one(wpawn_rear_fill(b));
}

Bitboard bpawn_east_attack_rear_spans(Bitboard b)
{
	return east_one(bpawn_rear_fill(b));
}

Bitboard bpawn_west_attack_rear_spans(Bitboard b)
{
	return west_one(bpawn_rear_fill(b));
}

Bitboard pawn_east_attacks_file_fill(Bitboard b)
{
	return east_one(pawn_file_fill(b));
}

Bitboard pawn_west_attacks_file_fill(Bitboard b)
{
	return west_one(pawn_file_fill(b));
}



Bitboard south_attacks(Bitboard piece_bb, Bitboard empty)
{
	Bitboard flood = piece_bb;
	flood |= piece_bb = (piece_bb >> 8) & empty;
	flood |= piece_bb = (piece_bb >> 8) & empty;
	flood |= piece_bb = (piece_bb >> 8) & empty;
	flood |= piece_bb = (piece_bb >> 8) & empty;
	flood |= piece_bb = (piece_bb >> 8) & empty;
	flood |= piece_bb = (piece_bb >> 8) & empty;
	flood |=            (piece_bb >> 8) & empty;
	return south_one(flood);
}

Bitboard north_attacks(Bitboard piece_bb, Bitboard empty)
{
	Bitboard flood = piece_bb;
	flood |= piece_bb = (piece_bb << 8) & empty;
	flood |= piece_bb = (piece_bb << 8) & empty;
	flood |= piece_bb = (piece_bb << 8) & empty;
	flood |= piece_bb = (piece_bb << 8) & empty;
	flood |= piece_bb = (piece_bb << 8) & empty;
	flood |= piece_bb = (piece_bb << 8) & empty;
	flood |=            (piece_bb << 8) & empty;
	return north_one(flood);
}

Bitboard east_attacks(Bitboard piece_bb, Bitboard empty)
{
	Bitboard flood = piece_bb;
	empty &= not_a_file;
	flood |= piece_bb = (piece_bb << 1) & empty;
	flood |= piece_bb = (piece_bb << 1) & empty;
	flood |= piece_bb = (piece_bb << 1) & empty;
	flood |= piece_bb = (piece_bb << 1) & empty;
	flood |= piece_bb = (piece_bb << 1) & empty;
	flood |= piece_bb = (piece_bb << 1) & empty;
	flood |=            (piece_bb << 1) & empty;
	return east_one(flood);
}

Bitboard west_attacks(Bitboard piece_bb, Bitboard empty)
{
	Bitboard flood = piece_bb;
	empty &= not_h_file;
	flood |= piece_bb = (piece_bb >> 1) & empty;
	flood |= piece_bb = (piece_bb >> 1) & empty;
	flood |= piece_bb = (piece_bb >> 1) & empty;
	flood |= piece_bb = (piece_bb >> 1) & empty;
	flood |= piece_bb = (piece_bb >> 1) & empty;
	flood |= piece_bb = (piece_bb >> 1) & empty;
	flood |=            (piece_bb >> 1) & empty;
	return west_one(flood);
}

Bitboard ne_attacks(Bitboard piece_bb, Bitboard empty)
{
	Bitboard flood = piece_bb;
	empty &= not_a_file;
	flood |= piece_bb = (piece_bb << 9) & empty;
	flood |= piece_bb = (piece_bb << 9) & empty;
	flood |= piece_bb = (piece_bb << 9) & empty;
	flood |= piece_bb = (piece_bb << 9) & empty;
	flood |= piece_bb = (piece_bb << 9) & empty;
	flood |= piece_bb = (piece_bb << 9) & empty;
	flood |=            (piece_bb << 9) & empty;
	return ne_one(flood);
}

Bitboard nw_attacks(Bitboard piece_bb, Bitboard empty)
{
	Bitboard flood = piece_bb;
	empty &= not_h_file;
	flood |= piece_bb = (piece_bb << 7) & empty;
	flood |= piece_bb = (piece_bb << 7) & empty;
	flood |= piece_bb = (piece_bb << 7) & empty;
	flood |= piece_bb = (piece_bb << 7) & empty;
	flood |= piece_bb = (piece_bb << 7) & empty;
	flood |= piece_bb = (piece_bb << 7) & empty;
	flood |=            (piece_bb << 7) & empty;
	return nw_one(flood);
}

Bitboard se_attacks(Bitboard piece_bb, Bitboard empty)
{
	Bitboard flood = piece_bb;
	empty &= not_a_file;
	flood |= piece_bb = (piece_bb >> 7) & empty;
	flood |= piece_bb = (piece_bb >> 7) & empty;
	flood |= piece_bb = (piece_bb >> 7) & empty;
	flood |= piece_bb = (piece_bb >> 7) & empty;
	flood |= piece_bb = (piece_bb >> 7) & empty;
	flood |= piece_bb = (piece_bb >> 7) & empty;
	flood |=            (piece_bb >> 7) & empty;
	return se_one(flood);
}

Bitboard sw_attacks(Bitboard piece_bb, Bitboard empty)
{
	Bitboard flood = piece_bb;
	empty &= not_h_file;
	flood |= piece_bb = (piece_bb >> 9) & empty;
	flood |= piece_bb = (piece_bb >> 9) & empty;
	flood |= piece_bb = (piece_bb >> 9) & empty;
	flood |= piece_bb = (piece_bb >> 9) & empty;
	flood |= piece_bb = (piece_bb >> 9) & empty;
	flood |= piece_bb = (piece_bb >> 9) & empty;
	flood |=            (piece_bb >> 9) & empty;
	return sw_one(flood);
}

// TODO Implement Kogge-Stone algorithm for occluded fill & compare with Dumb7 fill above


Bitboard knight_attacks(Bitboard b)
{
	Bitboard l1 = (b >> 1) & not_h_file;
	Bitboard l2 = (b >> 2) & not_g_h_file;
	Bitboard r1 = (b << 1) & not_a_file;
	Bitboard r2 = (b << 2) & not_a_b_file;
	Bitboard h1 = l1 | r1;
	Bitboard h2 = l2 | r2;
	return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

int calc_knight_distance(Bitboard b1, Bitboard b2)
{
	assert(b1 && b2);
	int d = 0;
	while ((b1 & b2) == 0)
	{
		b1 = knight_attacks(b1); ++d;
	}
	return d;
}

int knight_distance(ChessBoard::Position source, ChessBoard::Position target)
{
	return calc_knight_distance(C64(1) << source, C64(1) << target);
}
