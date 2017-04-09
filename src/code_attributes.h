/**
 * \file code_attributes.h
 *
 *  Created on: 9 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_CODE_ATTRIBUTES_H_
#define SRC_MIMIC_CODE_ATTRIBUTES_H_

#include "Common.h"
#include "common_attributes.h"

namespace mimic {
namespace attributes {

typedef struct
{
  u2 attribute_name_index;
  std::vector<u1> info;
} stack_map_table;

typedef struct
{
  u2 start_pc;
  u2 line_number;
} line_number_info;

typedef struct
{
  u2 attribute_name_index;
  std::vector<line_number_info> table;
} line_number_table;

typedef struct
{
  u2 start_pc;
  u2 length;
  u2 name_index;
  u2 descriptor_index;
  u2 index;
} local_variable_info;

typedef struct
{
  u2 attribute_name_index;
  std::vector<local_variable_info> table;
} local_variable_table;

typedef struct
{
  u2 start_pc;
  u2 length;
  u2 name_index;
  u2 signature_index;
  u2 index;
} local_variable_type_info;

typedef struct
{
  u2 attribute_name_index;
  std::vector<local_variable_type_info> table;
} local_variable_type_table;

typedef variant<stack_map_table,
                line_number_table,
                local_variable_table,
                local_variable_type_table,
                runtime_visible_type_annotations,
                runtime_invisible_type_annotations> code_attr_type;
};
};

#endif
