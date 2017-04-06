
#include "Utf8String.h"
#include "parsing/ParseFailureException.h"

namespace mimic
{

Utf8String::Utf8String(std::vector<u1>& bytes, u2 length)
	: bytes(bytes)
{
  checkString(length);
};

Utf8String::Utf8String(std::vector<u1>&& bytes, u2 length)
	: bytes(bytes)
{
  checkString(length);
};

void Utf8String::checkString(u2& numberOfBytes)
{
  if (numberOfBytes != bytes.size())
    throw parsing::parse_failure("Wrong length");
  for (int i = 0; i < numberOfBytes; i++)
  {
    if (bytes[i] == 0 || (bytes[i] >= 0xf0 && bytes[i] <= 0xff))
    {
      std::stringstream ss;
      ss << "Illegal character: " << std::hex << std::setw(2) << bytes[i] << std::dec << std::endl;
      throw parsing::parse_failure(ss.str().c_str());
    }
  }
}

u2 Utf8String::length() const
{
  u2 length = 0;
  for (auto i = bytes.begin(); i < bytes.end(); nextIndex(i))
  {
    length++;
  }
  return length;
}

void Utf8String::nextIndex(std::vector<u1>::const_iterator& index) const
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

u4 Utf8String::get(std::vector<u1>::const_iterator& index) const
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

void Utf8String::put(const std::vector<u1>::iterator& index, u4 code_point)
{
  std::vector<u1> code_point_bytes;
  if (code_point > 0xffff)
  {
    code_point_bytes = {
      0xed,
      ((code_point >> 16) & 0x0f) | 0xa0,
      ((code_point >> 10) & 0x3f) | 0x80,
      0xed,
      ((code_point >> 6) & 0x0f) | 0xb0,
      (code_point & 0x3f) | 0x80
    };
  }
  else if (code_point > 0x7ff)
  {
    code_point_bytes = {
      ((code_point >> 12) & 0x0f) | 0xe0,
      ((code_point >> 6) & 0x3f) | 0x80,
      (code_point & 0x3f) | 0x80
    };
  }
  else if (code_point == 0 || code_point > 0x7f)
  {
    code_point_bytes = {
      ((code_point >> 6) & 0x1f) | 0xc0,
      (code_point & 0x3f) | 0x80
    };
  }
  else
  {
    code_point_bytes = {
      static_cast<u1>(code_point & 0x7f)
    };
  }
  replace(index, code_point_bytes);
}

void Utf8String::replace(const std::vector<u1>::iterator& index, std::vector<u1> code_point_bytes)
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
