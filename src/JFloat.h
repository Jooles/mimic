/**
 * \file JFloat.h
 *
 *  Created on: 7 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_JFLOAT_H_
#define SRC_MIMIC_JFLOAT_H_

#include "Common.h"

class JFloat
{
public:
  JFloat(u4 bytes) : bytes(bytes);
private:
  JFloat() = delete;
  u4 bytes;
};

#endif
