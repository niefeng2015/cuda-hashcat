/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define _MD4_
#define _SCALAR_

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
#define VECT_COMPARE_S "check_single_vect1_comp4_warp.c"
#define VECT_COMPARE_M "check_multi_vect1_comp4_warp.c"
#endif

#ifdef  VECT_SIZE2
#define VECT_COMPARE_S "check_single_vect2_comp4_warp.c"
#define VECT_COMPARE_M "check_multi_vect2_comp4_warp.c"
#endif

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
  0x02080800, 0x00080000, 0x02000002, 0x02080802,
  0x02000000, 0x00080802, 0x00080002, 0x02000002,
  0x00080802, 0x02080800, 0x02080000, 0x00000802,
  0x02000802, 0x02000000, 0x00000000, 0x00080002,
  0x00080000, 0x00000002, 0x02000800, 0x00080800,
  0x02080802, 0x02080000, 0x00000802, 0x02000800,
  0x00000002, 0x00000800, 0x00080800, 0x02080002,
  0x00000800, 0x02000802, 0x02080002, 0x00000000,
  0x00000000, 0x02080802, 0x02000800, 0x00080002,
  0x02080800, 0x00080000, 0x00000802, 0x02000800,
  0x02080002, 0x00000800, 0x00080800, 0x02000002,
  0x00080802, 0x00000002, 0x02000002, 0x02080000,
  0x02080802, 0x00080800, 0x02080000, 0x02000802,
  0x02000000, 0x00000802, 0x00080002, 0x00000000,
  0x00080000, 0x02000000, 0x02000802, 0x02080800,
  0x00000002, 0x02080002, 0x00000800, 0x00080802,
  /* nibble 1 */
  0x40108010, 0x00000000, 0x00108000, 0x40100000,
  0x40000010, 0x00008010, 0x40008000, 0x00108000,
  0x00008000, 0x40100010, 0x00000010, 0x40008000,
  0x00100010, 0x40108000, 0x40100000, 0x00000010,
  0x00100000, 0x40008010, 0x40100010, 0x00008000,
  0x00108010, 0x40000000, 0x00000000, 0x00100010,
  0x40008010, 0x00108010, 0x40108000, 0x40000010,
  0x40000000, 0x00100000, 0x00008010, 0x40108010,
  0x00100010, 0x40108000, 0x40008000, 0x00108010,
  0x40108010, 0x00100010, 0x40000010, 0x00000000,
  0x40000000, 0x00008010, 0x00100000, 0x40100010,
  0x00008000, 0x40000000, 0x00108010, 0x40008010,
  0x40108000, 0x00008000, 0x00000000, 0x40000010,
  0x00000010, 0x40108010, 0x00108000, 0x40100000,
  0x40100010, 0x00100000, 0x00008010, 0x40008000,
  0x40008010, 0x00000010, 0x40100000, 0x00108000,
  /* nibble 2 */
  0x04000001, 0x04040100, 0x00000100, 0x04000101,
  0x00040001, 0x04000000, 0x04000101, 0x00040100,
  0x04000100, 0x00040000, 0x04040000, 0x00000001,
  0x04040101, 0x00000101, 0x00000001, 0x04040001,
  0x00000000, 0x00040001, 0x04040100, 0x00000100,
  0x00000101, 0x04040101, 0x00040000, 0x04000001,
  0x04040001, 0x04000100, 0x00040101, 0x04040000,
  0x00040100, 0x00000000, 0x04000000, 0x00040101,
  0x04040100, 0x00000100, 0x00000001, 0x00040000,
  0x00000101, 0x00040001, 0x04040000, 0x04000101,
  0x00000000, 0x04040100, 0x00040100, 0x04040001,
  0x00040001, 0x04000000, 0x04040101, 0x00000001,
  0x00040101, 0x04000001, 0x04000000, 0x04040101,
  0x00040000, 0x04000100, 0x04000101, 0x00040100,
  0x04000100, 0x00000000, 0x04040001, 0x00000101,
  0x04000001, 0x00040101, 0x00000100, 0x04040000,
  /* nibble 3 */
  0x00401008, 0x10001000, 0x00000008, 0x10401008,
  0x00000000, 0x10400000, 0x10001008, 0x00400008,
  0x10401000, 0x10000008, 0x10000000, 0x00001008,
  0x10000008, 0x00401008, 0x00400000, 0x10000000,
  0x10400008, 0x00401000, 0x00001000, 0x00000008,
  0x00401000, 0x10001008, 0x10400000, 0x00001000,
  0x00001008, 0x00000000, 0x00400008, 0x10401000,
  0x10001000, 0x10400008, 0x10401008, 0x00400000,
  0x10400008, 0x00001008, 0x00400000, 0x10000008,
  0x00401000, 0x10001000, 0x00000008, 0x10400000,
  0x10001008, 0x00000000, 0x00001000, 0x00400008,
  0x00000000, 0x10400008, 0x10401000, 0x00001000,
  0x10000000, 0x10401008, 0x00401008, 0x00400000,
  0x10401008, 0x00000008, 0x10001000, 0x00401008,
  0x00400008, 0x00401000, 0x10400000, 0x10001008,
  0x00001008, 0x10000000, 0x10000008, 0x10401000,
  /* nibble 4 */
  0x08000000, 0x00010000, 0x00000400, 0x08010420,
  0x08010020, 0x08000400, 0x00010420, 0x08010000,
  0x00010000, 0x00000020, 0x08000020, 0x00010400,
  0x08000420, 0x08010020, 0x08010400, 0x00000000,
  0x00010400, 0x08000000, 0x00010020, 0x00000420,
  0x08000400, 0x00010420, 0x00000000, 0x08000020,
  0x00000020, 0x08000420, 0x08010420, 0x00010020,
  0x08010000, 0x00000400, 0x00000420, 0x08010400,
  0x08010400, 0x08000420, 0x00010020, 0x08010000,
  0x00010000, 0x00000020, 0x08000020, 0x08000400,
  0x08000000, 0x00010400, 0x08010420, 0x00000000,
  0x00010420, 0x08000000, 0x00000400, 0x00010020,
  0x08000420, 0x00000400, 0x00000000, 0x08010420,
  0x08010020, 0x08010400, 0x00000420, 0x00010000,
  0x00010400, 0x08010020, 0x08000400, 0x00000420,
  0x00000020, 0x00010420, 0x08010000, 0x08000020,
  /* nibble 5 */
  0x80000040, 0x00200040, 0x00000000, 0x80202000,
  0x00200040, 0x00002000, 0x80002040, 0x00200000,
  0x00002040, 0x80202040, 0x00202000, 0x80000000,
  0x80002000, 0x80000040, 0x80200000, 0x00202040,
  0x00200000, 0x80002040, 0x80200040, 0x00000000,
  0x00002000, 0x00000040, 0x80202000, 0x80200040,
  0x80202040, 0x80200000, 0x80000000, 0x00002040,
  0x00000040, 0x00202000, 0x00202040, 0x80002000,
  0x00002040, 0x80000000, 0x80002000, 0x00202040,
  0x80202000, 0x00200040, 0x00000000, 0x80002000,
  0x80000000, 0x00002000, 0x80200040, 0x00200000,
  0x00200040, 0x80202040, 0x00202000, 0x00000040,
  0x80202040, 0x00202000, 0x00200000, 0x80002040,
  0x80000040, 0x80200000, 0x00202040, 0x00000000,
  0x00002000, 0x80000040, 0x80002040, 0x80202000,
  0x80200000, 0x00002040, 0x00000040, 0x80200040,
  /* nibble 6 */
  0x00004000, 0x00000200, 0x01000200, 0x01000004,
  0x01004204, 0x00004004, 0x00004200, 0x00000000,
  0x01000000, 0x01000204, 0x00000204, 0x01004000,
  0x00000004, 0x01004200, 0x01004000, 0x00000204,
  0x01000204, 0x00004000, 0x00004004, 0x01004204,
  0x00000000, 0x01000200, 0x01000004, 0x00004200,
  0x01004004, 0x00004204, 0x01004200, 0x00000004,
  0x00004204, 0x01004004, 0x00000200, 0x01000000,
  0x00004204, 0x01004000, 0x01004004, 0x00000204,
  0x00004000, 0x00000200, 0x01000000, 0x01004004,
  0x01000204, 0x00004204, 0x00004200, 0x00000000,
  0x00000200, 0x01000004, 0x00000004, 0x01000200,
  0x00000000, 0x01000204, 0x01000200, 0x00004200,
  0x00000204, 0x00004000, 0x01004204, 0x01000000,
  0x01004200, 0x00000004, 0x00004004, 0x01004204,
  0x01000004, 0x01004200, 0x01004000, 0x00004004,
  /* nibble 7 */
  0x20800080, 0x20820000, 0x00020080, 0x00000000,
  0x20020000, 0x00800080, 0x20800000, 0x20820080,
  0x00000080, 0x20000000, 0x00820000, 0x00020080,
  0x00820080, 0x20020080, 0x20000080, 0x20800000,
  0x00020000, 0x00820080, 0x00800080, 0x20020000,
  0x20820080, 0x20000080, 0x00000000, 0x00820000,
  0x20000000, 0x00800000, 0x20020080, 0x20800080,
  0x00800000, 0x00020000, 0x20820000, 0x00000080,
  0x00800000, 0x00020000, 0x20000080, 0x20820080,
  0x00020080, 0x20000000, 0x00000000, 0x00820000,
  0x20800080, 0x20020080, 0x20020000, 0x00800080,
  0x20820000, 0x00000080, 0x00800080, 0x20020000,
  0x20820080, 0x00800000, 0x20800000, 0x20000080,
  0x00820000, 0x00020080, 0x20020080, 0x20800000,
  0x00000080, 0x20820000, 0x00820080, 0x00000000,
  0x20000000, 0x20800080, 0x00020000, 0x00820080,
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

