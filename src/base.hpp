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
#include <exception>
#include <stdexcept>

namespace base {


using vec_t = std::vector<char>;
using it_t = std::vector<char>::iterator;


char16_t from8(char a,char b,bool rev=false);

class BinaryFile {
public:
	using iterator=vec_t::iterator;
	using const_iterator=vec_t::const_iterator;
private:
	vec_t data;

public:
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
	operator vec_t() { return data; }
};

class MP3Exception : public std::exception {
private:
	std::string message;

public:
	MP3Exception() : std::exception(), message() {}
	MP3Exception(const std::string &m) : std::exception(), message(m) {}
	MP3Exception(const char *m) : std::exception(), message(m) {}
	MP3Exception(const MP3Exception &) = default;
	MP3Exception & operator=(const MP3Exception &) = default;
	virtual ~MP3Exception() = default;

	virtual const char *what() const noexcept { return message.c_str(); }
	operator std::string() { return message; }
};


class ISO8859 {
private:

	std::string _utf8;
	vec_t _iso;

	static std::string toUTF(it_t begin,it_t end);
	static vec_t toISO(const std::string &s);
public:
	ISO8859(it_t begin,it_t end) : _utf8(toUTF(begin,end)), _iso(begin,end) {};
	ISO8859(const std::string &s) : _utf8(s), _iso(toISO(s)) {};
	virtual ~ISO8859()=default;

	operator std::string() const { return _utf8; }
	operator vec_t() const { return _iso; }

};

}


#endif /* BASE_HPP_ */
