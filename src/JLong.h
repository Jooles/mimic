/**
 * \file JLong.h
 *
 *  Created on: 7 Apr 2017
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_JLONG_H_
#define SRC_MIMIC_JLONG_H_

#include "Common.h"

class JLong
{
public:
  JLong(u4 high_bytes, u4 low_bytes)
    : bytes((high_bytes << 32) | low_bytes);
private:
  JLong() = delete;
  u8 value;
};

#endif
