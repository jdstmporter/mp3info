/*
 * ID3Frame.cpp
 *
 *  Created on: 29 Sep 2019
 *      Author: julianporter
 */

#include "./ID3Tag.hpp"
#include "ID3Frame.hpp"

#include <iostream>
#include <cctype>

namespace id3 { namespace v2 {




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


