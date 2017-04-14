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
 * Validation is kept out of the constructor so that it only has to be
 * done once, during class loading.
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

  static bool isValidDescriptor(const JUtf8String str)
  {
    try
    {
      FieldDescriptor{str};
      return true;
    }
    catch (const parsing::parse_failure&)
    {
      return false;
    }
  }
  bool isPrimitive() { return descriptor_type != type::jclass && array_dimensions == 0; };
  bool isArray() { return array_dimensions > 0; };
  u1 getArrayDimensions() { return array_dimensions; };
  type getType() { return descriptor_type; };
  JUtf8String getClassName() { return className; };

private:
  u1 MAX_ARRAY_DIMENSIONS = 255;
  type descriptor_type;
  u1 array_dimensions;
  JUtf8String className;

  FieldDescriptor() = delete;
};

}
#endif
