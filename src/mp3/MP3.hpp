/*
 * MP3.hpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */

#ifndef LIB_MP3_HPP_
#define LIB_MP3_HPP_


#include <base.hpp>
#include <memory>
#include <array>

namespace mp3 {



enum class MPEGVersion : unsigned {
	MPEG25  = 0, // 0xb00,
	MPEG2   = 2, // 0xb01,
	MPEG1   = 3, // 0xb11,
	Unknown = 255
};

enum class MPEGLayer : unsigned {
	Unknown=0,
	Layer3=1,
	Layer2=2,
	Layer1=3
};

enum class MPEGMode : unsigned {
	Stereo=0,
	JointStereo=2,
	DualChannel=1,
	Mono=3,
	Unknown=255
};

enum class MPEGEmphasis : unsigned {
	None=0,
	MS50_15=2,
	CCITJ17=3
};


using size_t = unsigned long;
using offset_t = unsigned long;



struct MPEGSpecification {
	MPEGVersion version;
	MPEGLayer layer;
	MPEGMode mode;

	MPEGSpecification() : version(MPEGVersion::Unknown), layer(MPEGLayer::Unknown), mode(MPEGMode::Unknown) {};
	MPEGSpecification(const MPEGVersion &v,const MPEGLayer &l,const MPEGMode &m) : version(v), layer(l), mode(m) {};
	MPEGSpecification(const MPEGSpecification &) = default;
	MPEGSpecification &operator=(const MPEGSpecification &) = default;
	virtual ~MPEGSpecification() = default;

	bool isGood() const;
	std::string str(const unsigned) const;
};




class MP3 {
	static std::map<MPEGLayer,unsigned> sizeIndex;
	static std::map<MPEGVersion,std::vector<unsigned>> freqs;
	static std::map<std::pair<MPEGVersion,MPEGLayer>,std::vector<unsigned>> rates;
	static void init();
private:
	MPEGVersion version;
	MPEGLayer layer;
public:
	static const offset_t FrameHeaderSize;
	static const size_t MinimumFrameSize;
	static bool verbose;

	MP3(const MPEGVersion &v = MPEGVersion::Unknown,const MPEGLayer &l = MPEGLayer::Unknown);
	virtual ~MP3() = default;

	unsigned frequency(const unsigned f) const;
	unsigned rate(const unsigned f) const;
	size_t frameSizeIndex() const;
};


}


std::ostream & operator<<(std::ostream &o,const mp3::MPEGVersion header);
std::ostream & operator<<(std::ostream &o,const mp3::MPEGLayer header);
std::ostream & operator<<(std::ostream &o,const mp3::MPEGMode mode);
std::ostream & operator<<(std::ostream &o,const mp3::MPEGSpecification &spec);

#endif /* LIB_MP3_HPP_ */
