/**
 * \file ByteConsumer.h
 *
 *  Created on: 23 Nov 2016
 *      Author: Julian Cromarty
 */

#ifndef SRC_PARSING_BYTECONSUMER_H_
#define SRC_PARSING_BYTECONSUMER_H_

#include <istream>
#include "Common.h"
#include "ParseFailureException.h"

namespace mimic
{
namespace parsing
{

/**
 * Reads a byte stream in small pieces, for use by parsers
 *
 * Multi-byte values are read as big-endian
 */
class ByteConsumer
{
public:
	ByteConsumer() = delete;
	ByteConsumer(const ByteConsumer&) = delete;

	/**
	 * Constructor
	 *
	 * @param buf Stream buffer to read from
	 * @param length the length of the stream, if known
	 */
	ByteConsumer(std::istream& stream, uintmax_t length = -1);

	virtual ~ByteConsumer();

	/**
	 * Read a 1-byte unsigned integer value from the stream
	 *
	 * @return the next byte from the stream as an unsigned integer
	 * @throws parse_failure if the end of the stream is reached when attempting to read
	 */
	u1 readU1();

	/**
	 * Read a 2-byte unsigned integer value from the stream
	 *
	 * @return the next 2 bytes from the stream as an unsigned integer
	 * @throws parse_failure if the end of the stream is reached when attempting to read
	 */
	u2 readU2();

	/**
	 * Read a 4-byte unsigned integer value from the stream
	 *
	 * @return the next 4 bytes from the stream as an unsigned integer
	 * @throws parse_failure if the end of the stream is reached when attempting to read
	 */
	u4 readU4();

	/**
	 * Read a vector of bytes from the stream
	 *
	 * @param numberOfBytes the number of bytes to read
	 * @return a vector containing the specified number of bytes
	 * @throws parse_failure if the end of the stream is reached when attempting to read
	 */
	std::vector<u1> readBytes(int numberOfBytes);

	/**
	 * @return the number of bytes remaining to be read in the stream or -1 if unknown
	 */
	uintmax_t bytesRemaining() { return bytesRemainingCount; };

private:
	std::istream& stream;
	uintmax_t bytesRemainingCount;
};

}
}

#endif /* SRC_PARSING_BYTECONSUMER_H_ */
