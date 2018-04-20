/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#if defined _MD4_ || defined _DCC2_ || defined _NETNTLMV2_ || defined _KRB5PA_ || defined _MS_DRSR_
/**
 * MD4 Functions
 */

#if __CUDA_ARCH__ >= 500
#define MD4_F(x,y,z)    lut3_ca ((x), (y), (z))
#define MD4_G(x,y,z)    lut3_e8 ((x), (y), (z))
#define MD4_H(x,y,z)    lut3_96 ((x), (y), (z))
#define MD4_H1(x,y,z)   lut3_96 ((x), (y), (z))
#define MD4_H2(x,y,z)   lut3_96 ((x), (y), (z))
#else
#define MD4_F(x,y,z)    (((x) & (y)) | ((~(x)) & (z)))
#define MD4_G(x,y,z)    (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define MD4_H(x,y,z)    ((x) ^ (y) ^ (z))
#define MD4_H1(x,y,z)   ((tmp2 = (x) ^ (y)) ^ (z))
#define MD4_H2(x,y,z)   ((x) ^ tmp2)
#endif

#ifdef IS_AMD
#define MD4_Fo(x,y,z)   (bitselect ((z), (y), (x)))
#define MD4_Go(x,y,z)   (bitselect ((x), (y), ((x) ^ (z))))
#else
#define MD4_Fo(x,y,z)   (MD4_F((x), (y), (z)))
#define MD4_Go(x,y,z)   (MD4_G((x), (y), (z)))
#endif

#define MD4_STEP(f,a,b,c,d,x,K,s)   \
{                                   \
  a += K;                           \
  a += x;                           \
  a += f (b, c, d);                 \
  a  = rotl32 (a, s);               \
}

#define MD4_STEP0(f,a,b,c,d,K,s)    \
{                                   \
  a += K;                           \
  a += f (b, c, d);                 \
  a  = rotl32 (a, s);               \
}

#endif

#if defined _MD5_ || defined _MD5H_ || defined _SAPB_ || defined _OLDOFFICE01_ || defined _WPA_ || defined _MD5_SHA1_ || defined _SHA1_MD5_ || defined _NETNTLMV2_ || defined _KRB5PA_ || defined _PBKDF2_MD5_
/**
 * MD5 Functions
 */

#if __CUDA_ARCH__ >= 500
#define MD5_F(x,y,z)    lut3_ca ((x), (y), (z))
#define MD5_G(x,y,z)    lut3_e4 ((x), (y), (z))
#define MD5_H(x,y,z)    lut3_96 ((x), (y), (z))
#define MD5_H1(x,y,z)   lut3_96 ((x), (y), (z))
#define MD5_H2(x,y,z)   lut3_96 ((x), (y), (z))
#else
#define MD5_F(x,y,z)    ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_G(x,y,z)    ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_H(x,y,z)    ((x) ^ (y) ^ (z))
#define MD5_H1(x,y,z)   ((tmp2 = (x) ^ (y)) ^ (z))
#define MD5_H2(x,y,z)   ((x) ^ tmp2)
#endif

#ifdef IS_AMD
#define MD5_I(x,y,z)    (bitselect (0xffffffffU, (x), (z)) ^ (y))
#else
#if __CUDA_ARCH__ >= 500
#define MD5_I(x,y,z)    lut3_39 ((x), (y), (z))
#else
#define MD5_I(x,y,z)    ((y) ^ ((x) | ~(z)))
#endif
#endif

#ifdef IS_AMD
#define MD5_Fo(x,y,z)   (bitselect ((z), (y), (x)))
#define MD5_Go(x,y,z)   (bitselect ((y), (x), (z)))
#else
#define MD5_Fo(x,y,z)   (MD5_F((x), (y), (z)))
#define MD5_Go(x,y,z)   (MD5_G((x), (y), (z)))
#endif

#define MD5_STEP(f,a,b,c,d,x,K,s)   \
{                                   \
  a += K;                           \
  a += x;                           \
  a += f (b, c, d);                 \
  a  = rotl32 (a, s);               \
  a += b;                           \
}

