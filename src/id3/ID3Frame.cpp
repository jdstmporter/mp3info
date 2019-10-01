/*
 * ID3Frame.cpp
 *
 *  Created on: 29 Sep 2019
 *      Author: julianporter
 */

#include <id3/ID3Frame.hpp>
#include <sstream>
#include <iostream>
#include <cctype>
#include "ID3Types.hpp"

namespace id3 { namespace v2 {

const std::string Header::tagString="ID3";

Header::Header(const mdata_t &data,const long o) : offset(o) {
	auto ptr=data.begin()+offset;
	std::copy_n(ptr,10,header);

	tag=std::string(header,3);
	version=Version(header[3],header[4]);
	flags=Flags(header[5]);
	length=SyncSafeInteger(header+6);
}

bool Header::check() const {
	if(tag!=Header::tagString) return false;
	if(!version) return false;
	if(!flags) return false;
	if(std::any_of(header+6,header+10,[](auto c) { return 127 < (unsigned)c; })) return false;
	return true;
}

std::vector<char> Frame::readBinaryFile(std::istream &stream) {
	stream.seekg (0, stream.end);
	auto length = stream.tellg();
	stream.seekg (0, stream.beg);

	std::vector<char> file(length,0);
	auto c=file.data();
	int pos=0;
	while(pos<length) {
	   stream.read(c+pos,1024);
	   pos+=stream.gcount();
	}
	return file;
}

void Frame::findHeader() {
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

bool isGood(char *p) {
	return std::all_of(p,p+4,[](char c) { return isalnum(c) && ! islower(c); });
}

bool Frame::parse() {
	try {
		findHeader();
		std::cout << header << std::endl;
		auto base = header.offset + 10;
		auto offset = 0;
		auto ptr = data.data() + base;
		if(header.flags.hasExtendedHeader()) {
			unsigned l=SyncSafeInteger(ptr+offset);
			offset+=l;
			std::cout << "Extended header: skipped " << l << " bytes" << std::endl;
		}
		while(offset<header.length) {
			std::cout << "Checking at offset " << offset << std::endl;
			if(!isGood(ptr+offset)) break;
			std::string key(ptr+offset,4);
			SyncSafeInteger l(ptr+offset+4);
			if(!l) break;
			auto flags0 = ptr[offset+8];
			auto flags1 = ptr[offset+9];

			auto lang=key=="COMM";
			StringField content(ptr+offset+10,l,lang);


			std::cout << key << " " << (uint32_t)l << " " << std::hex <<
					(unsigned)flags0 << ":" << (unsigned)flags1 << std:: dec <<
					"[" << (std::string)content << "]";
			if(content.hasLang()) std::cout << "(language: " << content.lang() << ")";
			std::cout << std::endl;
			offset+=10+(unsigned)l;
		}
		return true;
	}
	catch(...) {
		return false;
	}
}


} }/* namespace id3 */


