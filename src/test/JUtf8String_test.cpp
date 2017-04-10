/**
 * \file JUtf8String_test.cpp
 *
 *  Created on: 04 Apr 2017
 *      Author: Julian Cromarty
 */
#include <sstream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "JUtf8String.h"
#include "parsing/ByteConsumer.h"
#include "parsing/ParseFailureException.h"

namespace mimic
{

class JUtf8StringTest: public testing::Test
{

protected:
  JUtf8StringTest()
  {
  }

  virtual ~JUtf8StringTest()
  {
  }
};

TEST_F(JUtf8StringTest, TestNullByteThrowsOnConstruction)
{
  std::vector<u1> bytes = { 0x00 };
  ASSERT_THROW(JUtf8String{bytes}, parsing::parse_failure);
}

TEST_F(JUtf8StringTest, TestByteInInvalidRangeThrowsOnConstruction)
{
  for (int i = 0xf0; i <= 0xff; i++) {
    std::vector<u1> bytes = { (u1) i };
    ASSERT_THROW(JUtf8String{bytes}, parsing::parse_failure);
  }
}

TEST_F(JUtf8StringTest, TestOneByteCharacterGivesLength1)
{
  for (u1 i = 0x01; i < 0x80; i++) {
    std::vector<u1> bytes = { (u1) i };
    ASSERT_EQ(1, JUtf8String(bytes).length());
  }
}

TEST_F(JUtf8StringTest, TestTwoByteCharacterGivesLength1)
{
  for (int i = 0x80; i < 0x7ff; i ++) {
    if (i >= 0xf0 && i <= 0xff)
      continue;
    u1 byte1 = ((i >> 6) & 0x1f) | 0xc0;
    u1 byte2 = (i & 0x3f) | 0x80;
    std::vector<u1> bytes = { byte1, byte2 };
    ASSERT_EQ(2, bytes.size());
    ASSERT_EQ(1, JUtf8String(bytes).length());
  }
}

TEST_F(JUtf8StringTest, TestThreeByteCharacterGivesLength1)
{
  for (int i = 0x800; i < 0xffff; i ++) {
    u1 byte1 = ((i >> 12) & 0x0f) | 0xe0;
    u1 byte2 = ((i >> 6) & 0x3f) | 0x80;
    u1 byte3 = (i & 0x3f) | 0x80;
    std::vector<u1> bytes = { byte1, byte2, byte3 };
    ASSERT_EQ(3, bytes.size());
    ASSERT_EQ(1, JUtf8String(bytes).length());
  }
}

TEST_F(JUtf8StringTest, Test6ByteCharacterGivesLength1)
{
  for (int i = 0x10000; i < 0x1ffff; i ++) {
    u1 byte1 = 0xed;
    u1 byte2 = ((i >> 16) & 0x0f) | 0xa0;
    u1 byte3 = ((i >> 10) & 0x3f) | 0x80;
    u1 byte4 = 0xed;
    u1 byte5 = ((i >> 6) & 0x0f) | 0xb0;
    u1 byte6 = (i & 0x3f) | 0x80;
    std::vector<u1> bytes = { byte1, byte2, byte3, byte4, byte5, byte6 };
    ASSERT_EQ(6, bytes.size());
    ASSERT_EQ(1, JUtf8String(bytes).length());
  }
}

TEST_F(JUtf8StringTest, TestOstream1ByteCharacter)
{
  std::vector<u1> bytes = { 0x36 };
  JUtf8String str(bytes);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("6").c_str(), ss.str().c_str());
}

TEST_F(JUtf8StringTest, TestOstream2ByteCharacter)
{
  std::vector<u1> bytes = { 0xc4, 0xb6 };
  JUtf8String str(bytes);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("Ķ").c_str(), ss.str().c_str());
}

TEST_F(JUtf8StringTest, TestOstreamSingleNullCharacter)
{
  std::vector<u1> bytes = { 0xc0, 0x80 };
  JUtf8String str(bytes);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("\0").c_str(), ss.str().c_str());
}

TEST_F(JUtf8StringTest, TestOstreamEmbeddedNullCharacter)
{
  std::vector<u1> bytes = { 0x41, 0xc0, 0x80, 0x42 };
  JUtf8String str(bytes);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("A\0B").c_str(), ss.str().c_str());
}

