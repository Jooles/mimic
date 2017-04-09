/**
 * \file field_attributes.h
 *
 *  Created on: 9 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_FIELD_ATTRIBUTES_H_
#define SRC_MIMIC_FIELD_ATTRIBUTES_H_

#include "Common.h"
#include "common_attributes.h"

namespace mimic {
namespace attributes {

typedef struct
{
  u2 attribute_name_index;
  u2 constant_value_index;
} constant_value;

typedef variant<constant_value,
                synthetic,
                signature,
                deprecated,
                runtime_visible_annotations,
                runtime_invisible_annotations,
                runtime_visible_type_annotations,
                runtime_invisible_type_annotations> field_attr_type;
};
};

#endif
