/**
 * \file ConstantPool.cpp
 *
 *  Created on: 2 Apr 2017
 *      Author: Julian Cromarty
 */

#include "ConstantPool.h"

namespace mimic
{
ConstantPool::ConstantPool(parsing::ByteConsumer& bc, u2 constant_pool_count)
{
  // Set pool[0] to an invalid entry as 0 is an invalid index
  pool.push_back(Invalid);
  /* Constant pool entries can refer to entries with a higher index than themselves,i
   * so we can't verify until we've parsed everything */
  for (u2 i = 1; i < constant_pool_count; i++)
  {
    u1 cp_tag = bc.readU1();
    switch (cp_tag)
    {
    case Class:
    {
      u2 name_index = bc.readU2();
      pool.push_back(Class_info{name_index});
      break;
    }
    case Fieldref:
      pool.push_back(Fieldref_info{bc.readU2(), bc.readU2()});
      break;
    case Methodref:
      pool.push_back(Methodref_info{bc.readU2(), bc.readU2()});
      break;
    case InterfaceMethodref:
      pool.push_back(InterfaceMethodref_info{bc.readU2(), bc.readU2()});
      break;
    case String:
      pool.push_back(String_info{bc.readU2()});
      break;
    case Integer:
      pool.push_back(Integer_info{bc.readU4()});
      break;
    case Float:
      pool.push_back(Float_info{bc.readU4()});
      break;
    case Long:
    {
      Long_info info(bc.readU4(), bc.readU4());
      pool.push_back(info);
      pool.push_back(Invalid);
      break;
    }
    case Double:
    {
      Double_info info(bc.readU4(), bc.readU4());
      pool.push_back(info);
      pool.push_back(Invalid);
      break;
    }
    case NameAndType:
      pool.push_back(NameAndType_info{bc.readU2(), bc.readU2()});
      break;
    case Utf8:
    {
      u2 numberOfBytes = bc.readU2();
      pool.push_back(JUtf8String(bc.readBytes(numberOfBytes)));
      break;
    }
    case MethodHandle:
      pool.push_back(MethodHandle_info{bc.readU1(), bc.readU2()});
      break;
    case MethodType:
      pool.push_back(MethodType_info{bc.readU2()});
      break;
    case InvokeDynamic:
      pool.push_back(InvokeDynamic_info{bc.readU2(), bc.readU2()});
      break;
    default:
      std::stringstream ss;
      ss << "Unknown constant pool tag " << std::hex << std::setw(2) << cp_tag;
      throw parsing::parse_failure(ss.str().c_str());
    }
  }
}

template <typename T> T& ConstantPool::get(const u2 index)
{
#ifdef HAVE_VARIANT
  return (std::get<T>(pool[index]));
#else
  return (boost::get<T>(pool[index]));
#endif
}

}
