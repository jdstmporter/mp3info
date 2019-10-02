/*
 * ID3Frame.hpp
 *
 *  Created on: 29 Sep 2019
 *      Author: julianporter
 */

#ifndef PCM2MP3_CPP_SRC_ID3V2_ID3FRAME_HPP_
#define PCM2MP3_CPP_SRC_ID3V2_ID3FRAME_HPP_




#include "ID3Header.hpp"
#include "ID3Types.hpp"

namespace id3 { namespace v2 {

class Tag {
private:
	base::BinaryFile data;
	Header header;
	void findHeader();

public:
	Tag(const base::BinaryFile &file) : data(file), header() {};
	virtual ~Tag() = default;
	bool parse();
};


}} /* namespace id3 */



#endif /* PCM2MP3_CPP_SRC_ID3_ID3FRAME_HPP_ */
