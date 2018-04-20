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

__constant u8 ascii_to_ebcdic_pc[256] =
{
  // little hack, can't crack 0-bytes in password, but who cares
  //    0xab, 0xa8, 0xae, 0xad, 0xc4, 0xf1, 0xf7, 0xf4, 0x86, 0xa1, 0xe0, 0xbc, 0xb3, 0xb0, 0xb6, 0xb5,
  0x2a, 0xa8, 0xae, 0xad, 0xc4, 0xf1, 0xf7, 0xf4, 0x86, 0xa1, 0xe0, 0xbc, 0xb3, 0xb0, 0xb6, 0xb5,
  0x8a, 0x89, 0x8f, 0x8c, 0xd3, 0xd0, 0xce, 0xe6, 0x9b, 0x98, 0xd5, 0xe5, 0x92, 0x91, 0x97, 0x94,
  0x2a, 0x34, 0x54, 0x5d, 0x1c, 0x73, 0x0b, 0x51, 0x31, 0x10, 0x13, 0x37, 0x7c, 0x6b, 0x3d, 0x68,
  0x4a, 0x49, 0x4f, 0x4c, 0x43, 0x40, 0x46, 0x45, 0x5b, 0x58, 0x5e, 0x16, 0x32, 0x57, 0x76, 0x75,
  0x52, 0x29, 0x2f, 0x2c, 0x23, 0x20, 0x26, 0x25, 0x3b, 0x38, 0x08, 0x0e, 0x0d, 0x02, 0x01, 0x07,
  0x04, 0x1a, 0x19, 0x6e, 0x6d, 0x62, 0x61, 0x67, 0x64, 0x7a, 0x79, 0x3e, 0x6b, 0x1f, 0x15, 0x70,
  0x58, 0xa8, 0xae, 0xad, 0xa2, 0xa1, 0xa7, 0xa4, 0xba, 0xb9, 0x89, 0x8f, 0x8c, 0x83, 0x80, 0x86,
  0x85, 0x9b, 0x98, 0xef, 0xec, 0xe3, 0xe0, 0xe6, 0xe5, 0xfb, 0xf8, 0x2a, 0x7f, 0x0b, 0xe9, 0xa4,
  0xea, 0xe9, 0xef, 0xec, 0xe3, 0x80, 0xa7, 0x85, 0xfb, 0xf8, 0xfe, 0xfd, 0xf2, 0xb9, 0xbf, 0x9d,
  0xcb, 0xc8, 0x9e, 0xcd, 0xc2, 0xc1, 0xc7, 0xba, 0xda, 0xd9, 0xdf, 0xdc, 0xa2, 0x83, 0xd6, 0x68,
  0x29, 0x2f, 0x2c, 0x23, 0x20, 0x26, 0x25, 0x3b, 0x38, 0x08, 0x0e, 0x0d, 0x02, 0x01, 0x07, 0x04,
  0x1a, 0x19, 0x6e, 0x6d, 0x62, 0x61, 0x67, 0x64, 0x7a, 0x79, 0x4a, 0x49, 0x4f, 0x4c, 0x43, 0x40,
  0x46, 0x45, 0x5b, 0xab, 0xbf, 0xbc, 0xb3, 0xb0, 0xb6, 0xb5, 0x8a, 0x9e, 0x9d, 0x92, 0x91, 0x97,
  0x94, 0xea, 0xfe, 0xfd, 0xf2, 0xf1, 0xf7, 0xf4, 0xcb, 0xc8, 0xce, 0xcd, 0xc2, 0xc1, 0xc7, 0xc4,
  0xda, 0xd9, 0xdf, 0xdc, 0xd3, 0xd0, 0xd6, 0xd5, 0x3e, 0x3d, 0x32, 0x31, 0x37, 0x34, 0x1f, 0x1c,
  0x13, 0x10, 0x16, 0x15, 0x7f, 0x7c, 0x73, 0x70, 0x76, 0x75, 0x5e, 0x5d, 0x52, 0x51, 0x57, 0x54,
};

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
  0x00002822, 0x04002822, 0x00042822, 0x04042822,
};