__device__ static void _des_crypt_encrypt (u32x iv[2], u32x data[2], u32x Kc[16], u32x Kd[16], u32 s_SPtrans[8][64])
{
  u32x tt;

  u32x r = data[0];
  u32x l = data[1];

  #pragma unroll 16
  for (u32 i = 0; i < 16; i++)
  {
    u32x u = Kc[i] ^ r;
    u32x t = Kd[i] ^ rotl32 (r, 28u);

    l ^= BOX (((u >>  2) & 0x3f), 0, s_SPtrans)
       | BOX (((u >> 10) & 0x3f), 2, s_SPtrans)
       | BOX (((u >> 18) & 0x3f), 4, s_SPtrans)
       | BOX (((u >> 26) & 0x3f), 6, s_SPtrans)
       | BOX (((t >>  2) & 0x3f), 1, s_SPtrans)
       | BOX (((t >> 10) & 0x3f), 3, s_SPtrans)
       | BOX (((t >> 18) & 0x3f), 5, s_SPtrans)
       | BOX (((t >> 26) & 0x3f), 7, s_SPtrans);

    tt = l;
    l  = r;
    r  = tt;
  }

  iv[0] = l;
  iv[1] = r;
}

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

  #pragma unroll 16
  for (u32 i = 0; i < 16; i++)
  {
    const u32 shifts3s0[16] = {  1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1 };
    const u32 shifts3s1[16] = { 27, 27, 26, 26, 26, 26, 26, 26, 27, 26, 26, 26, 26, 26, 26, 27 };

    c = c >> shifts3s0[i] | c << shifts3s1[i];
    d = d >> shifts3s0[i] | d << shifts3s1[i];

    c = c & 0x0fffffff;
    d = d & 0x0fffffff;

    u32x s = BOX ((( c >>  0) & 0x3f),  0, s_skb)
            | BOX ((((c >>  6) & 0x03)
                  | ((c >>  7) & 0x3c)), 1, s_skb)
            | BOX ((((c >> 13) & 0x0f)
                  | ((c >> 14) & 0x30)), 2, s_skb)
            | BOX ((((c >> 20) & 0x01)
                  | ((c >> 21) & 0x06)
                  | ((c >> 22) & 0x38)), 3, s_skb);

    u32x t = BOX ((( d >>  0) & 0x3f),  4, s_skb)
            | BOX ((((d >>  7) & 0x03)
                  | ((d >>  8) & 0x3c)), 5, s_skb)
            | BOX ((((d >> 15) & 0x3f)), 6, s_skb)
            | BOX ((((d >> 21) & 0x0f)
                  | ((d >> 22) & 0x30)), 7, s_skb);

    #if __CUDA_ARCH__ >= 200
    Kc[i] = __byte_perm (s, t, 0x5410);
    Kd[i] = __byte_perm (s, t, 0x7632);
    #else
    Kc[i] = ((t << 16) | (s & 0x0000ffff));
    Kd[i] = ((s >> 16) | (t & 0xffff0000));
    #endif

    Kc[i] = rotl32 (Kc[i], 2u);
    Kd[i] = rotl32 (Kd[i], 2u);
  }
}

