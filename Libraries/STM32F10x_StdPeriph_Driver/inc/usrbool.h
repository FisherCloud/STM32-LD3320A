/* usrbool.h standard header */
/* Copyright 2011-2012 org.  */
#ifndef _USRBOOL
#define _USRBOOL

#include <stdint.h>

typedef __UINT32_T_TYPE__ bool;
#define FALSE ((__UINT32_T_TYPE__)0)
#define TRUE  (!FALSE)

#define __BV(__N)  ((__UINT32_T_TYPE__)1<<(__N))
#define __BM(__N)  (__BV(__N)-1)
#define __BME(__N1, __N2)  (__BM((__N2)+1)-__BM((__N1)))

#endif /* defined(_STD_USING) */

/*
 * Copyright (c) 2011-2012 by org.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V1.01:0001 */
