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
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::Class_info(2)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestClassInfoWrongUtf8Index)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::Class_info(2),
                                                     ConstantPool::Integer_info(1234)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestClassInfoInvalidClassName)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::Class_info(2),
                                                     JUtf8String("wib.ble")});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestClassInfoInvalidUnqualifiedClassName)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::Class_info(2),
                                                     JUtf8String("w/ib/bl/;e")});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestClassInfoValidArrayDescriptor)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::Class_info(2),
                                                     FieldDescriptor(JUtf8String("[[J"))});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestStringInfoMissingUtf8)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::String_info(2)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestStringInfoWrongUtf8Index)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::String_info(2),
                                                     ConstantPool::Integer_info(1234)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestStringInfoValid)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::String_info(2),
                                                     JUtf8String("foo")});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidReferenceKind)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(static_cast<ConstantPool::reference_kind>(10), 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexGetField)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::getField, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexGetStatic)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::getStatic, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexInvokeInterface)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeInterface, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexInvokeSpecial)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeSpecial, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexInvokeStatic)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeStatic, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexInvokeVirtual)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeVirtual, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexNewInvokeSpecial)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::newInvokeSpecial, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexPutField)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::putField, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoInvalidIndexPutStatic)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::putStatic, 1)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidGetField)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::getField, 2),
                                                     ConstantPool::Fieldref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidGetStatic)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::getStatic, 2),
                                                     ConstantPool::Fieldref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeInterface)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeInterface, 2),
                                                     ConstantPool::InterfaceMethodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeSpecialV51)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeSpecial, 2),
                                                     ConstantPool::Methodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 51, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeStaticV51)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeStatic, 2),
                                                     ConstantPool::Methodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 51, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeSpecialV52)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeSpecial, 2),
                                                     ConstantPool::Methodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeStaticV52)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeStatic, 2),
                                                     ConstantPool::Methodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeSpecialV51Interface)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeSpecial, 2),
                                                     ConstantPool::InterfaceMethodref_info(0, 0)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 51, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeStaticV51Interface)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeStatic, 2),
                                                     ConstantPool::InterfaceMethodref_info(0, 0)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 51, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeSpecialV52Interface)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeSpecial, 2),
                                                     ConstantPool::InterfaceMethodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeStaticV52Interface)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeStatic, 2),
                                                     ConstantPool::InterfaceMethodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidInvokeVirtual)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::invokeVirtual, 2),
                                                     ConstantPool::Methodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidNewInvokeSpecial)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::newInvokeSpecial, 2),
                                                     ConstantPool::Methodref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidPutField)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::putField, 2),
                                                     ConstantPool::Fieldref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodHandleInfoValidPutStatic)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodHandle_info(ConstantPool::reference_kind::putStatic, 2),
                                                     ConstantPool::Fieldref_info(0, 0)});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestMethodTypeInfoMissingUtf8)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodType_info(2)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodTypeInfoWrongUtf8Index)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodType_info(2),
                                                     ConstantPool::Integer_info(1234)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodTypeInfoInvalidClassName)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodType_info(2),
                                                     JUtf8String("wib.ble")});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodTypeInfoInvalidUnqualifiedClassName)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodType_info(2),
                                                     JUtf8String("w/ib/bl/;e")});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestMethodTypeInfoValidArrayDescriptor)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::MethodType_info(2),
                                                     MethodDescriptor(JUtf8String("()[[J"))});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestInvokeDynamicInfoMissingNameAndType)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::InvokeDynamic_info(3, 2)});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestInvokeDynamicInfoValid)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::InvokeDynamic_info(3, 2),
                                                     ConstantPool::NameAndType_info(4, 5),
                                                     ConstantPool::tag::Invalid,
                                                     JUtf8String("Foo"),
                                                     FieldDescriptor(JUtf8String("C"))});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}

TEST_F(ClassValidatorTest, TestNameAndTypeInfoMissingName)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::NameAndType_info(3, 2),
                                                     FieldDescriptor(JUtf8String("B"))});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestNameAndTypeInfoMissingDescriptor)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::NameAndType_info(2, 3),
                                                     JUtf8String("Foo")});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestNameAndTypeInfoInvalidName)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::NameAndType_info(2, 3),
                                                     FieldDescriptor(JUtf8String("B")),
                                                     FieldDescriptor(JUtf8String("B"))});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestNameAndTypeInfoInvalidDescriptor)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::NameAndType_info(2, 3),
                                                     JUtf8String("Foo"),
                                                     JUtf8String("Foo")});
	ASSERT_THROW(ClassValidator::validateConstantPool(cp, 52, 0), std::runtime_error);
}

TEST_F(ClassValidatorTest, TestNameAndTypeInfoValid)
{
  ConstantPool cp(std::vector<ConstantPool::cp_type>{ConstantPool::tag::Invalid,
                                                     ConstantPool::NameAndType_info(2, 3),
                                                     JUtf8String("Foo"),
                                                     FieldDescriptor(JUtf8String("B"))});
	ASSERT_NO_THROW(ClassValidator::validateConstantPool(cp, 52, 0));
}
}