#define MD5_STEP0(f,a,b,c,d,K,s)    \
{                                   \
  a += K;                           \
  a += f (b, c, d);                 \
  a  = rotl32 (a, s);               \
  a += b;                           \
}
#endif

#if defined _SHA1_ || defined _SAPG_ || defined _OFFICE2007_ || defined _OFFICE2010_ || defined _OLDOFFICE34_ || defined _ANDROIDFDE_ || defined _DCC2_ || defined _WPA_ || defined _MD5_SHA1_ || defined _SHA1_MD5_ || defined _PSAFE2_ || defined _LOTUS8_ || defined _PBKDF2_SHA1_ || defined _RAR3_ || defined _SHA256_SHA1_

/**
 * SHA1 Functions
 */

#if __CUDA_ARCH__ >= 500
#define SHA1_F0(x,y,z)  lut3_ca ((x), (y), (z))
#define SHA1_F1(x,y,z)  lut3_96 ((x), (y), (z))
#define SHA1_F2(x,y,z)  lut3_e8 ((x), (y), (z))
#else
#define SHA1_F0(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define SHA1_F1(x,y,z)  ((x) ^ (y) ^ (z))
#define SHA1_F2(x,y,z)  (((x) & (y)) | ((z) & ((x) ^ (y))))
#endif

#ifdef IS_AMD
#define SHA1_F0o(x,y,z) (bitselect ((z), (y), (x)))
#define SHA1_F2o(x,y,z) (bitselect ((x), (y), ((x) ^ (z))))
#else
#define SHA1_F0o(x,y,z) (SHA1_F0 ((x), (y), (z)))
#define SHA1_F2o(x,y,z) (SHA1_F2 ((x), (y), (z)))
#endif

#define SHA1_STEP(f,a,b,c,d,e,x)    \
{                                   \
  e += K;                           \
  e += x;                           \
  e += f (b, c, d);                 \
  e += rotl32 (a,  5u);             \
  b  = rotl32 (b, 30u);             \
}

#define SHA1_STEP0(f,a,b,c,d,e,x)   \
{                                   \
  e += K;                           \
  e += f (b, c, d);                 \
  e += rotl32 (a,  5u);             \
  b  = rotl32 (b, 30u);             \
}

#define SHA1_STEPX(f,a,b,c,d,e,x)   \
{                                   \
  e += x;                           \
  e += f (b, c, d);                 \
  e += rotl32 (a,  5u);             \
  b  = rotl32 (b, 30u);             \
}

#define SHA1_STEP_PE(f,a,b,c,d,e,x) \
{                                   \
  e += x;                           \
  e += f (b, c, d);                 \
  e += rotl32 (a,  5u);             \
}

#define SHA1_STEP_PB(f,a,b,c,d,e,x) \
{                                   \
  e += K;                           \
  b  = rotl32 (b, 30u);             \
}
#endif

#if defined _SHA256_ || defined _PDF17L8_ || defined _SEVEN_ZIP_ || defined _ANDROIDFDE_ || defined _CLOUDKEY_ || defined _SCRYPT_ || defined _PBKDF2_SHA256_  || defined _SHA256_SHA1_ || defined _MS_DRSR_
/**
 * SHA256 Functions
 */

#define SHIFT_RIGHT_32(x,n) ((x) >> (n))

#define SHA256_S0(x) (rotl32 ((x), 25u) ^ rotl32 ((x), 14u) ^ SHIFT_RIGHT_32 ((x),  3u))
#define SHA256_S1(x) (rotl32 ((x), 15u) ^ rotl32 ((x), 13u) ^ SHIFT_RIGHT_32 ((x), 10u))
#define SHA256_S2(x) (rotl32 ((x), 30u) ^ rotl32 ((x), 19u) ^ rotl32 ((x), 10u))
#define SHA256_S3(x) (rotl32 ((x), 26u) ^ rotl32 ((x), 21u) ^ rotl32 ((x),  7u))

#if __CUDA_ARCH__ >= 500
#define SHA256_F0(x,y,z)  lut3_e8 ((x), (y), (z))
#define SHA256_F1(x,y,z)  lut3_ca ((x), (y), (z))
#else
#define SHA256_F0(x,y,z)  (((x) & (y)) | ((z) & ((x) ^ (y))))
#define SHA256_F1(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#endif

