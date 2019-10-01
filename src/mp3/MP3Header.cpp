/*
 * MP3Header.cpp
 *
 *  Created on: 28 Sep 2019
 *      Author: julianporter
 */
#include "MP3Header.hpp"


std::ostream & operator<<(std::ostream &o,const mp3::MP3Header &header) {
	o << "HDR[" << std::hex << header.id << "] VERSION[" << std::dec << header.version << "] LAYER [" << header.layer;
	o << "] CRC[" << header.crc << "] RATE[" << header.rate << "] FREQ [" << header.frequency << "] PAD[" << header.pad;
	o << "] EXT[" << header.extension << "] MODE[" << header.mode << "] MODE_EXT[" << header.modeExtension;
	o << "] COPYRIGHT [" << header.copyright << "] ORIGINAL[" << header.original << "] EMPHASIS[" << header.emphasis << "]";
	return o;
}



