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

/**
 * Models the constant pool
 */
class ConstantPool
{
public:
  enum tag : u1 {
    Invalid = 0,
    Utf8 = 1,
    Integer = 3,
    Float = 4,
    Long = 5,
    Double = 6,
    Class = 7,
    String = 8,
    Fieldref = 9,
    Methodref = 10,
    InterfaceMethodref = 11,
    NameAndType = 12,
    MethodHandle = 15,
    MethodType = 16,
    InvokeDynamic = 18
  };

  /** Representation of a Class entry in the constant pool */
  typedef struct Class_info
  {
    Class_info(u2 name_index)
    : name_index(name_index) {
    }
    u2 name_index;
  } Class_info;

  /** Representation of a Field reference entry in the constant pool */
  typedef struct Fieldref_info
  {
    Fieldref_info(u2 class_index, u2 name_and_type_index)
    : name_and_type_index(name_and_type_index) {
    }
    u2 class_index;
    u2 name_and_type_index;
  } Fieldref_info;

  /** Representation of a Method reference entry in the constant pool */
  typedef struct Methodref_info
  {
    Methodref_info(u2 class_index, u2 name_and_type_index)
    : name_and_type_index(name_and_type_index) {
    }
    u2 class_index;
    u2 name_and_type_index;
  } Methodref_info;

  /** Representation of an Interface reference entry in the constant pool */
  typedef struct InterfaceMethodref_info
  {
    InterfaceMethodref_info(u2 class_index, u2 name_and_type_index)
    : name_and_type_index(name_and_type_index) {
    }
    u2 class_index;
    u2 name_and_type_index;
  } InterfaceMethodref_info;

  /** Representation of a String entry in the constant pool */
  typedef struct String_info
  {
    String_info(u2 string_index)
    : string_index(string_index) {
    }
    u1 tag;
    u2 string_index;
  } String_info;

  /** Representation of an Integer entry in the constant pool */
  typedef struct Integer_info
  {
    Integer_info(u4 bytes)
    : bytes(bytes) {
    }
    u4 bytes;
  } Integer_info;

  /** Representation of a Float entry in the constant pool. */
  typedef struct Float_info
  {
    Float_info(u4 bytes)
    : bytes(bytes) {
    }
    u4 bytes;
  } Float_info;

  /** Representation of a Long entry in the constant pool. Takes up two spaces. */
  typedef struct Long_info
  {
    Long_info(u4 high_bytes, u4 low_bytes)
    : low_bytes(low_bytes) {
    }
    u4 high_bytes;
    u4 low_bytes;
  } Long_info;

  /** Representation of a Double entry in the constant pool. Takes up two spaces. */
  typedef struct Double_info
  {
    Double_info(u4 high_bytes, u4 low_bytes)
    : low_bytes(low_bytes) {
    }
    u4 high_bytes;
    u4 low_bytes;
  } Double_info;

  /** Representation of a NameAndType entry in the constant pool. */
  typedef struct NameAndType_info
  {
    NameAndType_info(u2 name_index, u2 descriptor_index)
    : descriptor_index(descriptor_index) {
    }
    u2 name_index;
    u2 descriptor_index;
  } NameAndType_info;

  /** Representation of a Utf8 entry in the constant pool. */
  typedef struct Utf8_info
  {
    Utf8_info(std::vector<u1> bytes)
    : str(bytes) {
    }
    JUtf8String str;
  } Utf8_info;

  /** Representation of a Method Handle entry in the constant pool. */
  typedef struct MethodHandle_info
  {
    MethodHandle_info(u1 reference_kind, u2 reference_index)
    : reference_index(reference_index) {
    }
    u1 reference_kind;
    u2 reference_index;
  } MethodHandle_info;

  /** Representation of a Method Type entry in the constant pool. */
  typedef struct MethodType_info
  {
    MethodType_info(u2 descriptor_index)
    : descriptor_index(descriptor_index) {
    }
    u2 descriptor_index;
  } MethodType_info;

  /** Representation of an InvokeDynamic entry in the constant pool. */
  typedef struct InvokeDynamic_info
  {
    InvokeDynamic_info(u2 bootstrap_method_attr_index, u2 name_and_type_index)
    : name_and_type_index(name_and_type_index) {
    }
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
  } InvokeDynamic_info;

	ConstantPool() {};
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
  typedef variant<tag,
                  Class_info,
                  Double_info,
                  Fieldref_info,
                  Float_info,
                  Integer_info,
                  InterfaceMethodref_info,
                  InvokeDynamic_info,
                  Long_info,
                  MethodHandle_info,
                  MethodType_info,
                  Methodref_info,
                  NameAndType_info,
                  String_info,
                  Utf8_info> cp_type;
	std::vector<cp_type> pool;
};

}

#endif /* SRC_MIMIC_CONSTANTPOOL_H_ */
