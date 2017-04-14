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
  ConstantPoolVisitor(const ConstantPool& cp) : cp(cp) {};
  void operator() (const ConstantPool::tag& invalid) const {};
  void operator() (const ConstantPool::Class_info& info) const
  {
    try
    {
      const JUtf8String str = cp.get<const JUtf8String>(info.name_index);
      ClassValidator::validateClassOrInterfaceName(str);
    }
    catch (const boost::bad_get& e)
    {
      throw std::runtime_error(e.what());
    }
  }
  void operator() (const ConstantPool::Double_info& info) const
  {
    std::cout << "double" << std::endl;
  }
  void operator() (const ConstantPool::Fieldref_info& info) const
  {
    std::cout << "fieldref" << std::endl;
  }
  void operator() (const ConstantPool::Float_info& info) const
  {
    std::cout << "float" << std::endl;
  }
  void operator() (const ConstantPool::Integer_info& info) const
  {
    std::cout << "integer" << std::endl;
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
    std::cout << "long" << std::endl;
  }
  void operator() (const ConstantPool::MethodHandle_info& info) const
  {
    std::cout << "methodhandle" << std::endl;
  }
  void operator() (const ConstantPool::MethodType_info& info) const
  {
    std::cout << "methodtype" << std::endl;
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
    std::cout << "string" << std::endl;
  }
  void operator() (const JUtf8String& info) const
  {
    std::cout << "utf8" << std::endl;
  }
private:
  const ConstantPool& cp;
};
#endif

void ClassValidator::validateClassOrInterfaceName(const JUtf8String& str)
{
  if (str.contains(JUtf8String(".")))
    throw std::runtime_error("Invalid character in class or identifier name");
  auto identifiers = str.split(JUtf8String("/"));
  for (auto i = identifiers.begin(); i < identifiers.end(); ++i)
    validateUnqualifiedName(*i);
}

void ClassValidator::validateUnqualifiedName(const JUtf8String& str)
{
  if (!(*str.begin() == '[' && FieldDescriptor::isValidDescriptor(str))
      && str.contains(std::vector<JUtf8String>{JUtf8String("."), JUtf8String(";"), JUtf8String("["), JUtf8String("/")}))
    throw std::runtime_error("Invalid character in unqualified name");
}

void ClassValidator::validateConstantPool(const ConstantPool& cp)
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
    boost::apply_visitor( ConstantPoolVisitor(cp), entry );
#endif
  }
}

}
