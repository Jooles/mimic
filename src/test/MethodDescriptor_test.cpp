/**
 * \file MethodDescriptor_test.cpp
 *
 *  Created on: 02 Apr 2017
 *      Author: Julian Cromarty
 */

#include "test/TestCommon.h"
#include <algorithm>
#include <array>
#include "MethodDescriptor.h"

namespace mimic
{

class MethodDescriptorTest: public testing::Test
{
public:

protected:
	MethodDescriptorTest()
	{
	}

	virtual ~MethodDescriptorTest()
	{
	}
};
/* No need to test anything covered by FieldDescriptor */

TEST_F(MethodDescriptorTest, TestInvalidReturnType)
{
  ASSERT_THROW(MethodDescriptor(JUtf8String("()M")), std::runtime_error);
}

TEST_F(MethodDescriptorTest, TestMissingOpenParenthesis)
{
  ASSERT_THROW(MethodDescriptor(JUtf8String(")Z")), std::runtime_error);
}

TEST_F(MethodDescriptorTest, TestMissingCloseParenthesis)
{
  ASSERT_THROW(MethodDescriptor(JUtf8String("(Z")), std::runtime_error);
}

TEST_F(MethodDescriptorTest, TestMissingParentheses)
{
  ASSERT_THROW(MethodDescriptor(JUtf8String("Z")), std::runtime_error);
}

TEST_F(MethodDescriptorTest, TestDescriptorBeforeParentheses)
{
  ASSERT_THROW(MethodDescriptor(JUtf8String("Z()Z")), std::runtime_error);
}

TEST_F(MethodDescriptorTest, TestMissingReturnDescriptor)
{
  ASSERT_THROW(MethodDescriptor(JUtf8String("()")), std::runtime_error);
}

TEST_F(MethodDescriptorTest, TestVoidReturn)
{
  MethodDescriptor d(JUtf8String("()V"));
  ASSERT_FALSE(d.getReturnType());
}

TEST_F(MethodDescriptorTest, TestBoolReturn)
{
  MethodDescriptor d(JUtf8String("()Z"));
  auto return_type = d.getReturnType();
  ASSERT_TRUE(return_type);
  ASSERT_EQ(FieldDescriptor(JUtf8String("Z")), return_type.value());
}

TEST_F(MethodDescriptorTest, TestIntParamLongReturn)
{
  MethodDescriptor d(JUtf8String("(I)J"));
  auto return_type = d.getReturnType();
  ASSERT_TRUE(return_type);
  ASSERT_EQ(FieldDescriptor(JUtf8String("J")), return_type.value());
  auto parameters = d.getParameters();
  ASSERT_EQ(1u, parameters.size());
  ASSERT_EQ(FieldDescriptor(JUtf8String("I")), parameters.at(0));
}

TEST_F(MethodDescriptorTest, TestShortCharClassParamByteArrayReturn)
{
  MethodDescriptor d(JUtf8String("(SCLjava/lang/String;)[B"));
  auto return_type = d.getReturnType();
  ASSERT_TRUE(return_type);
  ASSERT_EQ(FieldDescriptor(JUtf8String("[B")), return_type.value());
  auto parameters = d.getParameters();
  ASSERT_EQ(3u, parameters.size());
  ASSERT_EQ(FieldDescriptor(JUtf8String("S")), parameters.at(0));
  ASSERT_EQ(FieldDescriptor(JUtf8String("C")), parameters.at(1));
  ASSERT_EQ(FieldDescriptor(JUtf8String("Ljava/lang/String;")), parameters.at(2));
}

TEST_F(MethodDescriptorTest, TestClassMissingSemicolonParamVoidReturn)
{
  ASSERT_THROW(MethodDescriptor(JUtf8String("(Ljava/lang/String)V")), std::runtime_error);
}

TEST_F(MethodDescriptorTest, TestMultiDimensionArrayReturnType)
{
  MethodDescriptor d(JUtf8String("()[[[J"));
  auto return_type = d.getReturnType();
  ASSERT_EQ(FieldDescriptor(JUtf8String("[[[J")), return_type.value());
  ASSERT_EQ(0u, d.getParameters().size());
}

TEST_F(MethodDescriptorTest, TestMultiDimensionArrayTypes)
{
  MethodDescriptor d(JUtf8String("([[I)[[[J"));
  auto return_type = d.getReturnType();
  ASSERT_TRUE(return_type);
  ASSERT_EQ(FieldDescriptor(JUtf8String("[[[J")), return_type.value());
  auto parameters = d.getParameters();
  ASSERT_EQ(1u, parameters.size());
  ASSERT_EQ(FieldDescriptor(JUtf8String("[[I")), parameters.at(0));
}
}