TEST_F(JUtf8StringTest, TestOstream3ByteCharacter)
{
  std::vector<u1> bytes = { 0xe2, 0x80, 0xb9 };
  JUtf8String str(bytes);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("‹").c_str(), ss.str().c_str());
}

TEST_F(JUtf8StringTest, TestOstream6ByteCharacter)
{
  std::vector<u1> bytes = { 0xed, 0xa1, 0x80, 0xed, 0xbc, 0x8a };
  JUtf8String str(bytes);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("𐌊").c_str(), ss.str().c_str());
}

TEST_F(JUtf8StringTest, TestOstream6ByteCharacterOverU10fff)
{
  std::vector<u1> bytes = { 0xed, 0xa2, 0x80, 0xed, 0xb1, 0xa8 };
  JUtf8String str(bytes);
  std::stringstream ss;
  ss << str;
  ASSERT_STREQ(std::string("𠁨").c_str(), ss.str().c_str());
}

TEST_F(JUtf8StringTest, TestIstream1ByteCharacter)
{
  std::vector<u1> bytes = { 0x36 };
  JUtf8String str;
  std::stringstream ss;
  ss << std::string("6");
  ss >> str;
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestIstream2ByteCharacter)
{
  std::vector<u1> bytes = { 0xc4, 0xb6 };
  JUtf8String str;
  std::stringstream ss;
  ss << std::string("Ķ");
  ss >> str;
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestIstreamSingleNullCharacter)
{
  std::vector<u1> bytes = { 0xc0, 0x80 };
  JUtf8String str;
  std::stringstream ss;
  ss << std::string(std::string("\0", 1));
  ss >> str;
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestIstreamEmbeddedNullCharacter)
{
  std::vector<u1> bytes = { 0x41, 0xc0, 0x80, 0x42 };
  JUtf8String str;
  std::stringstream ss;
  ss << std::string(std::string("A\0B",3));
  ss >> str;
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestIstream3ByteCharacter)
{
  std::vector<u1> bytes = { 0xe2, 0x80, 0xb9 };
  JUtf8String str;
  std::stringstream ss;
  ss << std::string("‹");
  ss >> str;
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestIstream6ByteCharacter)
{
  std::vector<u1> bytes = { 0xed, 0xa1, 0x80, 0xed, 0xbc, 0x8a };
  JUtf8String str;
  std::stringstream ss;
  ss << std::string("𐌊");
  ss >> str;
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestIstream6ByteCharacterOverU10fff)
{
  std::vector<u1> bytes = { 0xed, 0xa2, 0x80, 0xed, 0xb1, 0xa8 };
  JUtf8String str;
  std::stringstream ss;
  ss << std::string("𠁨");
  ss >> str;
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestConstructFromString1ByteCharacter)
{
  std::vector<u1> bytes = { 0x36 };
  JUtf8String str("6");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestConstructFromString2ByteCharacter)
{
  std::vector<u1> bytes = { 0xc4, 0xb6 };
  JUtf8String str("Ķ");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestConstructFromStringSingleNullCharacter)
{
  std::vector<u1> bytes = { 0xc0, 0x80 };
  JUtf8String str("\0", 1);
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestConstructFromStringEmbeddedNullCharacter)
{
  std::vector<u1> bytes = { 0x41, 0xc0, 0x80, 0x42 };
  JUtf8String str("A\0B", 3);
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestConstructFromString3ByteCharacter)
{
  std::vector<u1> bytes = { 0xe2, 0x80, 0xb9 };
  JUtf8String str("‹");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestConstructFromString6ByteCharacter)
{
  std::vector<u1> bytes = { 0xed, 0xa1, 0x80, 0xed, 0xbc, 0x8a };
  JUtf8String str("𐌊");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestConstructFromString6ByteCharacterOverU10fff)
{
  std::vector<u1> bytes = { 0xed, 0xa2, 0x80, 0xed, 0xb1, 0xa8 };
  JUtf8String str("𠁨");
  ASSERT_EQ(bytes, str.getBytes());
}

TEST_F(JUtf8StringTest, TestConstructFromEmptyString)
{
  std::vector<u1> bytes;
  JUtf8String str("");
  ASSERT_EQ(bytes, str.getBytes());
  ASSERT_EQ(0, str.length());
}

