/**
 * \file ClassFile.cpp
 *
 *  Created on: 26 Nov 2016
 *      Author: Julian Cromarty
 */

#include "ClassFile.h"
#include "parsing/ByteConsumer.h"

namespace mimic
{

ClassFile::ClassFile(parsing::ByteConsumer& bc)
{
	magic = bc.readU4();
	std::cout << "magic: " << std::setw(8) << std::hex << magic << std::dec << std::endl;
	if (magic != 0xCAFEBABE)
	{
		std::stringstream ss;
		ss << "Invalid magic number: ";
		ss << std::hex << std::setw(2) << magic;
		throw parsing::parse_failure(ss.str().c_str());
	}
	minor_version = bc.readU2();
	major_version = bc.readU2();
	std::cout << "version: " << major_version << "." << minor_version << std::endl;
	constant_pool_count = bc.readU2();
	std::cout << "constant pool count: " << constant_pool_count << std::endl;
	constant_pool = ConstantPool(bc, constant_pool_count);
	access_flags = bc.readU2();
	std::cout << "access flags: " << access_flags << std::endl;
	this_class = bc.readU2();
	std::cout << "this: " << this_class << std::endl;
	super_class = bc.readU2();
	std::cout << "super: " << super_class << std::endl;
	interfaces_count = bc.readU2();
	std::cout << "interfaces count: " << interfaces_count << std::endl;
	for (u2 i = 0; i < interfaces_count; i++) {
		interfaces.push_back(bc.readU2());
	}
	fields_count = bc.readU2();
	std::cout << "fields count: " << fields_count << std::endl;
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
	std::cout << "methods count: " << methods_count << std::endl;
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
	std::cout << "attributes count: " << attributes_count << std::endl;
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
