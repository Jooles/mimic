/**
 * \file ConstantPool.h
 *
 *  Created on: 1 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_CONSTANTPOOL_H_
#define SRC_MIMIC_CONSTANTPOOL_H_

#include "Common.h"
#include "FieldDescriptor.h"
#include "MethodDescriptor.h"
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

  enum reference_kind : u1 {
    getField = 1,
    getStatic = 2,
    putField = 3,
    putStatic = 4,
    invokeVirtual = 5,
    invokeStatic = 6,
    invokeSpecial = 7,
    newInvokeSpecial = 8,
    invokeInterface = 9
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
    : value((static_cast<u8>(high_bytes) << 32) | low_bytes) {
    }
    u8 value;
  } Long_info;

  /** Representation of a Double entry in the constant pool. Takes up two spaces. */
  typedef struct Double_info
  {
    Double_info(u4 high_bytes, u4 low_bytes)
    : bytes((static_cast<u8>(high_bytes) << 32) | low_bytes) {
    }
    u8 bytes;
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

  /** Representation of a Method Handle entry in the constant pool. */
  typedef struct MethodHandle_info
  {
    MethodHandle_info(reference_kind kind, u2 reference_index)
    : kind(kind), reference_index(reference_index) {
    }
    reference_kind kind;
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

  typedef variant<const tag,
                  const Class_info,
                  const Double_info,
                  const Fieldref_info,
                  const Float_info,
                  const Integer_info,
                  const InterfaceMethodref_info,
                  const InvokeDynamic_info,
                  const Long_info,
                  const MethodHandle_info,
                  const MethodType_info,
                  const Methodref_info,
                  const NameAndType_info,
                  const String_info,
                  const JUtf8String,
                  const FieldDescriptor,
                  const MethodDescriptor> cp_type;

  enum cp_type_index
  {
    cp_tag = 0,
    cp_class = 1,
    cp_double = 2,
    cp_fieldref = 3,
    cp_float = 4,
    cp_integer = 5,
    cp_interfaceMethodref = 6,
    cp_invokeDynamic = 7,
    cp_long = 8,
    cp_methodHandle = 9,
    cp_methodType = 10,
    cp_methodref = 11,
    cp_nameAndType = 12,
    cp_string = 13,
    cp_utf8 = 14,
    cp_fieldDescriptor = 15,
    cp_methodDescriptor = 16
  };

  ConstantPool() {};
  ConstantPool(std::vector<cp_type> other_pool) : pool(std::move(other_pool)) {};

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
  template <typename T> T& get(const u2& index) const
  {
    if (index >= pool.size() || index < 1)
      throw std::range_error("Invalid constant pool index");
#ifdef HAVE_VARIANT
    return (std::get<T>(pool[index]));
#else
    return (boost::get<T>(pool[index]));
#endif
  }

  /**
   * @param index the index of the constant pool entry to get
   * @return the constant pool type at the specified index
   */
  cp_type_index getType(const u2& index) const
  {
    if (index >= pool.size() || index < 1)
      throw std::range_error("Invalid constant pool index");
#ifdef HAVE_VARIANT
    return static_cast<cp_type_index>(pool[index].index());
#else
    return static_cast<cp_type_index>(pool[index].which());
#endif
  }

private:
  friend class ClassValidator;
  std::vector<cp_type> pool;
};

}

#endif /* SRC_MIMIC_CONSTANTPOOL_H_ */
