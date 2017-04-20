/**
 * \file FieldDescriptor.h
 *
 *  Created on: 11 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_FIELDDESCRIPTOR_H_
#define SRC_MIMIC_FIELDDESCRIPTOR_H_

#include "Common.h"
#include "JUtf8String.h"
#include "parsing/ParseFailureException.h"

namespace mimic {

/**
 * Field descriptor
 *
 * https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.3.2
 */
class FieldDescriptor {
public:
  enum type {
    jbyte = 'B',
    jchar = 'C',
    jdouble = 'D',
    jfloat = 'F',
    jint = 'I',
    jlong = 'J',
    jshort = 'S',
    jboolean = 'Z',
    jclass = 'L',
    jarray = '['
  };

  FieldDescriptor(const JUtf8String& str);

  bool isPrimitive() { return descriptor_type != type::jclass && array_dimensions == 0; };
  bool isArray() { return array_dimensions > 0; };
  u1 getArrayDimensions() { return array_dimensions; };
  type getType() { return descriptor_type; };
  JUtf8String getClassName() { return class_name; };
  bool operator ==(const FieldDescriptor& other) const
  {
    if (descriptor_type != other.descriptor_type)
      return false;
    if (array_dimensions != other.array_dimensions)
      return false;
    if (class_name != other.class_name)
      return false;
    return true;
  }

private:
  u1 MAX_ARRAY_DIMENSIONS = 255;
  type descriptor_type;
  u1 array_dimensions;
  JUtf8String class_name;

  FieldDescriptor() = delete;
};

}
#endif
