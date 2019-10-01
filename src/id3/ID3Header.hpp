/*
 * ID3FrameHeader.hpp
 *
 *  Created on: 1 Oct 2019
 *      Author: julianporter
 */

#ifndef ID3HEADER_HPP_
#define ID3HEADER_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace id3 { namespace v2 {

class Flags {
private:
	const static std::map<uint8_t,std::string> names;
	const static uint8_t unsynchronisation;
	const static uint8_t extendedHeader;
	const static uint8_t experimental;
	const static uint8_t footer;
	uint8_t raw;



public:
	Flags() : raw(0) {};
	Flags(const char c) : raw((uint8_t)c) {};
	Flags(const Flags &) = default;
	virtual ~Flags() = default;

	operator bool() const { return (raw & 0x0f)==0; }
	bool isUnsynchronised() const { return (raw & Flags::unsynchronisation); }
	bool hasExtendedHeader() const { return (raw & Flags::extendedHeader); }
	bool isExperimental() const { return (raw & Flags::experimental); }
	bool hasFooter() const { return (raw & Flags::footer); }
	bool null() const { return raw==0; }

	operator unsigned() const { return raw>>4; }
	std::string toString() const;
};

struct Version {
	unsigned major;
	unsigned minor;

	Version() : major(0),minor(0) {};
	Version(const char ma,const char mi) : major(ma), minor(mi) {};
	Version(const Version &) = default;
	Version & operator=(const Version &) = default;
	virtual ~Version() = default;

	operator bool() const {
		return (major != 255) && (minor != 255);
	}
	std::string toString() const;
};

class Header {
private:
	const static std::string tagString;
	std::vector<char> header;

	std::string tag;

public:
	using mdata_t = std::vector<char>;

	Version version;
	Flags flags;
	unsigned length;
	long offset;

	Header() : header(10,0), tag(), version(), flags(), length(0), offset(0) {};
	Header(const mdata_t &data,const long o);
	Header(const Header &) = default;
	virtual ~Header() = default;

	bool check() const;
};




}}

std::ostream & operator<<(std::ostream &o,const id3::v2::Flags &f);
std::ostream & operator<<(std::ostream &o,const id3::v2::Version &f);
std::ostream & operator<<(std::ostream &o,const id3::v2::Header &f);



#endif /* ID3HEADER_HPP_ */
