/*
 * MP3CheckError.hpp
 *
 *  Created on: 28 Sep 2019
 *      Author: julianporter
 */

#ifndef PCM2MP3_CPP_SRC_INFO_MP3CHECKERROR_HPP_
#define PCM2MP3_CPP_SRC_INFO_MP3CHECKERROR_HPP_


#include <exception>
#include <stdexcept>
#include <string>

namespace mp3 {

class MP3CheckError : public std::exception {
private:
	std::string message;
public:
	MP3CheckError() noexcept = default;
	MP3CheckError(const std::string &m) noexcept : std::exception(), message(m) {};

	const char * what() const noexcept { return message.c_str(); };
};

}


#endif /* PCM2MP3_CPP_SRC_INFO_MP3CHECKERROR_HPP_ */
