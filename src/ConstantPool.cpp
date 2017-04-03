/**
 * \file ConstantPool.cpp
 *
 *  Created on: 2 Apr 2017
 *      Author: Julian Cromarty
 */

#include <sstream>
#include "ConstantPool.h"

namespace mimic
{
void ConstantPool::assertTag(const u1 tag, const std::shared_ptr<constant_pool::Info> info)
{
	if (tag != info->tag)
	{
		std::stringstream ss;
		ss << "Incorrect tag. Expected " << tag << ", was " << info->tag;
		throw std::runtime_error(ss.str().c_str());
	}
}
template <typename T> auto ConstantPool::get(const u2 index)
{
	auto info = pool[index];
	assertTag(constant_pool::Class, info);
	return std::static_pointer_cast<T>(info);
}

}
