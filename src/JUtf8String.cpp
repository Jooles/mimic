
#include "JUtf8String.h"
#include "parsing/ParseFailureException.h"

namespace mimic
{

JUtf8String::JUtf8String(std::vector<u1> bytes)
  : bytes(std::move(bytes))
{
  for (auto i = this->bytes.begin(); i != this->bytes.end(); ++i)
  {
    u1 byte = *i;
    if (byte == 0 || (byte >= 0xf0 && byte <= 0xff))
    {
      std::stringstream ss;
      ss << "Illegal character: " << std::hex << std::setfill('0') << std::setw(2) << (int)byte << std::dec << std::endl;
      throw parsing::parse_failure(ss.str().c_str());
    }
  }
};

u2 JUtf8String::length() const
{
  u2 length = 0;
  for (auto i = begin(); i != end(); ++i)
  {
    length++;
  }
  return length;
}

void JUtf8String::replaceChar(const std::vector<u1>::iterator& index, std::vector<u1> code_point_bytes)
{
  if (index < bytes.end())
  {
    if (*index == 0xed)
    {
      bytes.erase(index, index + 5);
    }
    else if ((*index & 0xe0) == 0xe0)
    {
      bytes.erase(index, index + 2);
    }
    else if ((*index & 0xc0) == 0xc0)
    {
      bytes.erase(index, index + 1);
    }
    else
    {
      bytes.erase(index);
    }
  }
  bytes.insert(index, code_point_bytes.begin(), code_point_bytes.end());
}

std::vector<JUtf8String> JUtf8String::split(JUtf8String delimiter) const
{
  std::vector<JUtf8String> response;
  if (delimiter.length() == 0)
  {
    for (auto i = begin(); i != end(); ++i)
      response.push_back(JUtf8String(i, i + 1));
  }
  else
  {
    auto strStart = begin();
    for (auto i = begin(); i != end(); ++i)
    {
      if (*i == *(delimiter.begin()))
      {
        auto delimiterLength = delimiter.length();
        if (JUtf8String(i, i + delimiterLength) == delimiter)
        {
          response.push_back(JUtf8String(strStart, i));
          i += delimiterLength;
          strStart = i;
        }
      }
    }
    response.push_back(JUtf8String(strStart, end()));
  }
  return response;
}

JUtf8String::JUtf8StringIterator JUtf8String::find(JUtf8String needle) const
{
  if (needle.length() == 0)
    return end();
  for (auto i = begin(); i != end(); ++i)
  {
    if (*i == *(needle.begin()))
    {
      auto needleLength = needle.length();
      if (JUtf8String(i, i + needleLength) == needle)
        return i;
    }
  }
  return end();
}

bool JUtf8String::contains(JUtf8String needle) const
{
  return contains(std::vector<JUtf8String>{needle});
}

bool JUtf8String::contains(std::vector<JUtf8String> needles) const
{
  if (needles.size() == 0)
    return false;
  for (auto i = begin(); i != end(); ++i)
  {
    for (auto j = needles.begin(); j != needles.end(); ++j)
    {
      auto needleLength = (*j).length();
      if (needleLength == 0)
        continue;
      if ((*i == *(*j).begin()) && (JUtf8String(i, i + needleLength) == *j))
      { 
         return true;
      }
    }
  }
  return false;
}

}
