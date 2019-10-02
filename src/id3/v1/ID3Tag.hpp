/*
 * ID3Tag.hpp
 *
 *  Created on: 2 Oct 2019
 *      Author: julianporter
 */

#ifndef ID3V1_ID3TAG_HPP_
#define ID3V1_ID3TAG_HPP_

#include <base.hpp>
namespace id3 { namespace v1 {





class Tag {
private:
	enum class Frames {
		Title,
		Artist,
		Album,
		Year,
		Comment,
		Track,
		Genre
	};

	struct Spec {
		Frames frame;
		std::string name;
		unsigned length;

		Spec(const Frames f,const std::string &n,const unsigned l) : frame(f),name(n),length(l) {}
		virtual ~Spec() = default;
	};

	base::BinaryFile data;
	std::map<Frames,std::string> fields;
	unsigned _genre;
	unsigned _track;

	const static std::vector<Spec> names;

	bool exists;

	std::string get(const Frames field) const {
		try {
			return fields.at(field);
		}
		catch(...) {
			return "";
		}
	}

public:
	Tag(const base::BinaryFile &file) : data(file), fields(), _genre(255), _track(0), exists(false) {};
	virtual ~Tag() {
		std::cerr << "In TAG dealloc";
	}
	bool parse();


	std::string title() const { return get(Frames::Title) ; }
	std::string artist() const { return get(Frames::Artist) ; }
	std::string album() const { return get(Frames::Album) ; }
	std::string year() const { return get(Frames::Year) ; }
	std::string comment() const { return get(Frames::Comment) ; }
	unsigned track() const { return _track; }
	unsigned genre() const { return _genre; }
	bool hasTag() const { return exists; }
	operator std::string() const;
};


}}



#endif /* ID3TAG_HPP_ */
