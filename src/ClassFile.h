/**
 * \file ClassFile.h
 *
 *  Created on: 26 Nov 2016
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_CLASSFILE_H_
#define SRC_MIMIC_CLASSFILE_H_

#include "Common.h"
#include "class_attributes.h"
#include "code_attributes.h"
#include "field_attributes.h"
#include "method_attributes.h"
#include "ConstantPool.h"
#include "parsing/ByteConsumer.h"
#include "parsing/ParseFailureException.h"

namespace mimic
{

/**
 * Representation of a .class file
 */
class ClassFile
{
public:
  enum access_flags : u2
  {
    acc_public = 0x0001,
    acc_private = 0x0002,
    acc_protected = 0x0004,
    acc_static = 0x0008,
    acc_final = 0x0010,
    acc_synchronized = 0x0020,
    acc_bridge = 0x0040,
    acc_volatile = 0x0040,
    acc_transient = 0x0080,
    acc_varargs = 0x0080,
    acc_native = 0x0100,
    acc_abstract = 0x0400,
    acc_strict = 0x0800,
    acc_synthetic = 0x1000,
    acc_enum = 0x4000
  };

  typedef struct
  {
    access_flags flags;
    u2 name_index;
    u2 descriptor_index;
    std::vector<attributes::field_attr_type> attrs;
  } field_info;

  typedef struct
  {
    access_flags flags;
    u2 name_index;
    u2 descriptor_index;
    std::vector<attributes::method_attr_type> attrs;
  } method_info;

  ClassFile() = delete;
  ClassFile(const ClassFile&);
  ClassFile(const ClassFile&&);
  /**
   * Parses the Class file
   *
   * @param ByteConsumer containing the class data
   * @return the parsed ClassFile
   */
  ClassFile(parsing::ByteConsumer&);
  virtual ~ClassFile();

  auto getMajorVersion() { return major_version; };
  auto getMinorVersion() { return minor_version; };
  auto getConstantPoolCount() { return constant_pool_count; };
  auto getConstantPool() { return constant_pool; };
  auto getAccessFlags() { return flags; };
  auto getThisClass() { return this_class; };
  auto getSuperClass() { return super_class; };
  auto getInterfaceCount() { return interfaces.size(); };
  auto getInterfaces() { return interfaces; };
  auto getFieldCount() { return fields.size(); };
  auto getFields() { return fields; };
  auto getMethodsCount() { return methods.size(); };
  auto getMethods() { return methods; };
  auto getAttributesCount() { return attrs.size(); };
  auto getAttributes() { return attrs; };

private:
  u4 magic;
  u2 minor_version;
  u2 major_version;
  u2 constant_pool_count;
  ConstantPool constant_pool;
  access_flags flags;
  u2 this_class;
  u2 super_class;
  std::vector<u2> interfaces;
  std::vector<field_info> fields;
  std::vector<method_info> methods;
  std::vector<attributes::class_attr_type> attrs;

  void parseFieldInfoSection(parsing::ByteConsumer&, u2);
  void parseMethodInfoSection(parsing::ByteConsumer&, u2);
  void parseClassAttributesSection(parsing::ByteConsumer&, std::vector<attributes::class_attr_type>&, u2);
  void parseCodeAttributesSection(parsing::ByteConsumer&, std::vector<attributes::code_attr_type>&, u2);
  void parseFieldAttributesSection(parsing::ByteConsumer&, std::vector<attributes::field_attr_type>&, u2);
  void parseMethodAttributesSection(parsing::ByteConsumer&, std::vector<attributes::method_attr_type>&, u2);
};

}

#endif /* SRC_MIMIC_CLASSFILE_H_ */
