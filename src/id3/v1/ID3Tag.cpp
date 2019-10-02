/*
 * ID3Tag.cpp
 *
 *  Created on: 2 Oct 2019
 *      Author: julianporter
 */


#include "ID3Tag.hpp"

namespace id3 { namespace v1 {

const std::vector<std::pair<std::string,unsigned>> Tag::names = {
		std::make_pair("Title",30),
		std::make_pair("Artist",30),
		std::make_pair("Album",30),
		std::make_pair("Year",4),
		std::make_pair("Comment",28)
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
	_track=(unsigned)(unsigned char)*(++it);
	_genre=(unsigned)(unsigned char)*(++it);
	return true;
}

Tag::operator std::string() const {
	std::stringstream s;

	for(auto field = Tag::names.begin();field!=Tag::names.end();field++) {
		auto name=field->first;
		s << name << " : " << get(name) << std::endl;
	}
	s << "Track : " << track() << std::endl;
	s << "Genre : " << genre() << std::endl;
	return s.str();
}

}}
