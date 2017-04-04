/**
 * \file Common.h
 *
 *  Created on: 26 Nov 2016
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_COMMON_H_
#define SRC_MIMIC_COMMON_H_

#include <cstdint>
#ifdef HAVE_CXX_FILESYSTEM
#include <filesystem>
#elif defined(HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
#include <experimental/filesystem>
#endif
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace mimic
{
#ifdef HAVE_CXX_FILESYSTEM
namespace fs = std::filesystem;
#elif defined(HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
namespace fs = std::experimental::filesystem;
#endif

/*
 * Common definitions used in many places
 */
typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

}

#endif /* SRC_MIMIC_COMMON_H_ */
