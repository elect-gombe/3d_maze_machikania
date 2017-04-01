#ifndef __FONT_DATA
#define __FONT_DATA

#define _EXCLUDE_16X8 0
#define _EXCLUDE_12X6 0

#if _EXCLUDE_16X8==0
const uint8_t font16x8[]={
#include "font16x8.h"
};
#endif

#if _EXCLUDE_12X6==0
const uint8_t font12x6[]={
#include "font12x6.h"
};
#endif

#else
#warning this is font data. Do not include by user!
#endif