__device__ static void transform_netntlmv1_key (const u32x w0, const u32x w1, u32x out[2])
{
  u32x t[8];

  t[0] = (w0 >>  0) & 0xff;
  t[1] = (w0 >>  8) & 0xff;
  t[2] = (w0 >> 16) & 0xff;
  t[3] = (w0 >> 24) & 0xff;
  t[4] = (w1 >>  0) & 0xff;
  t[5] = (w1 >>  8) & 0xff;
  t[6] = (w1 >> 16) & 0xff;
  t[7] = (w1 >> 24) & 0xff;

  u32x k[8];

  k[0] =               (t[0] >> 0);
  k[1] = (t[0] << 7) | (t[1] >> 1);
  k[2] = (t[1] << 6) | (t[2] >> 2);
  k[3] = (t[2] << 5) | (t[3] >> 3);
  k[4] = (t[3] << 4) | (t[4] >> 4);
  k[5] = (t[4] << 3) | (t[5] >> 5);
  k[6] = (t[5] << 2) | (t[6] >> 6);
  k[7] = (t[6] << 1);

  out[0] = ((k[0] & 0xff) <<  0)
         | ((k[1] & 0xff) <<  8)
         | ((k[2] & 0xff) << 16)
         | ((k[3] & 0xff) << 24);

  out[1] = ((k[4] & 0xff) <<  0)
         | ((k[5] & 0xff) <<  8)
         | ((k[6] & 0xff) << 16)
         | ((k[7] & 0xff) << 24);
}

