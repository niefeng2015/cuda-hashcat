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

#ifdef  VLIW4
#define VECT_SIZE2
#endif

#ifdef  VLIW5
#define VECT_SIZE2
#endif

#define DGST_R0 0
#define DGST_R1 1
#define DGST_R2 2
#define DGST_R3 3

#include "include/kernel_functions.c"
#include "types_amd.c"
#include "common_amd.c"

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

#define IP(l,r,tt)                     \
{                                      \
  PERM_OP (r, l, tt,  4, 0x0f0f0f0f);  \
  PERM_OP (l, r, tt, 16, 0x0000ffff);  \
  PERM_OP (r, l, tt,  2, 0x33333333);  \
  PERM_OP (l, r, tt,  8, 0x00ff00ff);  \
  PERM_OP (r, l, tt,  1, 0x55555555);  \
}

#define FP(l,r,tt)                     \
{                                      \
  PERM_OP (l, r, tt,  1, 0x55555555);  \
  PERM_OP (r, l, tt,  8, 0x00ff00ff);  \
  PERM_OP (l, r, tt,  2, 0x33333333);  \
  PERM_OP (r, l, tt, 16, 0x0000ffff);  \
  PERM_OP (l, r, tt,  4, 0x0f0f0f0f);  \
}

__constant u32 c_SPtrans[8][64] =
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

__constant u32 c_skb[8][64] =
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

__constant u32 shifts3s0[16] = {  1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1 };
__constant u32 shifts3s1[16] = { 27, 27, 26, 26, 26, 26, 26, 26, 27, 26, 26, 26, 26, 26, 26, 27 };

#ifdef VECT_SIZE1
#define BOX(i,n,S) u32x ((S)[(n)][(i)])
#endif

#ifdef VECT_SIZE2
#define BOX(i,n,S) u32x ((S)[(n)][(i).s0], (S)[(n)][(i).s1])
#endif

#ifdef VECT_SIZE4
#define BOX(i,n,S) u32x ((S)[(n)][(i).s0], (S)[(n)][(i).s1], (S)[(n)][(i).s2], (S)[(n)][(i).s3])
#endif

static void _des_crypt_encrypt (u32x iv[2], u32x data[2], u32x Kc[16], u32x Kd[16], __local u32 s_SPtrans[8][64])
{
  u32x tt;

  u32x r = data[0];
  u32x l = data[1];

  IP (r, l, tt);

  r = rotl32 (r, 3u);
	l = rotl32 (l, 3u);

  #pragma unroll 16
  for (u32 i = 0; i < 16; i += 2)
  {
    u32x u;
    u32x t;

    u = Kc[i + 0] ^ r;
    t = Kd[i + 0] ^ rotl32 (r, 28u);

    l = l
      ^ BOX (amd_bfe (u,  2, 6), 0, s_SPtrans)
      ^ BOX (amd_bfe (u, 10, 6), 2, s_SPtrans)
      ^ BOX (amd_bfe (u, 18, 6), 4, s_SPtrans)
      ^ BOX (amd_bfe (u, 26, 6), 6, s_SPtrans)
      ^ BOX (amd_bfe (t,  2, 6), 1, s_SPtrans)
      ^ BOX (amd_bfe (t, 10, 6), 3, s_SPtrans)
      ^ BOX (amd_bfe (t, 18, 6), 5, s_SPtrans)
      ^ BOX (amd_bfe (t, 26, 6), 7, s_SPtrans);

    u = Kc[i + 1] ^ l;
    t = Kd[i + 1] ^ rotl32 (l, 28u);

    r = r
      ^ BOX (amd_bfe (u,  2, 6), 0, s_SPtrans)
      ^ BOX (amd_bfe (u, 10, 6), 2, s_SPtrans)
      ^ BOX (amd_bfe (u, 18, 6), 4, s_SPtrans)
      ^ BOX (amd_bfe (u, 26, 6), 6, s_SPtrans)
      ^ BOX (amd_bfe (t,  2, 6), 1, s_SPtrans)
      ^ BOX (amd_bfe (t, 10, 6), 3, s_SPtrans)
      ^ BOX (amd_bfe (t, 18, 6), 5, s_SPtrans)
      ^ BOX (amd_bfe (t, 26, 6), 7, s_SPtrans);
  }

  l = rotl32 (l, 29u);
  r = rotl32 (r, 29u);

  FP (r, l, tt);

  iv[0] = l;
  iv[1] = r;
}

