/**
 * \file JUtf8String.h
 *
 *  Created on: 4 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_JUTF8STRING_H_
#define SRC_MIMIC_JUTF8STRING_H_

#include "Common.h"
#include <codecvt>
#include <iterator>
#include <locale>
#include "parsing/ByteConsumer.h"

namespace mimic
{

class JUtf8StringIterator;

/**
 * A modified UTF-8 string
 *
 * See https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.7
 */
class JUtf8String
{
public:
  class JUtf8StringIterator: public std::forward_iterator_tag
  {
    std::vector<u1>::const_iterator b, e;
    
  public:

    using value_type = std::vector<u1>;

    JUtf8StringIterator(std::vector<u1>::const_iterator b_, std::vector<u1>::const_iterator e_)
    :b(b_), e(e_)
    {}
    
    u4 operator*()
    {
      u4 code_point;
      if (*b == 0xed)
      {
        code_point = ((*(b + 1) & 0x0f) << 16);
        code_point |= ((*(b + 2) & 0x3f) << 10);
        code_point |= ((*(b + 4) & 0x0f) << 6);
        code_point |= (*(b + 5) & 0x3f);
      }
      else if ((*b & 0xe0) == 0xe0)
      {
        code_point = (*b & 0x0f) << 12;
        code_point |= (*(b + 1) & 0x3f) << 6;
        code_point |= (*(b + 2) & 0x3f);
      }
      else if ((*b & 0xc0) == 0xc0)
      {
        code_point = (*b & 0x1f) << 6;
        code_point |= (*(b + 1) & 0x3f);
      }
      else
      {
        code_point = *b;
      }
      return code_point;
    }

    JUtf8StringIterator& operator++()
    {
      b = find_next();
      return *this;
    }

    JUtf8StringIterator operator++(int)
    {
      JUtf8StringIterator result = *this;
      ++(*this);
      return result;
    }

    JUtf8StringIterator operator+(int num)
    {
      JUtf8StringIterator result = *this;
      for (int i = 0; i < num; i++)
        ++result;
      return result;
    }

    JUtf8StringIterator& operator+=(int num)
    {
      for (int i = 0; i < num; i++)
        ++(*this);
      return *this;
    }

    bool operator<(const JUtf8StringIterator& i) const
    {
      return b < i.b;
    }
    
    bool operator==(const JUtf8StringIterator& i) const
    {
      return b == i.b;
    }

    bool operator!=(const JUtf8StringIterator& i) const
    {
      return b != i.b;
    }

  private:
    std::vector<u1>::const_iterator find_next()
    {
      auto current = b;
      if (*current == 0xed)
      {
        current += 6;
      }
      else if ((*current & 0xe0) == 0xe0)
      {
        current += 3;
      }
      else if ((*current & 0xc0) == 0xc0)
      {
        current += 2;
      }
      else
      {
        current += 1;
      }
      if (current > e)
        return e;
      return current;
    }
  };

  JUtf8String() {};

  /**
   * Construct a JUtf8String from a std::vector<u1>
   *
   * @param bytes The modified UTF-8 byte encoded string
   */
  JUtf8String(std::vector<u1> bytes);

  /**
   * Construct a JUtf8String from a std::string
   *
   * @param str The string to convert
   */
  JUtf8String(std::string str) {
    std::stringstream ss;
    ss << str;
    ss >> *this;
  };

  /**
   * Construct a JUtf8String from a char*
   *
   * @param str The C-style string to convert
   * @param length The number of bytes in the string
   */
  JUtf8String(const char* str, u2 length)
    : JUtf8String(std::string(str, length)) {};

  /**
   * Construct a JUtf8String from a range
   *
   * @param begin An iterator pointing to the first character of the new string
   * @param end An iterator pointing to the last character of the new string
   */
  JUtf8String(JUtf8StringIterator begin, JUtf8StringIterator end)
  {
    for (auto i = begin; i != end; ++i)
    {
      bytes.insert(bytes.end(), *i);
    }
  };

  /**
   * @return the number of characters in the string (not the number of bytes)
   */
  u2 length() const;

  /**
   * @return a copy of the internal byte buffer
   */
  std::vector<u1> getBytes() { return bytes; };

  /**
   * Splits the string where the specified delimiter is found
   *
   * @param delimiter The string around which to split, if found
   * @return an array of strings
   */
  std::vector<JUtf8String> split(JUtf8String delimiter) const;

  /**
   * Find a string within this string
   *
   * @param needle The string to search for
   * @return iterator pointing at the start of the found string if found,
   *         else end()
   */
  JUtf8StringIterator find(JUtf8String needle) const;

  /**
   * Find if a string is within this string
   *
   * @param needle The string to search for
   * @return true if found, else false
   */
  bool contains(JUtf8String needle) const;

  /**
   * Find if one of a set of strings is within this string
   *
   * @param needles The strings to search for
   * @return true if a needle was found, else false
   */
  bool contains(std::vector<JUtf8String> needles) const;

  JUtf8StringIterator begin() const {
    return JUtf8StringIterator(bytes.begin(), bytes.end());
  };

  JUtf8StringIterator end() const {
    return JUtf8StringIterator(bytes.end(), bytes.end());
  };

  bool operator==(const JUtf8String& other) const
  {
    if (length() != other.length())
      return false;
    auto thisIter = begin();
    auto otherIter = other.begin();
    for (int i = 0; i < length(); i++)
    {
      if (*thisIter != *otherIter)
        return false;
      ++thisIter;
      ++otherIter;
    }
    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, const JUtf8String& str)
  {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    for (auto i = str.begin(); i != str.end(); ++i)
    {
      os << converter.to_bytes(*i);
    }
    return os;
  }

  friend std::istream& operator>>(std::istream& is, JUtf8String& str)
  {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string wstr = converter.from_bytes(static_cast<std::stringstream const&>(std::stringstream() << is.rdbuf()).str());
    std::vector<u1> converted_bytes;
    for (auto code_point = wstr.begin(); code_point != wstr.end(); ++code_point)
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
   * Replaces a code point in the string
   *
   * @param index An iterator pointing to the code point to replace
   * @param *code_point_bytes the modified UTF-8 encoded code point to insert
   */
  void replaceChar(const std::vector<u1>::iterator& index, std::vector<u1> code_point_bytes);

  friend JUtf8StringIterator;

  std::vector<u1> bytes;
};

}

#endif
