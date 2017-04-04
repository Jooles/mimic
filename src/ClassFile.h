/**
 * \file ClassFile.h
 *
 *  Created on: 26 Nov 2016
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_CLASSFILE_H_
#define SRC_MIMIC_CLASSFILE_H_

#include "Common.h"
#include "ConstantPool.h"
#include "parsing/ByteConsumer.h"
#include "parsing/ParseFailureException.h"

namespace mimic
{

typedef struct
{
	u2 attribute_name_index;
	u4 attribute_length;
	std::vector<u1> info;
} attribute_info;
typedef std::shared_ptr<attribute_info> attribute_info_ptr;

typedef struct
{
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	std::vector<attribute_info_ptr> attributes;
} field_info, method_info;
typedef std::shared_ptr<field_info> field_info_ptr;
typedef std::shared_ptr<method_info> method_info_ptr;

/**
 * Representation of a .class file
 */
class ClassFile
{
private:
	u4 magic;
	u2 minor_version;
	u2 major_version;
	u2 constant_pool_count;
	ConstantPool constant_pool;
	u2 access_flags;
	u2 this_class;
	u2 super_class;
	u2 interfaces_count;
	std::vector<u2> interfaces;
	u2 fields_count;
	std::vector<field_info_ptr> fields;
	u2 methods_count;
	std::vector<method_info_ptr> methods;
	u2 attributes_count;
	std::vector<attribute_info_ptr> attributes;

	void parseAttributesSection(parsing::ByteConsumer&, std::vector<attribute_info_ptr>&, u2);

public:
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
	auto getAccessFlags() { return access_flags; };
	auto getThisClass() { return this_class; };
	auto getSuperClass() { return super_class; };
	auto getInterfaceCount() { return interfaces_count; };
	auto getInterfaces() { return interfaces; };
	auto getFieldCount() { return fields_count; };
	auto getFields() { return fields; };
	auto getMethodsCount() { return methods_count; };
	auto getMethods() { return methods; };
	auto getAttributesCount() { return attributes_count; };
	auto getAttributes() { return attributes; };
};

}

#endif /* SRC_MIMIC_CLASSFILE_H_ */
