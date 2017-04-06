/**
 * \file Utf8String.h
 *
 *  Created on: 4 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_UTF8STRING_H_
#define SRC_MIMIC_UTF8STRING_H_

#include <boost/iterator.hpp>
#include <boost/iterator_adaptors.hpp>
#include "Common.h"
#include <locale>
#include <codecvt>
#include "parsing/ByteConsumer.h"

namespace mimic
{

/**
 * A modified UTF-8 string
 *
 * See https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.7
 */
class Utf8String
{
public:
  Utf8String(std::vector<u1>& bytes, u2 length);
  Utf8String(std::vector<u1>&& bytes, u2 length);
  Utf8String(std::string& str)
  {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string wstr = converter.from_bytes(str);
    for (auto i = wstr.begin(); i != wstr.end(); i++)
      put(bytes.end(), static_cast<u4>(*i));
  };
  Utf8String(std::string&& str)
  {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string wstr = converter.from_bytes(str);
    for (auto i = wstr.begin(); i != wstr.end(); i++)
      put(bytes.end(), static_cast<u4>(*i));
  }
  /**
   * @return the number of characters in the string (not the number of bytes)
   */
  u2 length() const;
  /**
   * @return the 4-byte value of the character at the specified index
   */
  u4 get(std::vector<u1>::const_iterator& index) const;
  /**
   * @param index the index to set with the specified value
   * @param code_point the 4-byte value of the character at the specified index
   */
  void put(const std::vector<u1>::iterator& index, u4 code_point);
  /**
   * @return a copy of the internal byte buffer
   */
  std::vector<u1> getBytes() { return bytes; };
	friend std::ostream& operator<<(std::ostream& os, const Utf8String& str)
  {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    for (auto i = str.bytes.begin(); i < str.bytes.end(); str.nextIndex(i))
    {
      os << converter.to_bytes(str.get(i));
    }
    return os;
  }

private:
  void checkString(u2& numberOfBytes);
  void nextIndex(std::vector<u1>::const_iterator& index) const;
  void replace(const std::vector<u1>::iterator& index, std::vector<u1> code_point_bytes);

	std::vector<u1> bytes;
};

}

#endif