TEST_F(JUtf8StringTest, TestIteratorPlus)
{
  JUtf8String str("foo");
  auto iter = str.begin();
  ASSERT_EQ('f', *iter);
  ASSERT_EQ('o', *(iter + 2));
  ASSERT_EQ('f', *iter);
}

TEST_F(JUtf8StringTest, TestIteratorPlusEquals)
{
  JUtf8String str("bar");
  auto iter = str.begin();
  ASSERT_EQ('b', *iter);
  ASSERT_EQ('r', *(iter += 2));
  ASSERT_EQ('r', *iter);
}

TEST_F(JUtf8StringTest, TestConstructFromRange)
{
  JUtf8String str("foobar");
  ASSERT_EQ(JUtf8String("foo"), JUtf8String(str.begin(), str.begin()+3));
}

TEST_F(JUtf8StringTest, TestConstructFromRangeMiddleOfString)
{
  JUtf8String str("foobar");
  ASSERT_EQ(JUtf8String("oba"), JUtf8String(str.begin() + 2, str.begin() + 5));
}

TEST_F(JUtf8StringTest, TestFindSingleCharacter)
{
  JUtf8String str("foobar");
  ASSERT_EQ(str.begin() + 3, str.find(JUtf8String("b")));
}

TEST_F(JUtf8StringTest, TestFindString)
{
  JUtf8String str("foobar");
  ASSERT_EQ(str.begin() + 2, str.find(JUtf8String("oba")));
}

TEST_F(JUtf8StringTest, TestFindNotFound)
{
  JUtf8String str("foobar");
  ASSERT_EQ(str.end(), str.find(JUtf8String("wibble")));
}

TEST_F(JUtf8StringTest, TestContainsSingleCharacter)
{
  JUtf8String str("foobar");
  ASSERT_TRUE(str.contains(JUtf8String("b")));
}

TEST_F(JUtf8StringTest, TestContainsString)
{
  JUtf8String str("foobar");
  ASSERT_TRUE(str.contains(JUtf8String("oba")));
}

TEST_F(JUtf8StringTest, TestContainsNotFound)
{
  JUtf8String str("foobar");
  ASSERT_FALSE(str.contains(JUtf8String("wibble")));
}

TEST_F(JUtf8StringTest, TestContainsList)
{
  JUtf8String str("foobar");
  ASSERT_TRUE(str.contains(std::vector<JUtf8String> {JUtf8String("q"), JUtf8String("ba")}));
}

TEST_F(JUtf8StringTest, TestContainsListNotFound)
{
  JUtf8String str("foobar");
  ASSERT_FALSE(str.contains(std::vector<JUtf8String> {JUtf8String("q"), JUtf8String("wub")}));
}

TEST_F(JUtf8StringTest, TestSplitCharacterInMiddle)
{
  JUtf8String str("foobarbat");
  std::vector<JUtf8String> expected = {JUtf8String("fooba"), JUtf8String("bat")};
  auto actual = str.split(JUtf8String("r"));
  ASSERT_EQ(2, actual.size());
  ASSERT_EQ(expected[0], actual[0]);
  ASSERT_EQ(expected[1], actual[1]);
}

TEST_F(JUtf8StringTest, TestSplitStringInMiddle)
{
  JUtf8String str("foobarbat");
  std::vector<JUtf8String> expected = {JUtf8String("foo"), JUtf8String("bat")};
  auto actual = str.split(JUtf8String("bar"));
  ASSERT_EQ(2, actual.size());
  ASSERT_EQ(expected[0], actual[0]);
  ASSERT_EQ(expected[1], actual[1]);
}

TEST_F(JUtf8StringTest, TestSplitEmptyDelimiter)
{
  JUtf8String str("foo");
  std::vector<JUtf8String> expected = {JUtf8String("f"), JUtf8String("o"), JUtf8String("o")};
  auto actual = str.split(JUtf8String(""));
  ASSERT_EQ(3, actual.size());
  ASSERT_EQ(expected[0], actual[0]);
  ASSERT_EQ(expected[1], actual[1]);
  ASSERT_EQ(expected[2], actual[2]);
}
}
