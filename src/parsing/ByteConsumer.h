/**
 * \file ByteConsumer.h
 *
 *  Created on: 23 Nov 2016
 *      Author: Julian Cromarty
 */

#ifndef SRC_PARSING_BYTECONSUMER_H_
#define SRC_PARSING_BYTECONSUMER_H_

#include <istream>
#include <memory>

namespace parsing {

/**
 * Reads a byte stream in small pieces, for use by parsers
 *
 * Multi-byte values are read as big-endian
 */
class ByteConsumer {
public:
	ByteConsumer() = delete;
	ByteConsumer(const ByteConsumer&) = delete;

	/**
	 * Constructor
	 *
	 * @param buf Stream buffer to read from
	 */
	ByteConsumer(std::istream& stream);
	virtual ~ByteConsumer();

	/**
	 * Read a 1-byte unsigned integer value from the stream
	 *
	 * @return the next byte from the stream as an unsigned integer
	 */
	uint8_t readU1();

	/**
	 * Read a 2-byte unsigned integer value from the stream
	 *
	 * @return the next 2 bytes from the stream as an unsigned integer
	 */
	uint16_t readU2();

	/**
	 * Read a 4-byte unsigned integer value from the stream
	 *
	 * @return the next 4 bytes from the stream as an unsigned integer
	 */
	uint32_t readU4();

private:
	std::istream& stream;
};

} /* namespace parsing */

#endif /* SRC_PARSING_BYTECONSUMER_H_ */
