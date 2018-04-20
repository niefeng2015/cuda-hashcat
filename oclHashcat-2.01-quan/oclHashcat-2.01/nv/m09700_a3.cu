/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define _OLDOFFICE01_

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

#ifdef  VECT_SIZE4
#define VECT_COMPARE_S "check_single_vect4_comp4.c"
#define VECT_COMPARE_M "check_multi_vect4_comp4.c"
#endif

typedef struct
{
  u8 S[256];

  u32 wtf_its_faster;

} RC4_KEY;

__device__ static void swap (RC4_KEY *rc4_key, const u8 i, const u8 j)
{
  u8 tmp;

  tmp           = rc4_key->S[i];
  rc4_key->S[i] = rc4_key->S[j];
  rc4_key->S[j] = tmp;
}

__device__ static void rc4_init_16 (RC4_KEY *rc4_key, const u32 data[4])
{
  u32 v = 0x03020100;
  u32 a = 0x04040404;

  u32 *ptr = (u32 *) rc4_key->S;

  #pragma unroll 64
  for (u32 i = 0; i < 64; i++)
  {
    *ptr++ = v; v += a;
  }

  u32 j = 0;

  for (u32 i = 0; i < 16; i++)
  {
    u32 idx = i * 16;

    u32 v;

    v = data[0];

    j += rc4_key->S[idx] + (v >>  0); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >>  8); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >> 16); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >> 24); swap (rc4_key, idx, j); idx++;

    v = data[1];

    j += rc4_key->S[idx] + (v >>  0); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >>  8); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >> 16); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >> 24); swap (rc4_key, idx, j); idx++;

    v = data[2];

    j += rc4_key->S[idx] + (v >>  0); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >>  8); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >> 16); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >> 24); swap (rc4_key, idx, j); idx++;

    v = data[3];

    j += rc4_key->S[idx] + (v >>  0); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >>  8); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >> 16); swap (rc4_key, idx, j); idx++;
    j += rc4_key->S[idx] + (v >> 24); swap (rc4_key, idx, j); idx++;
  }
}

__device__ static u8 rc4_next_16 (RC4_KEY *rc4_key, u8 i, u8 j, const u32 in[4], u32 out[4])
{
  for (u32 k = 0; k < 4; k++)
  {
    u32 xor4 = 0;

    u8 idx;

    i += 1;
    j += rc4_key->S[i];

    swap (rc4_key, i, j);

    idx = rc4_key->S[i] + rc4_key->S[j];

    xor4 |= rc4_key->S[idx] <<  0;

    i += 1;
    j += rc4_key->S[i];

    swap (rc4_key, i, j);

    idx = rc4_key->S[i] + rc4_key->S[j];

    xor4 |= rc4_key->S[idx] <<  8;

    i += 1;
    j += rc4_key->S[i];

    swap (rc4_key, i, j);

    idx = rc4_key->S[i] + rc4_key->S[j];

    xor4 |= rc4_key->S[idx] << 16;

    i += 1;
    j += rc4_key->S[i];

    swap (rc4_key, i, j);

    idx = rc4_key->S[i] + rc4_key->S[j];

    xor4 |= rc4_key->S[idx] << 24;

    out[k] = in[k] ^ xor4;
  }

  return j;
}

