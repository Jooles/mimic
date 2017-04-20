/**
 * \file ClassValidator.cpp
 *
 *  Created on: 9 Apr 2017
 *      Author: Julian Cromarty
 */

#include "ClassValidator.h"
#include "FieldDescriptor.h"

namespace mimic {

#ifndef HAVE_VARIANT
class ConstantPoolVisitor : public boost::static_visitor<void>
{
public:
  ConstantPoolVisitor(ConstantPool& cp, const u2 major_version, const u2 minor_version)
   : cp(cp), major_version(major_version), minor_version(minor_version) {};
  void operator() (const ConstantPool::tag& invalid) const {};
  void operator() (const ConstantPool::Class_info& info) const
  {
    auto nameType = cp.getType(info.name_index);
    if (nameType == ConstantPool::cp_type_index::cp_utf8)
      ClassValidator::validateClassOrInterfaceName(cp.get<const JUtf8String>(info.name_index));
    else if (nameType != ConstantPool::cp_type_index::cp_fieldDescriptor)
      throw std::runtime_error("Invalid  name reference");
  }
  void operator() (const ConstantPool::Double_info& info) const
  {
    // Nothing to do
  }
  void operator() (const ConstantPool::Fieldref_info& info) const
  {
    std::cout << "fieldref" << std::endl;
  }
  void operator() (const ConstantPool::Float_info& info) const
  {
    // Nothing to do
  }
  void operator() (const ConstantPool::Integer_info& info) const
  {
    // Nothing to do
  }
  void operator() (const ConstantPool::InterfaceMethodref_info& info) const
  {
    std::cout << "interfacemethodref" << std::endl;
  }
  void operator() (const ConstantPool::InvokeDynamic_info& info) const
  {
    std::cout << "invokedynamic" << std::endl;
  }
  void operator() (const ConstantPool::Long_info& info) const
  {
    // Nothing to do
  }
  void operator() (const ConstantPool::MethodHandle_info& info) const
  {
    if (info.kind > 9 || info.kind < 1)
      throw std::range_error("Reference kind out of valid range");
    switch(info.kind)
    {
      case ConstantPool::reference_kind::getField:
      [[fallthrough]]
      case ConstantPool::reference_kind::getStatic:
      [[fallthrough]]
      case ConstantPool::reference_kind::putField:
      [[fallthrough]]
      case ConstantPool::reference_kind::putStatic:
      {
        if(cp.getType(info.reference_index) != ConstantPool::cp_type_index::cp_fieldref)
        {
          throw std::runtime_error("Invalid method handle reference");
        }
        break;
      }
      case ConstantPool::reference_kind::invokeVirtual:
      [[fallthrough]]
      case ConstantPool::reference_kind::newInvokeSpecial:
      {
        if(cp.getType(info.reference_index) != ConstantPool::cp_type_index::cp_methodref)
        {
          throw std::runtime_error("Invalid method handle reference");
        }
        break;
      }
      case ConstantPool::reference_kind::invokeStatic:
      [[fallthrough]]
      case ConstantPool::reference_kind::invokeSpecial:
      {
        switch (cp.getType(info.reference_index))
        {
        case ConstantPool::cp_type_index::cp_interfaceMethodref:
          if (major_version < 52)
            throw std::runtime_error("Invalid method handle reference");
          break;
        case ConstantPool::cp_type_index::cp_methodref:
          break;
        default:
          throw std::runtime_error("Invalid method handle reference");
        }
        break;
      }
      case ConstantPool::reference_kind::invokeInterface:
      {
        if(cp.getType(info.reference_index) != ConstantPool::cp_type_index::cp_interfaceMethodref)
        {
          throw std::runtime_error("Invalid method handle reference");
        }
      }
    }
  }
  void operator() (const ConstantPool::MethodType_info& info) const
  {
    if (cp.getType(info.descriptor_index) != ConstantPool::cp_type_index::cp_methodDescriptor)
      throw std::runtime_error("Invalid  name reference");
  }
  void operator() (const ConstantPool::Methodref_info& info) const
  {
    std::cout << "methodref" << std::endl;
  }
  void operator() (const ConstantPool::NameAndType_info& info) const
  {
    std::cout << "nameandtype" << std::endl;
  }
  void operator() (const ConstantPool::String_info& info) const
  {
    if(cp.getType(info.string_index) != ConstantPool::cp_type_index::cp_utf8)
    {
      throw std::runtime_error("Invalid string reference");
    }
  }
  void operator() (const JUtf8String& info) const
  {
    // Nothing to do. Strings are validated on construction
  }
  template <typename T> void operator() (const T&) const {}
private:
  ConstantPool& cp;
  const u2 major_version;
  const u2 minor_version;
};
#endif

void ClassValidator::validateClassOrInterfaceName(const JUtf8String str)
{
  if (str.contains(JUtf8String(".")))
    throw std::runtime_error("Invalid character in class or identifier name");
  auto identifiers = str.split(JUtf8String("/"));
  for (auto i = identifiers.begin(); i < identifiers.end(); ++i)
    validateUnqualifiedName(*i);
}

void ClassValidator::validateUnqualifiedName(const JUtf8String str)
{
  if (str.contains(std::vector<JUtf8String>{JUtf8String("."), JUtf8String(";"), JUtf8String("["), JUtf8String("/")}))
    throw std::runtime_error("Invalid character in unqualified name");
}

void ClassValidator::validateConstantPool(ConstantPool& cp, u2 major_version, u2 minor_version)
{
  for (auto entry : cp.pool)
  {
#ifdef HAVE_VARIANT
// TODO once checks have been written using Boost visitor
    //~ std::visit([](auto&& arg) {
            //~ using T = std::decay_t<decltype(arg)>;
            //~ if constexpr (std::is_same_v<T, ConstantPool::Class_info>)
                //~ std::cout << "Class " << arg << '\n';
            //~ else 
                //~ static_assert(always_false<T>::value, "non-exhaustive visitor!");
        //~ }, entry);
#else
    boost::apply_visitor( ConstantPoolVisitor(cp, major_version, minor_version), entry );
#endif
  }
}

}