__device__ __shared__ u32 s_skb[8][64];
__device__ __shared__ u32 s_SPtrans[8][64];

__device__ __constant__ u32x c_bfs[1024];

__device__ static void m05500m (u32 w[16], const u32 pw_len, const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *words_buf_r, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset)
{
  /**
   * modifier
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;
  const u32 lid = threadIdx.x;

  /**
   * salt
   */

  const u32 s0 = salt_bufs[salt_pos].salt_buf[0];
  const u32 s1 = salt_bufs[salt_pos].salt_buf[1];
  const u32 s2 = salt_bufs[salt_pos].salt_buf[2];

  u32x data[2];

  data[0] = s0;
  data[1] = s1;

  /**
   * loop
   */

  const u32 bf_loops = ceil ((float) bfs_cnt / VECT_DIV);

  u32x w0l = w[0];

  for (u32 il_pos = 0; il_pos < bf_loops; il_pos++)
  {
    const u32x w0r = c_bfs[il_pos];

    const u32x w0 = w0l | w0r;

    u32x a = MD4M_A;
    u32x b = MD4M_B;
    u32x c = MD4M_C;
    u32x d = MD4M_D;

    #define w0_t w0
    #define w1_t w[ 1]
    #define w2_t w[ 2]
    #define w3_t w[ 3]
    #define w4_t w[ 4]
    #define w5_t w[ 5]
    #define w6_t w[ 6]
    #define w7_t w[ 7]
    #define w8_t w[ 8]
    #define w9_t w[ 9]
    #define wa_t w[10]
    #define wb_t w[11]
    #define wc_t w[12]
    #define wd_t w[13]
    #define we_t w[14]
    #define wf_t w[15]

    MD4_STEP (MD4_Fo, a, b, c, d, w0_t, MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w1_t, MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w2_t, MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w3_t, MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w4_t, MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w5_t, MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w6_t, MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w7_t, MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w8_t, MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w9_t, MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, wa_t, MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, wb_t, MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, wc_t, MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, wd_t, MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, we_t, MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, wf_t, MD4C00, MD4S03);

    MD4_STEP (MD4_Go, a, b, c, d, w0_t, MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w4_t, MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w8_t, MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, wc_t, MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w1_t, MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w5_t, MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w9_t, MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, wd_t, MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w2_t, MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w6_t, MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, wa_t, MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, we_t, MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w3_t, MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w7_t, MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, wb_t, MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, wf_t, MD4C01, MD4S13);

    MD4_STEP (MD4_H , a, b, c, d, w0_t, MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w8_t, MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w4_t, MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, wc_t, MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w2_t, MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, wa_t, MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w6_t, MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, we_t, MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w1_t, MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w9_t, MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w5_t, MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, wd_t, MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w3_t, MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, wb_t, MD4C02, MD4S21);

    if (s2 != ((d + MD4M_D) >> 16)) continue;

    MD4_STEP (MD4_H , c, d, a, b, w7_t, MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, wf_t, MD4C02, MD4S23);

    a += MD4M_A;
    b += MD4M_B;
    c += MD4M_C;
    d += MD4M_D;

    /**
     * DES1
     */

    u32x key[2];

    transform_netntlmv1_key (a, b, key);

    u32x Kc[16];
    u32x Kd[16];

    _des_crypt_keysetup (key[0], key[1], Kc, Kd, s_skb);

    u32x iv1[2];

    _des_crypt_encrypt (iv1, data, Kc, Kd, s_SPtrans);

    /**
     * DES2
     */

    transform_netntlmv1_key (((b >> 24) | (c << 8)), ((c >> 24) | (d << 8)), key);

    _des_crypt_keysetup (key[0], key[1], Kc, Kd, s_skb);

    u32x iv2[2];

    _des_crypt_encrypt (iv2, data, Kc, Kd, s_SPtrans);

    /**
     * compare
     */

    const u32x r0 = iv1[0];
    const u32x r1 = iv1[1];
    const u32x r2 = iv2[0];
    const u32x r3 = iv2[1];

    #include VECT_COMPARE_M
  }
}

