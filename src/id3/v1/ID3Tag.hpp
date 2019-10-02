/*
 * ID3Tag.hpp
 *
 *  Created on: 2 Oct 2019
 *      Author: julianporter
 */

#ifndef ID3V1_ID3TAG_HPP_
#define ID3V1_ID3TAG_HPP_

#include "../../base.hpp"
namespace id3 { namespace v1 {

class Tag {
private:
	BinaryFile data;
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
	Tag(const BinaryFile &file) : data(file), fields(), _genre(255), _track(0), exists(false) {};
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


}}



#endif /* ID3TAG_HPP_ */
