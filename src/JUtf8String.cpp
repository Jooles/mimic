
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

}
