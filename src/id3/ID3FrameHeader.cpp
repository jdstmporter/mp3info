/*
 * ID3FrameHeader.cpp
 *
 *  Created on: 1 Oct 2019
 *      Author: julianporter
 */
#import "ID3FrameHeader.hpp"
#include <sstream>

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
