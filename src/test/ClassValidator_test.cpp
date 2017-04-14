/**
 * \file ClassValidator_test.cpp
 *
 *  Created on: 09 Apr 2017
 *      Author: Julian Cromarty
 */

#include "test/TestCommon.h"
#include "ClassValidator.h"
#include "parsing/ByteConsumer.h"

namespace mimic
{

class ClassValidatorTest: public testing::Test
{

protected:
	ClassValidatorTest()
	{
	}

	virtual ~ClassValidatorTest()
	{
	}
};

TEST_F(ClassValidatorTest, TestClassInfoMissingUtf8)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid, ConstantPool::Class_info(2)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestClassInfoInvalidClassName)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid, ConstantPool::Class_info(2), JUtf8String("wib.ble")});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestClassInfoInvalidUnqualifiedClassName)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid, ConstantPool::Class_info(2), JUtf8String("w/ib/bl/;e")});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestClassInfoValidArrayDescriptor)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid, ConstantPool::Class_info(2), JUtf8String("[[J")});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp));
}

}
