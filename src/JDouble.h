/**
 * \file JDouble.h
 *
 *  Created on: 7 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_JDOUBLE_H_
#define SRC_MIMIC_JDOUBLE_H_

#include "Common.h"

class JDouble
{
public:
  JDouble(u4 high_bytes, u4 low_bytes)
    : bytes((high_bytes << 32) | low_bytes);
private:
  JDouble() = delete;
  u8 bytes;
};

#endif
