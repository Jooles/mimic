/**
 * \file Utf8String_test.cpp
 *
 *  Created on: 04 Apr 2017
 *      Author: Julian Cromarty
 */
#include <sstream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Utf8String.h"
#include "parsing/ByteConsumer.h"
#include "parsing/ParseFailureException.h"

namespace mimic
{

class Utf8StringTest: public testing::Test
{

protected:
	Utf8StringTest()
	{
	}

	virtual ~Utf8StringTest()
	{
	}
};

TEST_F(Utf8StringTest, TestNullByteThrowsOnConstruction)
{
	std::vector<u1> bytes = { 0x00 };
	ASSERT_THROW(Utf8String(bytes, 1), parsing::parse_failure);
}

TEST_F(Utf8StringTest, TestByteInInvalidRangeThrowsOnConstruction)
{
	for (int i = 0xf0; i <= 0xff; i++) {
    std::vector<u1> bytes = { (u1) i };
    ASSERT_THROW(Utf8String(bytes, 1), parsing::parse_failure) << "Expected char " << std::setw(2) << std::hex << i << std::dec << " to throw";
  }
}

TEST_F(Utf8StringTest, TestOneByteCharacterGivesLength1)
{
	for (u1 i = 0x01; i < 0x80; i++) {
    std::vector<u1> bytes = { (u1) i };
    ASSERT_EQ(1, Utf8String(bytes, 1).length()) << "When checking char /U" << std::hex << std::setfill('0') << std::setw(4) << (int)i << std::dec;
  }
}

TEST_F(Utf8StringTest, TestTwoByteCharacterGivesLength1)
{
  for (int i = 0x80; i < 0x7ff; i ++) {
    u1 byte1 = ((i >> 6) & 0x1f) | 0xc0;
    u1 byte2 = (i & 0x3f) | 0x80;
    std::vector<u1> bytes = { byte1, byte2 };
    ASSERT_EQ(2, bytes.size());
    ASSERT_EQ(1, Utf8String(bytes, 2).length()) << "When checking char /U"
                                                << std::hex
                                                << std::setfill('0')
                                                << std::setw(4)
                                                << (int)i
                                                << ", bytes: "
                                                << std::setw(2) << (int)byte1 << ", "
                                                << std::setw(2) << (int)byte2
                                                << std::dec;
  }
}

TEST_F(Utf8StringTest, TestThreeByteCharacterGivesLength1)
{
  for (int i = 0x800; i < 0xffff; i ++) {
    u1 byte1 = ((i >> 12) & 0x0f) | 0xe0;
    u1 byte2 = ((i >> 6) & 0x3f) | 0x80;
    u1 byte3 = (i & 0x3f) | 0x80;
    std::vector<u1> bytes = { byte1, byte2, byte3 };
    ASSERT_EQ(3, bytes.size());
    ASSERT_EQ(1, Utf8String(bytes, 3).length()) << "When checking char /U"
                                                << std::hex
                                                << std::setfill('0')
                                                << std::setw(4)
                                                << (int)i
                                                << ", bytes: "
                                                << std::setw(2) << (int)byte1 << ", "
                                                << std::setw(2) << (int)byte2 << ", "
                                                << std::setw(2) << (int)byte3
                                                << std::dec;
  }
}

TEST_F(Utf8StringTest, Test6ByteCharacterGivesLength1)
{
  for (int i = 0x10000; i < 0x1ffff; i ++) {
    u1 byte1 = 0xed;
    u1 byte2 = ((i >> 16) & 0x0f) | 0xa0;
    u1 byte3 = ((i >> 10) & 0x3f) | 0x80;
    u1 byte4 = 0xed;
    u1 byte5 = ((i >> 6) & 0x0f) | 0xb0;
    u1 byte6 = (i & 0x3f) | 0x80;
    std::vector<u1> bytes = { byte1, byte2, byte3 };
    ASSERT_EQ(3, bytes.size());
    ASSERT_EQ(1, Utf8String(bytes, 3).length()) << "When checking char /U"
                                                << std::hex
                                                << std::setfill('0')
                                                << std::setw(4)
                                                << (int)i
                                                << ", bytes: "
                                                << std::setw(2) << (int)byte1 << ", "
                                                << std::setw(2) << (int)byte2 << ", "
                                                << std::setw(2) << (int)byte3
                                                << std::dec;
  }
}

TEST_F(Utf8StringTest, TestOstream1ByteCharacter)
{
  std::vector<u1> bytes = { 0x36 };
  Utf8String str(bytes, 1);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("6").c_str(), ss.str().c_str());
}

