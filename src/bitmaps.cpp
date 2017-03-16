//============================================================================
// Name        : bitmaps.cpp
// Author      : Mihir
// Version     :
// Copyright   : copy at your own risk
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "bitboard.h"

int main() {
    /*Bitboard a_file = C64(0x0101010101010101);
    disp_bb(a_file, "a-file");
    Bitboard h_file = C64(0x8080808080808080);
    disp_bb(h_file, "h-file");
    Bitboard rank_8 = C64(0xFF00000000000000);
    disp_bb(rank_8, "rank 8");
    Bitboard rank_1 = C64(0x00000000000000FF);
    disp_bb(rank_1, "rank 1");
    Bitboard a1h8_diag = C64(0x8040201008040201);
    disp_bb(a1h8_diag, "a1h8 diagonal");
    Bitboard light_sq = C64(0x55AA55AA55AA55AA);
    disp_bb(light_sq, "light squares");*/

	/*Bitboard b1 = C64(0x0100000000000000);
	disp_bb(b1, "Input");
	disp_bb(east_one(b1), "Shift east");
	disp_bb(west_one(b1), "Shift west");
	disp_bb(north_one(b1), "Shift north");
	disp_bb(south_one(b1), "Shift south");
	disp_bb(ne_one(b1), "Shift north-east");
	disp_bb(nw_one(b1), "Shift north-west");
	disp_bb(se_one(b1), "Shift south-east");
	disp_bb(sw_one(b1), "Shift south-west");*/

	//std::cout << "res: " << (rotate_one(b1, west) == shift_one(b1, west));
	//disp_bb(rotate_one(b1, se), "Rotate south-east");
	//disp_bb(shift_one(b1, se), "Shift south-east");
	//disp_bb(se_one(b1), "Shift south-east");

	/*setup_lookup_table();
	short pos = 56;
	std::cout << "test bit: " << pos << ". Result: " << test(b1, pos) << std::endl;
	std::cout << "set bit: " << pos+1 << ". Result: " << std::endl;
	disp_bb(set(b1, pos+1), "After bit set");
	std::cout << "toggle bit: " << pos << ". Result: " << std::endl;
	disp_bb(toggle(b1, pos), "After bit toggle");
	std::cout << "reset bit: " << pos << ". Result: " << std::endl;
	disp_bb(reset(b1, pos), "After bit reset");*/

	/*BitPos onebits = get_set_bits(b1);
	for (short n : onebits)
		std::cout << n << ", ";*/

	/*setup_lookup_table();
	ChessBoard board;
	board.disp_cboard();
	Move m1(ChessBoard::e2, ChessBoard::e4, ChessBoard::nPawn, ChessBoard::nWhite);
	board.quite_move(m1);
	board.disp_cboard("After move e2-e4");
	m1 = Move(ChessBoard::d7, ChessBoard::d5, ChessBoard::nPawn, ChessBoard::nBlack);
	board.quite_move(m1);
	board.disp_cboard("After move d7-d5");
	m1 = Move(ChessBoard::e4, ChessBoard::d5, ChessBoard::nPawn, ChessBoard::nWhite, ChessBoard::nPawn, ChessBoard::nBlack);
	board.capture_move(m1);
	board.disp_cboard("After move e4-d5");*/

	//disp_bb(upper_bits(ChessBoard::c3), "upper bits for c3");
	//disp_bb(lower_bits(ChessBoard::e5), "lower bits for e5");

	/*Bitboard b1 = C64(0x00005c00002a0000);
	disp_bb(b1, "Input");
	disp_bb(swap_n_bits(b1, ChessBoard::b3, ChessBoard::c6, 5), "swap_5_bits between b3 and c6");
	disp_bb(delta_swap(b1, C64(0x3e0000), (ChessBoard::c6 - ChessBoard::b3)), "delta_swap with delta=c6-b3");*/

	/*Bitboard b1 = C64(0x14220022140000);
	disp_bb(b1, "Input");
	disp_bb(ls1b_of_x(b1), "ls1b_of_x");
	disp_bb(reset_ls1b_of_x(b1), "reset_ls1b_of_x");
	disp_bb(above_ls1b_mask(b1), "above_ls1b_mask");
	disp_bb(below_ls1b_mask_including(b1), "below_ls1b_mask_including");
	disp_bb(below_ls1b_mask(b1), "below_ls1b_mask");
	disp_bb(smear_ls1b_up(b1), "smear_ls1b_up");
	disp_bb(smear_ls1b_down(b1), "smear_ls1b_down");*/

	/*Bitboard b1 = C64(0x80c0e0f0f8fcfeff);
	disp_bb(b1, "Input");
	disp_bb(ls0b_of_x(b1), "ls0b_of_x");
	disp_bb(set_ls0b_of_x(b1), "set_ls0b_of_x");
	disp_bb(below_ls0b_mask_including(b1), "below_ls0b_mask_including");
	disp_bb(below_ls0b_mask(b1), "below_ls0b_mask");
	disp_bb(above_ls0b_mask(b1), "above_ls0b_mask");
	disp_bb(smear_ls0b_up(b1), "smear_ls0b_up");
	disp_bb(smear_ls0b_down(b1), "smear_ls0b_down");*/

	Bitboard b1 = C64(0x80c0e0f0f8fcfeff);
	disp_bb(b1, "Input");
	std::cout << "Population count(1) for b1: " << pop_cnt_1(b1) << std::endl;
	init_pop_cnt_of_byte();
	std::cout << "Population count(2) for b1: " << pop_cnt_2(b1) << std::endl;



	return 0;
}
