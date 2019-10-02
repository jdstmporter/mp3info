/*
 * ID3FrameHeader.cpp
 *
 *  Created on: 1 Oct 2019
 *      Author: julianporter
 */
#import "ID3Types.hpp"
#include "ID3Header.hpp"

namespace id3 { namespace v2 {

const uint8_t Flags::unsynchronisation = 0b10000000;
const uint8_t Flags::extendedHeader    = 0b01000000;
const uint8_t Flags::experimental      = 0b00100000;
const uint8_t Flags::footer            = 0b00010000;

const std::map<uint8_t,std::string> Flags::names = {
		{ Flags::unsynchronisation, "unsyncronised" },
		{ Flags::extendedHeader, "extended header" },
		{ Flags::experimental, "experimental" },
		{ Flags::footer, "footer" }
};

std::string Flags::toString() const {
	if(null()) return "";

	std::stringstream s;
	for(auto it=Flags::names.begin();it!=Flags::names.end();++it) {
		auto key=it->first;
		if(key&raw) s << it->second << ", ";
	}
	auto ss=s.str();
	if(ss.length()>2) {
		ss.pop_back();
		ss.pop_back();
	}
	return ss;
}

std::string Version::toString() const {
		std::stringstream s;
		s << major << "." << minor;
		return s.str();
	}

const std::string Header::tagString="ID3";

Header::Header(const BinaryFile &data,const long o) : header(10,0), offset(o) {
	auto ptr=data.cbegin()+offset;
	std::copy_n(ptr,10,header.begin());

	tag=std::string(header.begin(),header.begin()+3);
	version=Version(header[3],header[4]);
	flags=Flags(header[5]);
	length=SyncSafeInteger(header.begin()+6);
}

bool Header::check() const {
	if(tag!=Header::tagString) return false;
	if(!version) return false;
	if(!flags) return false;
	if(std::any_of(header.begin()+6,header.end(),[](char c) { return 127 < (unsigned)c; })) return false;
	return true;
}

}}

std::ostream & operator<<(std::ostream &o,const id3::v2::Flags &f) {
	o << std::hex << (unsigned)f << std::dec << " (" << f.toString() << ")";
	return o;
}

std::ostream & operator<<(std::ostream &o,const id3::v2::Version &f) {
	o << f.toString();
	return o;
}

std::ostream & operator<<(std::ostream &o,const id3::v2::Header &f) {
	o << "Version " << f.version << std::endl;
	o << "Flags " << f.flags << std::endl;
	o << "Length " << f.length << std::endl;
	return o;
}
