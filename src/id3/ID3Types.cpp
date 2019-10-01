/*
 * ID3Types.cpp
 *
 *  Created on: 30 Sep 2019
 *      Author: julianporter
 */


#include "ID3Types.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>

namespace id3 { namespace v2 {

SyncSafeInteger::SyncSafeInteger(const uint32_t u) : i32(u) {
		auto v=u;
		for(auto i=0;i<4;i++) {
			i8[3-i]=(char)(v&127);
			v>>=7;
		}
	}
SyncSafeInteger::SyncSafeInteger(char *b) {
		uint32_t v=0;
		for(auto i=0;i<4;i++) {
			i8[i]=b[i];
			v=(v<<7) + (127 & (uint32_t)b[i]);
		}
		i32=v;
	}

SyncSafeInteger::operator bool() const {
	return std::none_of(i8,i8+4,[](auto c) { return 127 < (unsigned)c; });
}

	template<std::codecvt_mode M = (std::codecvt_mode)0>
	using convert = std::codecvt_utf8_utf16<char16_t,0x10fffful,M>;

	template<std::codecvt_mode M = (std::codecvt_mode)0>
	using converter = std::wstring_convert<convert<M>,char16_t>;

	template<std::codecvt_mode M = (std::codecvt_mode)0>
	class UTF16_8 {
	private:
		char *ptr;
		long length;
		converter<M> c;

		std::vector<char16_t> raw16;
		std::vector<std::u16string> strings16;
		std::string string8;

		void ch8To16() {
			for(auto i=0;i<length;i+=2) {
				raw16[i/2] = (((char16_t)ptr[i+1])<< 8) + ((char16_t)ptr[i]);
			}
		}
		void split() {
			auto pos=raw16.data();
			auto start=pos;
			auto end=pos+raw16.size();
			while(pos<end) {
				if(0==*pos) {
					strings16.push_back(std::u16string(start,pos));
					pos+=1;
					start=pos;
				}
				pos++;
			}
			if(start<end) strings16.push_back(std::u16string(start,end));
		}

		std::string convert() {
			std::stringstream stream;
			for(auto it=strings16.begin();it!=strings16.end();it++) {
				auto st=c.to_bytes(*it);
				st.erase(0,3);
				stream << st;
			}
			return stream.str();
		}

	public:
		UTF16_8(char *p,const long l) : ptr(p), length(l), raw16(length/2), strings16() {
			ch8To16();
			split();
			string8=convert();
		}

		operator std::string() { return string8; }

	};



	StringField::StringField(char *ptr,long length,bool languageField) {
		if(length==0) {
			str="";
			language="";
		}
		else {
			auto off=1;
			if(languageField && isalpha(ptr[1])) { // check to make sure that it is really there
				language=std::string(ptr+1,3);
				off+=3;
			}
			try {
				switch(ptr[0]) {
				case 0 : { // ISO8859-1
					std::vector<char> out;
					for(auto i=off;i<length-1;i++) {
						auto c=(unsigned char)ptr[i];
						if(c<128) out.push_back(c);
						else {
							out.push_back(0x80 | (c&0x3f));
							out.push_back(0xc0 | (c>>6));
						}
					}
					str=std::string(out.data(),out.size());
					break;
				}
				case 1 : { // UTF-16
					str=(std::string)UTF16_8<std::codecvt_mode::consume_header>(ptr+off,length-off);
					break;
				}
				case 2 : { // UTF-16BE
					str=(std::string)UTF16_8<>(ptr+off,length-off);
					break;
				}
				case 3 : //UTF-8
					str=std::string(ptr+off,length-off);
					break;
				default:
					str=std::string(ptr,length);
					break;
				}
			}
			catch(...) {
				str=std::string(ptr,length);
			}
		}
	}

}}

