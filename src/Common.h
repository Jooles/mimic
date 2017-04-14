/**
 * \file Common.h
 *
 *  Created on: 26 Nov 2016
 *      Author: Julian Cromarty
 */

#ifndef SRC_MIMIC_COMMON_H_
#define SRC_MIMIC_COMMON_H_

#include <array>
#include <cstdint>
#ifdef HAVE_CXX_FILESYSTEM
#include <filesystem>
#elif defined(HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
#include <experimental/filesystem>
#endif
#ifdef HAVE_CXX_VARIANT
#include <variant>
#else
#include <boost/variant.hpp>
#endif
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <exception>
#include <vector>

namespace mimic
{
#ifdef HAVE_CXX_FILESYSTEM
namespace fs = std::filesystem;
#elif defined(HAVE_CXX_EXPERIMENTAL_FILESYSTEM)
namespace fs = std::experimental::filesystem;
#endif
#ifdef HAVE_CXX_VARIANT
using std::variant;
#else
using boost::variant;
#endif

/*
 * Common definitions used in many places
 */
typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;

}

#endif /* SRC_MIMIC_COMMON_H_ */
