/**
 * \file FieldDescriptor.cpp
 *
 *  Created on: 13 Apr 2017
 *      Author: Julian Cromarty
 */

#include "FieldDescriptor.h"

namespace mimic {

FieldDescriptor::FieldDescriptor(const JUtf8String& str)
  :descriptor_type(type::jarray), array_dimensions(0)
{
  for (auto i = str.begin(); i != str.end(); ++i)
  {
    switch (*i)
    {
      case type::jbyte:
        class_name = JUtf8String("byte");
        descriptor_type = static_cast<type>(*i);
        break;
      case type::jchar:
        class_name = JUtf8String("char");
        descriptor_type = static_cast<type>(*i);
        break;
      case type::jdouble:
        class_name = JUtf8String("double");
        descriptor_type = static_cast<type>(*i);
        break;
      case type::jfloat:
        class_name = JUtf8String("float");
        descriptor_type = static_cast<type>(*i);
        break;
      case type::jint:
        class_name = JUtf8String("int");
        descriptor_type = static_cast<type>(*i);
        break;
      case type::jlong:
        class_name = JUtf8String("long");
        descriptor_type = static_cast<type>(*i);
        break;
      case type::jshort:
        class_name = JUtf8String("short");
        descriptor_type = static_cast<type>(*i);
        break;
      case type::jboolean:
        class_name = JUtf8String("boolean");
        descriptor_type = static_cast<type>(*i);
        break;
      case type::jclass:
      {
        descriptor_type = type::jclass;
        auto nameEnd = str.find(JUtf8String(";"));
        if (nameEnd == str.end())
          throw parsing::parse_failure("No semicolon after class or interface name");
        if (nameEnd == (i + 1))
          throw parsing::parse_failure("Class name missing");
        class_name = JUtf8String(++i, nameEnd);
        i = ++nameEnd;
        break;
      }
      case type::jarray:
        if (descriptor_type == type::jarray && array_dimensions < MAX_ARRAY_DIMENSIONS)
        {
          array_dimensions++;
          break;
        }
      [[fallthrough]]
      default:
        throw parsing::parse_failure("Invalid descriptor type");
    }
  }
  std::stringstream ss;
  for (int i = 0; i < array_dimensions; i++)
  {
    ss << "[]";
  }
  ss >> class_name;
}

}