__device__ static void m05500s (u32 w[16], const u32 pw_len, const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *words_buf_r, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset)
{
  /**
   * modifier
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;
  const u32 lid = threadIdx.x;

  /**
   * salt
   */

  const u32 s0 = salt_bufs[salt_pos].salt_buf[0];
  const u32 s1 = salt_bufs[salt_pos].salt_buf[1];
  const u32 s2 = salt_bufs[salt_pos].salt_buf[2];

  u32x data[2];

  data[0] = s0;
  data[1] = s1;

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
   * loop
   */

  const u32 bf_loops = ceil ((float) bfs_cnt / VECT_DIV);

  u32x w0l = w[0];

  for (u32 il_pos = 0; il_pos < bf_loops; il_pos++)
  {
    const u32x w0r = c_bfs[il_pos];

    const u32x w0 = w0l | w0r;

    u32x a = MD4M_A;
    u32x b = MD4M_B;
    u32x c = MD4M_C;
    u32x d = MD4M_D;

    #define w0_t w0
    #define w1_t w[ 1]
    #define w2_t w[ 2]
    #define w3_t w[ 3]
    #define w4_t w[ 4]
    #define w5_t w[ 5]
    #define w6_t w[ 6]
    #define w7_t w[ 7]
    #define w8_t w[ 8]
    #define w9_t w[ 9]
    #define wa_t w[10]
    #define wb_t w[11]
    #define wc_t w[12]
    #define wd_t w[13]
    #define we_t w[14]
    #define wf_t w[15]

    MD4_STEP (MD4_Fo, a, b, c, d, w0_t, MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w1_t, MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w2_t, MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w3_t, MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w4_t, MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w5_t, MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w6_t, MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w7_t, MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w8_t, MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w9_t, MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, wa_t, MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, wb_t, MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, wc_t, MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, wd_t, MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, we_t, MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, wf_t, MD4C00, MD4S03);

    MD4_STEP (MD4_Go, a, b, c, d, w0_t, MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w4_t, MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w8_t, MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, wc_t, MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w1_t, MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w5_t, MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w9_t, MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, wd_t, MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w2_t, MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w6_t, MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, wa_t, MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, we_t, MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w3_t, MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w7_t, MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, wb_t, MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, wf_t, MD4C01, MD4S13);

    MD4_STEP (MD4_H , a, b, c, d, w0_t, MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w8_t, MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w4_t, MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, wc_t, MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w2_t, MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, wa_t, MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w6_t, MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, we_t, MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w1_t, MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w9_t, MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w5_t, MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, wd_t, MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w3_t, MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, wb_t, MD4C02, MD4S21);

    if (s2 != ((d + MD4M_D) >> 16)) continue;

    MD4_STEP (MD4_H , c, d, a, b, w7_t, MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, wf_t, MD4C02, MD4S23);

    a += MD4M_A;
    b += MD4M_B;
    c += MD4M_C;
    d += MD4M_D;

    /**
     * DES1
     */

    u32x key[2];

    transform_netntlmv1_key (a, b, key);

    u32x Kc[16];
    u32x Kd[16];

    _des_crypt_keysetup (key[0], key[1], Kc, Kd, s_skb);

    u32x iv1[2];

    _des_crypt_encrypt (iv1, data, Kc, Kd, s_SPtrans);

    /**
     * DES2
     */

    transform_netntlmv1_key (((b >> 24) | (c << 8)), ((c >> 24) | (d << 8)), key);

    _des_crypt_keysetup (key[0], key[1], Kc, Kd, s_skb);

    u32x iv2[2];

    _des_crypt_encrypt (iv2, data, Kc, Kd, s_SPtrans);

    /**
     * compare
     */

    const u32x r0 = iv1[0];
    const u32x r1 = iv1[1];
    const u32x r2 = iv2[0];
    const u32x r3 = iv2[1];

    #include VECT_COMPARE_S
  }
}

