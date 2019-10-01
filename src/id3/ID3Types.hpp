/*
 * ID3Types.hpp
 *
 *  Created on: 30 Sep 2019
 *      Author: julianporter
 */

#ifndef ID3TYPES_HPP_
#define ID3TYPES_HPP_

#include <string>
#include <locale>
#include <codecvt>

namespace id3 { namespace v2 {

class SyncSafeInteger {
private:
	uint32_t i32;
	char i8[4];

public:
	SyncSafeInteger(const uint32_t u);
	SyncSafeInteger(char *b);
	virtual ~SyncSafeInteger() = default;
	template <typename T, class = typename std::enable_if<std::is_integral<T>::value>::type>
	operator T() const { return (T)i32; }
	operator char *() { return i8; }
	operator bool() const;
};



	class StringField {
	private:
		std::string str;
		std::string language;
	public:
		StringField() : str(), language() {};
		StringField(char *ptr,const long length,bool languageField=false);
		StringField(const StringField &) = default;
		StringField & operator=(const StringField &) = default;
		virtual ~StringField() = default;

		operator std::string() const { return str; }
		std::string lang() const { return language; }
		bool hasLang() const { return language.length()>0; }
	};

}}




#endif /* ID3TYPES_HPP_ */