#ifdef IS_AMD
#define SHA256_F0o(x,y,z) (bitselect ((x), (y), ((x) ^ (z))))
#define SHA256_F1o(x,y,z) (bitselect ((z), (y), (x)))
#else
#define SHA256_F0o(x,y,z) (SHA256_F0 ((x), (y), (z)))
#define SHA256_F1o(x,y,z) (SHA256_F1 ((x), (y), (z)))
#endif

#define SHA256_STEP(F0,F1,a,b,c,d,e,f,g,h,x,K)  \
{                                               \
  h += K;                                       \
  h += x;                                       \
  h += SHA256_S3 (e);                           \
  h += F1 (e,f,g);                              \
  d += h;                                       \
  h += SHA256_S2 (a);                           \
  h += F0 (a,b,c);                              \
}

#define SHA256_EXPAND(x,y,z,w) (SHA256_S1 (x) + y + SHA256_S0 (z) + w)
#endif


#if defined _SHA384_ || defined _PDF17L8_
/**
 * SHA384 Functions
 */

#define SHIFT_RIGHT_64(x,n) ((x) >> (n))

#define SHA384_S0(x) (rotr64 ((x), 28) ^ rotr64 ((x), 34) ^ rotr64 ((x), 39))
#define SHA384_S1(x) (rotr64 ((x), 14) ^ rotr64 ((x), 18) ^ rotr64 ((x), 41))
#define SHA384_S2(x) (rotr64 ((x),  1) ^ rotr64 ((x),  8) ^ SHIFT_RIGHT_64 ((x), 7))
#define SHA384_S3(x) (rotr64 ((x), 19) ^ rotr64 ((x), 61) ^ SHIFT_RIGHT_64 ((x), 6))

#define SHA384_F0(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))
#define SHA384_F1(x,y,z) (((x) & (y)) | ((z) & ((x) ^ (y))))

#ifdef IS_AMD
#define SHA384_F0o(x,y,z) (bitselect ((z), (y), (x)))
#define SHA384_F1o(x,y,z) (bitselect ((x), (y), ((x) ^ (z))))
#else
#define SHA384_F0o(x,y,z) (SHA384_F0 ((x), (y), (z)))
#define SHA384_F1o(x,y,z) (SHA384_F1 ((x), (y), (z)))
#endif

#define SHA384_STEP(F0,F1,a,b,c,d,e,f,g,h,x,K)  \
{                                               \
  u64x   temp0;                                 \
  temp0  = K;                                   \
  temp0 += x;                                   \
  temp0 += h;                                   \
  temp0 += SHA384_S1 (e);                       \
  temp0 += F0 (e, f, g);                        \
  d     += temp0;                               \
  h      = SHA384_S0 (a);                       \
  h     += F1 (a, b, c);                        \
  h     += temp0;                               \
}

#define SHA384_EXPAND(x,y,z,w) (SHA384_S3 (x) + y + SHA384_S2 (z) + w)
#endif

#if defined _SHA512_ || defined _CLOUDKEY_ || defined _OFFICE2013_ || defined _PDF17L8_ || defined _PBKDF2_SHA512_
/**
 * SHA512 Functions
 */

#define SHIFT_RIGHT_64(x,n) ((x) >> (n))

#define SHA512_S0(x) (rotr64 ((x), 28) ^ rotr64 ((x), 34) ^ rotr64 ((x), 39))
#define SHA512_S1(x) (rotr64 ((x), 14) ^ rotr64 ((x), 18) ^ rotr64 ((x), 41))
#define SHA512_S2(x) (rotr64 ((x),  1) ^ rotr64 ((x),  8) ^ SHIFT_RIGHT_64 ((x), 7))
#define SHA512_S3(x) (rotr64 ((x), 19) ^ rotr64 ((x), 61) ^ SHIFT_RIGHT_64 ((x), 6))

