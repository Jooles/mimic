/**
 * \file ConstantPool.cpp
 *
 *  Created on: 2 Apr 2017
 *      Author: Julian Cromarty
 */

#include "ConstantPool.h"
#include <iostream>

namespace mimic
{

ConstantPool::ConstantPool(parsing::ByteConsumer& bc, u2 constant_pool_count)
{
	/* Constant pool entries can refer to entries with a higher index than themselves,i
	 * so we can't verify until we've parsed everything */
	for (u2 i = 1; i < constant_pool_count; i++)
	{
		u1 tag = bc.readU1();
		switch (tag)
		{
		case constant_pool::Class:
		{
			u2 name_index = bc.readU2();
			std::cout << "ConstantPool[" << i << "]: Class (name index: " << name_index << ")" << std::endl;
			pool.push_back(std::static_pointer_cast<constant_pool::Info>(std::make_shared<constant_pool::Class_info>(name_index)));
			break;
		}
		case constant_pool::Fieldref:
			std::cout << "ConstantPool[" << i << "]: FieldRef" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Fieldref_info>(
									bc.readU2(), bc.readU2())));
			break;
		case constant_pool::Methodref:
			std::cout << "ConstantPool[" << i << "]: MethodRef" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Methodref_info>(
									bc.readU2(), bc.readU2())));
			break;
		case constant_pool::InterfaceMethodref:
			std::cout << "ConstantPool[" << i << "]: InterfaceMethodRef" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::InterfaceMethodref_info>(
									bc.readU2(), bc.readU2())));
			break;
		case constant_pool::String:
			std::cout << "ConstantPool[" << i << "]: String" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::String_info>(
									bc.readU2())));
			break;
		case constant_pool::Integer:
			std::cout << "ConstantPool[" << i << "]: Integer" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Integer_info>(
									bc.readU4())));
			break;
		case constant_pool::Float:
			std::cout << "ConstantPool[" << i << "]: Float" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Float_info>(
									bc.readU4())));
			break;
		case constant_pool::Long:
		{
			std::cout << "ConstantPool[" << i << "]: Long" << std::endl;
			std::shared_ptr<constant_pool::Info> info =
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Long_info>(
									bc.readU4(), bc.readU4()));
			pool.push_back(info);
			pool.push_back(info);
			break;
		}
		case constant_pool::Double:
		{
			std::cout << "ConstantPool[" << i << "]: Double" << std::endl;
			std::shared_ptr<constant_pool::Info> info =
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Double_info>(
									bc.readU4(), bc.readU4()));
			pool.push_back(info);
			pool.push_back(info);
			break;
		}
		case constant_pool::NameAndType:
			std::cout << "ConstantPool[" << i << "]: NameAndType" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::NameAndType_info>(
									bc.readU2(), bc.readU2())));
			break;
		case constant_pool::Utf8:
		{
			std::cout << "ConstantPool[" << i << "]: Utf8" << std::endl;
			u2 numberOfBytes = bc.readU2();
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Utf8_info>(
									numberOfBytes, bc.readBytes(numberOfBytes))));
			break;
		}
		case constant_pool::MethodHandle:
			std::cout << "ConstantPool[" << i << "]: MethodHandle" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::MethodHandle_info>(
									bc.readU1(), bc.readU2())));
			break;
		case constant_pool::MethodType:
			std::cout << "ConstantPool[" << i << "]: MethodType" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::MethodType_info>(
									bc.readU2())));
			break;
		case constant_pool::InvokeDynamic:
			std::cout << "ConstantPool[" << i << "]: InvokeDynamic" << std::endl;
			pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::InvokeDynamic_info>(
									bc.readU2(), bc.readU2())));
			break;
		default:
			std::cout << "ConstantPool[" << i << "]: Unknown" << std::endl;
			std::stringstream ss;
			ss << "Unknown constant pool tag " << std::hex << std::setw(2) << tag;
			throw parsing::parse_failure(ss.str().c_str());
		}
	}
}

void ConstantPool::assertTag(const u1 tag, const std::shared_ptr<constant_pool::Info> info)
{
	if (tag != info->tag)
	{
		std::stringstream ss;
		ss << "Incorrect tag. Expected " << tag << ", was " << info->tag;
		throw std::runtime_error(ss.str().c_str());
	}
}
template <typename T> std::shared_ptr<T> ConstantPool::get(const u2 index)
{
	return std::dynamic_pointer_cast<T>(pool[index]);
}

}
