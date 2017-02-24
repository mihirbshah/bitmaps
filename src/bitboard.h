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

// macro to generate unsigned long long constants
#define C64(constantU64) constantU64##ULL

// 64 bit
typedef unsigned long long Bitboard;

void display_bb(Bitboard b, const std::string& header = std::string())
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
	const char* hdash  = "\u2500\u2500\u2500\u2500\u2500\u2500\u2500";
	std::cout << hdash << hdash << hdash << hdash << "\n";
	std::cout << "  | a  b  c  d  e  f  g  h  \n\n";
}

#endif /* BITBOARD_H_ */
