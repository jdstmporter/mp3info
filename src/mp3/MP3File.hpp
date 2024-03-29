/*
 * MP3File.hpp
 *
 *  Created on: 22 Apr 2018
 *      Author: julianporter
 */

#ifndef LIB_MP3FILE_HPP_
#define LIB_MP3FILE_HPP_

#include <base.hpp>
#include "MP3.hpp"
#include "MP3Frame.hpp"


namespace mp3 {

struct TestResult;


class MP3File {
	friend struct MP3TestResult;
public:
	using frameset_t = std::vector<MP3Frame>;
	using iterator = std::vector<MP3Frame>::const_iterator;


private:
	base::BinaryFile mp3;
	frameset_t frames;
	MP3Frame initial;
	offset_t offset;
	MPEGSpecification spec;


public:
	MP3File() : mp3(), frames(), initial(), offset(0) {};
	MP3File(const base::BinaryFile &file) ;
	virtual ~MP3File() = default;

	void parse();
	size_t size() const;
	double duration() const;
	unsigned bitrate() const;
	unsigned samplerate() const;
	MPEGSpecification specification() const { return spec; }

	iterator begin() const { return frames.cbegin(); }
	iterator end() const { return frames.cend(); }
	unsigned nframes() const { return frames.size(); }



};

} /* namespace mp3 */


std::ostream & operator<<(std::ostream &o,const mp3::MP3File &f);

#endif /* LIB_MP3FILE_HPP_ */
