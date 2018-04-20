/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define _DES_

#include "include/constants.h"
#include "include/kernel_vendor.h"

#ifdef  VLIW1
#define VECT_SIZE1
#endif

#ifdef  VLIW2
#define VECT_SIZE1
#endif

#define DGST_R0 0
#define DGST_R1 1
#define DGST_R2 2
#define DGST_R3 3

#include "include/kernel_functions.c"
#include "types_nv.c"
#include "common_nv.c"

#ifdef  VECT_SIZE1
#define VECT_COMPARE_S "check_single_vect1_comp4.c"
#define VECT_COMPARE_M "check_multi_vect1_comp4.c"
#endif

#ifdef  VECT_SIZE2
#define VECT_COMPARE_S "check_single_vect2_comp4.c"
#define VECT_COMPARE_M "check_multi_vect2_comp4.c"
#endif

#ifdef  VECT_SIZE4
#define VECT_COMPARE_S "check_single_vect4_comp4.c"
#define VECT_COMPARE_M "check_multi_vect4_comp4.c"
#endif

__device__ __constant__ comb_t c_combs[1024];

#define PERM_OP(a,b,tt,n,m) \
{                           \
  tt = a >> n;              \
  tt = tt ^ b;              \
  tt = tt & m;              \
  b = b ^ tt;               \
  tt = tt << n;             \
  a = a ^ tt;               \
}

#define HPERM_OP(a,tt,n,m)  \
{                           \
  tt = a << (16 + n);       \
  tt = tt ^ a;              \
  tt = tt & m;              \
  a  = a ^ tt;              \
  tt = tt >> (16 + n);      \
  a  = a ^ tt;              \
}

