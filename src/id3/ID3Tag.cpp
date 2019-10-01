/*
 * ID3Frame.cpp
 *
 *  Created on: 29 Sep 2019
 *      Author: julianporter
 */

#include "ID3Tag.hpp"
#include "ID3Frame.hpp"

#include <sstream>
#include <iostream>
#include <cctype>


namespace id3 {

mdata_t readBinaryFile(std::istream &stream) {
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

namespace v1 {

const std::vector<std::pair<std::string,unsigned>> Tag::names = {
		std::make_pair("Title",30),
		std::make_pair("Artist",30),
		std::make_pair("Album",30),
		std::make_pair("Year",4),
		std::make_pair("Comment",30)
};


bool Tag::parse() {
	if(data.size()<128) {
		exists=false;
		return false;
	}
	auto it=data.begin()+data.size()-128;
	if(std::string(it,it+3)!="TAG") {
		exists=false;
		return false;
	}
	it+=3;
	for(auto field = Tag::names.begin();field!=Tag::names.end();field++) {
		auto name=field->first;
		auto length=field->second;
		fields[name]=std::string(it,it+length);
		it+=length;
	}
	_genre=(unsigned)(unsigned char)*it;
	return true;
}

Tag::operator std::string() const {
	std::stringstream s;

	for(auto field = Tag::names.begin();field!=Tag::names.end();field++) {
		auto name=field->first;
		s << name << " : " << get(name) << std::endl;
	}
	s << "Genre : " << genre() << std::endl;
	return s.str();
}


}


namespace v2 {




void Tag::findHeader() {
	long offset=0;
	while(offset<data.size()-10) {
		try {
			Header h(data,offset);
			if(h.check()) {
				header=h;
				return;
			}
		}
		catch(...) {}
		offset+=1;
	}
	throw std::runtime_error("No ID3");
}

//bool isGood(it_t p) {
//	return std::all_of(p,p+4,[](char c) { return isalnum(c) && ! islower(c); });
//}

bool Tag::parse() {
	try {
		findHeader();
		std::cout << header << std::endl;
		std::vector<char> tag(header.length,0);
		std::copy_n(data.begin() + header.offset+10,(unsigned)header.length,tag.begin());
		std::cout << "Constructed tag vector of length " << tag.size() << std::endl;
		it_t it = tag.begin();
		if(header.flags.hasExtendedHeader()) {
			unsigned l=SyncSafeInteger(it);
			it+=l;
			std::cout << "Extended header: skipped " << l << " bytes" << std::endl;
		}
		else std::cout << "No extended header" << std::endl;
		while(it!=tag.end()) {
			std::cout << "Checking at offset " << (it-tag.begin()) << std::endl;
			Frame frame(it,tag.end());
			if(!frame.parse()) break;
			std::cout << (std::string)frame << std::endl;
			it+=10+frame.size();
		}
		return true;
	}
	catch(...) {
		return false;
	}
}



} }/* namespace id3 */


