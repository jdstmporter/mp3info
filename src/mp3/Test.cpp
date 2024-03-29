/*
 * Test.cpp
 *
 *  Created on: 21 Oct 2018
 *      Author: julianporter
 */

#include "Test.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>


namespace mp3 {


inline std::string _(const MP3Frame &f) {
	std::stringstream s;
	s << f;
	return s.str();
}

MP3TestResult::MP3TestResult(const MP3File &file,const bool verbose) :
		spec(file.specification()),
		duration(file.duration()),
		nsamples(file.size()),
		bitrate(1000*file.bitrate()),
		samplerate(file.samplerate()),
		nframes(file.nframes()),
		goodSampleRate(bitrate>=samplerate),
		log(verbose ? file.size() : 0) {

	nCRC=std::count_if(file.begin(),file.end(),[](auto f) { return f.hasCRC(); });

	message=isGood() ? "PASSED" : "FAILED";
	if(verbose) {
		std::transform(file.begin(),file.end(),log.begin(), [](auto f) { return _(f); });
	}
}


bool MP3TestResult::isGood() const {
	return spec.isGood() && goodSampleRate;
}

void MP3Test::parse(const bool verbose) {
	try {
		MP3::verbose=verbose;
		MP3File mp(file);
		mp.parse();
		result=std::make_shared<MP3TestResult>(mp,verbose);
	}
	catch(std::exception &e) {
		result=std::make_shared<MP3TestResult>(e.what());
	}

}
} /* namespace mp3 */

inline std::string _(const bool b) { return b ? "PASS" : "FAIL"; }

std::ostream & operator<<(std::ostream &o,const mp3::MP3TestResult &r) {
	o << "Header: " << r.spec << " (" << _(r.spec.isGood()) << ")" << std::endl;
	o << "Byte-rate: " << r.bitrate << " per second" << std::endl;
	o << "Sample-rate: " << r.samplerate  << " per second (" << _(r.goodSampleRate) << ")" << std::endl;
	o << "Duration: " << std::fixed << std::setprecision(3) << r.duration << " seconds"<< std::endl;
	o << "CRC: " << r.nCRC << " of " << r.nframes << " frames have a CRC" << std::endl;
	o << "RESULT: (" << _(r.isGood()) <<") ; " << r.message << std::endl;
	return o;
}