__device__ __constant__ u32 c_SPtrans[8][64] =
{
  /* nibble 0 */
  0x00820200, 0x00020000, 0x80800000, 0x80820200,
  0x00800000, 0x80020200, 0x80020000, 0x80800000,
  0x80020200, 0x00820200, 0x00820000, 0x80000200,
  0x80800200, 0x00800000, 0x00000000, 0x80020000,
  0x00020000, 0x80000000, 0x00800200, 0x00020200,
  0x80820200, 0x00820000, 0x80000200, 0x00800200,
  0x80000000, 0x00000200, 0x00020200, 0x80820000,
  0x00000200, 0x80800200, 0x80820000, 0x00000000,
  0x00000000, 0x80820200, 0x00800200, 0x80020000,
  0x00820200, 0x00020000, 0x80000200, 0x00800200,
  0x80820000, 0x00000200, 0x00020200, 0x80800000,
  0x80020200, 0x80000000, 0x80800000, 0x00820000,
  0x80820200, 0x00020200, 0x00820000, 0x80800200,
  0x00800000, 0x80000200, 0x80020000, 0x00000000,
  0x00020000, 0x00800000, 0x80800200, 0x00820200,
  0x80000000, 0x80820000, 0x00000200, 0x80020200,
  /* nibble 1 */
  0x10042004, 0x00000000, 0x00042000, 0x10040000,
  0x10000004, 0x00002004, 0x10002000, 0x00042000,
  0x00002000, 0x10040004, 0x00000004, 0x10002000,
  0x00040004, 0x10042000, 0x10040000, 0x00000004,
  0x00040000, 0x10002004, 0x10040004, 0x00002000,
  0x00042004, 0x10000000, 0x00000000, 0x00040004,
  0x10002004, 0x00042004, 0x10042000, 0x10000004,
  0x10000000, 0x00040000, 0x00002004, 0x10042004,
  0x00040004, 0x10042000, 0x10002000, 0x00042004,
  0x10042004, 0x00040004, 0x10000004, 0x00000000,
  0x10000000, 0x00002004, 0x00040000, 0x10040004,
  0x00002000, 0x10000000, 0x00042004, 0x10002004,
  0x10042000, 0x00002000, 0x00000000, 0x10000004,
  0x00000004, 0x10042004, 0x00042000, 0x10040000,
  0x10040004, 0x00040000, 0x00002004, 0x10002000,
  0x10002004, 0x00000004, 0x10040000, 0x00042000,
  /* nibble 2 */
  0x41000000, 0x01010040, 0x00000040, 0x41000040,
  0x40010000, 0x01000000, 0x41000040, 0x00010040,
  0x01000040, 0x00010000, 0x01010000, 0x40000000,
  0x41010040, 0x40000040, 0x40000000, 0x41010000,
  0x00000000, 0x40010000, 0x01010040, 0x00000040,
  0x40000040, 0x41010040, 0x00010000, 0x41000000,
  0x41010000, 0x01000040, 0x40010040, 0x01010000,
  0x00010040, 0x00000000, 0x01000000, 0x40010040,
  0x01010040, 0x00000040, 0x40000000, 0x00010000,
  0x40000040, 0x40010000, 0x01010000, 0x41000040,
  0x00000000, 0x01010040, 0x00010040, 0x41010000,
  0x40010000, 0x01000000, 0x41010040, 0x40000000,
  0x40010040, 0x41000000, 0x01000000, 0x41010040,
  0x00010000, 0x01000040, 0x41000040, 0x00010040,
  0x01000040, 0x00000000, 0x41010000, 0x40000040,
  0x41000000, 0x40010040, 0x00000040, 0x01010000,
  /* nibble 3 */
  0x00100402, 0x04000400, 0x00000002, 0x04100402,
  0x00000000, 0x04100000, 0x04000402, 0x00100002,
  0x04100400, 0x04000002, 0x04000000, 0x00000402,
  0x04000002, 0x00100402, 0x00100000, 0x04000000,
  0x04100002, 0x00100400, 0x00000400, 0x00000002,
  0x00100400, 0x04000402, 0x04100000, 0x00000400,
  0x00000402, 0x00000000, 0x00100002, 0x04100400,
  0x04000400, 0x04100002, 0x04100402, 0x00100000,
  0x04100002, 0x00000402, 0x00100000, 0x04000002,
  0x00100400, 0x04000400, 0x00000002, 0x04100000,
  0x04000402, 0x00000000, 0x00000400, 0x00100002,
  0x00000000, 0x04100002, 0x04100400, 0x00000400,
  0x04000000, 0x04100402, 0x00100402, 0x00100000,
  0x04100402, 0x00000002, 0x04000400, 0x00100402,
  0x00100002, 0x00100400, 0x04100000, 0x04000402,
  0x00000402, 0x04000000, 0x04000002, 0x04100400,
  /* nibble 4 */
  0x02000000, 0x00004000, 0x00000100, 0x02004108,
  0x02004008, 0x02000100, 0x00004108, 0x02004000,
  0x00004000, 0x00000008, 0x02000008, 0x00004100,
  0x02000108, 0x02004008, 0x02004100, 0x00000000,
  0x00004100, 0x02000000, 0x00004008, 0x00000108,
  0x02000100, 0x00004108, 0x00000000, 0x02000008,
  0x00000008, 0x02000108, 0x02004108, 0x00004008,
  0x02004000, 0x00000100, 0x00000108, 0x02004100,
  0x02004100, 0x02000108, 0x00004008, 0x02004000,
  0x00004000, 0x00000008, 0x02000008, 0x02000100,
  0x02000000, 0x00004100, 0x02004108, 0x00000000,
  0x00004108, 0x02000000, 0x00000100, 0x00004008,
  0x02000108, 0x00000100, 0x00000000, 0x02004108,
  0x02004008, 0x02004100, 0x00000108, 0x00004000,
  0x00004100, 0x02004008, 0x02000100, 0x00000108,
  0x00000008, 0x00004108, 0x02004000, 0x02000008,
  /* nibble 5 */
  0x20000010, 0x00080010, 0x00000000, 0x20080800,
  0x00080010, 0x00000800, 0x20000810, 0x00080000,
  0x00000810, 0x20080810, 0x00080800, 0x20000000,
  0x20000800, 0x20000010, 0x20080000, 0x00080810,
  0x00080000, 0x20000810, 0x20080010, 0x00000000,
  0x00000800, 0x00000010, 0x20080800, 0x20080010,
  0x20080810, 0x20080000, 0x20000000, 0x00000810,
  0x00000010, 0x00080800, 0x00080810, 0x20000800,
  0x00000810, 0x20000000, 0x20000800, 0x00080810,
  0x20080800, 0x00080010, 0x00000000, 0x20000800,
  0x20000000, 0x00000800, 0x20080010, 0x00080000,
  0x00080010, 0x20080810, 0x00080800, 0x00000010,
  0x20080810, 0x00080800, 0x00080000, 0x20000810,
  0x20000010, 0x20080000, 0x00080810, 0x00000000,
  0x00000800, 0x20000010, 0x20000810, 0x20080800,
  0x20080000, 0x00000810, 0x00000010, 0x20080010,
  /* nibble 6 */
  0x00001000, 0x00000080, 0x00400080, 0x00400001,
  0x00401081, 0x00001001, 0x00001080, 0x00000000,
  0x00400000, 0x00400081, 0x00000081, 0x00401000,
  0x00000001, 0x00401080, 0x00401000, 0x00000081,
  0x00400081, 0x00001000, 0x00001001, 0x00401081,
  0x00000000, 0x00400080, 0x00400001, 0x00001080,
  0x00401001, 0x00001081, 0x00401080, 0x00000001,
  0x00001081, 0x00401001, 0x00000080, 0x00400000,
  0x00001081, 0x00401000, 0x00401001, 0x00000081,
  0x00001000, 0x00000080, 0x00400000, 0x00401001,
  0x00400081, 0x00001081, 0x00001080, 0x00000000,
  0x00000080, 0x00400001, 0x00000001, 0x00400080,
  0x00000000, 0x00400081, 0x00400080, 0x00001080,
  0x00000081, 0x00001000, 0x00401081, 0x00400000,
  0x00401080, 0x00000001, 0x00001001, 0x00401081,
  0x00400001, 0x00401080, 0x00401000, 0x00001001,
  /* nibble 7 */
  0x08200020, 0x08208000, 0x00008020, 0x00000000,
  0x08008000, 0x00200020, 0x08200000, 0x08208020,
  0x00000020, 0x08000000, 0x00208000, 0x00008020,
  0x00208020, 0x08008020, 0x08000020, 0x08200000,
  0x00008000, 0x00208020, 0x00200020, 0x08008000,
  0x08208020, 0x08000020, 0x00000000, 0x00208000,
  0x08000000, 0x00200000, 0x08008020, 0x08200020,
  0x00200000, 0x00008000, 0x08208000, 0x00000020,
  0x00200000, 0x00008000, 0x08000020, 0x08208020,
  0x00008020, 0x08000000, 0x00000000, 0x00208000,
  0x08200020, 0x08008020, 0x08008000, 0x00200020,
  0x08208000, 0x00000020, 0x00200020, 0x08008000,
  0x08208020, 0x00200000, 0x08200000, 0x08000020,
  0x00208000, 0x00008020, 0x08008020, 0x08200000,
  0x00000020, 0x08208000, 0x00208020, 0x00000000,
  0x08000000, 0x08200020, 0x00008000, 0x00208020
};

