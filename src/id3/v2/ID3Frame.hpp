/*
 * ID3Frame.hpp
 *
 *  Created on: 1 Oct 2019
 *      Author: julianporter
 */

#ifndef ID3FRAME_HPP_
#define ID3FRAME_HPP_

#include "ID3Types.hpp"

namespace id3 { namespace v2 {

class Frame {
private:
	std::string key;
	std::string content;
	std::string language;

	unsigned length;
	char16_t flags;

	it_t begin;
	it_t end;

public:
	Frame(it_t b,it_t e) : key(), content(), language(), length(0), flags(0), begin(b), end(e) {};
	virtual ~Frame() = default;

	bool parse();
	std::string frameTag() const { return key; }
	std::string frameContent() const { return content; }
	std::string frameLanguage() const { return language; }
	bool hasLanguage() const { return language!=""; }
	unsigned size() const { return length; }

	operator std::string() const;

};

}}




#endif /* ID3FRAME_HPP_ */
