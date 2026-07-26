/* jconfig.h - generated for berialdraw (libjpeg-turbo 2.1.5.1) */
#define JPEG_LIB_VERSION  62
#define LIBJPEG_TURBO_VERSION  2.1.5.1
#define LIBJPEG_TURBO_VERSION_NUMBER  2001005
/* #undef C_ARITH_CODING_SUPPORTED */
/* #undef D_ARITH_CODING_SUPPORTED */
#define MEM_SRCDST_SUPPORTED 1
/* #undef WITH_SIMD */

#ifndef BITS_IN_JSAMPLE
#define BITS_IN_JSAMPLE  8
#endif

#ifdef _WIN32

#undef RIGHT_SHIFT_IS_UNSIGNED

/* Define "boolean" as unsigned char, not int, per Windows custom */
#ifndef __RPCNDR_H__
typedef unsigned char boolean;
#endif
#define HAVE_BOOLEAN

/* Define "INT32" as int, not long, per Windows custom */
#if !(defined(_BASETSD_H_) || defined(_BASETSD_H))
typedef short INT16;
typedef signed int INT32;
#endif
#define XMD_H

#else

/* #undef RIGHT_SHIFT_IS_UNSIGNED */

#endif