__device__ __constant__ u32 c_skb[8][64] =
{
  /* for C bits (numbered as per FIPS 46) 1 2 3 4 5 6 */
  0x00000000, 0x00000010, 0x20000000, 0x20000010,
  0x00010000, 0x00010010, 0x20010000, 0x20010010,
  0x00000800, 0x00000810, 0x20000800, 0x20000810,
  0x00010800, 0x00010810, 0x20010800, 0x20010810,
  0x00000020, 0x00000030, 0x20000020, 0x20000030,
  0x00010020, 0x00010030, 0x20010020, 0x20010030,
  0x00000820, 0x00000830, 0x20000820, 0x20000830,
  0x00010820, 0x00010830, 0x20010820, 0x20010830,
  0x00080000, 0x00080010, 0x20080000, 0x20080010,
  0x00090000, 0x00090010, 0x20090000, 0x20090010,
  0x00080800, 0x00080810, 0x20080800, 0x20080810,
  0x00090800, 0x00090810, 0x20090800, 0x20090810,
  0x00080020, 0x00080030, 0x20080020, 0x20080030,
  0x00090020, 0x00090030, 0x20090020, 0x20090030,
  0x00080820, 0x00080830, 0x20080820, 0x20080830,
  0x00090820, 0x00090830, 0x20090820, 0x20090830,
  /* for C bits (numbered as per FIPS 46) 7 8 10 11 12 13 */
  0x00000000, 0x02000000, 0x00002000, 0x02002000,
  0x00200000, 0x02200000, 0x00202000, 0x02202000,
  0x00000004, 0x02000004, 0x00002004, 0x02002004,
  0x00200004, 0x02200004, 0x00202004, 0x02202004,
  0x00000400, 0x02000400, 0x00002400, 0x02002400,
  0x00200400, 0x02200400, 0x00202400, 0x02202400,
  0x00000404, 0x02000404, 0x00002404, 0x02002404,
  0x00200404, 0x02200404, 0x00202404, 0x02202404,
  0x10000000, 0x12000000, 0x10002000, 0x12002000,
  0x10200000, 0x12200000, 0x10202000, 0x12202000,
  0x10000004, 0x12000004, 0x10002004, 0x12002004,
  0x10200004, 0x12200004, 0x10202004, 0x12202004,
  0x10000400, 0x12000400, 0x10002400, 0x12002400,
  0x10200400, 0x12200400, 0x10202400, 0x12202400,
  0x10000404, 0x12000404, 0x10002404, 0x12002404,
  0x10200404, 0x12200404, 0x10202404, 0x12202404,
  /* for C bits (numbered as per FIPS 46) 14 15 16 17 19 20 */
  0x00000000, 0x00000001, 0x00040000, 0x00040001,
  0x01000000, 0x01000001, 0x01040000, 0x01040001,
  0x00000002, 0x00000003, 0x00040002, 0x00040003,
  0x01000002, 0x01000003, 0x01040002, 0x01040003,
  0x00000200, 0x00000201, 0x00040200, 0x00040201,
  0x01000200, 0x01000201, 0x01040200, 0x01040201,
  0x00000202, 0x00000203, 0x00040202, 0x00040203,
  0x01000202, 0x01000203, 0x01040202, 0x01040203,
  0x08000000, 0x08000001, 0x08040000, 0x08040001,
  0x09000000, 0x09000001, 0x09040000, 0x09040001,
  0x08000002, 0x08000003, 0x08040002, 0x08040003,
  0x09000002, 0x09000003, 0x09040002, 0x09040003,
  0x08000200, 0x08000201, 0x08040200, 0x08040201,
  0x09000200, 0x09000201, 0x09040200, 0x09040201,
  0x08000202, 0x08000203, 0x08040202, 0x08040203,
  0x09000202, 0x09000203, 0x09040202, 0x09040203,
  /* for C bits (numbered as per FIPS 46) 21 23 24 26 27 28 */
  0x00000000, 0x00100000, 0x00000100, 0x00100100,
  0x00000008, 0x00100008, 0x00000108, 0x00100108,
  0x00001000, 0x00101000, 0x00001100, 0x00101100,
  0x00001008, 0x00101008, 0x00001108, 0x00101108,
  0x04000000, 0x04100000, 0x04000100, 0x04100100,
  0x04000008, 0x04100008, 0x04000108, 0x04100108,
  0x04001000, 0x04101000, 0x04001100, 0x04101100,
  0x04001008, 0x04101008, 0x04001108, 0x04101108,
  0x00020000, 0x00120000, 0x00020100, 0x00120100,
  0x00020008, 0x00120008, 0x00020108, 0x00120108,
  0x00021000, 0x00121000, 0x00021100, 0x00121100,
  0x00021008, 0x00121008, 0x00021108, 0x00121108,
  0x04020000, 0x04120000, 0x04020100, 0x04120100,
  0x04020008, 0x04120008, 0x04020108, 0x04120108,
  0x04021000, 0x04121000, 0x04021100, 0x04121100,
  0x04021008, 0x04121008, 0x04021108, 0x04121108,
  /* for D bits (numbered as per FIPS 46) 1 2 3 4 5 6 */
  0x00000000, 0x10000000, 0x00010000, 0x10010000,
  0x00000004, 0x10000004, 0x00010004, 0x10010004,
  0x20000000, 0x30000000, 0x20010000, 0x30010000,
  0x20000004, 0x30000004, 0x20010004, 0x30010004,
  0x00100000, 0x10100000, 0x00110000, 0x10110000,
  0x00100004, 0x10100004, 0x00110004, 0x10110004,
  0x20100000, 0x30100000, 0x20110000, 0x30110000,
  0x20100004, 0x30100004, 0x20110004, 0x30110004,
  0x00001000, 0x10001000, 0x00011000, 0x10011000,
  0x00001004, 0x10001004, 0x00011004, 0x10011004,
  0x20001000, 0x30001000, 0x20011000, 0x30011000,
  0x20001004, 0x30001004, 0x20011004, 0x30011004,
  0x00101000, 0x10101000, 0x00111000, 0x10111000,
  0x00101004, 0x10101004, 0x00111004, 0x10111004,
  0x20101000, 0x30101000, 0x20111000, 0x30111000,
  0x20101004, 0x30101004, 0x20111004, 0x30111004,
  /* for D bits (numbered as per FIPS 46) 8 9 11 12 13 14 */
  0x00000000, 0x08000000, 0x00000008, 0x08000008,
  0x00000400, 0x08000400, 0x00000408, 0x08000408,
  0x00020000, 0x08020000, 0x00020008, 0x08020008,
  0x00020400, 0x08020400, 0x00020408, 0x08020408,
  0x00000001, 0x08000001, 0x00000009, 0x08000009,
  0x00000401, 0x08000401, 0x00000409, 0x08000409,
  0x00020001, 0x08020001, 0x00020009, 0x08020009,
  0x00020401, 0x08020401, 0x00020409, 0x08020409,
  0x02000000, 0x0A000000, 0x02000008, 0x0A000008,
  0x02000400, 0x0A000400, 0x02000408, 0x0A000408,
  0x02020000, 0x0A020000, 0x02020008, 0x0A020008,
  0x02020400, 0x0A020400, 0x02020408, 0x0A020408,
  0x02000001, 0x0A000001, 0x02000009, 0x0A000009,
  0x02000401, 0x0A000401, 0x02000409, 0x0A000409,
  0x02020001, 0x0A020001, 0x02020009, 0x0A020009,
  0x02020401, 0x0A020401, 0x02020409, 0x0A020409,
  /* for D bits (numbered as per FIPS 46) 16 17 18 19 20 21 */
  0x00000000, 0x00000100, 0x00080000, 0x00080100,
  0x01000000, 0x01000100, 0x01080000, 0x01080100,
  0x00000010, 0x00000110, 0x00080010, 0x00080110,
  0x01000010, 0x01000110, 0x01080010, 0x01080110,
  0x00200000, 0x00200100, 0x00280000, 0x00280100,
  0x01200000, 0x01200100, 0x01280000, 0x01280100,
  0x00200010, 0x00200110, 0x00280010, 0x00280110,
  0x01200010, 0x01200110, 0x01280010, 0x01280110,
  0x00000200, 0x00000300, 0x00080200, 0x00080300,
  0x01000200, 0x01000300, 0x01080200, 0x01080300,
  0x00000210, 0x00000310, 0x00080210, 0x00080310,
  0x01000210, 0x01000310, 0x01080210, 0x01080310,
  0x00200200, 0x00200300, 0x00280200, 0x00280300,
  0x01200200, 0x01200300, 0x01280200, 0x01280300,
  0x00200210, 0x00200310, 0x00280210, 0x00280310,
  0x01200210, 0x01200310, 0x01280210, 0x01280310,
  /* for D bits (numbered as per FIPS 46) 22 23 24 25 27 28 */
  0x00000000, 0x04000000, 0x00040000, 0x04040000,
  0x00000002, 0x04000002, 0x00040002, 0x04040002,
  0x00002000, 0x04002000, 0x00042000, 0x04042000,
  0x00002002, 0x04002002, 0x00042002, 0x04042002,
  0x00000020, 0x04000020, 0x00040020, 0x04040020,
  0x00000022, 0x04000022, 0x00040022, 0x04040022,
  0x00002020, 0x04002020, 0x00042020, 0x04042020,
  0x00002022, 0x04002022, 0x00042022, 0x04042022,
  0x00000800, 0x04000800, 0x00040800, 0x04040800,
  0x00000802, 0x04000802, 0x00040802, 0x04040802,
  0x00002800, 0x04002800, 0x00042800, 0x04042800,
  0x00002802, 0x04002802, 0x00042802, 0x04042802,
  0x00000820, 0x04000820, 0x00040820, 0x04040820,
  0x00000822, 0x04000822, 0x00040822, 0x04040822,
  0x00002820, 0x04002820, 0x00042820, 0x04042820,
  0x00002822, 0x04002822, 0x00042822, 0x04042822
};

