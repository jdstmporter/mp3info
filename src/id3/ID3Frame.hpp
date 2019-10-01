/*
 * ID3Frame.hpp
 *
 *  Created on: 29 Sep 2019
 *      Author: julianporter
 */

#ifndef PCM2MP3_CPP_SRC_ID3_ID3FRAME_HPP_
#define PCM2MP3_CPP_SRC_ID3_ID3FRAME_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <memory>
#include <type_traits>
#include "ID3FrameHeader.hpp"

namespace id3 { namespace v2 {


class Frame {
public:
	using mdata_t = std::vector<char>;

private:
	static mdata_t readBinaryFile(std::istream &stream);

	mdata_t data;
	Header header;

	void findHeader();

public:

	Frame(std::istream &stream) : data(Frame::readBinaryFile(stream)), header() {};
	virtual ~Frame() = default;

	bool parse();
};


}} /* namespace id3 */



#endif /* PCM2MP3_CPP_SRC_ID3_ID3FRAME_HPP_ */