__device__ static void md5_transform (const u32x w0[4], const u32x w1[4], const u32x w2[4], const u32x w3[4], u32x digest[4])
{
  u32x a = digest[0];
  u32x b = digest[1];
  u32x c = digest[2];
  u32x d = digest[3];

  u32x w0_t = w0[0];
  u32x w1_t = w0[1];
  u32x w2_t = w0[2];
  u32x w3_t = w0[3];
  u32x w4_t = w1[0];
  u32x w5_t = w1[1];
  u32x w6_t = w1[2];
  u32x w7_t = w1[3];
  u32x w8_t = w2[0];
  u32x w9_t = w2[1];
  u32x wa_t = w2[2];
  u32x wb_t = w2[3];
  u32x wc_t = w3[0];
  u32x wd_t = w3[1];
  u32x we_t = w3[2];
  u32x wf_t = w3[3];

  MD5_STEP (MD5_Fo, a, b, c, d, w0_t, MD5C00, MD5S00);
  MD5_STEP (MD5_Fo, d, a, b, c, w1_t, MD5C01, MD5S01);
  MD5_STEP (MD5_Fo, c, d, a, b, w2_t, MD5C02, MD5S02);
  MD5_STEP (MD5_Fo, b, c, d, a, w3_t, MD5C03, MD5S03);
  MD5_STEP (MD5_Fo, a, b, c, d, w4_t, MD5C04, MD5S00);
  MD5_STEP (MD5_Fo, d, a, b, c, w5_t, MD5C05, MD5S01);
  MD5_STEP (MD5_Fo, c, d, a, b, w6_t, MD5C06, MD5S02);
  MD5_STEP (MD5_Fo, b, c, d, a, w7_t, MD5C07, MD5S03);
  MD5_STEP (MD5_Fo, a, b, c, d, w8_t, MD5C08, MD5S00);
  MD5_STEP (MD5_Fo, d, a, b, c, w9_t, MD5C09, MD5S01);
  MD5_STEP (MD5_Fo, c, d, a, b, wa_t, MD5C0a, MD5S02);
  MD5_STEP (MD5_Fo, b, c, d, a, wb_t, MD5C0b, MD5S03);
  MD5_STEP (MD5_Fo, a, b, c, d, wc_t, MD5C0c, MD5S00);
  MD5_STEP (MD5_Fo, d, a, b, c, wd_t, MD5C0d, MD5S01);
  MD5_STEP (MD5_Fo, c, d, a, b, we_t, MD5C0e, MD5S02);
  MD5_STEP (MD5_Fo, b, c, d, a, wf_t, MD5C0f, MD5S03);

  MD5_STEP (MD5_Go, a, b, c, d, w1_t, MD5C10, MD5S10);
  MD5_STEP (MD5_Go, d, a, b, c, w6_t, MD5C11, MD5S11);
  MD5_STEP (MD5_Go, c, d, a, b, wb_t, MD5C12, MD5S12);
  MD5_STEP (MD5_Go, b, c, d, a, w0_t, MD5C13, MD5S13);
  MD5_STEP (MD5_Go, a, b, c, d, w5_t, MD5C14, MD5S10);
  MD5_STEP (MD5_Go, d, a, b, c, wa_t, MD5C15, MD5S11);
  MD5_STEP (MD5_Go, c, d, a, b, wf_t, MD5C16, MD5S12);
  MD5_STEP (MD5_Go, b, c, d, a, w4_t, MD5C17, MD5S13);
  MD5_STEP (MD5_Go, a, b, c, d, w9_t, MD5C18, MD5S10);
  MD5_STEP (MD5_Go, d, a, b, c, we_t, MD5C19, MD5S11);
  MD5_STEP (MD5_Go, c, d, a, b, w3_t, MD5C1a, MD5S12);
  MD5_STEP (MD5_Go, b, c, d, a, w8_t, MD5C1b, MD5S13);
  MD5_STEP (MD5_Go, a, b, c, d, wd_t, MD5C1c, MD5S10);
  MD5_STEP (MD5_Go, d, a, b, c, w2_t, MD5C1d, MD5S11);
  MD5_STEP (MD5_Go, c, d, a, b, w7_t, MD5C1e, MD5S12);
  MD5_STEP (MD5_Go, b, c, d, a, wc_t, MD5C1f, MD5S13);

  MD5_STEP (MD5_H , a, b, c, d, w5_t, MD5C20, MD5S20);
  MD5_STEP (MD5_H , d, a, b, c, w8_t, MD5C21, MD5S21);
  MD5_STEP (MD5_H , c, d, a, b, wb_t, MD5C22, MD5S22);
  MD5_STEP (MD5_H , b, c, d, a, we_t, MD5C23, MD5S23);
  MD5_STEP (MD5_H , a, b, c, d, w1_t, MD5C24, MD5S20);
  MD5_STEP (MD5_H , d, a, b, c, w4_t, MD5C25, MD5S21);
  MD5_STEP (MD5_H , c, d, a, b, w7_t, MD5C26, MD5S22);
  MD5_STEP (MD5_H , b, c, d, a, wa_t, MD5C27, MD5S23);
  MD5_STEP (MD5_H , a, b, c, d, wd_t, MD5C28, MD5S20);
  MD5_STEP (MD5_H , d, a, b, c, w0_t, MD5C29, MD5S21);
  MD5_STEP (MD5_H , c, d, a, b, w3_t, MD5C2a, MD5S22);
  MD5_STEP (MD5_H , b, c, d, a, w6_t, MD5C2b, MD5S23);
  MD5_STEP (MD5_H , a, b, c, d, w9_t, MD5C2c, MD5S20);
  MD5_STEP (MD5_H , d, a, b, c, wc_t, MD5C2d, MD5S21);
  MD5_STEP (MD5_H , c, d, a, b, wf_t, MD5C2e, MD5S22);
  MD5_STEP (MD5_H , b, c, d, a, w2_t, MD5C2f, MD5S23);

  MD5_STEP (MD5_I , a, b, c, d, w0_t, MD5C30, MD5S30);
  MD5_STEP (MD5_I , d, a, b, c, w7_t, MD5C31, MD5S31);
  MD5_STEP (MD5_I , c, d, a, b, we_t, MD5C32, MD5S32);
  MD5_STEP (MD5_I , b, c, d, a, w5_t, MD5C33, MD5S33);
  MD5_STEP (MD5_I , a, b, c, d, wc_t, MD5C34, MD5S30);
  MD5_STEP (MD5_I , d, a, b, c, w3_t, MD5C35, MD5S31);
  MD5_STEP (MD5_I , c, d, a, b, wa_t, MD5C36, MD5S32);
  MD5_STEP (MD5_I , b, c, d, a, w1_t, MD5C37, MD5S33);
  MD5_STEP (MD5_I , a, b, c, d, w8_t, MD5C38, MD5S30);
  MD5_STEP (MD5_I , d, a, b, c, wf_t, MD5C39, MD5S31);
  MD5_STEP (MD5_I , c, d, a, b, w6_t, MD5C3a, MD5S32);
  MD5_STEP (MD5_I , b, c, d, a, wd_t, MD5C3b, MD5S33);
  MD5_STEP (MD5_I , a, b, c, d, w4_t, MD5C3c, MD5S30);
  MD5_STEP (MD5_I , d, a, b, c, wb_t, MD5C3d, MD5S31);
  MD5_STEP (MD5_I , c, d, a, b, w2_t, MD5C3e, MD5S32);
  MD5_STEP (MD5_I , b, c, d, a, w9_t, MD5C3f, MD5S33);

  digest[0] += a;
  digest[1] += b;
  digest[2] += c;
  digest[3] += d;
}

__device__ __constant__ bf_t c_bfs[1024];