#ifdef VECT_SIZE1
#define BOX(i,n,S) u32x ((S)[(n)][(i)])
#endif

#ifdef VECT_SIZE2
#define BOX(i,n,S) u32x ((S)[(n)][(i).x], (S)[(n)][(i).y])
#endif

__device__ static void _des_crypt_keysetup (u32x c, u32x d, u32x Kc[16], u32x Kd[16], u32 s_skb[8][64])
{
  u32x tt;

  PERM_OP  (d, c, tt, 4, 0x0f0f0f0f);
  HPERM_OP (c,    tt, 2, 0xcccc0000);
  HPERM_OP (d,    tt, 2, 0xcccc0000);
  PERM_OP  (d, c, tt, 1, 0x55555555);
  PERM_OP  (c, d, tt, 8, 0x00ff00ff);
  PERM_OP  (d, c, tt, 1, 0x55555555);

  d = ((d & 0x000000ff) << 16)
    | ((d & 0x0000ff00) <<  0)
    | ((d & 0x00ff0000) >> 16)
    | ((c & 0xf0000000) >>  4);

  c = c & 0x0fffffff;

  #pragma unroll
  for (u32 i = 0; i < 16; i++)
  {
    if ((i < 2) || (i == 8) || (i == 15))
    {
      c = ((c >> 1) | (c << 27));
      d = ((d >> 1) | (d << 27));
    }
    else
    {
      c = ((c >> 2) | (c << 26));
      d = ((d >> 2) | (d << 26));
    }

    c = c & 0x0fffffff;
    d = d & 0x0fffffff;

    const u32x c00 = (c >>  0) & 0x0000003f;
    const u32x c06 = (c >>  6) & 0x00383003;
    const u32x c07 = (c >>  7) & 0x0000003c;
    const u32x c13 = (c >> 13) & 0x0000060f;
    const u32x c20 = (c >> 20) & 0x00000001;

    u32x s = BOX (((c00 >>  0) & 0xff), 0, s_skb)
            | BOX (((c06 >>  0) & 0xff)
                  |((c07 >>  0) & 0xff), 1, s_skb)
            | BOX (((c13 >>  0) & 0xff)
                  |((c06 >>  8) & 0xff), 2, s_skb)
            | BOX (((c20 >>  0) & 0xff)
                  |((c13 >>  8) & 0xff)
                  |((c06 >> 16) & 0xff), 3, s_skb);

    const u32x d00 = (d >>  0) & 0x00003c3f;
    const u32x d07 = (d >>  7) & 0x00003f03;
    const u32x d21 = (d >> 21) & 0x0000000f;
    const u32x d22 = (d >> 22) & 0x00000030;

    u32x t = BOX (((d00 >>  0) & 0xff), 4, s_skb)
            | BOX (((d07 >>  0) & 0xff)
                  |((d00 >>  8) & 0xff), 5, s_skb)
            | BOX (((d07 >>  8) & 0xff), 6, s_skb)
            | BOX (((d21 >>  0) & 0xff)
                  |((d22 >>  0) & 0xff), 7, s_skb);

    #if __CUDA_ARCH__ >= 200
    Kc[i] = __byte_perm (s, t, 0x5410);
    Kd[i] = __byte_perm (s, t, 0x7632);
    #else
    Kc[i] = ((t << 16) | (s & 0x0000ffff));
    Kd[i] = ((s >> 16) | (t & 0xffff0000));
    #endif
  }
}

