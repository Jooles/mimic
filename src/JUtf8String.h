/**
 * \file JUtf8String.h
 *
 *  Created on: 4 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_JUTF8STRING_H_
#define SRC_MIMIC_JUTF8STRING_H_

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
class JUtf8String
{
public:
  JUtf8String() {};

  /**
   * Construct a JUtf8String from a std::vector<u1>
   *
   * @param bytes The modified UTF-8 byte encoded string
   */
  JUtf8String(std::vector<u1> bytes);

  /**
   * @return the number of characters in the string (not the number of bytes)
   */
  u2 length() const;

  /**
   * @return the 4-byte value of the character at the specified index
   */
  u4 charAt(std::vector<u1>::const_iterator& index) const;

  /**
   * @return a copy of the internal byte buffer
   */
  std::vector<u1> getBytes() { return bytes; };

  friend std::ostream& operator<<(std::ostream& os, const JUtf8String& str)
  {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    for (auto i = str.bytes.begin(); i < str.bytes.end(); str.nextIndex(i))
    {
      os << converter.to_bytes(str.charAt(i));
    }
    return os;
  }

  friend std::istream& operator>>(std::istream& is, JUtf8String& str)
  {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string wstr = converter.from_bytes(static_cast<std::stringstream const&>(std::stringstream() << is.rdbuf()).str());
    std::vector<u1> converted_bytes;
    for (auto code_point = wstr.begin(); code_point != wstr.end(); code_point++)
    {
      if (*code_point > 0xffff)
      {
        converted_bytes.push_back(0xed);
        converted_bytes.push_back(((*code_point >> 16) & 0x0f) | 0xa0);
        converted_bytes.push_back(((*code_point >> 10) & 0x3f) | 0x80);
        converted_bytes.push_back(0xed);
        converted_bytes.push_back(((*code_point >> 6) & 0x0f) | 0xb0);
        converted_bytes.push_back((*code_point & 0x3f) | 0x80);
      }
      else if (*code_point > 0x7ff)
      {
        converted_bytes.push_back(((*code_point >> 12) & 0x0f) | 0xe0);
        converted_bytes.push_back(((*code_point >> 6) & 0x3f) | 0x80);
        converted_bytes.push_back((*code_point & 0x3f) | 0x80);
      }
      else if (*code_point == 0 || *code_point > 0x7f)
      {
        converted_bytes.push_back(((*code_point >> 6) & 0x1f) | 0xc0);
        converted_bytes.push_back((*code_point & 0x3f) | 0x80);
      }
      else
      {
        converted_bytes.push_back(*code_point & 0x7f);
      }
    }
    str.bytes.insert(str.bytes.end(), converted_bytes.cbegin(), converted_bytes.cend());
    return is;
  }

private:
  /**
   * Modifies the given iterator to point at the next code point
   *
   * @param index an interator pointing to a code point in the string
   */
  void nextIndex(std::vector<u1>::const_iterator& index) const;

  /**
   * Replaces a code point in the string
   *
   * @param index An iterator pointing to the code point to replace
   * @param *code_point_bytes the modified UTF-8 encoded code point to insert
   */
  void replaceChar(const std::vector<u1>::iterator& index, std::vector<u1> code_point_bytes);

	std::vector<u1> bytes;
};

}

#endif
