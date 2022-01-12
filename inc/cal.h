#pragma once
#include "HpsCommon.h"

#define def_UseCLI//!<C++/CLI使用時

#ifdef def_UseCLI
using namespace System;
#else
//#define _USE_MATH_DEFINE
#include <Math.h>
#endif

//inlineを使ってもよい？

#ifdef def_UseCLI
#define fCal_PI()                              Math::PI
#define fCal_Sin(input)                        Math::Sin(input)
#define fCal_Cos(input)                        Math::Cos(input)
#define fCal_Tan(input)                        Math::Tan(input)
#define fCal_Sqrt(input)                       Math::Sqrt(input)
#define fCal_Atan(input)                       Math::Atan(input)
#define fCal_Acos(input)                       Math::Acos(input)
#define fCal_Atan2(input_1, input_2)           Math::Atan2(input_1, input_2)
#define fCal_Abs(input)                        Math::Abs(input)
#define fCal_Pow(input_1, input_2)             Math::Pow(input_1, input_2)
#else
#define fCal_PI()                              3.14159265358979323846
//#define fCal_PI()                            M_PI
#define fCal_Sin(input)                        sin(input)
#define fCal_Cos(input)                        cos(input)
#define fCal_Tan(input)                        tan(input)
#define fCal_Sqrt(input)                       sqrt(input)
#define fCal_Atan(input)                       atan(input)
#define fCal_Acos(input)                       acos(input)
#define fCal_Atan2(input_1, input_2)           atan2(input_1, input_2)
#define fCal_Abs(input)                        abs(input)
#define fCal_Pow(input_1, input_2)             pow(input_1, input_2)
#endif