__device__ static void _des_crypt_encrypt (u32x iv[2], u32 mask, u32x Kc[16], u32x Kd[16], u32 s_SPtrans[8][64])
{
  const u32 E1 = (mask >> 2) & 0x3f0;

  const u32 E0 = mask & 0x3f;

  u32x r = 0;
  u32x l = 0;

  for (u32 i = 0; i < 25; i++)
  {
    #pragma unroll
    for (u32 j = 0; j < 16; j += 2)
    {
      u32x t;
      u32x u;

      t = r ^ (r >> 16);
      u = t & E0;
      t = t & E1;
      u = u ^ (u << 16);
      u = u ^ r;
      u = u ^ Kc[j + 0];
      t = t ^ (t << 16);
      t = t ^ r;
      t = rotl32 (t, 28u);
      t = t ^ Kd[j + 0];

      l ^= BOX (((u >>  0) & 0x3f), 0, s_SPtrans)
         | BOX (((u >>  8) & 0x3f), 2, s_SPtrans)
         | BOX (((u >> 16) & 0x3f), 4, s_SPtrans)
         | BOX (((u >> 24) & 0x3f), 6, s_SPtrans)
         | BOX (((t >>  0) & 0x3f), 1, s_SPtrans)
         | BOX (((t >>  8) & 0x3f), 3, s_SPtrans)
         | BOX (((t >> 16) & 0x3f), 5, s_SPtrans)
         | BOX (((t >> 24) & 0x3f), 7, s_SPtrans);

      t = l ^ (l >> 16);
      u = t & E0;
      t = t & E1;
      u = u ^ (u << 16);
      u = u ^ l;
      u = u ^ Kc[j + 1];
      t = t ^ (t << 16);
      t = t ^ l;
      t = rotl32 (t, 28u);
      t = t ^ Kd[j + 1];

      r ^= BOX (((u >>  0) & 0x3f), 0, s_SPtrans)
         | BOX (((u >>  8) & 0x3f), 2, s_SPtrans)
         | BOX (((u >> 16) & 0x3f), 4, s_SPtrans)
         | BOX (((u >> 24) & 0x3f), 6, s_SPtrans)
         | BOX (((t >>  0) & 0x3f), 1, s_SPtrans)
         | BOX (((t >>  8) & 0x3f), 3, s_SPtrans)
         | BOX (((t >> 16) & 0x3f), 5, s_SPtrans)
         | BOX (((t >> 24) & 0x3f), 7, s_SPtrans);
    }

    u32x tt;

    tt = l;
    l  = r;
    r  = tt;
  }

  iv[0] = rotl32 (r, 31);
  iv[1] = rotl32 (l, 31);
}

