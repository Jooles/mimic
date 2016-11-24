/**
 * \file ByteConsumer_test.cpp
 *
 *  Created on: 23 Nov 2016
 *      Author: Julian Cromarty
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "parsing/ByteConsumer.h"
//#include "parsing/ParseFailureException.h"

namespace parsing {

class ByteConsumerTest : public testing::Test
{

protected:
	ByteConsumerTest()
	: bc(ss)
	{
		for (uint8_t i = 1; i <= 10; i++)
		{
			ss.put(i);
		}
	}

	virtual ~ByteConsumerTest()
	{
	}

	ByteConsumer bc;

private:
	std::stringstream ss;
};

TEST_F(ByteConsumerTest, TestReadU1)
{
	// 0x01
	EXPECT_EQ(1, bc.readU1());
}

TEST_F(ByteConsumerTest, TestReadU2)
{
	// 0x0102
	EXPECT_EQ(258, bc.readU2());
}

TEST_F(ByteConsumerTest, TestReadU4)
{
	// 0x01020304
	EXPECT_EQ(16909060, bc.readU4());
}

TEST_F(ByteConsumerTest, TestReadConsecutiveU1)
{
	EXPECT_EQ(1, bc.readU1());
	EXPECT_EQ(2, bc.readU1());
	EXPECT_EQ(3, bc.readU1());
	EXPECT_EQ(4, bc.readU1());
	EXPECT_EQ(5, bc.readU1());
	EXPECT_EQ(6, bc.readU1());
	EXPECT_EQ(7, bc.readU1());
	EXPECT_EQ(8, bc.readU1());
	EXPECT_EQ(9, bc.readU1());
	EXPECT_EQ(10, bc.readU1());
}

TEST_F(ByteConsumerTest, TestReadConsecutiveU2)
{
	EXPECT_EQ(258, bc.readU2());
	EXPECT_EQ(772, bc.readU2());
	EXPECT_EQ(1286, bc.readU2());
	EXPECT_EQ(1800, bc.readU2());
	EXPECT_EQ(2314, bc.readU2());
}

TEST_F(ByteConsumerTest, TestReadConsecutiveU4)
{
	EXPECT_EQ(16909060, bc.readU4());
	EXPECT_EQ(84281096, bc.readU4());
}

/*TEST_F(ByteConsumerTest, TestReadPastEndOfStream)
{
	bc.readU4();
	bc.readU4();
	ASSERT_THROW(bc.readU4(), parse_failure_exception);
}

TEST_F(ByteConsumerTest, TestReadEmptyStream)
{
	std::stringstream ss2;
	ByteConsumer bc2(ss2);
	ASSERT_THROW(bc2.readU1(), parse_failure_exception);
}*/

} /* namespace parsing */