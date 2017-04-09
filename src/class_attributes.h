/**
 * \file class_attributes.h
 *
 *  Created on: 9 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_CLASS_ATTRIBUTES_H_
#define SRC_MIMIC_CLASS_ATTRIBUTES_H_

#include "Common.h"
#include "common_attributes.h"
#include "JUtf8String.h"
#include "parsing/ByteConsumer.h"

namespace mimic {
namespace attributes {
typedef struct
{
  u2 inner_class_info_index;
  u2 outer_class_info_index;
  u2 inner_name_index;
  u2 inner_class_access_flags;
} inner_class_info;

typedef struct
{
  u2 attribute_name_index;
  std::vector<inner_class_info> classes;
} inner_classes;

typedef struct
{
  u2 attribute_name_index;
  u2 class_index;
  u2 method_index;
} enclosing_method;

typedef struct
{
  u2 attribute_name_index;
  u2 sourcefile_index;
} source_file;

typedef struct
{
  u2 attribute_name_index;
  JUtf8String debug_extension;
} source_debug_extension;

typedef struct
{
  u2 bootstrap_method_ref;
  std::vector<u2> methods;
} bootstrap_method_info;

typedef struct
{
  u2 attribute_name_index;
  std::vector<bootstrap_method_info> info;
} bootstrap_methods;

typedef variant<inner_classes,
                enclosing_method,
                synthetic,
                signature,
                source_file,
                source_debug_extension,
                deprecated,
                runtime_visible_annotations,
                runtime_invisible_annotations,
                runtime_visible_type_annotations,
                runtime_invisible_type_annotations,
                bootstrap_methods> class_attr_type;
};
};

#endif