extern "C" __global__ void __launch_bounds__ (256, 1) m01500_m04 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = threadIdx.x;

  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  u32x wordl0[4];

  wordl0[0] = pws[gid].i[ 0];
  wordl0[1] = pws[gid].i[ 1];
  wordl0[2] = 0;
  wordl0[3] = 0;

  u32x wordl1[4];

  wordl1[0] = 0;
  wordl1[1] = 0;
  wordl1[2] = 0;
  wordl1[3] = 0;

  u32x wordl2[4];

  wordl2[0] = 0;
  wordl2[1] = 0;
  wordl2[2] = 0;
  wordl2[3] = 0;

  u32x wordl3[4];

  wordl3[0] = 0;
  wordl3[1] = 0;
  wordl3[2] = 0;
  wordl3[3] = 0;

  const u32 pw_l_len = pws[gid].pw_len;

  if (combs_mode == COMBINATOR_MODE_BASE_RIGHT)
  {
    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, c_combs[0].pw_len);
  }

  /**
   * sbox, kbox
   */

  __shared__ u32 s_skb[8][64];
  __shared__ u32 s_SPtrans[8][64];

  if (lid < 64)
  {
    s_skb[0][lid] = c_skb[0][lid];
    s_skb[1][lid] = c_skb[1][lid];
    s_skb[2][lid] = c_skb[2][lid];
    s_skb[3][lid] = c_skb[3][lid];
    s_skb[4][lid] = c_skb[4][lid];
    s_skb[5][lid] = c_skb[5][lid];
    s_skb[6][lid] = c_skb[6][lid];
    s_skb[7][lid] = c_skb[7][lid];

    s_SPtrans[0][lid] = c_SPtrans[0][lid];
    s_SPtrans[1][lid] = c_SPtrans[1][lid];
    s_SPtrans[2][lid] = c_SPtrans[2][lid];
    s_SPtrans[3][lid] = c_SPtrans[3][lid];
    s_SPtrans[4][lid] = c_SPtrans[4][lid];
    s_SPtrans[5][lid] = c_SPtrans[5][lid];
    s_SPtrans[6][lid] = c_SPtrans[6][lid];
    s_SPtrans[7][lid] = c_SPtrans[7][lid];
  }

  __syncthreads ();

  if (gid >= gid_max) return;

  /**
   * salt
   */

  const u32 mask = salt_bufs[salt_pos].salt_buf[0];

  /**
   * main
   */

  for (u32 il_pos = 0; il_pos < combs_cnt; il_pos++)
  {
    const u32 pw_r_len = c_combs[il_pos].pw_len;

    u32 pw_len = pw_l_len + pw_r_len;

    pw_len = (pw_len >= 8) ? 8 : pw_len;

    u32 wordr0[4];

    wordr0[0] = c_combs[il_pos].i[0];
    wordr0[1] = c_combs[il_pos].i[1];
    wordr0[2] = 0;
    wordr0[3] = 0;

    u32 wordr1[4];

    wordr1[0] = 0;
    wordr1[1] = 0;
    wordr1[2] = 0;
    wordr1[3] = 0;

    u32 wordr2[4];

    wordr2[0] = 0;
    wordr2[1] = 0;
    wordr2[2] = 0;
    wordr2[3] = 0;

    u32 wordr3[4];

    wordr3[0] = 0;
    wordr3[1] = 0;
    wordr3[2] = 0;
    wordr3[3] = 0;

    if (combs_mode == COMBINATOR_MODE_BASE_LEFT)
    {
      switch_buffer_by_offset (wordr0, wordr1, wordr2, wordr3, pw_l_len);
    }

    u32x w0[4];

    w0[0] = wordl0[0] | wordr0[0];
    w0[1] = wordl0[1] | wordr0[1];
    w0[2] = 0;
    w0[3] = 0;

    u32x w1[4];

    w1[0] = 0;
    w1[1] = 0;
    w1[2] = 0;
    w1[3] = 0;

    u32x w2[4];

    w2[0] = 0;
    w2[1] = 0;
    w2[2] = 0;
    w2[3] = 0;

    u32x w3[4];

    w3[0] = 0;
    w3[1] = 0;
    w3[2] = 0;
    w3[3] = 0;

    u32x data[2];

    data[0] = (w0[0] << 1) & 0xfefefefe;
    data[1] = (w0[1] << 1) & 0xfefefefe;

    u32x Kc[16];
    u32x Kd[16];

    _des_crypt_keysetup (data[0], data[1], Kc, Kd, s_skb);

    u32x iv[2];

    _des_crypt_encrypt (iv, mask, Kc, Kd, s_SPtrans);

    const u32x r0 = iv[0];
    const u32x r1 = iv[1];
    const u32x r2 = 0;
    const u32x r3 = 0;

    #include VECT_COMPARE_M
  }
}

