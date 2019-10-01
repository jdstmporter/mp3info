/*
 * MP3Frame.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */
#include "MP3Frame.hpp"

#include <iomanip>

namespace mp3 {

size_t frame_size_index[] = {24000, 72000, 72000};



MP3FrameFailureMode::MP3FrameFailureMode(const MP3Header &h) {
	id = !(h.id==0x7ff);
	version = h.version==1;
	layer = h.layer==0;
	rate = h.rate==15;
	freq = h.frequency==3;
	modeExt = (h.mode!=2) && (h.modeExtension!=0);
	emphasis = h.emphasis==1;
	sequence=false;
}

bool MP3FrameFailureMode::Fails() const  {
	return id || version || layer || rate || freq || modeExt || emphasis || sequence;
}

void MP3Frame::build() {
	mpeg = static_cast<MPEGVersion>(header.version);
	layer=static_cast<MPEGLayer>(header.layer);
	mode=static_cast<MPEGMode>(header.mode);
	spec=MPEGSpecification(mpeg,layer,mode);
	mp3=MP3(mpeg,layer);


	bitRate=mp3.rate(header.rate);
	sampleRate=mp3.frequency(header.frequency);
	crc=header.crc!=0;
}

MP3Frame::MP3Frame(const MP3Header &h) : header(h) {
	build();
}

offset_t MP3Frame::match(const MP3ValidFrame &v)  {

	//std::cerr << "Matching initial : " << v.initial << " @ " << start << std::endl;
	auto it=data.begin()+start;
	auto end=data.end()-MP3::MinimumFrameSize;

	MP3HeaderConverter block;
	block.bytes=0;
	try {
		while ((it<end) && v(block.header) ) block.push(*(it++));
	}
	catch(std::exception &e) {
		std::cerr << "ERROR : " << e.what() << std::endl;
		throw std::runtime_error("No frame header found");
	}

	if(it==end) throw std::runtime_error("No frame header found");

	header=block.header;
	build();
	/*
	mpeg = static_cast<MPEGVersion>(header.version);
	layer=static_cast<MPEGLayer>(header.layer);
	mode=static_cast<MPEGMode>(header.mode);
	spec=MPEGSpecification(mpeg,layer,mode);
	mp3=MP3(mpeg,layer);


	bitRate=mp3.rate(block.header.rate);
	sampleRate=mp3.frequency(block.header.frequency);
	crc=block.header.crc!=0;
	*/

	//zero=std::all_of(it,end,[](char x) { return x == 0; });

	offset=it-data.begin();
	//std::cerr << "Matched " << std::hex << block.bytes << std::dec << " at " << (offset-4) << std::endl;
	return offset-MP3::FrameHeaderSize+size();
}


size_t MP3Frame::size() const {
	auto ratio=(1000.0*(double)bitRate)/(double)sampleRate;
	//std::cout << "Index : " << mp3.frameSizeIndex() << " br : " << bitRate << " sr : " << sampleRate << " ratio : " << ratio << " pad: " << header.pad << std::endl;
	return size_t(mp3.frameSizeIndex()*ratio)+header.pad;
}



}



std::ostream & operator<<(std::ostream &o,const mp3::MP3Frame &f) {
	auto c=(f.hasCRC()) ? "Yes" : " No";
	if(f.fileOffset()>=4) {
		o << std::setw(8) << std::hex  << f.address()
			<< std::dec << " (" << std::setw(8) << f.address() << ") ";
	}
	o << f.Header() << " ; " << std::setw(8) <<  "Samples: " << f.SampleRate()
			<< ", Bits: " << f.BitRate() << ", Has CRC: " << c << ", "
			<< f.Version() << ", " << f.Layer() << ", " << f.Mode()
			<< ", Length :" << f.size();
	return o;
}

std::string _(const bool b) {
	return b ? "BAD" : "GOOD";
}

std::ostream & operator<<(std::ostream &o,const mp3::MP3FrameFailureMode &m) {
	if(!m.Fails()) o << "Frame good";
	else {
		o << "id: " << _(m.id) << ", version: " << _(m.version) << ", layer: " << _(m.layer)
				<< ", bit rate: " << _(m.rate) << ", sample rate: " << _(m.freq)
				<< ", mode: " << _(m.modeExt) << ", emphasis: " << _(m.emphasis);
	}
	return o;
}