__device__ static void m09700m (RC4_KEY rc4_keys[64], u32x w0[4], u32x w1[4], u32x w2[4], u32x w3[4], const u32 pw_len, const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *bfs_buf, void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const oldoffice01_t *oldoffice01_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset)
{
  /**
   * modifier
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;
  const u32 lid = threadIdx.x;

  RC4_KEY *rc4_key = &rc4_keys[lid];

  /**
   * salt
   */

  u32 salt_buf_t0[4];
  u32 salt_buf_t1[5];
  u32 salt_buf_t2[5];
  u32 salt_buf_t3[5];

  salt_buf_t0[0] = salt_bufs[salt_pos].salt_buf[0];
  salt_buf_t0[1] = salt_bufs[salt_pos].salt_buf[1];
  salt_buf_t0[2] = salt_bufs[salt_pos].salt_buf[2];
  salt_buf_t0[3] = salt_bufs[salt_pos].salt_buf[3];

  salt_buf_t1[0] =                        salt_buf_t0[0] <<  8;
  salt_buf_t1[1] = salt_buf_t0[0] >> 24 | salt_buf_t0[1] <<  8;
  salt_buf_t1[2] = salt_buf_t0[1] >> 24 | salt_buf_t0[2] <<  8;
  salt_buf_t1[3] = salt_buf_t0[2] >> 24 | salt_buf_t0[3] <<  8;
  salt_buf_t1[4] = salt_buf_t0[3] >> 24;

  salt_buf_t2[0] =                        salt_buf_t0[0] << 16;
  salt_buf_t2[1] = salt_buf_t0[0] >> 16 | salt_buf_t0[1] << 16;
  salt_buf_t2[2] = salt_buf_t0[1] >> 16 | salt_buf_t0[2] << 16;
  salt_buf_t2[3] = salt_buf_t0[2] >> 16 | salt_buf_t0[3] << 16;
  salt_buf_t2[4] = salt_buf_t0[3] >> 16;

  salt_buf_t3[0] =                        salt_buf_t0[0] << 24;
  salt_buf_t3[1] = salt_buf_t0[0] >>  8 | salt_buf_t0[1] << 24;
  salt_buf_t3[2] = salt_buf_t0[1] >>  8 | salt_buf_t0[2] << 24;
  salt_buf_t3[3] = salt_buf_t0[2] >>  8 | salt_buf_t0[3] << 24;
  salt_buf_t3[4] = salt_buf_t0[3] >>  8;

  const u32 salt_len = 16;

  /**
   * esalt
   */

  const u32 version = oldoffice01_bufs[salt_pos].version;

  u32 encryptedVerifier[4];

  encryptedVerifier[0] = oldoffice01_bufs[salt_pos].encryptedVerifier[0];
  encryptedVerifier[1] = oldoffice01_bufs[salt_pos].encryptedVerifier[1];
  encryptedVerifier[2] = oldoffice01_bufs[salt_pos].encryptedVerifier[2];
  encryptedVerifier[3] = oldoffice01_bufs[salt_pos].encryptedVerifier[3];

  /**
   * loop
   */

  u32x w0l = w0[0];

  for (u32 il_pos = 0; il_pos < bfs_cnt; il_pos++)
  {
    const u32 w0r = c_bfs[il_pos].i;

    w0[0] = w0l | w0r;

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
    w3_t[2] = pw_len * 8;
    w3_t[3] = 0;

    u32x digest_t0[4];
    u32x digest_t1[2]; // need only first 5 byte
    u32x digest_t2[2];
    u32x digest_t3[2];

    digest_t0[0] = MD5M_A;
    digest_t0[1] = MD5M_B;
    digest_t0[2] = MD5M_C;
    digest_t0[3] = MD5M_D;

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest_t0);

    // prepare 16 * 21 buffer stuff

    u32x digest[4];

    digest[0] = MD5M_A;
    digest[1] = MD5M_B;
    digest[2] = MD5M_C;
    digest[3] = MD5M_D;

    // offsets

    digest_t0[0] &= 0xffffffff;
    digest_t0[1] &= 0x000000ff;
    digest_t0[2] &= 0x00000000;
    digest_t0[3] &= 0x00000000;

    digest_t1[0] =                      digest_t0[0] <<  8;
    digest_t1[1] = digest_t0[0] >> 24 | digest_t0[1] <<  8;

    digest_t2[0] =                      digest_t0[0] << 16;
    digest_t2[1] = digest_t0[0] >> 16 | digest_t0[1] << 16;

    digest_t3[0] =                      digest_t0[0] << 24;
    digest_t3[1] = digest_t0[0] >>  8 | digest_t0[1] << 24;

    // generate the 16 * 21 buffer

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..5
    w0_t[0]  = digest_t0[0];
    w0_t[1]  = digest_t0[1];

    // 5..21
    w0_t[1] |= salt_buf_t1[0];
    w0_t[2]  = salt_buf_t1[1];
    w0_t[3]  = salt_buf_t1[2];
    w1_t[0]  = salt_buf_t1[3];
    w1_t[1]  = salt_buf_t1[4];

    // 21..26
    w1_t[1] |= digest_t1[0];
    w1_t[2]  = digest_t1[1];

    // 26..42
    w1_t[2] |= salt_buf_t2[0];
    w1_t[3]  = salt_buf_t2[1];
    w2_t[0]  = salt_buf_t2[2];
    w2_t[1]  = salt_buf_t2[3];
    w2_t[2]  = salt_buf_t2[4];

    // 42..47
    w2_t[2] |= digest_t2[0];
    w2_t[3]  = digest_t2[1];

    // 47..63
    w2_t[3] |= salt_buf_t3[0];
    w3_t[0]  = salt_buf_t3[1];
    w3_t[1]  = salt_buf_t3[2];
    w3_t[2]  = salt_buf_t3[3];
    w3_t[3]  = salt_buf_t3[4];

    // 63..

    w3_t[3] |= digest_t3[0];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..4
    w0_t[0]  = digest_t3[1];

    // 4..20
    w0_t[1]  = salt_buf_t0[0];
    w0_t[2]  = salt_buf_t0[1];
    w0_t[3]  = salt_buf_t0[2];
    w1_t[0]  = salt_buf_t0[3];

    // 20..25
    w1_t[1]  = digest_t0[0];
    w1_t[2]  = digest_t0[1];

    // 25..41
    w1_t[2] |= salt_buf_t1[0];
    w1_t[3]  = salt_buf_t1[1];
    w2_t[0]  = salt_buf_t1[2];
    w2_t[1]  = salt_buf_t1[3];
    w2_t[2]  = salt_buf_t1[4];

    // 41..46
    w2_t[2] |= digest_t1[0];
    w2_t[3]  = digest_t1[1];

    // 46..62
    w2_t[3] |= salt_buf_t2[0];
    w3_t[0]  = salt_buf_t2[1];
    w3_t[1]  = salt_buf_t2[2];
    w3_t[2]  = salt_buf_t2[3];
    w3_t[3]  = salt_buf_t2[4];

    // 62..
    w3_t[3] |= digest_t2[0];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..3
    w0_t[0]  = digest_t2[1];

    // 3..19
    w0_t[0] |= salt_buf_t3[0];
    w0_t[1]  = salt_buf_t3[1];
    w0_t[2]  = salt_buf_t3[2];
    w0_t[3]  = salt_buf_t3[3];
    w1_t[0]  = salt_buf_t3[4];

    // 19..24
    w1_t[0] |= digest_t3[0];
    w1_t[1]  = digest_t3[1];

    // 24..40
    w1_t[2]  = salt_buf_t0[0];
    w1_t[3]  = salt_buf_t0[1];
    w2_t[0]  = salt_buf_t0[2];
    w2_t[1]  = salt_buf_t0[3];

    // 40..45
    w2_t[2]  = digest_t0[0];
    w2_t[3]  = digest_t0[1];

    // 45..61
    w2_t[3] |= salt_buf_t1[0];
    w3_t[0]  = salt_buf_t1[1];
    w3_t[1]  = salt_buf_t1[2];
    w3_t[2]  = salt_buf_t1[3];
    w3_t[3]  = salt_buf_t1[4];

    // 61..
    w3_t[3] |= digest_t1[0];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..2
    w0_t[0]  = digest_t1[1];

    // 2..18
    w0_t[0] |= salt_buf_t2[0];
    w0_t[1]  = salt_buf_t2[1];
    w0_t[2]  = salt_buf_t2[2];
    w0_t[3]  = salt_buf_t2[3];
    w1_t[0]  = salt_buf_t2[4];

    // 18..23
    w1_t[0] |= digest_t2[0];
    w1_t[1]  = digest_t2[1];

    // 23..39
    w1_t[1] |= salt_buf_t3[0];
    w1_t[2]  = salt_buf_t3[1];
    w1_t[3]  = salt_buf_t3[2];
    w2_t[0]  = salt_buf_t3[3];
    w2_t[1]  = salt_buf_t3[4];

    // 39..44
    w2_t[1] |= digest_t3[0];
    w2_t[2]  = digest_t3[1];

    // 44..60
    w2_t[3]  = salt_buf_t0[0];
    w3_t[0]  = salt_buf_t0[1];
    w3_t[1]  = salt_buf_t0[2];
    w3_t[2]  = salt_buf_t0[3];

    // 60..
    w3_t[3]  = digest_t0[0];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..1
    w0_t[0]  = digest_t0[1];

    // 1..17
    w0_t[0] |= salt_buf_t1[0];
    w0_t[1]  = salt_buf_t1[1];
    w0_t[2]  = salt_buf_t1[2];
    w0_t[3]  = salt_buf_t1[3];
    w1_t[0]  = salt_buf_t1[4];

    // 17..22
    w1_t[0] |= digest_t1[0];
    w1_t[1]  = digest_t1[1];

    // 22..38
    w1_t[1] |= salt_buf_t2[0];
    w1_t[2]  = salt_buf_t2[1];
    w1_t[3]  = salt_buf_t2[2];
    w2_t[0]  = salt_buf_t2[3];
    w2_t[1]  = salt_buf_t2[4];

    // 38..43
    w2_t[1] |= digest_t2[0];
    w2_t[2]  = digest_t2[1];

    // 43..59
    w2_t[2] |= salt_buf_t3[0];
    w2_t[3]  = salt_buf_t3[1];
    w3_t[0]  = salt_buf_t3[2];
    w3_t[1]  = salt_buf_t3[3];
    w3_t[2]  = salt_buf_t3[4];

    // 59..
    w3_t[2] |= digest_t3[0];
    w3_t[3]  = digest_t3[1];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0]  = salt_buf_t0[0];
    w0_t[1]  = salt_buf_t0[1];
    w0_t[2]  = salt_buf_t0[2];
    w0_t[3]  = salt_buf_t0[3];
    w1_t[0]  = 0x80;
    w1_t[1]  = 0;
    w1_t[2]  = 0;
    w1_t[3]  = 0;
    w2_t[0]  = 0;
    w2_t[1]  = 0;
    w2_t[2]  = 0;
    w2_t[3]  = 0;
    w3_t[0]  = 0;
    w3_t[1]  = 0;
    w3_t[2]  = 21 * 16 * 8;
    w3_t[3]  = 0;

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    // now the 40 bit input for the MD5 which then will generate the RC4 key, so it's precomputable!

    w0_t[0]  = digest[0];
    w0_t[1]  = digest[1] & 0xff;
    w0_t[2]  = 0x8000;
    w0_t[3]  = 0;
    w1_t[0]  = 0;
    w1_t[1]  = 0;
    w1_t[2]  = 0;
    w1_t[3]  = 0;
    w2_t[0]  = 0;
    w2_t[1]  = 0;
    w2_t[2]  = 0;
    w2_t[3]  = 0;
    w3_t[0]  = 0;
    w3_t[1]  = 0;
    w3_t[2]  = 9 * 8;
    w3_t[3]  = 0;

    digest[0] = MD5M_A;
    digest[1] = MD5M_B;
    digest[2] = MD5M_C;
    digest[3] = MD5M_D;

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    // now the RC4 part

    u32x key[4];

    key[0] = digest[0];
    key[1] = digest[1];
    key[2] = digest[2];
    key[3] = digest[3];

    rc4_init_16 (rc4_key, key);

    u32x out[4];

    u8 j = rc4_next_16 (rc4_key, 0, 0, encryptedVerifier, out);

    w0_t[0] = out[0];
    w0_t[1] = out[1];
    w0_t[2] = out[2];
    w0_t[3] = out[3];
    w1_t[0] = 0x80;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 16 * 8;
    w3_t[3] = 0;

    digest[0] = MD5M_A;
    digest[1] = MD5M_B;
    digest[2] = MD5M_C;
    digest[3] = MD5M_D;

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    rc4_next_16 (rc4_key, 16, j, digest, out);

    const u32x r0 = out[0];
    const u32x r1 = out[1];
    const u32x r2 = out[2];
    const u32x r3 = out[3];

    #include VECT_COMPARE_M
  }
}