extern "C" __global__ void __launch_bounds__ (256, 1) m01500_m08 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

extern "C" __global__ void __launch_bounds__ (256, 1) m01500_m16 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

extern "C" __global__ void __launch_bounds__ (256, 1) m01500_s04 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = threadIdx.x;

  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  u32x wordl0[4];

  wordl0[0] = pws[gid].i[ 0];
  wordl0[1] = pws[gid].i[ 1];
  wordl0[2] = 0;
  wordl0[3] = 0;

  u32x wordl1[4];

  wordl1[0] = 0;
  wordl1[1] = 0;
  wordl1[2] = 0;
  wordl1[3] = 0;

  u32x wordl2[4];

  wordl2[0] = 0;
  wordl2[1] = 0;
  wordl2[2] = 0;
  wordl2[3] = 0;

  u32x wordl3[4];

  wordl3[0] = 0;
  wordl3[1] = 0;
  wordl3[2] = 0;
  wordl3[3] = 0;

  const u32 pw_l_len = pws[gid].pw_len;

  if (combs_mode == COMBINATOR_MODE_BASE_RIGHT)
  {
    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, c_combs[0].pw_len);
  }

  /**
   * sbox, kbox
   */

  __shared__ u32 s_skb[8][64];
  __shared__ u32 s_SPtrans[8][64];

  if (lid < 64)
  {
    s_skb[0][lid] = c_skb[0][lid];
    s_skb[1][lid] = c_skb[1][lid];
    s_skb[2][lid] = c_skb[2][lid];
    s_skb[3][lid] = c_skb[3][lid];
    s_skb[4][lid] = c_skb[4][lid];
    s_skb[5][lid] = c_skb[5][lid];
    s_skb[6][lid] = c_skb[6][lid];
    s_skb[7][lid] = c_skb[7][lid];

    s_SPtrans[0][lid] = c_SPtrans[0][lid];
    s_SPtrans[1][lid] = c_SPtrans[1][lid];
    s_SPtrans[2][lid] = c_SPtrans[2][lid];
    s_SPtrans[3][lid] = c_SPtrans[3][lid];
    s_SPtrans[4][lid] = c_SPtrans[4][lid];
    s_SPtrans[5][lid] = c_SPtrans[5][lid];
    s_SPtrans[6][lid] = c_SPtrans[6][lid];
    s_SPtrans[7][lid] = c_SPtrans[7][lid];
  }

  __syncthreads ();

  if (gid >= gid_max) return;

  /**
   * salt
   */

  const u32 mask = salt_bufs[salt_pos].salt_buf[0];

  /**
   * digest
   */

  const u32 search[4] =
  {
    digests_buf[digests_offset].digest_buf[DGST_R0],
    digests_buf[digests_offset].digest_buf[DGST_R1],
    digests_buf[digests_offset].digest_buf[DGST_R2],
    digests_buf[digests_offset].digest_buf[DGST_R3]
  };

  /**
   * main
   */

  for (u32 il_pos = 0; il_pos < combs_cnt; il_pos++)
  {
    const u32 pw_r_len = c_combs[il_pos].pw_len;

    u32 pw_len = pw_l_len + pw_r_len;

    pw_len = (pw_len >= 8) ? 8 : pw_len;

    u32 wordr0[4];

    wordr0[0] = c_combs[il_pos].i[0];
    wordr0[1] = c_combs[il_pos].i[1];
    wordr0[2] = 0;
    wordr0[3] = 0;

    u32 wordr1[4];

    wordr1[0] = 0;
    wordr1[1] = 0;
    wordr1[2] = 0;
    wordr1[3] = 0;

    u32 wordr2[4];

    wordr2[0] = 0;
    wordr2[1] = 0;
    wordr2[2] = 0;
    wordr2[3] = 0;

    u32 wordr3[4];

    wordr3[0] = 0;
    wordr3[1] = 0;
    wordr3[2] = 0;
    wordr3[3] = 0;

    if (combs_mode == COMBINATOR_MODE_BASE_LEFT)
    {
      switch_buffer_by_offset (wordr0, wordr1, wordr2, wordr3, pw_l_len);
    }

    u32x w0[4];

    w0[0] = wordl0[0] | wordr0[0];
    w0[1] = wordl0[1] | wordr0[1];
    w0[2] = 0;
    w0[3] = 0;

    u32x w1[4];

    w1[0] = 0;
    w1[1] = 0;
    w1[2] = 0;
    w1[3] = 0;

    u32x w2[4];

    w2[0] = 0;
    w2[1] = 0;
    w2[2] = 0;
    w2[3] = 0;

    u32x w3[4];

    w3[0] = 0;
    w3[1] = 0;
    w3[2] = 0;
    w3[3] = 0;

    u32x data[2];

    data[0] = (w0[0] << 1) & 0xfefefefe;
    data[1] = (w0[1] << 1) & 0xfefefefe;

    u32x Kc[16];
    u32x Kd[16];

    _des_crypt_keysetup (data[0], data[1], Kc, Kd, s_skb);

    u32x iv[2];

    _des_crypt_encrypt (iv, mask, Kc, Kd, s_SPtrans);

    const u32x r0 = iv[0];
    const u32x r1 = iv[1];
    const u32x r2 = 0;
    const u32x r3 = 0;

    #include VECT_COMPARE_S
  }
}

extern "C" __global__ void __launch_bounds__ (256, 1) m01500_s08 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

extern "C" __global__ void __launch_bounds__ (256, 1) m01500_s16 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}
