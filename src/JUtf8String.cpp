
#include "JUtf8String.h"
#include "parsing/ParseFailureException.h"

namespace mimic
{

JUtf8String::JUtf8String(std::vector<u1> bytes)
  : bytes(std::move(bytes))
{
  for (auto i = this->bytes.begin(); i < this->bytes.end(); i++)
  {
    u1 byte = *i;
    if (byte == 0 || (byte >= 0xf0 && byte <= 0xff))
    {
      std::stringstream ss;
      ss << "Illegal character: " << std::hex << std::setw(2) << byte << std::dec << std::endl;
      throw parsing::parse_failure(ss.str().c_str());
    }
  }
};

u2 JUtf8String::length() const
{
  u2 length = 0;
  for (auto i = bytes.begin(); i < bytes.end(); nextIndex(i))
  {
    length++;
  }
  return length;
}

void JUtf8String::nextIndex(std::vector<u1>::const_iterator& index) const
{
  u1 byte = *index;
  if (byte == 0xed)
  {
    index += 6;
  }
  else if ((byte & 0xe0) == 0xe0)
  {
    index += 3;
  }
  else if ((byte & 0xc0) == 0xc0)
  {
    index += 2;
  }
  else
  {
    index += 1;
  }
}

u4 JUtf8String::charAt(std::vector<u1>::const_iterator& index) const
{
  u4 code_point;
  if (*index == 0xed)
  {
    code_point = ((*(index + 1) & 0x0f) << 16);
    code_point |= ((*(index + 2) & 0x3f) << 10);
    code_point |= ((*(index + 4) & 0x0f) << 6);
    code_point |= (*(index + 5) & 0x3f);
  }
  else if ((*index & 0xe0) == 0xe0)
  {
    code_point = (*index & 0x0f) << 12;
    code_point |= (*(index + 1) & 0x3f) << 6;
    code_point |= (*(index + 2) & 0x3f);
  }
  else if ((*index & 0xc0) == 0xc0)
  {
    code_point = (*index & 0x1f) << 6;
    code_point |= (*(index + 1) & 0x3f);
  }
  else
  {
    code_point = *index;
  }
  return code_point;
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
