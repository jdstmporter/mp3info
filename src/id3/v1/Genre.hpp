/*
 * Genre.hpp
 *
 *  Created on: 6 Oct 2019
 *      Author: julianporter
 */

#ifndef GENRE_HPP_
#define GENRE_HPP_

#include <base.hpp>
namespace id3 {
namespace v1 {

class Genre {
private:
	static const std::map<unsigned,std::string> code2Genre;
	unsigned value;

public:
	Genre() : value(255) {}
	Genre(const char c) : value((unsigned)(unsigned char)c) {};

	operator unsigned() const { return value; }
	operator std::string() const;
};

} /* namespace v1 */
} /* namespace id3 */

#endif /* GENRE_HPP_ */
