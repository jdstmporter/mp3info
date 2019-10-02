/*
 * util.cpp
 *
 *  Created on: 2 Oct 2019
 *      Author: julianporter
 */


#include "base.hpp"
#include <fstream>




char16_t from8(char a,char b,bool rev) {
		unsigned char x = rev ? b : a;
		unsigned char y = rev ? a : b;
		return ((char16_t)(x<< 8)) + ((char16_t)y);
	}

mdata_t readFile(std::istream &stream) {
	stream.seekg (0, stream.end);
	auto length = stream.tellg();
	stream.seekg (0, stream.beg);

	mdata_t file(length,0);
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



