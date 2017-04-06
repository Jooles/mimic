/**
 * \file ConstantPool.h
 *
 *  Created on: 1 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_CONSTANTPOOL_H_
#define SRC_MIMIC_CONSTANTPOOL_H_

#include "Common.h"
#include "JUtf8String.h"
#include "parsing/ByteConsumer.h"

namespace mimic
{

namespace constant_pool
{

/** Tag value for a Class entry in the constant pool */
const u1 Class = 7;
/** Tag value for a Field reference entry in the constant pool */
const u1 Fieldref = 9;
/** Tag value for a Method reference entry in the constant pool */
const u1 Methodref = 10;
/** Tag value for a Interface method reference entry in the constant pool */
const u1 InterfaceMethodref = 11;
/** Tag value for a String entry in the constant pool */
const u1 String = 8;
/** Tag value for an Integer entry in the constant pool */
const u1 Integer = 3;
/** Tag value for a Float entry in the constant pool */
const u1 Float = 4;
/** Tag value for a Long entry in the constant pool */
const u1 Long = 5;
/** Tag value for a Double entry in the constant pool */
const u1 Double = 6;
/** Tag value for a NameAndType entry in the constant pool */
const u1 NameAndType = 12;
/** Tag value for a Utf8 entry in the constant pool */
const u1 Utf8 = 1;
/** Tag value for a Method Handle entry in the constant pool */
const u1 MethodHandle = 15;
/** Tag value for a Method Type entry in the constant pool */
const u1 MethodType = 16;
/** Tag value for an InvokeDynamic entry in the constant pool */
const u1 InvokeDynamic = 18;

/** Base representation of a constant pool entry */
typedef struct Info
{
	Info(const u1& tag) : tag(tag) {}
	virtual ~Info() {};
	u1 tag;
} Info;

/** Representation of a Class entry in the constant pool */
typedef struct Class_info : Info
{
	Class_info(u2 name_index)
	: Info(Class), name_index(name_index) {
	}
	u2 name_index;
} Class_info;

/** Representation of a Field reference entry in the constant pool */
typedef struct Fieldref_info : Info
{
	Fieldref_info(u2 class_index, u2 name_and_type_index)
	: Info(Fieldref), class_index(class_index), name_and_type_index(name_and_type_index) {
	}
	u2 class_index;
	u2 name_and_type_index;
} Fieldref_info;

/** Representation of a Method reference entry in the constant pool */
typedef struct Methodref_info : Info
{
	Methodref_info(u2 class_index, u2 name_and_type_index)
	: Info(Methodref), class_index(class_index), name_and_type_index(name_and_type_index) {
	}
	u2 class_index;
	u2 name_and_type_index;
} Methodref_info;

/** Representation of an Interface reference entry in the constant pool */
typedef struct InterfaceMethodref_info : Info
{
	InterfaceMethodref_info(u2 class_index, u2 name_and_type_index)
	: Info(InterfaceMethodref), class_index(class_index), name_and_type_index(name_and_type_index) {
	}
	u2 class_index;
	u2 name_and_type_index;
} InterfaceMethodref_info;

/** Representation of a String entry in the constant pool */
typedef struct String_info : Info
{
	String_info(u2 string_index)
	: Info(String), string_index(string_index) {
	}
	u1 tag;
	u2 string_index;
} String_info;

/** Representation of an Integer entry in the constant pool */
typedef struct Integer_info : Info
{
	Integer_info(u4 bytes)
	: Info(Integer), bytes(bytes) {
	}
	u4 bytes;
} Integer_info;

/** Representation of a Float entry in the constant pool. */
typedef struct Float_info : Info
{
	Float_info(u4 bytes)
	: Info(Float), bytes(bytes) {
	}
	u4 bytes;
} Float_info;

/** Representation of a Long entry in the constant pool. Takes up two spaces. */
typedef struct Long_info : Info
{
	Long_info(u4 high_bytes, u4 low_bytes)
	: Info(Long), high_bytes(high_bytes), low_bytes(low_bytes) {
	}
	u4 high_bytes;
	u4 low_bytes;
} Long_info;

/** Representation of a Double entry in the constant pool. Takes up two spaces. */
typedef struct Double_info : Info
{
	Double_info(u4 high_bytes, u4 low_bytes)
	: Info(Double), high_bytes(high_bytes), low_bytes(low_bytes) {
	}
	u4 high_bytes;
	u4 low_bytes;
} Double_info;

/** Representation of a NameAndType entry in the constant pool. */
typedef struct NameAndType_info : Info
{
	NameAndType_info(u2 name_index, u2 descriptor_index)
	: Info(NameAndType), name_index(name_index), descriptor_index(descriptor_index) {
	}
	u2 name_index;
	u2 descriptor_index;
} NameAndType_info;

/** Representation of a Utf8 entry in the constant pool. */
typedef struct Utf8_info : Info
{
	Utf8_info(u2 length, std::vector<u1> bytes)
	: Info(Utf8), str(bytes, length) {
	}
	JUtf8String str;
} Utf8_info;

/** Representation of a Method Handle entry in the constant pool. */
typedef struct MethodHandle_info : Info
{
	MethodHandle_info(u1 reference_kind, u2 reference_index)
	: Info(MethodHandle), reference_kind(reference_kind), reference_index(reference_index) {
	}
	u1 reference_kind;
	u2 reference_index;
} MethodHandle_info;

/** Representation of a Method Type entry in the constant pool. */
typedef struct MethodType_info : Info
{
	MethodType_info(u2 descriptor_index)
	: Info(MethodType), descriptor_index(descriptor_index) {
	}
	u2 descriptor_index;
} MethodType_info;

/** Representation of an InvokeDynamic entry in the constant pool. */
typedef struct InvokeDynamic_info : Info
{
	InvokeDynamic_info(u2 bootstrap_method_attr_index, u2 name_and_type_index)
	: Info(InvokeDynamic), bootstrap_method_attr_index(bootstrap_method_attr_index), name_and_type_index(name_and_type_index) {
	}
	u2 bootstrap_method_attr_index;
	u2 name_and_type_index;
} InvokeDynamic_info;

}

/**
 * Models the constant pool
 */
class ConstantPool
{
public:
	ConstantPool() {
		// Set pool[0] to an invalid entry as 0 is an invalid index
		constant_pool::Info invalid(-1);
		pool.push_back(invalid);
	};
	/**
	 * Parses the constant pool entries from a ByteConsumer
	 *
	 * @param the ByteConsumer to use
	 * @param the number of constant pool entries
	 */
	ConstantPool(parsing::ByteConsumer&, u2);
	/**
	 * @param index the index of the constant pool entry to get
	 * @return the constant pool entry at the specified index
	 */
	template <typename T> T& get(const u2 index);
private:
  typedef variant<constant_pool::Info,
                  constant_pool::Class_info,
                  constant_pool::Double_info,
                  constant_pool::Fieldref_info,
                  constant_pool::Float_info,
                  constant_pool::Integer_info,
                  constant_pool::InterfaceMethodref_info,
                  constant_pool::InvokeDynamic_info,
                  constant_pool::Long_info,
                  constant_pool::MethodHandle_info,
                  constant_pool::MethodType_info,
                  constant_pool::Methodref_info,
                  constant_pool::NameAndType_info,
                  constant_pool::String_info,
                  constant_pool::Utf8_info> cp_type;
	std::vector<cp_type> pool;
	
	void assertTag(const u1 tag, const std::shared_ptr<constant_pool::Info> info);
};

}

#endif /* SRC_MIMIC_CONSTANTPOOL_H_ */
