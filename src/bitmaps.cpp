//============================================================================
// Name        : bitmaps.cpp
// Author      : Mihir
// Version     :
// Copyright   : copy at your own risk
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "bitboard.h"

int main() {
    Bitboard a_file = C64(0x0101010101010101);
    display_bb(a_file, "a-file");
    Bitboard h_file = C64(0x8080808080808080);
    display_bb(h_file, "h-file");
    Bitboard rank_8 = C64(0xFF00000000000000);
    display_bb(rank_8, "rank 8");
    Bitboard rank_1 = C64(0x00000000000000FF);
    display_bb(rank_1, "rank 1");
    Bitboard a1h8_diag = C64(0x8040201008040201);
    display_bb(a1h8_diag, "a1h8 diagonal");
    Bitboard light_sq = C64(0x55AA55AA55AA55AA);
    display_bb(light_sq, "light squares");
	return 0;
}
