/**
 * \file MethodDescriptor.h
 *
 *  Created on: 19 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_METHODDESCRIPTOR_H_
#define SRC_MIMIC_METHODDESCRIPTOR_H_

#include "Common.h"
#include "FieldDescriptor.h"
#include "JUtf8String.h"
#include "parsing/ParseFailureException.h"

namespace mimic {

/**
 * Method descriptor
 *
 * https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.3.3
 */
class MethodDescriptor {
public:
  MethodDescriptor(const JUtf8String& str);

  auto getReturnType() { return return_type_descriptor; };
  auto getParameters() { return parameter_type_descriptors; };

private:
  optional<FieldDescriptor> return_type_descriptor;
  std::vector<FieldDescriptor> parameter_type_descriptors;

  MethodDescriptor() = delete;
};

}
#endif
