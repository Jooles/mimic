/**
 * \file ConstantPool.cpp
 *
 *  Created on: 2 Apr 2017
 *      Author: Julian Cromarty
 */

#include "ConstantPool.h"

namespace mimic
{
constant_pool::Info ConstantPool::INVALID(-1);

ConstantPool::ConstantPool(parsing::ByteConsumer& bc, u2 constant_pool_count)
{
  // Set pool[0] to an invalid entry as 0 is an invalid index
  pool.push_back(INVALID);
  /* Constant pool entries can refer to entries with a higher index than themselves,i
   * so we can't verify until we've parsed everything */
  for (u2 i = 1; i < constant_pool_count; i++)
  {
    u1 tag = bc.readU1();
    switch (tag)
    {
    case constant_pool::Class:
    {
      u2 name_index = bc.readU2();
      pool.push_back(constant_pool::Class_info{name_index});
      break;
    }
    case constant_pool::Fieldref:
      pool.push_back(constant_pool::Fieldref_info{bc.readU2(), bc.readU2()});
      break;
    case constant_pool::Methodref:
      pool.push_back(constant_pool::Methodref_info{bc.readU2(), bc.readU2()});
      break;
    case constant_pool::InterfaceMethodref:
      pool.push_back(constant_pool::InterfaceMethodref_info{bc.readU2(), bc.readU2()});
      break;
    case constant_pool::String:
      pool.push_back(constant_pool::String_info{bc.readU2()});
      break;
    case constant_pool::Integer:
      pool.push_back(constant_pool::Integer_info{bc.readU4()});
      break;
    case constant_pool::Float:
      pool.push_back(constant_pool::Float_info{bc.readU4()});
      break;
    case constant_pool::Long:
    {
      constant_pool::Long_info info(bc.readU4(), bc.readU4());
      pool.push_back(info);
      pool.push_back(INVALID);
      break;
    }
    case constant_pool::Double:
    {
      constant_pool::Double_info info(bc.readU4(), bc.readU4());
      pool.push_back(info);
      pool.push_back(INVALID);
      break;
    }
    case constant_pool::NameAndType:
      pool.push_back(constant_pool::NameAndType_info{bc.readU2(), bc.readU2()});
      break;
    case constant_pool::Utf8:
    {
      u2 numberOfBytes = bc.readU2();
      pool.push_back(constant_pool::Utf8_info{numberOfBytes, bc.readBytes(numberOfBytes)});
      break;
    }
    case constant_pool::MethodHandle:
      pool.push_back(constant_pool::MethodHandle_info{bc.readU1(), bc.readU2()});
      break;
    case constant_pool::MethodType:
      pool.push_back(constant_pool::MethodType_info{bc.readU2()});
      break;
    case constant_pool::InvokeDynamic:
      pool.push_back(constant_pool::InvokeDynamic_info{bc.readU2(), bc.readU2()});
      break;
    default:
      std::stringstream ss;
      ss << "Unknown constant pool tag " << std::hex << std::setw(2) << tag;
      throw parsing::parse_failure(ss.str().c_str());
    }
  }
}

void ConstantPool::assertTag(const u1 tag, const std::shared_ptr<constant_pool::Info> info)
{
  if (tag != info->tag)
  {
    std::stringstream ss;
    ss << "Incorrect tag. Expected " << tag << ", was " << info->tag;
    throw std::runtime_error(ss.str().c_str());
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
