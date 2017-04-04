/**
 * \file ClassFile_test.cpp
 *
 *  Created on: 02 Apr 2017
 *      Author: Julian Cromarty
 */

#include "test/TestCommon.h"
#include "ClassFile.h"
#include "parsing/ByteConsumer.h"
#include "parsing/ParseFailureException.h"

namespace mimic
{

class ClassFileTest: public testing::Test
{

protected:
	ClassFileTest()
	{
	}

	virtual ~ClassFileTest()
	{
	}
};

TEST_F(ClassFileTest, TestInvalidMagicNumber)
{
	std::stringstream ss;
	ss.put(0x12);
	ss.put(0x34);
	ss.put(0x56);
	ss.put(0x78);
	parsing::ByteConsumer bc(ss);
	ASSERT_THROW(ClassFile
	{ bc }, parsing::parse_failure);
}

TEST_F(ClassFileTest, TestHelloWorld)
{
	fs::path path("src/test/resources/HelloWorld.class");
	std::ifstream file;
	file.open(path);
	parsing::ByteConsumer bc(file, fs::file_size(path));
	ClassFile clazz(bc);
	ASSERT_EQ(52, clazz.getMajorVersion());
	ASSERT_EQ(0, clazz.getMinorVersion());
}

}