__device__ static void m09700s (RC4_KEY rc4_keys[64], u32x w0[4], u32x w1[4], u32x w2[4], u32x w3[4], const u32 pw_len, const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *bfs_buf, void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const oldoffice01_t *oldoffice01_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset)
{
  /**
   * modifier
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;
  const u32 lid = threadIdx.x;

  RC4_KEY *rc4_key = &rc4_keys[lid];

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
   * salt
   */

  u32 salt_buf_t0[4];
  u32 salt_buf_t1[5];
  u32 salt_buf_t2[5];
  u32 salt_buf_t3[5];

  salt_buf_t0[0] = salt_bufs[salt_pos].salt_buf[0];
  salt_buf_t0[1] = salt_bufs[salt_pos].salt_buf[1];
  salt_buf_t0[2] = salt_bufs[salt_pos].salt_buf[2];
  salt_buf_t0[3] = salt_bufs[salt_pos].salt_buf[3];

  salt_buf_t1[0] =                        salt_buf_t0[0] <<  8;
  salt_buf_t1[1] = salt_buf_t0[0] >> 24 | salt_buf_t0[1] <<  8;
  salt_buf_t1[2] = salt_buf_t0[1] >> 24 | salt_buf_t0[2] <<  8;
  salt_buf_t1[3] = salt_buf_t0[2] >> 24 | salt_buf_t0[3] <<  8;
  salt_buf_t1[4] = salt_buf_t0[3] >> 24;

  salt_buf_t2[0] =                        salt_buf_t0[0] << 16;
  salt_buf_t2[1] = salt_buf_t0[0] >> 16 | salt_buf_t0[1] << 16;
  salt_buf_t2[2] = salt_buf_t0[1] >> 16 | salt_buf_t0[2] << 16;
  salt_buf_t2[3] = salt_buf_t0[2] >> 16 | salt_buf_t0[3] << 16;
  salt_buf_t2[4] = salt_buf_t0[3] >> 16;

  salt_buf_t3[0] =                        salt_buf_t0[0] << 24;
  salt_buf_t3[1] = salt_buf_t0[0] >>  8 | salt_buf_t0[1] << 24;
  salt_buf_t3[2] = salt_buf_t0[1] >>  8 | salt_buf_t0[2] << 24;
  salt_buf_t3[3] = salt_buf_t0[2] >>  8 | salt_buf_t0[3] << 24;
  salt_buf_t3[4] = salt_buf_t0[3] >>  8;

  const u32 salt_len = 16;

  /**
   * esalt
   */

  const u32 version = oldoffice01_bufs[salt_pos].version;

  u32 encryptedVerifier[4];

  encryptedVerifier[0] = oldoffice01_bufs[salt_pos].encryptedVerifier[0];
  encryptedVerifier[1] = oldoffice01_bufs[salt_pos].encryptedVerifier[1];
  encryptedVerifier[2] = oldoffice01_bufs[salt_pos].encryptedVerifier[2];
  encryptedVerifier[3] = oldoffice01_bufs[salt_pos].encryptedVerifier[3];

  /**
   * loop
   */

  u32x w0l = w0[0];

  for (u32 il_pos = 0; il_pos < bfs_cnt; il_pos++)
  {
    const u32 w0r = c_bfs[il_pos].i;

    w0[0] = w0l | w0r;

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
    w3_t[2] = pw_len * 8;
    w3_t[3] = 0;

    u32x digest_t0[4];
    u32x digest_t1[2]; // need only first 5 byte
    u32x digest_t2[2];
    u32x digest_t3[2];

    digest_t0[0] = MD5M_A;
    digest_t0[1] = MD5M_B;
    digest_t0[2] = MD5M_C;
    digest_t0[3] = MD5M_D;

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest_t0);

    // prepare 16 * 21 buffer stuff

    u32x digest[4];

    digest[0] = MD5M_A;
    digest[1] = MD5M_B;
    digest[2] = MD5M_C;
    digest[3] = MD5M_D;

    // offsets

    digest_t0[0] &= 0xffffffff;
    digest_t0[1] &= 0x000000ff;
    digest_t0[2] &= 0x00000000;
    digest_t0[3] &= 0x00000000;

    digest_t1[0] =                      digest_t0[0] <<  8;
    digest_t1[1] = digest_t0[0] >> 24 | digest_t0[1] <<  8;

    digest_t2[0] =                      digest_t0[0] << 16;
    digest_t2[1] = digest_t0[0] >> 16 | digest_t0[1] << 16;

    digest_t3[0] =                      digest_t0[0] << 24;
    digest_t3[1] = digest_t0[0] >>  8 | digest_t0[1] << 24;

    // generate the 16 * 21 buffer

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..5
    w0_t[0]  = digest_t0[0];
    w0_t[1]  = digest_t0[1];

    // 5..21
    w0_t[1] |= salt_buf_t1[0];
    w0_t[2]  = salt_buf_t1[1];
    w0_t[3]  = salt_buf_t1[2];
    w1_t[0]  = salt_buf_t1[3];
    w1_t[1]  = salt_buf_t1[4];

    // 21..26
    w1_t[1] |= digest_t1[0];
    w1_t[2]  = digest_t1[1];

    // 26..42
    w1_t[2] |= salt_buf_t2[0];
    w1_t[3]  = salt_buf_t2[1];
    w2_t[0]  = salt_buf_t2[2];
    w2_t[1]  = salt_buf_t2[3];
    w2_t[2]  = salt_buf_t2[4];

    // 42..47
    w2_t[2] |= digest_t2[0];
    w2_t[3]  = digest_t2[1];

    // 47..63
    w2_t[3] |= salt_buf_t3[0];
    w3_t[0]  = salt_buf_t3[1];
    w3_t[1]  = salt_buf_t3[2];
    w3_t[2]  = salt_buf_t3[3];
    w3_t[3]  = salt_buf_t3[4];

    // 63..

    w3_t[3] |= digest_t3[0];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..4
    w0_t[0]  = digest_t3[1];

    // 4..20
    w0_t[1]  = salt_buf_t0[0];
    w0_t[2]  = salt_buf_t0[1];
    w0_t[3]  = salt_buf_t0[2];
    w1_t[0]  = salt_buf_t0[3];

    // 20..25
    w1_t[1]  = digest_t0[0];
    w1_t[2]  = digest_t0[1];

    // 25..41
    w1_t[2] |= salt_buf_t1[0];
    w1_t[3]  = salt_buf_t1[1];
    w2_t[0]  = salt_buf_t1[2];
    w2_t[1]  = salt_buf_t1[3];
    w2_t[2]  = salt_buf_t1[4];

    // 41..46
    w2_t[2] |= digest_t1[0];
    w2_t[3]  = digest_t1[1];

    // 46..62
    w2_t[3] |= salt_buf_t2[0];
    w3_t[0]  = salt_buf_t2[1];
    w3_t[1]  = salt_buf_t2[2];
    w3_t[2]  = salt_buf_t2[3];
    w3_t[3]  = salt_buf_t2[4];

    // 62..
    w3_t[3] |= digest_t2[0];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..3
    w0_t[0]  = digest_t2[1];

    // 3..19
    w0_t[0] |= salt_buf_t3[0];
    w0_t[1]  = salt_buf_t3[1];
    w0_t[2]  = salt_buf_t3[2];
    w0_t[3]  = salt_buf_t3[3];
    w1_t[0]  = salt_buf_t3[4];

    // 19..24
    w1_t[0] |= digest_t3[0];
    w1_t[1]  = digest_t3[1];

    // 24..40
    w1_t[2]  = salt_buf_t0[0];
    w1_t[3]  = salt_buf_t0[1];
    w2_t[0]  = salt_buf_t0[2];
    w2_t[1]  = salt_buf_t0[3];

    // 40..45
    w2_t[2]  = digest_t0[0];
    w2_t[3]  = digest_t0[1];

    // 45..61
    w2_t[3] |= salt_buf_t1[0];
    w3_t[0]  = salt_buf_t1[1];
    w3_t[1]  = salt_buf_t1[2];
    w3_t[2]  = salt_buf_t1[3];
    w3_t[3]  = salt_buf_t1[4];

    // 61..
    w3_t[3] |= digest_t1[0];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..2
    w0_t[0]  = digest_t1[1];

    // 2..18
    w0_t[0] |= salt_buf_t2[0];
    w0_t[1]  = salt_buf_t2[1];
    w0_t[2]  = salt_buf_t2[2];
    w0_t[3]  = salt_buf_t2[3];
    w1_t[0]  = salt_buf_t2[4];

    // 18..23
    w1_t[0] |= digest_t2[0];
    w1_t[1]  = digest_t2[1];

    // 23..39
    w1_t[1] |= salt_buf_t3[0];
    w1_t[2]  = salt_buf_t3[1];
    w1_t[3]  = salt_buf_t3[2];
    w2_t[0]  = salt_buf_t3[3];
    w2_t[1]  = salt_buf_t3[4];

    // 39..44
    w2_t[1] |= digest_t3[0];
    w2_t[2]  = digest_t3[1];

    // 44..60
    w2_t[3]  = salt_buf_t0[0];
    w3_t[0]  = salt_buf_t0[1];
    w3_t[1]  = salt_buf_t0[2];
    w3_t[2]  = salt_buf_t0[3];

    // 60..
    w3_t[3]  = digest_t0[0];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0] = 0;
    w0_t[1] = 0;
    w0_t[2] = 0;
    w0_t[3] = 0;
    w1_t[0] = 0;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 0;
    w3_t[3] = 0;

    // 0..1
    w0_t[0]  = digest_t0[1];

    // 1..17
    w0_t[0] |= salt_buf_t1[0];
    w0_t[1]  = salt_buf_t1[1];
    w0_t[2]  = salt_buf_t1[2];
    w0_t[3]  = salt_buf_t1[3];
    w1_t[0]  = salt_buf_t1[4];

    // 17..22
    w1_t[0] |= digest_t1[0];
    w1_t[1]  = digest_t1[1];

    // 22..38
    w1_t[1] |= salt_buf_t2[0];
    w1_t[2]  = salt_buf_t2[1];
    w1_t[3]  = salt_buf_t2[2];
    w2_t[0]  = salt_buf_t2[3];
    w2_t[1]  = salt_buf_t2[4];

    // 38..43
    w2_t[1] |= digest_t2[0];
    w2_t[2]  = digest_t2[1];

    // 43..59
    w2_t[2] |= salt_buf_t3[0];
    w2_t[3]  = salt_buf_t3[1];
    w3_t[0]  = salt_buf_t3[2];
    w3_t[1]  = salt_buf_t3[3];
    w3_t[2]  = salt_buf_t3[4];

    // 59..
    w3_t[2] |= digest_t3[0];
    w3_t[3]  = digest_t3[1];

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    w0_t[0]  = salt_buf_t0[0];
    w0_t[1]  = salt_buf_t0[1];
    w0_t[2]  = salt_buf_t0[2];
    w0_t[3]  = salt_buf_t0[3];
    w1_t[0]  = 0x80;
    w1_t[1]  = 0;
    w1_t[2]  = 0;
    w1_t[3]  = 0;
    w2_t[0]  = 0;
    w2_t[1]  = 0;
    w2_t[2]  = 0;
    w2_t[3]  = 0;
    w3_t[0]  = 0;
    w3_t[1]  = 0;
    w3_t[2]  = 21 * 16 * 8;
    w3_t[3]  = 0;

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    // now the 40 bit input for the MD5 which then will generate the RC4 key, so it's precomputable!

    w0_t[0]  = digest[0];
    w0_t[1]  = digest[1] & 0xff;
    w0_t[2]  = 0x8000;
    w0_t[3]  = 0;
    w1_t[0]  = 0;
    w1_t[1]  = 0;
    w1_t[2]  = 0;
    w1_t[3]  = 0;
    w2_t[0]  = 0;
    w2_t[1]  = 0;
    w2_t[2]  = 0;
    w2_t[3]  = 0;
    w3_t[0]  = 0;
    w3_t[1]  = 0;
    w3_t[2]  = 9 * 8;
    w3_t[3]  = 0;

    digest[0] = MD5M_A;
    digest[1] = MD5M_B;
    digest[2] = MD5M_C;
    digest[3] = MD5M_D;

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    // now the RC4 part

    u32x key[4];

    key[0] = digest[0];
    key[1] = digest[1];
    key[2] = digest[2];
    key[3] = digest[3];

    rc4_init_16 (rc4_key, key);

    u32x out[4];

    u8 j = rc4_next_16 (rc4_key, 0, 0, encryptedVerifier, out);

    w0_t[0] = out[0];
    w0_t[1] = out[1];
    w0_t[2] = out[2];
    w0_t[3] = out[3];
    w1_t[0] = 0x80;
    w1_t[1] = 0;
    w1_t[2] = 0;
    w1_t[3] = 0;
    w2_t[0] = 0;
    w2_t[1] = 0;
    w2_t[2] = 0;
    w2_t[3] = 0;
    w3_t[0] = 0;
    w3_t[1] = 0;
    w3_t[2] = 16 * 8;
    w3_t[3] = 0;

    digest[0] = MD5M_A;
    digest[1] = MD5M_B;
    digest[2] = MD5M_C;
    digest[3] = MD5M_D;

    md5_transform (w0_t, w1_t, w2_t, w3_t, digest);

    rc4_next_16 (rc4_key, 16, j, digest, out);

    const u32x r0 = out[0];
    const u32x r1 = out[1];
    const u32x r2 = out[2];
    const u32x r3 = out[3];

    #include VECT_COMPARE_S
  }
}

