/*
 * util.hpp
 *
 *  Created on: 2 Oct 2019
 *      Author: julianporter
 */

#ifndef BASE_MP3INFO_HPP_
#define BASE_MP3INFO_HPP_

#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <type_traits>
#include <cstdint>
#include <string>
#include <algorithm>

using mdata_t = std::vector<char>;
using it_t = mdata_t::iterator;


char16_t from8(char a,char b,bool rev=false);

class BinaryFile {
private:
	mdata_t data;

public:
	using iterator=mdata_t::iterator;
	using const_iterator=mdata_t::const_iterator;

	BinaryFile() : data() {};
	BinaryFile(const std::string &filename);
	BinaryFile(std::istream &stream);
	BinaryFile(const BinaryFile &) = default;
	BinaryFile & operator=(const BinaryFile &) = default;
	virtual ~BinaryFile() = default;

	iterator begin() { return data.begin(); }
	iterator end() { return data.end(); }
	const_iterator cbegin() const { return data.cbegin(); }
	const_iterator cend() const { return data.cend(); }
	long size() { return data.size(); }
	operator mdata_t() { return data; }
};


#endif /* BASE_HPP_ */
