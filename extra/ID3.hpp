/*
 * ID3Header.hpp
 *
 *  Created on: 29 Oct 2018
 *      Author: julianporter
 */

#ifndef PCM2MP3_CPP_SRC_ID3_ID3HEADER_HPP_
#define PCM2MP3_CPP_SRC_ID3_ID3HEADER_HPP_


#include <iostream>
#include <map>
#include <taglib/tag.h>

namespace id3 {

	class ID3Tags {
	protected:
		std::map<std::string,std::string> tags;
		unsigned version;
		long offset;

		void getDefaultTags(TagLib::Tag *tag);

	public:
		using iterator = std::map<std::string,std::string>::iterator;
		using const_iterator = std::map<std::string,std::string>::const_iterator;

		ID3Tags() : tags(), version(0), offset(0) {};
		ID3Tags(const ID3Tags &) = default;
		ID3Tags(ID3Tags &&) = default;
		ID3Tags & operator=(const ID3Tags &) = default;
		ID3Tags & operator=(ID3Tags &&) = default;
		virtual ~ID3Tags() = default;

		std::string operator[](const std::string &key) const { return tags.at(key); }
		bool has(const std::string &key) const;
		std::vector<std::string> keys() const;

		iterator begin() { return tags.begin(); }
		iterator end() { return tags.end(); }
		const_iterator cbegin() const { return tags.cbegin(); }
		const_iterator cend() const { return tags.cend(); }
		unsigned size() const { return tags.size(); }

		bool isID3v1() const { return version==1; }
		bool isID3v2() const { return version==2; }
		unsigned id3Version() const { return version; }
	};

	class ID3HumanFriendlyTags : public ID3Tags {
	public:
		ID3HumanFriendlyTags() : ID3Tags() {};

		ID3HumanFriendlyTags(std::istream &stream);
		ID3HumanFriendlyTags(const ID3HumanFriendlyTags &) = default;
		ID3HumanFriendlyTags(ID3HumanFriendlyTags &&) = default;
		virtual ~ID3HumanFriendlyTags() = default;

	};

	class ID3RawTags : public ID3Tags {
	public:
		ID3RawTags() : ID3Tags() {};
		ID3RawTags(std::istream &stream);
		ID3RawTags(const ID3RawTags &) = default;
		ID3RawTags(ID3RawTags &&) = default;
		virtual ~ID3RawTags() = default;


	};


class ID3 {
private:
	unsigned version;
	long offset;

public:
	static std::vector<char> readBinaryFile(std::istream &stream);
	ID3Tags raw;
	ID3Tags friendly;

	ID3(const std::string &filename);
	virtual ~ID3() = default;


};

} /* namespace id3 */


std::ostream &operator<<(std::ostream &o,const id3::ID3Tags &hdr);

#endif /* PCM2MP3_CPP_SRC_ID3_ID3HEADER_HPP_ */
