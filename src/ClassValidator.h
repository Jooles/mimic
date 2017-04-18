/**
 * \file ClassValidator.h
 *
 *  Created on: 9 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_CLASS_VALIDATOR_H_
#define SRC_MIMIC_CLASS_VALIDATOR_H_

#include "Common.h"
#include "ConstantPool.h"
#include "JUtf8String.h"

namespace mimic {
class ClassValidator
{
public:
  /**
   * Checks that the given binary class or interface name does not
   * contain any '.' characters, and that the identifiers are valid
   *
   * https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.2.1
   * @param str The string representation of the class or interface name
   */
  static void validateClassOrInterfaceName(const JUtf8String& str);

  /**
   * Checks that the given unqualified name does not contain any of the
   * invalid characters specified in the Java specification: '.', ';',
   * '[', or '/'
   *
   * https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.2.2
   * @param str The string representation of the unqualified identifier
   */
  static void validateUnqualifiedName(const JUtf8String& str);

  /**
   * Validates the contents of the provided constant pool
   *
   * @param cp the ConstantPool object to validate
   * @param major_version The class' major version number
   * @param minor_version The class' minor version number
   * @throws runtime_error if validation failed
   */
  static void validateConstantPool(const ConstantPool& cp, u2 major_version, u2 minor_version);
};
};

#endif
