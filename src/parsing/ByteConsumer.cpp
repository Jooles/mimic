/**
 * \file ByteConsumer.cpp
 *
 *  Created on: 23 Nov 2016
 *      Author: Julian Cromarty
 */

#include "ByteConsumer.h"
#include <type_traits>

namespace mimic
{
namespace parsing
{

ByteConsumer::ByteConsumer(std::istream& stream, uintmax_t length) :
		stream(stream), bytesRemainingCount(length)
{
}

ByteConsumer::~ByteConsumer()
{

}

uint8_t ByteConsumer::readU1()
{
	if (stream.peek() == std::istream::traits_type::eof())
		throw parse_failure("No bytes available to read");
	if (bytesRemainingCount > 0)
		bytesRemainingCount--;
	return (static_cast<uint8_t>(stream.get()));
}

uint16_t ByteConsumer::readU2()
{
	if (stream.peek() == std::istream::traits_type::eof())
		throw parse_failure("No bytes available to read");
	uint16_t value = stream.get() << 8;
	if (stream.eof())
		throw parse_failure("No bytes available to read");
	if (bytesRemainingCount > 0)
		bytesRemainingCount -= 2;
	return (static_cast<uint16_t>(value | stream.get()));
}

uint32_t ByteConsumer::readU4()
{
	if (stream.peek() == std::istream::traits_type::eof())
		throw parse_failure("No bytes available to read");
	uint32_t value = 0;
	for (int i = 0; i < 4; i++)
	{
		if (stream.eof())
			throw parse_failure("No bytes available to read");
		value |= (stream.get() << (8 * (3 - i)));
	}
	if (bytesRemainingCount > 0)
		bytesRemainingCount -= 4;
	return (value);
}

std::vector<uint8_t> ByteConsumer::readBytes(int numberOfBytes)
{
	if (stream.peek() == std::istream::traits_type::eof())
		throw parse_failure("No bytes available to read");
	std::vector<uint8_t> bytes;
	for (int i = 0; i < numberOfBytes; i++)
	{
		if (stream.peek() == std::istream::traits_type::eof())
			throw parse_failure("No bytes available to read");
		bytes.push_back(stream.get());
	}
	if (bytesRemainingCount > 0)
		bytesRemainingCount -= numberOfBytes;
	return bytes;
}

}
}

