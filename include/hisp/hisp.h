#ifndef _HISP_
#define _HISP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* HFALSE and HTRUE are bool value used in C layer
 * While Qfalse and Qtrue are bool value used in Hisp and C layer 
 */
#define HFALSE  0
#define HTRUE   1

#define HISP_VERSION "0.0.1"
#define HISP_DEV     HTRUE

#if sizeof(unsigned long) == sizeof(void *)
    /* FIXME: HObject must be one machine-word long and platform free. */
    typedef unsigned long HObject;
#else
#   error "Cannot create basic HObject."
#endif

typedef HObject ID;

/* Common Header */
struct hp_basic {
    /* FIXME: flags should be 32bit at all platform */
	int flags;
};

typedef struct hp_basic HBasic;

#define HCAST(t, v) ((t)(v))
#define HBASIC(v)   HCAST(HBasic *, v) 
#define HOBJECT(v)  HCAST(HObject, v)

#define Qfalse HOBJECT(0)
#define Qtrue  HOBJECT(2)
#define Qnull  HOBJECT(4)
#define Qundef HOBJECT(6)
#define Qeof   HOBJECT(8)

#define TYPE_MASK 0x00FF
#define TYPE(v) ((HBASIC(v)->flags) & TYPE_MASK)

/* HISP type system */
enum hp_type {
	TFixnum = 0x01,
    TSymbol = 0x0E
};

typedef enum hp_type HType;

/* Codes are stolen from ruby.h */
#ifdef __STDC__
# include <limits.h>
#else
# ifndef LONG_MAX
#  ifdef HAVE_LIMITS_H
#   include <limits.h>
#  else
    /* assuming 32bit(2's complement) long */
#   define LONG_MAX 2147483647
#  endif
# endif
# ifndef LONG_MIN
#  define LONG_MIN (-LONG_MAX-1)
# endif
# ifndef CHAR_BIT
#  define CHAR_BIT 8
# endif
#endif

#ifdef HAVE_LONG_LONG
# ifndef LLONG_MAX
#  ifdef LONG_LONG_MAX
#   define LLONG_MAX  LONG_LONG_MAX
#  else
#   ifdef _I64_MAX
#    define LLONG_MAX _I64_MAX
#   else
    /* assuming 64bit(2's complement) long long */
#    define LLONG_MAX 9223372036854775807LL
#   endif
#  endif
# endif
# ifndef LLONG_MIN
#  ifdef LONG_LONG_MIN
#   define LLONG_MIN  LONG_LONG_MIN
#  else
#   ifdef _I64_MIN
#    define LLONG_MIN _I64_MIN
#   else
#    define LLONG_MIN (-LLONG_MAX-1)
#   endif
#  endif
# endif
#endif

#define RSHIFT(x, b) ((x)>>(b))

#define FIXNUM_MAX (LONG_MAX>>1)
#define FIXNUM_MIN RSHIFT((long)LONG_MIN, 1)

#define INT2FIX(i) ((HObject)(((long)(i))<<1|TFixnum))
#define LONG2FIX(i) INT2FIX(i)

#define ID2SYM(x) ((HObject)(((long)(x))<<8|TSymbol))
#define SYM2ID(x) RSHIFT((long)x, 8)

#define HCOMPOUND_P(v) ((HOBJECT(v) & TFixnum) != 1)
#define     HNULL_P(v) (HOBJECT(v) == Qnull)
#define     HTRUE_P(v) (HOBJECT(v) == Qtrue)
#define    HFALSE_P(v) (HOBJECT(v) == Qfalse)
#define    HUNDEF_P(v) (HOBJECT(v) == Qundef)
#define      HEOF_P(v) (HOBJECT(v) == Qeof)
#define     HBOOL_P(v) (HNIL_P(v) || HTRUE_P(v) || HFALSE_P(v))

/* bool test in C layer */
#define HTEST(v) (((HObject)(v) & ~Qnull) != 0)

/* memory free */
#define HFREE(m) free(m)

/* memory allocation */
#define HMALLOC(sz) malloc(sz)

#endif
