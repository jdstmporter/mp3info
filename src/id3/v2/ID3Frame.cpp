/*
 * ID3Frame.cpp
 *
 *  Created on: 1 Oct 2019
 *      Author: julianporter
 */

#include "ID3Frame.hpp"
#include <cctype>

namespace id3 { namespace v2 {

bool Frame::parse() {
	auto isGood=std::all_of(begin,begin+4,[](char c) { return isalnum(c) && ! islower(c); });
	if(!isGood) return false;
	key=std::string(begin,begin+4);
	SyncSafeInteger l(begin+4);
	if(!l) return false;
	length=(unsigned)l;
	flags=base::from8(begin[8],begin[9]);
	auto lang=key=="COMM";
	StringField c(begin+10,begin+10+l,lang);
	c.parse();
	content=(std::string)c;
	if(c.hasLang()) language=c.lang();
	return true;
}

Frame::operator std::string() const {
	std::stringstream s;
	s << key << " " << length << " " << std::hex << flags << std:: dec << "[" << content << "]";
	if(hasLanguage()) s << "(language: " << language << ")";
	return s.str();
}

}}

