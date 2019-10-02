/*
 * ID3Tag.cpp
 *
 *  Created on: 2 Oct 2019
 *      Author: julianporter
 */


#include "ID3Tag.hpp"

namespace id3 { namespace v1 {

const std::vector<Tag::Spec> Tag::names = {
		Spec(Frames::Title,"Title",30),
		Spec(Frames::Artist,"Artist",30),
		Spec(Frames::Album,"Album",30),
		Spec(Frames::Year,"Year",4),
		Spec(Frames::Comment,"Comment",28)
};


bool Tag::parse() {
	try {
		if(data.size()<128) throw std::runtime_error("Too short");
		auto it=data.begin()+data.size()-128;
		if(std::string(it,it+3)!="TAG") throw std::runtime_error("No header");
		it+=3;
		for(auto field = Tag::names.begin();field!=Tag::names.end();field++) {
			fields[field->frame]=std::string(it,it+field->length);
			it+=field->length;
		}
		_track=(unsigned)(unsigned char)*(++it);
		_genre=(unsigned)(unsigned char)*(++it);
		exists=true;
	}
	catch(...) {
		exists=false;
	}
	return exists;
}

Tag::operator std::string() const {
	std::stringstream s;

	for(auto field = Tag::names.begin();field!=Tag::names.end();field++) {
		auto name=field->name;
		s << field->name << " : " << get(field->frame) << std::endl;
	}
	s << "Track : " << track() << std::endl;
	s << "Genre : " << genre() << std::endl;
	return s.str();
}

}}
