/*
 * mp3.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: julianporter
 */

#include <iostream>
#include <fstream>
#include "mp3info.hpp"
#include <ArgParseStandalone.h>

int main(int argc,char *argv[]) {
	ArgParse::ArgParser parser("MP3 file verification and analysis");
	bool verbose = false;
	bool rawTags = false;
	bool tags = false;
	std::string infile = "";

	parser.AddArgument("-r/--rawtags","Print raw ID3 tags",&rawTags,ArgParse::Argument::Optional);
	parser.AddArgument("-t/--tags","Print human-friendly ID3 tags",&tags,ArgParse::Argument::Optional);
	parser.AddArgument("-v/--verbose","Produce verbose output",&verbose,ArgParse::Argument::Optional);
	parser.AddArgument("-i/--infile","File to analyse",&infile,ArgParse::Argument::Required);

	if(parser.ParseArgs(argc,argv)<0) {
		std::cerr << "Cannot parse arguments correctly" << std::endl;
		return -1;
	}
	if(parser.HelpPrinted()) return 0;



	try {
		std::cout << "Analysing " << infile;
		if(verbose) std::cout << " with verbose output";
		std::cout << std::endl;

		base::BinaryFile file(infile);

		mp3::MP3Test test(file);
		test.parse(verbose);
		auto result=test();
		std::cout << *result << std::endl;

		if(tags || rawTags) {
			id3::v2::Tag id(file);
			id.parse();

			id3::v1::Tag id1(file);
			if(id1.parse()) {
				std::cout << "ID3v1 tag:" << std::endl << (std::string)id1 << std::endl;
			}
			else {
				std::cout << "No ID3v1 tag";
			}
		}

	}
	catch(std::exception &e) {
			std::cerr << "ERROR: " << e.what() << std::endl;
		}
		return 0;
}



