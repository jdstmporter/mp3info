/*
 * ID3Header.cpp
 *
 *  Created on: 29 Oct 2018
 *      Author: julianporter
 */

#include "ID3.hpp"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <sstream>
#include <fstream>
#include "ID3Frame.hpp"


namespace  id3 {


void ID3Tags::getDefaultTags(TagLib::Tag *tag) {
	auto props = tag->properties();
	for(auto it=props.begin();it!=props.end();++it) {
		auto key=it->first.to8Bit();
		auto value=it->second.toString().to8Bit();
		tags[key]=value;
	}
}

bool ID3Tags::has(const std::string &key) const {
	return tags.find(key) != tags.end();
}

std::vector<std::string> ID3Tags::keys() const {
	std::vector<std::string> k;
	for(auto it=tags.begin();it!=tags.end();++it) {
		k.push_back(it->first);
	}
	return k;
}

ID3HumanFriendlyTags::ID3HumanFriendlyTags(std::istream &stream) : ID3Tags(1) {
	TagLib::FileRef file(stream);
	getDefaultTags(file.tag());
}


ID3RawTags::ID3RawTags(std::istream &stream) : ID3Tags() {
	auto data = ID3::readBinaryFile(stream);

	try {
		auto frame=ID3Frame::find(data);
		offset=frame.offset;
		version=2;
	}
	catch(...) {
		offset=0;
		version=1;
	}

	if(version==1) {
		TagLib::FileRef file(stream);
		getDefaultTags(file.tag());
	}
	else if(version==2) {
		TagLib::MPEG::File file(stream);
		TagLib::ID3v2::Tag t2(file,offset);
		auto list=t2.frameListMap();
		for(auto it=list.begin();it!=list.end();++it) {
			auto key = std::string(it->first.data(),4);
			auto values = it->second;
			std::stringstream s;
			for(auto it=values.begin();it!=values.end();++it) {
				s << (*it)->toString() << ";";
			}
			tags[key]=s.str();
		}
	}
}

std::vector<char> ID3::readBinaryFile(std::istream &stream) {
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

ID3::ID3(const std::string &filename)  {
	std::ifstream mp(filename,std::ifstream::binary);
	if(mp.fail()) throw std::runtime_error("Cannot open file");
	auto data = ID3::readBinaryFile(mp);

	try {
		auto frame=ID3Frame::find(data);
		offset=frame.offset;
		version=2;
	}
	catch(...) {
		offset=0;
		version=1;
	}
	auto file = TagLib::FileRef(filename.c_str());
	auto tag=file.tag();

	raw = static_cast<ID3Tags>(ID3RawTags(tag));
	friendly = static_cast<ID3Tags>(ID3HumanFriendlyTags(tag));
}

}

std::ostream &operator<<(std::ostream &o,const id3::ID3Tags &hdr) {
	o << "ID3v" << hdr.id3Version() << std::endl;
	for(auto it=hdr.cbegin();it!=hdr.cend();++it) {
		o << it->first << ": " << it->second << std::endl;
	}
	return o;
}

