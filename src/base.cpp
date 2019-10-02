/*
 * util.cpp
 *
 *  Created on: 2 Oct 2019
 *      Author: julianporter
 */


#include "base.hpp"
#include <fstream>

namespace base {


char16_t from8(char a,char b,bool rev) {
		unsigned char x = rev ? b : a;
		unsigned char y = rev ? a : b;
		return ((char16_t)(x<< 8)) + ((char16_t)y);
	}

vec_t readFile(std::istream &stream) {
	stream.seekg (0, stream.end);
	auto length = stream.tellg();
	stream.seekg (0, stream.beg);

	vec_t file(length,0);
	auto c=file.data();
	int pos=0;
	while(pos<length) {
	   stream.read(c+pos,1024);
	   pos+=stream.gcount();
	}
	return file;
}

BinaryFile::BinaryFile(std::istream &stream) : data(readFile(stream)) {};
BinaryFile::BinaryFile(const std::string &filename) {
	std::ifstream mp(filename,std::ifstream::binary);
	if(mp.fail()) throw std::runtime_error("Cannot open file");
	data=readFile(mp);
};

std::string ISO8859::toUTF(it_t begin,it_t end ) {
	std::stringstream s;
	auto it=begin;
	while(it!=end) {
		auto c=(unsigned char)*(it++);
		if(c<128) s << (char)c;
		else {
			s << (char)(0x80 | (c&0x3f));
			s << (char)(0xc0 | (c>>6));
		}
	}
	return s.str();
}
vec_t ISO8859::toISO(const std::string &s) {
	vec_t out;
	auto it=s.begin();
	while(it!=s.end()) {
		auto c=(unsigned char)*(it++);
		if(c<128) out.push_back(c);
		else if (c>0xc0) {
			auto d=3&(unsigned char)*(it++);
			out.push_back((c&0x3f) | (d<<6));
		}
	}
	return out;
}


}

