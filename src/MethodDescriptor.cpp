/**
 * \file MethodDescriptor.cpp
 *
 *  Created on: 19 Apr 2017
 *      Author: Julian Cromarty
 */

#include "MethodDescriptor.h"

namespace mimic {

MethodDescriptor::MethodDescriptor(const JUtf8String& str)
{
  if (*(str.begin()) != '(')
    throw std::runtime_error("Missing parameter list");
  auto param_list_end = str.find(JUtf8String(")"));
  if (param_list_end + 1 >= str.end())
    throw std::runtime_error("Missing return type");
  JUtf8String param_list(str.begin() + 1, param_list_end);
  auto descriptor_start = param_list.begin();
  if (param_list.length() > 0)
  {
    for (auto i = param_list.begin(); i != param_list.end(); ++i)
    {
      if (*i == FieldDescriptor::type::jarray)
      {
        continue;
      }
      else if (*i == FieldDescriptor::type::jclass)
      {
        auto descriptor_end = param_list.find(JUtf8String(";"));
        if (descriptor_end == param_list.end())
          throw std::runtime_error("No semicolon after class name");
        descriptor_end++;
        parameter_type_descriptors.push_back(FieldDescriptor(JUtf8String(descriptor_start, descriptor_end)));
        descriptor_start = descriptor_end;
        i = descriptor_start; 
      }
      else
      {
        parameter_type_descriptors.push_back(FieldDescriptor(JUtf8String(descriptor_start, i + 1)));
        descriptor_start = i + 1;
      }
    }
    if (descriptor_start < param_list.end())
      throw std::runtime_error("Array parameter missing type");
  }
  JUtf8String return_type(param_list_end + 1, str.end());
  if (*return_type.begin() != 'V')
    return_type_descriptor = FieldDescriptor(return_type);
}

}