#define SHA512_F0(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))
#define SHA512_F1(x,y,z) (((x) & (y)) | ((z) & ((x) ^ (y))))

#ifdef IS_AMD
#define SHA512_F0o(x,y,z) (bitselect ((z), (y), (x)))
#define SHA512_F1o(x,y,z) (bitselect ((x), (y), ((x) ^ (z))))
#else
#define SHA512_F0o(x,y,z) (SHA512_F0 ((x), (y), (z)))
#define SHA512_F1o(x,y,z) (SHA512_F1 ((x), (y), (z)))
#endif

#define SHA512_STEP(F0,F1,a,b,c,d,e,f,g,h,x,K)  \
{                                               \
  u64x   temp0;                                 \
  temp0  = K;                                   \
  temp0 += x;                                   \
  temp0 += h;                                   \
  temp0 += SHA512_S1 (e);                       \
  temp0 += F0 (e, f, g);                        \
  d     += temp0;                               \
  h      = SHA512_S0 (a);                       \
  h     += F1 (a, b, c);                        \
  h     += temp0;                               \
}

#define SHA512_EXPAND(x,y,z,w) (SHA512_S3 (x) + y + SHA512_S2 (z) + w)

#define SHA512_S2_WO(x) (rotate ((x), 64- 1ull) ^ rotate ((x), 64- 8ull) ^ SHIFT_RIGHT_64 ((x), 7))
#define SHA512_S3_WO(x) (rotate ((x), 64-19ull) ^ rotate ((x), 64-61ull) ^ SHIFT_RIGHT_64 ((x), 6))

#define SHA512_EXPAND_WO(x,y,z,w) (SHA512_S3_WO (x) + y + SHA512_S2_WO (z) + w)
#endif

#ifdef _RIPEMD160_
/**
 * RIPEMD160 Functions
 */

#if __CUDA_ARCH__ >= 500
#define RIPEMD160_F(x,y,z)    lut3_96 ((x), (y), (z))
#define RIPEMD160_G(x,y,z)    lut3_ca ((x), (y), (z))
#define RIPEMD160_H(x,y,z)    lut3_59 ((x), (y), (z))
#define RIPEMD160_I(x,y,z)    lut3_e4 ((x), (y), (z))
#define RIPEMD160_J(x,y,z)    lut3_2d ((x), (y), (z))
#else
#define RIPEMD160_F(x,y,z)    ((x) ^ (y) ^ (z))
#define RIPEMD160_G(x,y,z)    ((z) ^ ((x) & ((y) ^ (z)))) /* x ? y : z */
#define RIPEMD160_H(x,y,z)    (((x) | ~(y)) ^ (z))
#define RIPEMD160_I(x,y,z)    ((y) ^ ((z) & ((x) ^ (y)))) /* z ? x : y */
#define RIPEMD160_J(x,y,z)    ((x) ^ ((y) | ~(z)))
#endif

#ifdef IS_AMD
#define RIPEMD160_Go(x,y,z)   (bitselect ((z), (y), (x)))
#define RIPEMD160_Io(x,y,z)   (bitselect ((y), (x), (z)))
#else
#define RIPEMD160_Go(x,y,z)   (RIPEMD160_G ((x), (y), (z)))
#define RIPEMD160_Io(x,y,z)   (RIPEMD160_I ((x), (y), (z)))
#endif

#define RIPEMD160_STEP(f,a,b,c,d,e,x,K,s) \
{                                         \
  a += K;                                 \
  a += x;                                 \
  a += f (b, c, d);                       \
  a  = rotl32 (a, s);                     \
  a += e;                                 \
  c  = rotl32 (c, 10u);                   \
}

#define ROTATE_LEFT_WORKAROUND_BUG(a,n) ((a << n) | (a >> (32 - n)))

#define RIPEMD160_STEP_WORKAROUND_BUG(f,a,b,c,d,e,x,K,s)  \
{                                         \
  a += K;                                 \
  a += x;                                 \
  a += f (b, c, d);                       \
  a  = ROTATE_LEFT_WORKAROUND_BUG (a, s); \
  a += e;                                 \
  c  = rotl32 (c, 10u);                   \
}

#endif

