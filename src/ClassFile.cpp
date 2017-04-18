/**
 * \file ClassFile.cpp
 *
 *  Created on: 26 Nov 2016
 *      Author: Julian Cromarty
 */

#include "ClassFile.h"
#include "ClassValidator.h"
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
  ClassValidator::validateConstantPool(constant_pool, major_version, minor_version);
  flags = static_cast<access_flags>(bc.readU2());
  std::cout << "access flags: " << flags << std::endl;
  this_class = bc.readU2();
  std::cout << "this: " << this_class << std::endl;
  super_class = bc.readU2();
  std::cout << "super: " << super_class << std::endl;
  u2 interfaces_count = bc.readU2();
  std::cout << "interfaces count: " << interfaces_count << std::endl;
  for (u2 i = 0; i < interfaces_count; i++) {
    interfaces.push_back(bc.readU2());
  }
  u2 fields_count = bc.readU2();
  std::cout << "fields count: " << fields_count << std::endl;
  parseFieldInfoSection(bc, fields_count);
  u2 methods_count = bc.readU2();
  std::cout << "methods count: " << methods_count << std::endl;
  parseMethodInfoSection(bc, methods_count);
  u2 attributes_count = bc.readU2();
  std::cout << "attributes count: " << attributes_count << std::endl;
  parseClassAttributesSection(bc, attrs, attributes_count);
  if (bc.bytesRemaining() != 0) {
    throw parsing::parse_failure("Trailing bytes at end of class file");
  }
}

void ClassFile::parseFieldInfoSection(parsing::ByteConsumer& bc,
                                      u2 count)
{
  for (u2 i = 0; i < count; i++) {
    field_info info;
    info.flags = static_cast<access_flags>(bc.readU2());
    info.name_index = bc.readU2();
    info.descriptor_index = bc.readU2();
    u2 attributes_count = bc.readU2();
    parseFieldAttributesSection(bc, info.attrs, attributes_count);
    fields.push_back(info);
  }
}

void ClassFile::parseMethodInfoSection(parsing::ByteConsumer& bc,
                                       u2 count)
{
  for (u2 i = 0; i < count; i++) {
    method_info info;
    info.flags = static_cast<access_flags>(bc.readU2());
    info.name_index = bc.readU2();
    info.descriptor_index = bc.readU2();
    u2 attributes_count = bc.readU2();
    parseMethodAttributesSection(bc, info.attrs, attributes_count);
    methods.push_back(info);
  }
}

void ClassFile::parseClassAttributesSection(parsing::ByteConsumer& bc,
                                            std::vector<attributes::class_attr_type>& attributes,
                                            u2 attributes_count)
{
  for(u2 i = 0; i < attributes_count; i++) {
    u2 attribute_name_index = bc.readU2();
    u4 attribute_length = bc.readU4();
    std::vector<u1> info = bc.readBytes(attribute_length);
    //attributes.push_back(info);
  }
}

void ClassFile::parseCodeAttributesSection(parsing::ByteConsumer& bc,
                                           std::vector<attributes::code_attr_type>& attributes,
                                           u2 attributes_count)
{
  for(u2 i = 0; i < attributes_count; i++) {
    u2 attribute_name_index = bc.readU2();
    u4 attribute_length = bc.readU4();
    std::vector<u1> info = bc.readBytes(attribute_length);
    //attributes.push_back(info);
  }
}

void ClassFile::parseFieldAttributesSection(parsing::ByteConsumer& bc,
                                            std::vector<attributes::field_attr_type>& attributes,
                                            u2 attributes_count)
{
  for(u2 i = 0; i < attributes_count; i++) {
    u2 attribute_name_index = bc.readU2();
    u4 attribute_length = bc.readU4();
    std::vector<u1> info = bc.readBytes(attribute_length);
    //attributes.push_back(info);
  }
}

void ClassFile::parseMethodAttributesSection(parsing::ByteConsumer& bc,
                                             std::vector<attributes::method_attr_type>& attributes,
                                             u2 attributes_count)
{
  for(u2 i = 0; i < attributes_count; i++) {
    u2 attribute_name_index = bc.readU2();
    u4 attribute_length = bc.readU4();
    std::vector<u1> info = bc.readBytes(attribute_length);
    //attributes.push_back(info);
  }
}

ClassFile::~ClassFile()
{
}

}