#define NBOX(i,n,S) (S)[(n)][(i)]

static void _des_crypt_encrypt (u32x iv[2], u32x data[2], u32x Kc[16], u32x Kd[16], __local u32 s_SPtrans[8][64])
{
  u32x tt;

  u32x r = data[0];
  u32x l = data[1];

  #pragma unroll 16
  for (u32 i = 0; i < 16; i++)
  {
    u32x u = Kc[i] ^ r;
    u32x t = Kd[i] ^ rotl32 (r, 28u);

    #ifdef VECT_SIZE1
    l    ^= NBOX (((u    >>  2) & 0x3f), 0, s_SPtrans)
          | NBOX (((u    >> 10) & 0x3f), 2, s_SPtrans)
          | NBOX (((u    >> 18) & 0x3f), 4, s_SPtrans)
          | NBOX (((u    >> 26) & 0x3f), 6, s_SPtrans)
          | NBOX (((t    >>  2) & 0x3f), 1, s_SPtrans)
          | NBOX (((t    >> 10) & 0x3f), 3, s_SPtrans)
          | NBOX (((t    >> 18) & 0x3f), 5, s_SPtrans)
          | NBOX (((t    >> 26) & 0x3f), 7, s_SPtrans);
    #endif

    #ifdef VECT_SIZE2
    l.s0 ^= NBOX (((u.s0 >>  2) & 0x3f), 0, s_SPtrans)
          | NBOX (((u.s0 >> 10) & 0x3f), 2, s_SPtrans)
          | NBOX (((u.s0 >> 18) & 0x3f), 4, s_SPtrans)
          | NBOX (((u.s0 >> 26) & 0x3f), 6, s_SPtrans)
          | NBOX (((t.s0 >>  2) & 0x3f), 1, s_SPtrans)
          | NBOX (((t.s0 >> 10) & 0x3f), 3, s_SPtrans)
          | NBOX (((t.s0 >> 18) & 0x3f), 5, s_SPtrans)
          | NBOX (((t.s0 >> 26) & 0x3f), 7, s_SPtrans);

    l.s1 ^= NBOX (((u.s1 >>  2) & 0x3f), 0, s_SPtrans)
          | NBOX (((u.s1 >> 10) & 0x3f), 2, s_SPtrans)
          | NBOX (((u.s1 >> 18) & 0x3f), 4, s_SPtrans)
          | NBOX (((u.s1 >> 26) & 0x3f), 6, s_SPtrans)
          | NBOX (((t.s1 >>  2) & 0x3f), 1, s_SPtrans)
          | NBOX (((t.s1 >> 10) & 0x3f), 3, s_SPtrans)
          | NBOX (((t.s1 >> 18) & 0x3f), 5, s_SPtrans)
          | NBOX (((t.s1 >> 26) & 0x3f), 7, s_SPtrans);
    #endif

    #ifdef VECT_SIZE4
    l.s0 ^= NBOX (((u.s0 >>  2) & 0x3f), 0, s_SPtrans)
          | NBOX (((u.s0 >> 10) & 0x3f), 2, s_SPtrans)
          | NBOX (((u.s0 >> 18) & 0x3f), 4, s_SPtrans)
          | NBOX (((u.s0 >> 26) & 0x3f), 6, s_SPtrans)
          | NBOX (((t.s0 >>  2) & 0x3f), 1, s_SPtrans)
          | NBOX (((t.s0 >> 10) & 0x3f), 3, s_SPtrans)
          | NBOX (((t.s0 >> 18) & 0x3f), 5, s_SPtrans)
          | NBOX (((t.s0 >> 26) & 0x3f), 7, s_SPtrans);

    l.s1 ^= NBOX (((u.s1 >>  2) & 0x3f), 0, s_SPtrans)
          | NBOX (((u.s1 >> 10) & 0x3f), 2, s_SPtrans)
          | NBOX (((u.s1 >> 18) & 0x3f), 4, s_SPtrans)
          | NBOX (((u.s1 >> 26) & 0x3f), 6, s_SPtrans)
          | NBOX (((t.s1 >>  2) & 0x3f), 1, s_SPtrans)
          | NBOX (((t.s1 >> 10) & 0x3f), 3, s_SPtrans)
          | NBOX (((t.s1 >> 18) & 0x3f), 5, s_SPtrans)
          | NBOX (((t.s1 >> 26) & 0x3f), 7, s_SPtrans);

    l.s2 ^= NBOX (((u.s2 >>  2) & 0x3f), 0, s_SPtrans)
          | NBOX (((u.s2 >> 10) & 0x3f), 2, s_SPtrans)
          | NBOX (((u.s2 >> 18) & 0x3f), 4, s_SPtrans)
          | NBOX (((u.s2 >> 26) & 0x3f), 6, s_SPtrans)
          | NBOX (((t.s2 >>  2) & 0x3f), 1, s_SPtrans)
          | NBOX (((t.s2 >> 10) & 0x3f), 3, s_SPtrans)
          | NBOX (((t.s2 >> 18) & 0x3f), 5, s_SPtrans)
          | NBOX (((t.s2 >> 26) & 0x3f), 7, s_SPtrans);

    l.s3 ^= NBOX (((u.s3 >>  2) & 0x3f), 0, s_SPtrans)
          | NBOX (((u.s3 >> 10) & 0x3f), 2, s_SPtrans)
          | NBOX (((u.s3 >> 18) & 0x3f), 4, s_SPtrans)
          | NBOX (((u.s3 >> 26) & 0x3f), 6, s_SPtrans)
          | NBOX (((t.s3 >>  2) & 0x3f), 1, s_SPtrans)
          | NBOX (((t.s3 >> 10) & 0x3f), 3, s_SPtrans)
          | NBOX (((t.s3 >> 18) & 0x3f), 5, s_SPtrans)
          | NBOX (((t.s3 >> 26) & 0x3f), 7, s_SPtrans);
    #endif

    tt = l;
    l  = r;
    r  = tt;
  }

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
    const u32 shifts3s0[16] = {  1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1 };
    const u32 shifts3s1[16] = { 27, 27, 26, 26, 26, 26, 26, 26, 27, 26, 26, 26, 26, 26, 26, 27 };

    c = c >> shifts3s0[i] | c << shifts3s1[i];
    d = d >> shifts3s0[i] | d << shifts3s1[i];

    c = c & 0x0fffffff;
    d = d & 0x0fffffff;

    u32x s;
    u32x t;

    #ifdef VECT_SIZE1
    s     = NBOX ((( c    >>  0) & 0x3f),  0, s_skb)
          | NBOX ((((c    >>  6) & 0x03)
                 | ((c    >>  7) & 0x3c)), 1, s_skb)
          | NBOX ((((c    >> 13) & 0x0f)
                 | ((c    >> 14) & 0x30)), 2, s_skb)
          | NBOX ((((c    >> 20) & 0x01)
                 | ((c    >> 21) & 0x06)
                 | ((c    >> 22) & 0x38)), 3, s_skb);

    t     = NBOX ((( d    >>  0) & 0x3f),  4, s_skb)
          | NBOX ((((d    >>  7) & 0x03)
                 | ((d    >>  8) & 0x3c)), 5, s_skb)
          | NBOX ((((d    >> 15) & 0x3f)), 6, s_skb)
          | NBOX ((((d    >> 21) & 0x0f)
                 | ((d    >> 22) & 0x30)), 7, s_skb);
    #endif

    #ifdef VECT_SIZE2
    s.s0  = NBOX ((( c.s0 >>  0) & 0x3f),  0, s_skb)
          | NBOX ((((c.s0 >>  6) & 0x03)
                 | ((c.s0 >>  7) & 0x3c)), 1, s_skb)
          | NBOX ((((c.s0 >> 13) & 0x0f)
                 | ((c.s0 >> 14) & 0x30)), 2, s_skb)
          | NBOX ((((c.s0 >> 20) & 0x01)
                 | ((c.s0 >> 21) & 0x06)
                 | ((c.s0 >> 22) & 0x38)), 3, s_skb);

    t.s0  = NBOX ((( d.s0 >>  0) & 0x3f),  4, s_skb)
          | NBOX ((((d.s0 >>  7) & 0x03)
                 | ((d.s0 >>  8) & 0x3c)), 5, s_skb)
          | NBOX ((((d.s0 >> 15) & 0x3f)), 6, s_skb)
          | NBOX ((((d.s0 >> 21) & 0x0f)
                 | ((d.s0 >> 22) & 0x30)), 7, s_skb);

    s.s1  = NBOX ((( c.s1 >>  0) & 0x3f),  0, s_skb)
          | NBOX ((((c.s1 >>  6) & 0x03)
                 | ((c.s1 >>  7) & 0x3c)), 1, s_skb)
          | NBOX ((((c.s1 >> 13) & 0x0f)
                 | ((c.s1 >> 14) & 0x30)), 2, s_skb)
          | NBOX ((((c.s1 >> 20) & 0x01)
                 | ((c.s1 >> 21) & 0x06)
                 | ((c.s1 >> 22) & 0x38)), 3, s_skb);

    t.s1  = NBOX ((( d.s1 >>  0) & 0x3f),  4, s_skb)
          | NBOX ((((d.s1 >>  7) & 0x03)
                 | ((d.s1 >>  8) & 0x3c)), 5, s_skb)
          | NBOX ((((d.s1 >> 15) & 0x3f)), 6, s_skb)
          | NBOX ((((d.s1 >> 21) & 0x0f)
                 | ((d.s1 >> 22) & 0x30)), 7, s_skb);
    #endif

    #ifdef VECT_SIZE4
    s.s0  = NBOX ((( c.s0 >>  0) & 0x3f),  0, s_skb)
          | NBOX ((((c.s0 >>  6) & 0x03)
                 | ((c.s0 >>  7) & 0x3c)), 1, s_skb)
          | NBOX ((((c.s0 >> 13) & 0x0f)
                 | ((c.s0 >> 14) & 0x30)), 2, s_skb)
          | NBOX ((((c.s0 >> 20) & 0x01)
                 | ((c.s0 >> 21) & 0x06)
                 | ((c.s0 >> 22) & 0x38)), 3, s_skb);

    t.s0  = NBOX ((( d.s0 >>  0) & 0x3f),  4, s_skb)
          | NBOX ((((d.s0 >>  7) & 0x03)
                 | ((d.s0 >>  8) & 0x3c)), 5, s_skb)
          | NBOX ((((d.s0 >> 15) & 0x3f)), 6, s_skb)
          | NBOX ((((d.s0 >> 21) & 0x0f)
                 | ((d.s0 >> 22) & 0x30)), 7, s_skb);

    s.s1  = NBOX ((( c.s1 >>  0) & 0x3f),  0, s_skb)
          | NBOX ((((c.s1 >>  6) & 0x03)
                 | ((c.s1 >>  7) & 0x3c)), 1, s_skb)
          | NBOX ((((c.s1 >> 13) & 0x0f)
                 | ((c.s1 >> 14) & 0x30)), 2, s_skb)
          | NBOX ((((c.s1 >> 20) & 0x01)
                 | ((c.s1 >> 21) & 0x06)
                 | ((c.s1 >> 22) & 0x38)), 3, s_skb);

    t.s1  = NBOX ((( d.s1 >>  0) & 0x3f),  4, s_skb)
          | NBOX ((((d.s1 >>  7) & 0x03)
                 | ((d.s1 >>  8) & 0x3c)), 5, s_skb)
          | NBOX ((((d.s1 >> 15) & 0x3f)), 6, s_skb)
          | NBOX ((((d.s1 >> 21) & 0x0f)
                 | ((d.s1 >> 22) & 0x30)), 7, s_skb);

    s.s2  = NBOX ((( c.s2 >>  0) & 0x3f),  0, s_skb)
          | NBOX ((((c.s2 >>  6) & 0x03)
                 | ((c.s2 >>  7) & 0x3c)), 1, s_skb)
          | NBOX ((((c.s2 >> 13) & 0x0f)
                 | ((c.s2 >> 14) & 0x30)), 2, s_skb)
          | NBOX ((((c.s2 >> 20) & 0x01)
                 | ((c.s2 >> 21) & 0x06)
                 | ((c.s2 >> 22) & 0x38)), 3, s_skb);

    t.s2  = NBOX ((( d.s2 >>  0) & 0x3f),  4, s_skb)
          | NBOX ((((d.s2 >>  7) & 0x03)
                 | ((d.s2 >>  8) & 0x3c)), 5, s_skb)
          | NBOX ((((d.s2 >> 15) & 0x3f)), 6, s_skb)
          | NBOX ((((d.s2 >> 21) & 0x0f)
                 | ((d.s2 >> 22) & 0x30)), 7, s_skb);

    s.s3  = NBOX ((( c.s3 >>  0) & 0x3f),  0, s_skb)
          | NBOX ((((c.s3 >>  6) & 0x03)
                 | ((c.s3 >>  7) & 0x3c)), 1, s_skb)
          | NBOX ((((c.s3 >> 13) & 0x0f)
                 | ((c.s3 >> 14) & 0x30)), 2, s_skb)
          | NBOX ((((c.s3 >> 20) & 0x01)
                 | ((c.s3 >> 21) & 0x06)
                 | ((c.s3 >> 22) & 0x38)), 3, s_skb);

    t.s3  = NBOX ((( d.s3 >>  0) & 0x3f),  4, s_skb)
          | NBOX ((((d.s3 >>  7) & 0x03)
                 | ((d.s3 >>  8) & 0x3c)), 5, s_skb)
          | NBOX ((((d.s3 >> 15) & 0x3f)), 6, s_skb)
          | NBOX ((((d.s3 >> 21) & 0x0f)
                 | ((d.s3 >> 22) & 0x30)), 7, s_skb);
    #endif

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

static void transform_racf_key (const u32x w0, const u32x w1, u32x key[2])
{
  #ifdef VECT_SIZE1

  key[0]    = (ascii_to_ebcdic_pc[(w0    >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w0    >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w0    >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w0    >> 24) & 0xff]) << 24;

  key[1]    = (ascii_to_ebcdic_pc[(w1    >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w1    >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w1    >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w1    >> 24) & 0xff]) << 24;
  #endif

  #ifdef VECT_SIZE2

  key[0].s0 = (ascii_to_ebcdic_pc[(w0.s0 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w0.s0 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w0.s0 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w0.s0 >> 24) & 0xff]) << 24;

  key[0].s1 = (ascii_to_ebcdic_pc[(w0.s1 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w0.s1 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w0.s1 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w0.s1 >> 24) & 0xff]) << 24;

  key[1].s0 = (ascii_to_ebcdic_pc[(w1.s0 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w1.s0 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w1.s0 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w1.s0 >> 24) & 0xff]) << 24;

  key[1].s1 = (ascii_to_ebcdic_pc[(w1.s1 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w1.s1 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w1.s1 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w1.s1 >> 24) & 0xff]) << 24;
  #endif

  #ifdef VECT_SIZE4
  key[0].s0 = (ascii_to_ebcdic_pc[(w0.s0 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w0.s0 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w0.s0 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w0.s0 >> 24) & 0xff]) << 24;

  key[0].s1 = (ascii_to_ebcdic_pc[(w0.s1 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w0.s1 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w0.s1 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w0.s1 >> 24) & 0xff]) << 24;

  key[0].s2 = (ascii_to_ebcdic_pc[(w0.s2 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w0.s2 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w0.s2 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w0.s2 >> 24) & 0xff]) << 24;

  key[0].s3 = (ascii_to_ebcdic_pc[(w0.s3 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w0.s3 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w0.s3 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w0.s3 >> 24) & 0xff]) << 24;

  key[1].s0 = (ascii_to_ebcdic_pc[(w1.s0 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w1.s0 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w1.s0 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w1.s0 >> 24) & 0xff]) << 24;

  key[1].s1 = (ascii_to_ebcdic_pc[(w1.s1 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w1.s1 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w1.s1 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w1.s1 >> 24) & 0xff]) << 24;

  key[1].s2 = (ascii_to_ebcdic_pc[(w1.s2 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w1.s2 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w1.s2 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w1.s2 >> 24) & 0xff]) << 24;

  key[1].s3 = (ascii_to_ebcdic_pc[(w1.s3 >>  0) & 0xff]) <<  0
            | (ascii_to_ebcdic_pc[(w1.s3 >>  8) & 0xff]) <<  8
            | (ascii_to_ebcdic_pc[(w1.s3 >> 16) & 0xff]) << 16
            | (ascii_to_ebcdic_pc[(w1.s3 >> 24) & 0xff]) << 24;
  #endif
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08500_m04 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
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
    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, combs_buf[0].pw_len);
  }

  /**
   * salt
   */

  u32 salt_buf0[2];

  salt_buf0[0] = salt_bufs[salt_pos].salt_buf_pc[0];
  salt_buf0[1] = salt_bufs[salt_pos].salt_buf_pc[1];

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
   * main
   */

  for (u32 il_pos = 0; il_pos < combs_cnt; il_pos++)
  {
    const u32 pw_r_len = combs_buf[il_pos].pw_len;

    u32 pw_len = pw_l_len + pw_r_len;

    pw_len = (pw_len >= 8) ? 8 : pw_len;

    u32 wordr0[4];

    wordr0[0] = combs_buf[il_pos].i[0];
    wordr0[1] = combs_buf[il_pos].i[1];
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

    u32x key[2];

    transform_racf_key (w0[0], w0[1], key);

    const u32x c = key[0];
    const u32x d = key[1];

    u32x Kc[16];
    u32x Kd[16];

    _des_crypt_keysetup (c, d, Kc, Kd, s_skb);

    u32x data[2];

    data[0] = salt_buf0[0];
    data[1] = salt_buf0[1];

    u32x iv[2];

    _des_crypt_encrypt (iv, data, Kc, Kd, s_SPtrans);

    const u32x r0 = iv[0];
    const u32x r1 = iv[1];
    const u32x r2 = 0;
    const u32x r3 = 0;

    #include VECT_COMPARE_M
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08500_m08 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08500_m16 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08500_s04 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
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
    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, combs_buf[0].pw_len);
  }

  /**
   * salt
   */

  u32 salt_buf0[2];

  salt_buf0[0] = salt_bufs[salt_pos].salt_buf_pc[0];
  salt_buf0[1] = salt_bufs[salt_pos].salt_buf_pc[1];

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
    const u32 pw_r_len = combs_buf[il_pos].pw_len;

    u32 pw_len = pw_l_len + pw_r_len;

    pw_len = (pw_len >= 8) ? 8 : pw_len;

    u32 wordr0[4];

    wordr0[0] = combs_buf[il_pos].i[0];
    wordr0[1] = combs_buf[il_pos].i[1];
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

    u32x key[2];

    transform_racf_key (w0[0], w0[1], key);

    const u32x c = key[0];
    const u32x d = key[1];

    u32x Kc[16];
    u32x Kd[16];

    _des_crypt_keysetup (c, d, Kc, Kd, s_skb);

    u32x data[2];

    data[0] = salt_buf0[0];
    data[1] = salt_buf0[1];

    u32x iv[2];

    _des_crypt_encrypt (iv, data, Kc, Kd, s_SPtrans);

    const u32x r0 = iv[0];
    const u32x r1 = iv[1];
    const u32x r2 = 0;
    const u32x r3 = 0;

    #include VECT_COMPARE_S
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08500_s08 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08500_s16 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}
