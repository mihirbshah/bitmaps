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


#endif /* BITBOARD_H_ */
