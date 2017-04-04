/**
 * \file ParseFailureException.h
 *
 *  Created on: 25 Nov 2016
 *      Author: Julian Cromarty
 */

#ifndef SRC_PARSING_PARSEFAILUREEXCEPTION_H_
#define SRC_PARSING_PARSEFAILUREEXCEPTION_H_

#include <exception>
namespace mimic
{
namespace parsing
{

/**
 * Exception thrown when something cannot be parsed
 */
class parse_failure: public std::runtime_error
{
public:
	parse_failure() = delete;

	/**
	 * Constructor
	 * @param what A message detailing the reason for the failure
	 */
	parse_failure(const std::string& what) :
			std::runtime_error(what)
	{
	}

	/**
	 * Constructor
	 * @param what A message detailing the reason for the failure
	 */
	parse_failure(const char* what) :
			std::runtime_error(what)
	{
	}

	virtual ~parse_failure()
	{
	}
};

}
}

#endif /* SRC_PARSING_PARSEFAILUREEXCEPTION_H_ */
