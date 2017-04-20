/**
 * \file FieldDescriptor_test.cpp
 *
 *  Created on: 02 Apr 2017
 *      Author: Julian Cromarty
 */

#include "test/TestCommon.h"
#include <algorithm>
#include <array>
#include "FieldDescriptor.h"

namespace mimic
{

class FieldDescriptorTest: public testing::Test
{
public:
  std::array<u1, 10> types {{'B','C','D','F','I','J','L','S','Z','['}};

protected:
	FieldDescriptorTest()
	{
	}

	virtual ~FieldDescriptorTest()
	{
	}
};

TEST_F(FieldDescriptorTest, TestInvalidType)
{
  for (u1 i = 1; i < 128; i++)
  {
    if (std::find(types.begin(), types.end(), i) != types.end())
      continue;
    ASSERT_THROW(FieldDescriptor(JUtf8String(std::vector<u1> {i})), parsing::parse_failure);
  }
}

TEST_F(FieldDescriptorTest, TestInvalidArrayAfterType)
{
  ASSERT_THROW(FieldDescriptor(JUtf8String("[[B[[[C")), parsing::parse_failure);
}

TEST_F(FieldDescriptorTest, TestInvalidArrayTooManyDimensions)
{
  std::stringstream descriptor;
  for (int i = 0; i < 256; i++)
  {
    descriptor << '[';
  }
  descriptor << 'B';
  ASSERT_THROW(FieldDescriptor(JUtf8String(descriptor.str())), parsing::parse_failure);
}

TEST_F(FieldDescriptorTest, TestValidArrayMaxDimensions)
{
  std::stringstream ss;
  for (int i = 0; i < 255; i++)
  {
    ss << '[';
  }
  ss << 'B';
  FieldDescriptor descriptor(JUtf8String(ss.str()));
  ASSERT_EQ(255, descriptor.getArrayDimensions());
  ASSERT_FALSE(descriptor.isPrimitive());
  ss = std::stringstream();
  ss << "byte";
  for (int i = 0; i < 255; i++)
  {
    ss << "[]";
  }
  ASSERT_EQ(JUtf8String(ss.str()), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestByte)
{
  FieldDescriptor descriptor(JUtf8String("B"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_TRUE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("byte"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestChar)
{
  FieldDescriptor descriptor(JUtf8String("C"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_TRUE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("char"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestDouble)
{
  FieldDescriptor descriptor(JUtf8String("D"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_TRUE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("double"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestFloat)
{
  FieldDescriptor descriptor(JUtf8String("F"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_TRUE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("float"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestInt)
{
  FieldDescriptor descriptor(JUtf8String("I"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_TRUE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("int"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestLong)
{
  FieldDescriptor descriptor(JUtf8String("J"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_TRUE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("long"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestShort)
{
  FieldDescriptor descriptor(JUtf8String("S"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_TRUE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("short"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestBoolean)
{
  FieldDescriptor descriptor(JUtf8String("Z"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_TRUE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("boolean"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestClassWithoutSemicolon)
{
  ASSERT_THROW(FieldDescriptor(JUtf8String("Ljava/lang/String")), parsing::parse_failure);
}

TEST_F(FieldDescriptorTest, TestClassWithoutName)
{
  ASSERT_THROW(FieldDescriptor(JUtf8String("L;")), parsing::parse_failure);
}

TEST_F(FieldDescriptorTest, TestClass)
{
  FieldDescriptor descriptor(JUtf8String("Ljava/lang/String;"));
  ASSERT_EQ(0, descriptor.getArrayDimensions());
  ASSERT_FALSE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("java/lang/String"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestClassArray)
{
  FieldDescriptor descriptor(JUtf8String("[[Ljava/lang/String;"));
  ASSERT_EQ(2, descriptor.getArrayDimensions());
  ASSERT_FALSE(descriptor.isPrimitive());
  ASSERT_EQ(JUtf8String("java/lang/String[][]"), descriptor.getClassName());
}

TEST_F(FieldDescriptorTest, TestEquals)
{
  ASSERT_EQ(FieldDescriptor(JUtf8String("Z")), FieldDescriptor(JUtf8String("Z")));
}
}
