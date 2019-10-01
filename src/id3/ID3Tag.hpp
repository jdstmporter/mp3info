/*
 * ID3Frame.hpp
 *
 *  Created on: 29 Sep 2019
 *      Author: julianporter
 */

#ifndef PCM2MP3_CPP_SRC_ID3_ID3FRAME_HPP_
#define PCM2MP3_CPP_SRC_ID3_ID3FRAME_HPP_


#include <sstream>
#include <map>
#include <memory>
#include <type_traits>

#include "ID3Header.hpp"
#include "ID3Types.hpp"

namespace id3 {

using mdata_t = std::vector<char>;
mdata_t readBinaryFile(std::istream &stream);

namespace v1 {

class Tag {
private:
	mdata_t data;
	std::map<std::string,std::string> fields;
	unsigned _genre;
	unsigned _track;

	const static std::vector<std::pair<std::string,unsigned>> names;

	bool exists;

	std::string get(const std::string &field) const {
		try {
			return fields.at(field);
		}
		catch(...) {
			return "";
		}
	}

public:
	Tag(std::istream &stream) : data(id3::readBinaryFile(stream)), fields(), _genre(255), _track(0), exists(false) {};
	virtual ~Tag() = default;
	bool parse();

	std::string title() const { return get("title") ; }
	std::string artist() const { return get("artist") ; }
	std::string album() const { return get("album") ; }
	std::string year() const { return get("year") ; }
	std::string comment() const { return get("comment") ; }
	unsigned track() const { return _track; }
	unsigned genre() const { return _genre; }
	bool hasTag() const { return exists; }
	operator std::string() const;
};



}

namespace v2 {

class Tag {
private:
	mdata_t data;
	Header header;
	void findHeader();

public:
	Tag(std::istream &stream) : data(id3::readBinaryFile(stream)), header() {};
	virtual ~Tag() = default;
	bool parse();
};


}} /* namespace id3 */



#endif /* PCM2MP3_CPP_SRC_ID3_ID3FRAME_HPP_ */