extern "C" __global__ void __launch_bounds__ (64, 1) m09700_m04 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *bfs_buf, void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const oldoffice01_t *oldoffice01_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  if (gid >= gid_max) return;

  u32x w0[4];

  w0[0] = pws[gid].i[ 0];
  w0[1] = pws[gid].i[ 1];
  w0[2] = pws[gid].i[ 2];
  w0[3] = pws[gid].i[ 3];

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

  const u32 pw_len = pws[gid].pw_len;

  /**
   * main
   */

  __shared__ RC4_KEY rc4_keys[64];

  m09700m (rc4_keys, w0, w1, w2, w3, pw_len, pws, rules_buf, combs_buf, bfs_buf, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, oldoffice01_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (64, 1) m09700_m08 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *bfs_buf, void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const oldoffice01_t *oldoffice01_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  if (gid >= gid_max) return;

  u32x w0[4];

  w0[0] = pws[gid].i[ 0];
  w0[1] = pws[gid].i[ 1];
  w0[2] = pws[gid].i[ 2];
  w0[3] = pws[gid].i[ 3];

  u32x w1[4];

  w1[0] = pws[gid].i[ 4];
  w1[1] = pws[gid].i[ 5];
  w1[2] = pws[gid].i[ 6];
  w1[3] = pws[gid].i[ 7];

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

  const u32 pw_len = pws[gid].pw_len;

  /**
   * main
   */

  __shared__ RC4_KEY rc4_keys[64];

  m09700m (rc4_keys, w0, w1, w2, w3, pw_len, pws, rules_buf, combs_buf, bfs_buf, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, oldoffice01_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (64, 1) m09700_m16 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *bfs_buf, void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const oldoffice01_t *oldoffice01_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  if (gid >= gid_max) return;

  u32x w0[4];

  w0[0] = pws[gid].i[ 0];
  w0[1] = pws[gid].i[ 1];
  w0[2] = pws[gid].i[ 2];
  w0[3] = pws[gid].i[ 3];

  u32x w1[4];

  w1[0] = pws[gid].i[ 4];
  w1[1] = pws[gid].i[ 5];
  w1[2] = pws[gid].i[ 6];
  w1[3] = pws[gid].i[ 7];

  u32x w2[4];

  w2[0] = pws[gid].i[ 8];
  w2[1] = pws[gid].i[ 9];
  w2[2] = pws[gid].i[10];
  w2[3] = pws[gid].i[11];

  u32x w3[4];

  w3[0] = pws[gid].i[12];
  w3[1] = pws[gid].i[13];
  w3[2] = 0;
  w3[3] = 0;

  const u32 pw_len = pws[gid].pw_len;

  /**
   * main
   */

  __shared__ RC4_KEY rc4_keys[64];

  m09700m (rc4_keys, w0, w1, w2, w3, pw_len, pws, rules_buf, combs_buf, bfs_buf, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, oldoffice01_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (64, 1) m09700_s04 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *bfs_buf, void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const oldoffice01_t *oldoffice01_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  if (gid >= gid_max) return;

  u32x w0[4];

  w0[0] = pws[gid].i[ 0];
  w0[1] = pws[gid].i[ 1];
  w0[2] = pws[gid].i[ 2];
  w0[3] = pws[gid].i[ 3];

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

  const u32 pw_len = pws[gid].pw_len;

  /**
   * main
   */

  __shared__ RC4_KEY rc4_keys[64];

  m09700s (rc4_keys, w0, w1, w2, w3, pw_len, pws, rules_buf, combs_buf, bfs_buf, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, oldoffice01_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (64, 1) m09700_s08 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *bfs_buf, void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const oldoffice01_t *oldoffice01_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  if (gid >= gid_max) return;

  u32x w0[4];

  w0[0] = pws[gid].i[ 0];
  w0[1] = pws[gid].i[ 1];
  w0[2] = pws[gid].i[ 2];
  w0[3] = pws[gid].i[ 3];

  u32x w1[4];

  w1[0] = pws[gid].i[ 4];
  w1[1] = pws[gid].i[ 5];
  w1[2] = pws[gid].i[ 6];
  w1[3] = pws[gid].i[ 7];

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

  const u32 pw_len = pws[gid].pw_len;

  /**
   * main
   */

  __shared__ RC4_KEY rc4_keys[64];

  m09700s (rc4_keys, w0, w1, w2, w3, pw_len, pws, rules_buf, combs_buf, bfs_buf, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, oldoffice01_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}

extern "C" __global__ void __launch_bounds__ (64, 1) m09700_s16 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const u32x *bfs_buf, void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const oldoffice01_t *oldoffice01_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 bfs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  if (gid >= gid_max) return;

  u32x w0[4];

  w0[0] = pws[gid].i[ 0];
  w0[1] = pws[gid].i[ 1];
  w0[2] = pws[gid].i[ 2];
  w0[3] = pws[gid].i[ 3];

  u32x w1[4];

  w1[0] = pws[gid].i[ 4];
  w1[1] = pws[gid].i[ 5];
  w1[2] = pws[gid].i[ 6];
  w1[3] = pws[gid].i[ 7];

  u32x w2[4];

  w2[0] = pws[gid].i[ 8];
  w2[1] = pws[gid].i[ 9];
  w2[2] = pws[gid].i[10];
  w2[3] = pws[gid].i[11];

  u32x w3[4];

  w3[0] = pws[gid].i[12];
  w3[1] = pws[gid].i[13];
  w3[2] = 0;
  w3[3] = 0;

  const u32 pw_len = pws[gid].pw_len;

  /**
   * main
   */

  __shared__ RC4_KEY rc4_keys[64];

  m09700s (rc4_keys, w0, w1, w2, w3, pw_len, pws, rules_buf, combs_buf, bfs_buf, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, oldoffice01_bufs, d_return_buf, d_scryptV_buf, bitmap_mask, bitmap_shift1, bitmap_shift2, salt_pos, loop_pos, loop_cnt, bfs_cnt, digests_cnt, digests_offset);
}