TEST_F(Utf8StringTest, TestOstream2ByteCharacter)
{
  std::vector<u1> bytes = { 0xc4, 0xb6 };
  Utf8String str(bytes, 2);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("Ķ").c_str(), ss.str().c_str());
}

TEST_F(Utf8StringTest, TestOstreamSingleNullCharacter)
{
  std::vector<u1> bytes = { 0xc0, 0x80 };
  Utf8String str(bytes, 2);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("\0").c_str(), ss.str().c_str());
}

TEST_F(Utf8StringTest, TestOstreamEmbeddedNullCharacter)
{
  std::vector<u1> bytes = { 0x41, 0xc0, 0x80, 0x42 };
  Utf8String str(bytes, 4);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("A\0B").c_str(), ss.str().c_str());
}

TEST_F(Utf8StringTest, TestOstream3ByteCharacter)
{
  std::vector<u1> bytes = { 0xe2, 0x80, 0xb9 };
  Utf8String str(bytes, 3);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("‹").c_str(), ss.str().c_str());
}

TEST_F(Utf8StringTest, TestOstream6ByteCharacter)
{
  std::vector<u1> bytes = { 0xed, 0xa1, 0x80, 0xed, 0xbc, 0x8a };
  Utf8String str(bytes, 6);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("𐌊").c_str(), ss.str().c_str());
}

TEST_F(Utf8StringTest, TestOstream6ByteCharacterOverU10fff)
{
  std::vector<u1> bytes = { 0xed, 0xa2, 0x80, 0xed, 0xb1, 0xa8 };
  Utf8String str(bytes, 6);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("𠁨").c_str(), ss.str().c_str());
}

TEST_F(Utf8StringTest, TestConstructFromString1ByteCharacter)
{
  std::vector<u1> bytes = { 0x36 };
  Utf8String str("6");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(Utf8StringTest, TestConstructFromString2ByteCharacter)
{
  std::vector<u1> bytes = { 0xc4, 0xb6 };
  Utf8String str("Ķ");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(Utf8StringTest, TestConstructFromStringSingleNullCharacter)
{
  std::vector<u1> bytes = { 0xc0, 0x80 };
  Utf8String str(std::string("\0", 1));
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(Utf8StringTest, TestConstructFromStringEmbeddedNullCharacter)
{
  std::vector<u1> bytes = { 0x41, 0xc0, 0x80, 0x42 };
  Utf8String str(std::string("A\0B",3));
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(Utf8StringTest, TestConstructFromString3ByteCharacter)
{
  std::vector<u1> bytes = { 0xe2, 0x80, 0xb9 };
  Utf8String str("‹");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(Utf8StringTest, TestConstructFromString6ByteCharacter)
{
  std::vector<u1> bytes = { 0xed, 0xa1, 0x80, 0xed, 0xbc, 0x8a };
  Utf8String str("𐌊");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(Utf8StringTest, TestConstructFromString6ByteCharacterOverU10fff)
{
  std::vector<u1> bytes = { 0xed, 0xa2, 0x80, 0xed, 0xb1, 0xa8 };
  Utf8String str("𠁨");
  ASSERT_EQ(bytes, str.getBytes());
}
}
