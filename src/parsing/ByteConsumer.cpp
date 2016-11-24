/**
 * \file ByteConsumer.cpp
 *
 *  Created on: 23 Nov 2016
 *      Author: Julian Cromarty
 */

#include "ByteConsumer.h"

namespace parsing {

ByteConsumer::ByteConsumer(std::istream& stream)
: stream(stream)
{
}

ByteConsumer::~ByteConsumer() {

}

uint8_t ByteConsumer::readU1() {
	return static_cast<uint8_t>(stream.get());
}

uint16_t ByteConsumer::readU2() {
	return static_cast<uint16_t>(stream.get() << 8 | stream.get());
}

uint32_t ByteConsumer::readU4() {
	return static_cast<uint32_t>(stream.get() << 24 | stream.get() << 16 | stream.get() << 8 | stream.get());
}

} /* namespace parsing */