extern "C" __global__ void __launch_bounds__ (256, 1) m05500_m04 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *words_buf_r, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;
  const u32 lid = threadIdx.x;

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = 0;
  w[ 5] = 0;
  w[ 6] = 0;
  w[ 7] = 0;
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len;

  /**
   * sbox, kbox
   */

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
   * main
   */

  m05500m (w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (256, 1) m05500_m08 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *words_buf_r, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;
  const u32 lid = threadIdx.x;

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = pws[gid].i[ 4];
  w[ 5] = pws[gid].i[ 5];
  w[ 6] = pws[gid].i[ 6];
  w[ 7] = pws[gid].i[ 7];
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len;

  /**
   * sbox, kbox
   */

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
   * main
   */

  m05500m (w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (256, 1) m05500_m16 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *words_buf_r, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

extern "C" __global__ void __launch_bounds__ (256, 1) m05500_s04 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *words_buf_r, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;
  const u32 lid = threadIdx.x;

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = 0;
  w[ 5] = 0;
  w[ 6] = 0;
  w[ 7] = 0;
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len;

  /**
   * sbox, kbox
   */

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
   * main
   */

  m05500s (w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (256, 1) m05500_s08 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *words_buf_r, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;
  const u32 lid = threadIdx.x;

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = pws[gid].i[ 4];
  w[ 5] = pws[gid].i[ 5];
  w[ 6] = pws[gid].i[ 6];
  w[ 7] = pws[gid].i[ 7];
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len;

  /**
   * sbox, kbox
   */

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
   * main
   */

  m05500s (w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (256, 1) m05500_s16 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *words_buf_r, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}
