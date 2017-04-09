/**
 * \file common_attributes.h
 *
 *  Created on: 9 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_COMMON_ATTRIBUTES_H_
#define SRC_MIMIC_COMMON_ATTRIBUTES_H_

#include "Common.h"

namespace mimic {
namespace attributes {

typedef struct
{
  u2 attribute_name_index;
} synthetic;

typedef struct
{
  u2 attribute_name_index;
  u2 signature_index;
} signature;

typedef struct
{
  u2 attribute_name_index;
} deprecated;

typedef struct annotation_element_value
{
  u1 type_tag;
  variant<u2,
          std::tuple<u2, u2>,
          std::tuple<u2, std::vector<struct annotation_element_value>>> value;
} annotation_element_value;

typedef struct
{
  u2 type_index;
  std::vector<annotation_element_value> elements;
} annotation;

typedef struct
{
  u2 attribute_name_index;
  std::vector<annotation> annotations;
} runtime_visible_annotations;

typedef struct
{
  u2 attribute_name_index;
  std::vector<annotation> annotations;
} runtime_invisible_annotations;

typedef struct
{
  std::vector<std::tuple<u1, u1>> path;
} type_path;

/*typedef struct
{
  u1 type;
  variant<> target_info;
  type_path target_path;
  u2 type_index;
  std::vector<element_value_pairs> elements;
} type_annotation;*/

typedef struct
{
  u2 attribute_name_index;
  u4 attribute_length;
  std::vector<u1> info;
} runtime_visible_type_annotations;

typedef struct
{
  u2 attribute_name_index;
  std::vector<u1> info;
} runtime_invisible_type_annotations;

};
};

#endif
