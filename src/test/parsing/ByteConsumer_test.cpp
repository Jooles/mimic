/**
 * \file ByteConsumer_test.cpp
 *
 *  Created on: 23 Nov 2016
 *      Author: Julian Cromarty
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "parsing/ByteConsumer.h"

namespace parsing
{

class ByteConsumerTest: public testing::Test
{

protected:
	ByteConsumerTest() :
			bc(ss, 10)
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
	EXPECT_EQ(10, bc.bytesRemaining());
	EXPECT_EQ(1, bc.readU1());
	EXPECT_EQ(9, bc.bytesRemaining());
}

TEST_F(ByteConsumerTest, TestReadU2)
{
	// 0x0102
	EXPECT_EQ(10, bc.bytesRemaining());
	EXPECT_EQ(258, bc.readU2());
	EXPECT_EQ(8, bc.bytesRemaining());
}

TEST_F(ByteConsumerTest, TestReadU4)
{
	// 0x01020304
	EXPECT_EQ(10, bc.bytesRemaining());
	EXPECT_EQ(16909060, bc.readU4());
	EXPECT_EQ(6, bc.bytesRemaining());
}

TEST_F(ByteConsumerTest, TestReadBytes)
{
	EXPECT_EQ(10, bc.bytesRemaining());
	std::vector<uint8_t> expected =
	{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a };
	EXPECT_EQ(expected, bc.readBytes(10));
	EXPECT_EQ(0, bc.bytesRemaining());
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

TEST_F(ByteConsumerTest, TestReadBytesConsecutive)
{
	std::vector<uint8_t> expected1 =
	{ 0x01, 0x02, 0x03, 0x04 };
	std::vector<uint8_t> expected2 =
	{ 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a };
	EXPECT_EQ(expected1, bc.readBytes(4));
	EXPECT_EQ(expected2, bc.readBytes(6));
}

TEST_F(ByteConsumerTest, TestReadPastEndOfStreamU1)
{
	EXPECT_EQ(10, bc.bytesRemaining());
	for (int i = 0; i < 10; i++)
	{
		bc.readU1();
	}
	EXPECT_EQ(0, bc.bytesRemaining());
	ASSERT_THROW(bc.readU1(), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadPastEndOfStreamU2)
{
	EXPECT_EQ(10, bc.bytesRemaining());
	for (int i = 0; i < 5; i++)
	{
		bc.readU2();
	}
	EXPECT_EQ(0, bc.bytesRemaining());
	ASSERT_THROW(bc.readU2(), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadPastEndOfStreamU4)
{
	EXPECT_EQ(10, bc.bytesRemaining());
	bc.readU4();
	bc.readU4();
	EXPECT_EQ(2, bc.bytesRemaining());
	ASSERT_THROW(bc.readU4(), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadPastEndOfStreamBytes)
{
	ASSERT_THROW(bc.readBytes(11), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadEmptyStreamU1)
{
	std::stringstream ss2;
	ByteConsumer bc2(ss2);
	ASSERT_THROW(bc2.readU1(), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadEmptyStreamU2)
{
	std::stringstream ss2;
	ByteConsumer bc2(ss2);
	ASSERT_THROW(bc2.readU2(), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadEmptyStreamU4)
{
	std::stringstream ss2;
	ByteConsumer bc2(ss2);
	ASSERT_THROW(bc2.readU2(), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadEmptyStreamBytes)
{
	std::stringstream ss2;
	ByteConsumer bc2(ss2);
	ASSERT_THROW(bc2.readBytes(1), parse_failure);
}

} /* namespace parsing */
