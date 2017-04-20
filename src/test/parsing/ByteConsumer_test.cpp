/**
 * \file ByteConsumer_test.cpp
 *
 *  Created on: 23 Nov 2016
 *      Author: Julian Cromarty
 */

#include "test/TestCommon.h"
#include "parsing/ByteConsumer.h"

namespace mimic
{
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
	EXPECT_EQ(10u, bc.bytesRemaining());
	EXPECT_EQ(1u, bc.readU1());
	EXPECT_EQ(9u, bc.bytesRemaining());
}

TEST_F(ByteConsumerTest, TestReadU2)
{
	// 0x0102
	EXPECT_EQ(10u, bc.bytesRemaining());
	EXPECT_EQ(258u, bc.readU2());
	EXPECT_EQ(8u, bc.bytesRemaining());
}

TEST_F(ByteConsumerTest, TestReadU4)
{
	// 0x01020304
	EXPECT_EQ(10u, bc.bytesRemaining());
	EXPECT_EQ(16909060u, bc.readU4());
	EXPECT_EQ(6u, bc.bytesRemaining());
}

TEST_F(ByteConsumerTest, TestReadBytes)
{
	EXPECT_EQ(10u, bc.bytesRemaining());
	std::vector<uint8_t> expected =
	{ 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u, 0x09u, 0x0au };
	EXPECT_EQ(expected, bc.readBytes(10));
	EXPECT_EQ(0u, bc.bytesRemaining());
}

TEST_F(ByteConsumerTest, TestReadConsecutiveU1)
{
	EXPECT_EQ(1u, bc.readU1());
	EXPECT_EQ(2u, bc.readU1());
	EXPECT_EQ(3u, bc.readU1());
	EXPECT_EQ(4u, bc.readU1());
	EXPECT_EQ(5u, bc.readU1());
	EXPECT_EQ(6u, bc.readU1());
	EXPECT_EQ(7u, bc.readU1());
	EXPECT_EQ(8u, bc.readU1());
	EXPECT_EQ(9u, bc.readU1());
	EXPECT_EQ(10u, bc.readU1());
}

TEST_F(ByteConsumerTest, TestReadConsecutiveU2)
{
	EXPECT_EQ(258u, bc.readU2());
	EXPECT_EQ(772u, bc.readU2());
	EXPECT_EQ(1286u, bc.readU2());
	EXPECT_EQ(1800u, bc.readU2());
	EXPECT_EQ(2314u, bc.readU2());
}

TEST_F(ByteConsumerTest, TestReadConsecutiveU4)
{
	EXPECT_EQ(16909060u, bc.readU4());
	EXPECT_EQ(84281096u, bc.readU4());
}

TEST_F(ByteConsumerTest, TestReadBytesConsecutive)
{
	std::vector<uint8_t> expected1 =
	{ 0x01u, 0x02u, 0x03u, 0x04u };
	std::vector<uint8_t> expected2 =
	{ 0x05u, 0x06u, 0x07u, 0x08u, 0x09u, 0x0au };
	EXPECT_EQ(expected1, bc.readBytes(4));
	EXPECT_EQ(expected2, bc.readBytes(6));
}

TEST_F(ByteConsumerTest, TestReadPastEndOfStreamU1)
{
	EXPECT_EQ(10u, bc.bytesRemaining());
	for (int i = 0; i < 10; i++)
	{
		bc.readU1();
	}
	EXPECT_EQ(0u, bc.bytesRemaining());
	ASSERT_THROW(bc.readU1(), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadPastEndOfStreamU2)
{
	EXPECT_EQ(10u, bc.bytesRemaining());
	for (int i = 0; i < 5; i++)
	{
		bc.readU2();
	}
	EXPECT_EQ(0u, bc.bytesRemaining());
	ASSERT_THROW(bc.readU2(), parse_failure);
}

TEST_F(ByteConsumerTest, TestReadPastEndOfStreamU4)
{
	EXPECT_EQ(10u, bc.bytesRemaining());
	bc.readU4();
	bc.readU4();
	EXPECT_EQ(2u, bc.bytesRemaining());
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

}
}