static void _des_crypt_keysetup (u32x c, u32x d, u32x Kc[16], u32x Kd[16], __local u32 s_skb[8][64])
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

    #if defined cl_amd_media_ops
    Kc[i] = amd_bytealign (t, s << 16, 2);
    Kd[i] = amd_bytealign (t >> 16, s, 2);
    #else
    Kc[i] = ((t << 16) | (s & 0x0000ffff));
    Kd[i] = ((s >> 16) | (t & 0xffff0000));
    #endif

    Kc[i] = rotl32 (Kc[i], 2u);
    Kd[i] = rotl32 (Kd[i], 2u);
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m03100_m04 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = get_local_id (0);

  /**
   * base
   */

  const u32 gid = get_global_id (0);

  u32x wordl0[4];

  wordl0[0] = pws[gid].i[ 0];
  wordl0[1] = pws[gid].i[ 1];
  wordl0[2] = pws[gid].i[ 2];
  wordl0[3] = pws[gid].i[ 3];

  u32x wordl1[4];

  wordl1[0] = pws[gid].i[ 4];
  wordl1[1] = pws[gid].i[ 5];
  wordl1[2] = pws[gid].i[ 6];
  wordl1[3] = pws[gid].i[ 7];

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
    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, combs_buf[0].pw_len);
  }

  /**
   * sbox, kbox
   */

  __local u32 s_SPtrans[8][64];

  __local u32 s_skb[8][64];

  s_SPtrans[0][lid] = c_SPtrans[0][lid];
  s_SPtrans[1][lid] = c_SPtrans[1][lid];
  s_SPtrans[2][lid] = c_SPtrans[2][lid];
  s_SPtrans[3][lid] = c_SPtrans[3][lid];
  s_SPtrans[4][lid] = c_SPtrans[4][lid];
  s_SPtrans[5][lid] = c_SPtrans[5][lid];
  s_SPtrans[6][lid] = c_SPtrans[6][lid];
  s_SPtrans[7][lid] = c_SPtrans[7][lid];

  s_skb[0][lid] = c_skb[0][lid];
  s_skb[1][lid] = c_skb[1][lid];
  s_skb[2][lid] = c_skb[2][lid];
  s_skb[3][lid] = c_skb[3][lid];
  s_skb[4][lid] = c_skb[4][lid];
  s_skb[5][lid] = c_skb[5][lid];
  s_skb[6][lid] = c_skb[6][lid];
  s_skb[7][lid] = c_skb[7][lid];

  barrier (CLK_LOCAL_MEM_FENCE);

  if (gid >= gid_max) return;

  /**
   * salt
   */

  u32 salt_buf0[4];

  salt_buf0[0] = salt_bufs[salt_pos].salt_buf[0];
  salt_buf0[1] = salt_bufs[salt_pos].salt_buf[1];
  salt_buf0[2] = salt_bufs[salt_pos].salt_buf[2];
  salt_buf0[3] = salt_bufs[salt_pos].salt_buf[3];

  u32 salt_buf1[4];

  salt_buf1[0] = salt_bufs[salt_pos].salt_buf[4];
  salt_buf1[1] = salt_bufs[salt_pos].salt_buf[5];
  salt_buf1[2] = salt_bufs[salt_pos].salt_buf[6];
  salt_buf1[3] = salt_bufs[salt_pos].salt_buf[7];

  const u32 salt_len = salt_bufs[salt_pos].salt_len;

  /**
   * loop
   */

  for (u32 il_pos = 0; il_pos < combs_cnt; il_pos++)
  {
    const u32 pw_r_len = combs_buf[il_pos].pw_len;

    const u32 pw_len = pw_l_len + pw_r_len;

    const u32 salt_word_len = (salt_len + pw_len) * 2;

    u32 wordr0[4];

    wordr0[0] = combs_buf[il_pos].i[0];
    wordr0[1] = combs_buf[il_pos].i[1];
    wordr0[2] = combs_buf[il_pos].i[2];
    wordr0[3] = combs_buf[il_pos].i[3];

    u32 wordr1[4];

    wordr1[0] = combs_buf[il_pos].i[4];
    wordr1[1] = combs_buf[il_pos].i[5];
    wordr1[2] = combs_buf[il_pos].i[6];
    wordr1[3] = combs_buf[il_pos].i[7];

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
    w0[2] = wordl0[2] | wordr0[2];
    w0[3] = wordl0[3] | wordr0[3];

    u32x w1[4];

    w1[0] = wordl1[0] | wordr1[0];
    w1[1] = wordl1[1] | wordr1[1];
    w1[2] = wordl1[2] | wordr1[2];
    w1[3] = wordl1[3] | wordr1[3];

    u32x w2[4];

    w2[0] = wordl2[0] | wordr2[0];
    w2[1] = wordl2[1] | wordr2[1];
    w2[2] = wordl2[2] | wordr2[2];
    w2[3] = wordl2[3] | wordr2[3];

    u32x w3[4];

    w3[0] = wordl3[0] | wordr3[0];
    w3[1] = wordl3[1] | wordr3[1];
    w3[2] = wordl3[2] | wordr3[2];
    w3[3] = wordl3[3] | wordr3[3];

    /**
     * prepend salt
     */

    u32x w0_t[4];
    u32x w1_t[4];
    u32x w2_t[4];
    u32x w3_t[4];

    w0_t[0] = w0[0];
    w0_t[1] = w0[1];
    w0_t[2] = w0[2];
    w0_t[3] = w0[3];
    w1_t[0] = w1[0];
    w1_t[1] = w1[1];
    w1_t[2] = w1[2];
    w1_t[3] = w1[3];
    w2_t[0] = w2[0];
    w2_t[1] = w2[1];
    w2_t[2] = w2[2];
    w2_t[3] = w2[3];
    w3_t[0] = w3[0];
    w3_t[1] = w3[1];
    w3_t[2] = w3[2];
    w3_t[3] = w3[3];

    switch_buffer_by_offset (w0_t, w1_t, w2_t, w3_t, salt_len);

    w0_t[0] |= salt_buf0[0];
    w0_t[1] |= salt_buf0[1];
    w0_t[2] |= salt_buf0[2];
    w0_t[3] |= salt_buf0[3];
    w1_t[0] |= salt_buf1[0];
    w1_t[1] |= salt_buf1[1];
    w1_t[2] |= salt_buf1[2];
    w1_t[3] |= salt_buf1[3];

    u32x dst[16];

    dst[ 0] = w0_t[0];
    dst[ 1] = w0_t[1];
    dst[ 2] = w0_t[2];
    dst[ 3] = w0_t[3];
    dst[ 4] = w1_t[0];
    dst[ 5] = w1_t[1];
    dst[ 6] = w1_t[2];
    dst[ 7] = w1_t[3];
    dst[ 8] = w2_t[0];
    dst[ 9] = w2_t[1];
    dst[10] = w2_t[2];
    dst[11] = w2_t[3];
    dst[12] = 0;
    dst[13] = 0;
    dst[14] = 0;
    dst[15] = 0;

    /**
     * precompute key1 since key is static: 0x0123456789abcdef
     * plus LEFT_ROTATE by 2
     */

    u32x Kc[16];

    Kc[ 0] = 0x64649040;
    Kc[ 1] = 0x14909858;
    Kc[ 2] = 0xc4b44888;
    Kc[ 3] = 0x9094e438;
    Kc[ 4] = 0xd8a004f0;
    Kc[ 5] = 0xa8f02810;
    Kc[ 6] = 0xc84048d8;
    Kc[ 7] = 0x68d804a8;
    Kc[ 8] = 0x0490e40c;
    Kc[ 9] = 0xac183024;
    Kc[10] = 0x24c07c10;
    Kc[11] = 0x8c88c038;
    Kc[12] = 0xc048c824;
    Kc[13] = 0x4c0470a8;
    Kc[14] = 0x584020b4;
    Kc[15] = 0x00742c4c;

    u32x Kd[16];

    Kd[ 0] = 0xa42ce40c;
    Kd[ 1] = 0x64689858;
    Kd[ 2] = 0x484050b8;
    Kd[ 3] = 0xe8184814;
    Kd[ 4] = 0x405cc070;
    Kd[ 5] = 0xa010784c;
    Kd[ 6] = 0x6074a800;
    Kd[ 7] = 0x80701c1c;
    Kd[ 8] = 0x9cd49430;
    Kd[ 9] = 0x4c8ce078;
    Kd[10] = 0x5c18c088;
    Kd[11] = 0x28a8a4c8;
    Kd[12] = 0x3c180838;
    Kd[13] = 0xb0b86c20;
    Kd[14] = 0xac84a094;
    Kd[15] = 0x4ce0c0c4;

    /**
     * key1 (generate key)
     */

    u32x iv[2];

    iv[0] = 0;
    iv[1] = 0;

    for (u32 j = 0, k = 0; j < salt_word_len; j += 8, k++)
    {
      u32x data[2];

      data[0] = ((dst[k] << 16) & 0xff000000) | ((dst[k] << 8) & 0x0000ff00);
      data[1] = ((dst[k] >>  0) & 0xff000000) | ((dst[k] >> 8) & 0x0000ff00);

      data[0] ^= iv[0];
      data[1] ^= iv[1];

      _des_crypt_encrypt (iv, data, Kc, Kd, s_SPtrans);
    }

    /**
     * key2 (generate hash)
     */

    _des_crypt_keysetup (iv[0], iv[1], Kc, Kd, s_skb);

    iv[0] = 0;
    iv[1] = 0;

    for (u32 j = 0, k = 0; j < salt_word_len; j += 8, k++)
    {
      u32x data[2];

      data[0] = ((dst[k] << 16) & 0xff000000) | ((dst[k] << 8) & 0x0000ff00);
      data[1] = ((dst[k] >>  0) & 0xff000000) | ((dst[k] >> 8) & 0x0000ff00);

      data[0] ^= iv[0];
      data[1] ^= iv[1];

      _des_crypt_encrypt (iv, data, Kc, Kd, s_SPtrans);
    }

    /**
     * cmp
     */

    const u32x r0 = iv[0];
    const u32x r1 = iv[1];
    const u32x r2 = 0;
    const u32x r3 = 0;

    #include VECT_COMPARE_M
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m03100_m08 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m03100_m16 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m03100_s04 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = get_local_id (0);

  /**
   * base
   */

  const u32 gid = get_global_id (0);

  u32x wordl0[4];

  wordl0[0] = pws[gid].i[ 0];
  wordl0[1] = pws[gid].i[ 1];
  wordl0[2] = pws[gid].i[ 2];
  wordl0[3] = pws[gid].i[ 3];

  u32x wordl1[4];

  wordl1[0] = pws[gid].i[ 4];
  wordl1[1] = pws[gid].i[ 5];
  wordl1[2] = pws[gid].i[ 6];
  wordl1[3] = pws[gid].i[ 7];

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
    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, combs_buf[0].pw_len);
  }

  /**
   * sbox, kbox
   */

  __local u32 s_SPtrans[8][64];

  __local u32 s_skb[8][64];

  s_SPtrans[0][lid] = c_SPtrans[0][lid];
  s_SPtrans[1][lid] = c_SPtrans[1][lid];
  s_SPtrans[2][lid] = c_SPtrans[2][lid];
  s_SPtrans[3][lid] = c_SPtrans[3][lid];
  s_SPtrans[4][lid] = c_SPtrans[4][lid];
  s_SPtrans[5][lid] = c_SPtrans[5][lid];
  s_SPtrans[6][lid] = c_SPtrans[6][lid];
  s_SPtrans[7][lid] = c_SPtrans[7][lid];

  s_skb[0][lid] = c_skb[0][lid];
  s_skb[1][lid] = c_skb[1][lid];
  s_skb[2][lid] = c_skb[2][lid];
  s_skb[3][lid] = c_skb[3][lid];
  s_skb[4][lid] = c_skb[4][lid];
  s_skb[5][lid] = c_skb[5][lid];
  s_skb[6][lid] = c_skb[6][lid];
  s_skb[7][lid] = c_skb[7][lid];

  barrier (CLK_LOCAL_MEM_FENCE);

  if (gid >= gid_max) return;

  /**
   * salt
   */

  u32 salt_buf0[4];

  salt_buf0[0] = salt_bufs[salt_pos].salt_buf[0];
  salt_buf0[1] = salt_bufs[salt_pos].salt_buf[1];
  salt_buf0[2] = salt_bufs[salt_pos].salt_buf[2];
  salt_buf0[3] = salt_bufs[salt_pos].salt_buf[3];

  u32 salt_buf1[4];

  salt_buf1[0] = salt_bufs[salt_pos].salt_buf[4];
  salt_buf1[1] = salt_bufs[salt_pos].salt_buf[5];
  salt_buf1[2] = salt_bufs[salt_pos].salt_buf[6];
  salt_buf1[3] = salt_bufs[salt_pos].salt_buf[7];

  const u32 salt_len = salt_bufs[salt_pos].salt_len;

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

  for (u32 il_pos = 0; il_pos < combs_cnt; il_pos++)
  {
    const u32 pw_r_len = combs_buf[il_pos].pw_len;

    const u32 pw_len = pw_l_len + pw_r_len;

    const u32 salt_word_len = (salt_len + pw_len) * 2;

    u32 wordr0[4];

    wordr0[0] = combs_buf[il_pos].i[0];
    wordr0[1] = combs_buf[il_pos].i[1];
    wordr0[2] = combs_buf[il_pos].i[2];
    wordr0[3] = combs_buf[il_pos].i[3];

    u32 wordr1[4];

    wordr1[0] = combs_buf[il_pos].i[4];
    wordr1[1] = combs_buf[il_pos].i[5];
    wordr1[2] = combs_buf[il_pos].i[6];
    wordr1[3] = combs_buf[il_pos].i[7];

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
    w0[2] = wordl0[2] | wordr0[2];
    w0[3] = wordl0[3] | wordr0[3];

    u32x w1[4];

    w1[0] = wordl1[0] | wordr1[0];
    w1[1] = wordl1[1] | wordr1[1];
    w1[2] = wordl1[2] | wordr1[2];
    w1[3] = wordl1[3] | wordr1[3];

    u32x w2[4];

    w2[0] = wordl2[0] | wordr2[0];
    w2[1] = wordl2[1] | wordr2[1];
    w2[2] = wordl2[2] | wordr2[2];
    w2[3] = wordl2[3] | wordr2[3];

    u32x w3[4];

    w3[0] = wordl3[0] | wordr3[0];
    w3[1] = wordl3[1] | wordr3[1];
    w3[2] = wordl3[2] | wordr3[2];
    w3[3] = wordl3[3] | wordr3[3];

    /**
     * prepend salt
     */

    u32x w0_t[4];
    u32x w1_t[4];
    u32x w2_t[4];
    u32x w3_t[4];

    w0_t[0] = w0[0];
    w0_t[1] = w0[1];
    w0_t[2] = w0[2];
    w0_t[3] = w0[3];
    w1_t[0] = w1[0];
    w1_t[1] = w1[1];
    w1_t[2] = w1[2];
    w1_t[3] = w1[3];
    w2_t[0] = w2[0];
    w2_t[1] = w2[1];
    w2_t[2] = w2[2];
    w2_t[3] = w2[3];
    w3_t[0] = w3[0];
    w3_t[1] = w3[1];
    w3_t[2] = w3[2];
    w3_t[3] = w3[3];

    switch_buffer_by_offset (w0_t, w1_t, w2_t, w3_t, salt_len);

    w0_t[0] |= salt_buf0[0];
    w0_t[1] |= salt_buf0[1];
    w0_t[2] |= salt_buf0[2];
    w0_t[3] |= salt_buf0[3];
    w1_t[0] |= salt_buf1[0];
    w1_t[1] |= salt_buf1[1];
    w1_t[2] |= salt_buf1[2];
    w1_t[3] |= salt_buf1[3];

    u32x dst[16];

    dst[ 0] = w0_t[0];
    dst[ 1] = w0_t[1];
    dst[ 2] = w0_t[2];
    dst[ 3] = w0_t[3];
    dst[ 4] = w1_t[0];
    dst[ 5] = w1_t[1];
    dst[ 6] = w1_t[2];
    dst[ 7] = w1_t[3];
    dst[ 8] = w2_t[0];
    dst[ 9] = w2_t[1];
    dst[10] = w2_t[2];
    dst[11] = w2_t[3];
    dst[12] = 0;
    dst[13] = 0;
    dst[14] = 0;
    dst[15] = 0;

    /**
     * precompute key1 since key is static: 0x0123456789abcdef
     * plus LEFT_ROTATE by 2
     */

    u32x Kc[16];

    Kc[ 0] = 0x64649040;
    Kc[ 1] = 0x14909858;
    Kc[ 2] = 0xc4b44888;
    Kc[ 3] = 0x9094e438;
    Kc[ 4] = 0xd8a004f0;
    Kc[ 5] = 0xa8f02810;
    Kc[ 6] = 0xc84048d8;
    Kc[ 7] = 0x68d804a8;
    Kc[ 8] = 0x0490e40c;
    Kc[ 9] = 0xac183024;
    Kc[10] = 0x24c07c10;
    Kc[11] = 0x8c88c038;
    Kc[12] = 0xc048c824;
    Kc[13] = 0x4c0470a8;
    Kc[14] = 0x584020b4;
    Kc[15] = 0x00742c4c;

    u32x Kd[16];

    Kd[ 0] = 0xa42ce40c;
    Kd[ 1] = 0x64689858;
    Kd[ 2] = 0x484050b8;
    Kd[ 3] = 0xe8184814;
    Kd[ 4] = 0x405cc070;
    Kd[ 5] = 0xa010784c;
    Kd[ 6] = 0x6074a800;
    Kd[ 7] = 0x80701c1c;
    Kd[ 8] = 0x9cd49430;
    Kd[ 9] = 0x4c8ce078;
    Kd[10] = 0x5c18c088;
    Kd[11] = 0x28a8a4c8;
    Kd[12] = 0x3c180838;
    Kd[13] = 0xb0b86c20;
    Kd[14] = 0xac84a094;
    Kd[15] = 0x4ce0c0c4;

    /**
     * key1 (generate key)
     */

    u32x iv[2];

    iv[0] = 0;
    iv[1] = 0;

    for (u32 j = 0, k = 0; j < salt_word_len; j += 8, k++)
    {
      u32x data[2];

      data[0] = ((dst[k] << 16) & 0xff000000) | ((dst[k] << 8) & 0x0000ff00);
      data[1] = ((dst[k] >>  0) & 0xff000000) | ((dst[k] >> 8) & 0x0000ff00);

      data[0] ^= iv[0];
      data[1] ^= iv[1];

      _des_crypt_encrypt (iv, data, Kc, Kd, s_SPtrans);
    }

    /**
     * key2 (generate hash)
     */

    _des_crypt_keysetup (iv[0], iv[1], Kc, Kd, s_skb);

    iv[0] = 0;
    iv[1] = 0;

    for (u32 j = 0, k = 0; j < salt_word_len; j += 8, k++)
    {
      u32x data[2];

      data[0] = ((dst[k] << 16) & 0xff000000) | ((dst[k] << 8) & 0x0000ff00);
      data[1] = ((dst[k] >>  0) & 0xff000000) | ((dst[k] >> 8) & 0x0000ff00);

      data[0] ^= iv[0];
      data[1] ^= iv[1];

      _des_crypt_encrypt (iv, data, Kc, Kd, s_SPtrans);
    }

    /**
     * cmp
     */

    const u32x r0 = iv[0];
    const u32x r1 = iv[1];
    const u32x r2 = 0;
    const u32x r3 = 0;

    #include VECT_COMPARE_S
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m03100_s08 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m03100_s16 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}
