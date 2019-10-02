/*
 * ID3Types.cpp
 *
 *  Created on: 30 Sep 2019
 *      Author: julianporter
 */


#include "ID3Types.hpp"
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

	SyncSafeInteger::SyncSafeInteger(base::it_t it) {
		uint32_t v=0;
		for(auto i=0;i<4;i++) {
			auto c=(*it++);
			i8[i]=c;
			v=(v<<7) + (127 & (uint32_t)c);
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
		base::it_t begin;
		base::it_t end;
		long length;
		converter<M> c;

		std::vector<char16_t> raw16;
		std::vector<std::u16string> strings16;
		std::string string8;

		void ch8To16() {
			//for(auto it=begin;it!=end;it++) std::cout << std::hex << (unsigned)(unsigned char)*it << " ";
			//std::cout << std::dec << std::endl;
			auto it=begin;
			auto offset=0;

			char16_t bom=base::from8(it[0],it[1]);
			auto rev=bom==0xfffe;
			if(bom==0xfffe||bom==0xfeff) it+=2;

			while(it!=end) {
				raw16[offset++] = base::from8(it[0],it[1],rev);
				it+=2;
			}
			//for(auto it=raw16.begin();it!=raw16.end();it++) std::cout << std::hex << *it << " ";
			//std::cout << std::dec << std::endl;
		}
		void split() {

			auto it=raw16.begin();
			auto start=it;
			auto end=raw16.end();
			while(it<end) {
				if(0==it[0]) {
					strings16.push_back(std::u16string(start,it));
					it++;
					start=it;
				}
				it++;
			}
			if(start!=end) strings16.push_back(std::u16string(start,end));
		}

		std::string convert() {
			std::stringstream stream;
			for(auto it=strings16.begin();it!=strings16.end();it++) {
				std::u16string s=*it;
				//std::cout << "String16 ";
				//for(auto i=s.begin();i!=s.end();i++) std::cout << std::hex << *i << " ";
				//std::cout << std::dec << std::endl;
				auto st=c.to_bytes(s);
				//for(auto i=st.begin();i!=st.end();i++) std::cout << std::hex << (unsigned)(unsigned char)*i << " ";
				//std::cout << std::dec << std::endl;
				//st.erase(0,3);
				stream << st;
			}
			return stream.str();
		}

	public:
		UTF16_8(base::it_t b,base::it_t e) : begin(b), end(e), length(end-begin), raw16(length/2,0), strings16() {
			//std::cout << "UTF16-8 converter with length " << length << std::endl;
			ch8To16();
			//std::cout << "8 to 16" << std::endl;
			split();
			//std::cout << "split" << std::endl;
			string8=convert();
			//std::cout << "converted" << std::endl;
		}

		operator std::string() { return string8; }

	};

	void StringField::parse() {
		if(begin==end) return;

		it=begin;
		char type=*(it++);
		if(languageField && isalpha(*it)) { // check to make sure that it is really there
			language=std::string(it,it+3);
			it+=3;
			//std::cout << "Language tag is " << language << std::endl;
		}
			else {
				//std::cout << "No language tag" << std::endl;
			}
			try {
				switch(type) {
				case 0 : { // ISO8859-1
					std::cout << "ISO8859-1" << std::endl;
					str=base::ISO8859(it,end);
					break;
				}
				case 1 : { // UTF-16
					std::cout << "UTF-16" << std::endl;
					str=(std::string)UTF16_8<>(it,end);
					break;
				}
				case 2 : { // UTF-16BE
					std::cout << "UTF16-BE" << std::endl;
					str=(std::string)UTF16_8<>(it,end);
					break;
				}
				case 3 : //UTF-8
					std::cout << "UTF-8" << std::endl;
					str=std::string(it,end);
					break;
				default:
					std::cout << "Unknown charset" << std::endl;
					str=std::string(begin,end);
					break;
				}
			}
			catch(...) {
				str=std::string(begin,end);
			}
		}



/*

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
	}*/

}}

