/**
 * \file ClassFile.cpp
 *
 *  Created on: 26 Nov 2016
 *      Author: Julian Cromarty
 */

#include <ClassFile.h>
#include <iomanip>
#include <fstream>
#include <memory>
#include <sstream>
#include <parsing/ByteConsumer.h>

namespace mimic
{

ClassFile::ClassFile(parsing::ByteConsumer& bc)
{
	magic = bc.readU4();
	if (magic != 0xCAFEBABE)
	{
		std::stringstream ss;
		ss << "Invalid magic number: ";
		ss << std::hex << std::setw(2) << magic;
		throw parsing::parse_failure(ss.str().c_str());
	}
	minor_version = bc.readU2();
	major_version = bc.readU2();
	constant_pool_count = bc.readU2();
	std::vector<std::shared_ptr<constant_pool::Info>> parsed_pool;
	for (u2 i = 0; i < constant_pool_count - 1; i++)
	{
		u1 tag = bc.readU1();
		switch (tag)
		{
		case constant_pool::Class:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Class_info>(
									bc.readU2())));
			break;
		case constant_pool::Fieldref:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Fieldref_info>(
									bc.readU2(), bc.readU2())));
			break;
		case constant_pool::Methodref:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Methodref_info>(
									bc.readU2(), bc.readU2())));
			break;
		case constant_pool::InterfaceMethodref:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::InterfaceMethodref_info>(
									bc.readU2(), bc.readU2())));
			break;
		case constant_pool::String:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::String_info>(
									bc.readU2())));
			break;
		case constant_pool::Integer:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Integer_info>(
									bc.readU4())));
			break;
		case constant_pool::Float:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Float_info>(
									bc.readU4())));
			break;
		case constant_pool::Long:
		{
			std::shared_ptr<constant_pool::Info> info =
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Long_info>(
									bc.readU4(), bc.readU4()));
			parsed_pool.push_back(info);
			parsed_pool.push_back(info);
			break;
		}
		case constant_pool::Double:
		{
			std::shared_ptr<constant_pool::Info> info =
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Double_info>(
									bc.readU4(), bc.readU4()));
			parsed_pool.push_back(info);
			parsed_pool.push_back(info);
			break;
		}
		case constant_pool::NameAndType:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::NameAndType_info>(
									bc.readU2(), bc.readU2())));
			break;
		case constant_pool::Utf8:
		{
			u2 numberOfBytes = bc.readU2();
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::Utf8_info>(
									numberOfBytes, bc.readBytes(numberOfBytes))));
			break;
		}
		case constant_pool::MethodHandle:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::MethodHandle_info>(
									bc.readU1(), bc.readU2())));
			break;
		case constant_pool::MethodType:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::MethodType_info>(
									bc.readU2())));
			break;
		case constant_pool::InvokeDynamic:
			parsed_pool.push_back(
					std::static_pointer_cast<constant_pool::Info>(
							std::make_shared<constant_pool::InvokeDynamic_info>(
									bc.readU2(), bc.readU2())));
			break;
		default:
			std::stringstream ss;
			ss << "Unknown constant pool tag " << std::hex << std::setw(2) << tag;
			throw parsing::parse_failure(ss.str().c_str());
		}
	}
	constant_pool = ConstantPool(parsed_pool);
	access_flags = bc.readU2();
    this_class = bc.readU2();
    super_class = bc.readU2();
    interfaces_count = bc.readU2();
    for (u2 i = 0; i < interfaces_count; i++) {
    	interfaces.push_back(bc.readU2());
    }
    fields_count = bc.readU2();
    for (u2 i = 0; i < fields_count; i++) {
    	auto info = std::make_shared<field_info>();
    	info->access_flags = bc.readU2();
    	info->name_index = bc.readU2();
    	info->descriptor_index = bc.readU2();
    	info->attributes_count = bc.readU2();
    	parseAttributesSection(bc, info->attributes, info->attributes_count);
		fields.push_back(info);
	}
    methods_count = bc.readU2();
    for (u2 i = 0; i < methods_count; i++) {
		auto info = std::make_shared<method_info>();
		info->access_flags = bc.readU2();
		info->name_index = bc.readU2();
		info->descriptor_index = bc.readU2();
		info->attributes_count = bc.readU2();
		parseAttributesSection(bc, info->attributes, info->attributes_count);
		methods.push_back(info);
	}
    attributes_count = bc.readU2();
    parseAttributesSection(bc, attributes, attributes_count);
    if (bc.bytesRemaining() != 0) {
    	throw parsing::parse_failure("Trailing bytes at end of class file");
    }
}

void ClassFile::parseAttributesSection(parsing::ByteConsumer& bc,
									   std::vector<attribute_info_ptr>& attributes,
									   u2 attributes_count)
{
	for(u2 i = 0; i < attributes_count; i++) {
		auto attr_info = std::make_shared<attribute_info>();
		attr_info->attribute_name_index = bc.readU2();
		attr_info->attribute_length = bc.readU4();
		attr_info->info = bc.readBytes(attr_info->attribute_length);
		attributes.push_back(attr_info);
	}
}

ClassFile::~ClassFile()
{
}

}
