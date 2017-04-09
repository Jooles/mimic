/**
 * \file method_attributes.h
 *
 *  Created on: 9 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_METHOD_ATTRIBUTES_H_
#define SRC_MIMIC_METHOD_ATTRIBUTES_H_

#include "Common.h"
#include "common_attributes.h"

namespace mimic {
namespace attributes {
typedef struct
{
  u2 start_pc;
  u2 end_pc;
  u2 handler_pc;
  u2 catch_type;
} exception_info;

typedef struct
{
  u2 attribute_name_index;
  u2 max_stack;
  u2 max_locals;
  std::vector<u1> code;
  std::vector<exception_info> exception_table;
  std::vector<code_attr_type> attrs;
} code;

typedef struct
{
  u2 attribute_name_index;
  std::vector<u2> excption_index_table;
} exceptions;

typedef struct
{
  u2 attribute_name_index;
  std::vector<std::vector<annotation>> parameter_annotations;
} runtime_visible_parameter_annotations;

typedef struct
{
  u2 attribute_name_index;
  std::vector<std::vector<annotation>> parameter_annotations;
} runtime_invisible_parameter_annotations;

typedef struct
{
  u2 attribute_name_index;
  annotation_element_value default_value;
} annotation_default;

typedef struct
{
  u2 attribute_name_index;
  std::vector<std::tuple<u2, u2>> parameters;
} method_parameters;

typedef variant<code,
                exceptions,
                synthetic,
                signature,
                deprecated,
                runtime_visible_annotations,
                runtime_invisible_annotations,
                runtime_visible_parameter_annotations,
                runtime_invisible_parameter_annotations,
                runtime_visible_type_annotations,
                runtime_invisible_type_annotations,
                method_parameters> method_attr_type;
};
};
#endif
