/*
 * MP3Header.hpp
 *
 *  Created on: 28 Sep 2019
 *      Author: julianporter
 */

#ifndef PCM2MP3_CPP_SRC_INFO_MP3HEADER_HPP_
#define PCM2MP3_CPP_SRC_INFO_MP3HEADER_HPP_

#include <cstdint>
#include <iostream>

namespace mp3 {
struct MP3Header {
	/*
	 * AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM
	 */

	unsigned emphasis : 2;			// M
	unsigned original : 1;			// L
	unsigned copyright : 1;			// K
	unsigned modeExtension : 2;		// J
	unsigned mode : 2;				// I
	unsigned extension : 1;			// H
	unsigned pad : 1;				// G
	unsigned frequency : 2;			// F
	unsigned rate : 4;				// E
	unsigned crc : 1;				// D
	unsigned layer : 2;				// C
	unsigned version : 2;			// B
	unsigned id : 11;				// A
};

union MP3HeaderConverter {
	uint32_t bytes;
	MP3Header header;
	uint8_t b[4];

	MP3HeaderConverter() : bytes(0) {};
	void push(const char c) {
		bytes=(bytes<<8)+(uint8_t)c;
	};
	MP3HeaderConverter(const MP3Header h) : header(h) {};
	MP3HeaderConverter(const uint32_t b) : bytes(b) {};
};

}

std::ostream & operator<<(std::ostream &o,const mp3::MP3Header &header);

#endif /* PCM2MP3_CPP_SRC_INFO_MP3HEADER_HPP_ */
