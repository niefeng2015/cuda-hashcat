/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define _MD5_

#include "include/constants.h"
#include "include/kernel_vendor.h"

#ifdef  VLIW1
#define VECT_SIZE1
#endif

#ifdef  VLIW4
#define VECT_SIZE4
#endif

#ifdef  VLIW5
#define VECT_SIZE4
#endif

#define DGST_R0 0
#define DGST_R1 1
#define DGST_R2 2
#define DGST_R3 3

#include "include/kernel_functions.c"
#include "types_amd.c"
#include "common_amd.c"

#ifdef  VECT_SIZE1
#define VECT_COMPARE_M "check_multi_vect1_comp4.c"
#endif

#ifdef  VECT_SIZE4
#define VECT_COMPARE_M "check_multi_vect4_comp4.c"
#endif

#define md5apr1_magic0 0x72706124
#define md5apr1_magic1 0x00002431

static void md5_transform (const u32x w0[4], const u32x w1[4], const u32x w2[4], const u32x w3[4], u32x digest[4])
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
  u32x wf_t = 0;

  u32x tmp2;

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

  MD5_STEP (MD5_H1, a, b, c, d, w5_t, MD5C20, MD5S20);
  MD5_STEP (MD5_H2, d, a, b, c, w8_t, MD5C21, MD5S21);
  MD5_STEP (MD5_H1, c, d, a, b, wb_t, MD5C22, MD5S22);
  MD5_STEP (MD5_H2, b, c, d, a, we_t, MD5C23, MD5S23);
  MD5_STEP (MD5_H1, a, b, c, d, w1_t, MD5C24, MD5S20);
  MD5_STEP (MD5_H2, d, a, b, c, w4_t, MD5C25, MD5S21);
  MD5_STEP (MD5_H1, c, d, a, b, w7_t, MD5C26, MD5S22);
  MD5_STEP (MD5_H2, b, c, d, a, wa_t, MD5C27, MD5S23);
  MD5_STEP (MD5_H1, a, b, c, d, wd_t, MD5C28, MD5S20);
  MD5_STEP (MD5_H2, d, a, b, c, w0_t, MD5C29, MD5S21);
  MD5_STEP (MD5_H1, c, d, a, b, w3_t, MD5C2a, MD5S22);
  MD5_STEP (MD5_H2, b, c, d, a, w6_t, MD5C2b, MD5S23);
  MD5_STEP (MD5_H1, a, b, c, d, w9_t, MD5C2c, MD5S20);
  MD5_STEP (MD5_H2, d, a, b, c, wc_t, MD5C2d, MD5S21);
  MD5_STEP (MD5_H1, c, d, a, b, wf_t, MD5C2e, MD5S22);
  MD5_STEP (MD5_H2, b, c, d, a, w2_t, MD5C2f, MD5S23);

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

static void memcat16 (u32x block0[4], u32x block1[4], u32x block2[4], u32x block3[4], const u32 block_len, const u32x append[4])
{
  switch (block_len)
  {
    case 0:
      block0[0] = append[0];
      block0[1] = append[1];
      block0[2] = append[2];
      block0[3] = append[3];
      break;

    case 1:
      block0[0] = block0[0]       | append[0] <<  8;
      block0[1] = append[0] >> 24 | append[1] <<  8;
      block0[2] = append[1] >> 24 | append[2] <<  8;
      block0[3] = append[2] >> 24 | append[3] <<  8;
      block1[0] = append[3] >> 24;
      break;

    case 2:
      block0[0] = block0[0]       | append[0] << 16;
      block0[1] = append[0] >> 16 | append[1] << 16;
      block0[2] = append[1] >> 16 | append[2] << 16;
      block0[3] = append[2] >> 16 | append[3] << 16;
      block1[0] = append[3] >> 16;
      break;

    case 3:
      block0[0] = block0[0]       | append[0] << 24;
      block0[1] = append[0] >>  8 | append[1] << 24;
      block0[2] = append[1] >>  8 | append[2] << 24;
      block0[3] = append[2] >>  8 | append[3] << 24;
      block1[0] = append[3] >>  8;
      break;

    case 4:
      block0[1] = append[0];
      block0[2] = append[1];
      block0[3] = append[2];
      block1[0] = append[3];
      break;

    case 5:
      block0[1] = block0[1]       | append[0] <<  8;
      block0[2] = append[0] >> 24 | append[1] <<  8;
      block0[3] = append[1] >> 24 | append[2] <<  8;
      block1[0] = append[2] >> 24 | append[3] <<  8;
      block1[1] = append[3] >> 24;
      break;

    case 6:
      block0[1] = block0[1]       | append[0] << 16;
      block0[2] = append[0] >> 16 | append[1] << 16;
      block0[3] = append[1] >> 16 | append[2] << 16;
      block1[0] = append[2] >> 16 | append[3] << 16;
      block1[1] = append[3] >> 16;
      break;

    case 7:
      block0[1] = block0[1]       | append[0] << 24;
      block0[2] = append[0] >>  8 | append[1] << 24;
      block0[3] = append[1] >>  8 | append[2] << 24;
      block1[0] = append[2] >>  8 | append[3] << 24;
      block1[1] = append[3] >>  8;
      break;

    case 8:
      block0[2] = append[0];
      block0[3] = append[1];
      block1[0] = append[2];
      block1[1] = append[3];
      break;

    case 9:
      block0[2] = block0[2]       | append[0] <<  8;
      block0[3] = append[0] >> 24 | append[1] <<  8;
      block1[0] = append[1] >> 24 | append[2] <<  8;
      block1[1] = append[2] >> 24 | append[3] <<  8;
      block1[2] = append[3] >> 24;
      break;

    case 10:
      block0[2] = block0[2]       | append[0] << 16;
      block0[3] = append[0] >> 16 | append[1] << 16;
      block1[0] = append[1] >> 16 | append[2] << 16;
      block1[1] = append[2] >> 16 | append[3] << 16;
      block1[2] = append[3] >> 16;
      break;

    case 11:
      block0[2] = block0[2]       | append[0] << 24;
      block0[3] = append[0] >>  8 | append[1] << 24;
      block1[0] = append[1] >>  8 | append[2] << 24;
      block1[1] = append[2] >>  8 | append[3] << 24;
      block1[2] = append[3] >>  8;
      break;

    case 12:
      block0[3] = append[0];
      block1[0] = append[1];
      block1[1] = append[2];
      block1[2] = append[3];
      break;

    case 13:
      block0[3] = block0[3]       | append[0] <<  8;
      block1[0] = append[0] >> 24 | append[1] <<  8;
      block1[1] = append[1] >> 24 | append[2] <<  8;
      block1[2] = append[2] >> 24 | append[3] <<  8;
      block1[3] = append[3] >> 24;
      break;

    case 14:
      block0[3] = block0[3]       | append[0] << 16;
      block1[0] = append[0] >> 16 | append[1] << 16;
      block1[1] = append[1] >> 16 | append[2] << 16;
      block1[2] = append[2] >> 16 | append[3] << 16;
      block1[3] = append[3] >> 16;
      break;

    case 15:
      block0[3] = block0[3]       | append[0] << 24;
      block1[0] = append[0] >>  8 | append[1] << 24;
      block1[1] = append[1] >>  8 | append[2] << 24;
      block1[2] = append[2] >>  8 | append[3] << 24;
      block1[3] = append[3] >>  8;
      break;

    case 16:
      block1[0] = append[0];
      block1[1] = append[1];
      block1[2] = append[2];
      block1[3] = append[3];
      break;

    case 17:
      block1[0] = block1[0]       | append[0] <<  8;
      block1[1] = append[0] >> 24 | append[1] <<  8;
      block1[2] = append[1] >> 24 | append[2] <<  8;
      block1[3] = append[2] >> 24 | append[3] <<  8;
      block2[0] = append[3] >> 24;
      break;

    case 18:
      block1[0] = block1[0]       | append[0] << 16;
      block1[1] = append[0] >> 16 | append[1] << 16;
      block1[2] = append[1] >> 16 | append[2] << 16;
      block1[3] = append[2] >> 16 | append[3] << 16;
      block2[0] = append[3] >> 16;
      break;

    case 19:
      block1[0] = block1[0]       | append[0] << 24;
      block1[1] = append[0] >>  8 | append[1] << 24;
      block1[2] = append[1] >>  8 | append[2] << 24;
      block1[3] = append[2] >>  8 | append[3] << 24;
      block2[0] = append[3] >>  8;
      break;

    case 20:
      block1[1] = append[0];
      block1[2] = append[1];
      block1[3] = append[2];
      block2[0] = append[3];
      break;

    case 21:
      block1[1] = block1[1]       | append[0] <<  8;
      block1[2] = append[0] >> 24 | append[1] <<  8;
      block1[3] = append[1] >> 24 | append[2] <<  8;
      block2[0] = append[2] >> 24 | append[3] <<  8;
      block2[1] = append[3] >> 24;
      break;

    case 22:
      block1[1] = block1[1]       | append[0] << 16;
      block1[2] = append[0] >> 16 | append[1] << 16;
      block1[3] = append[1] >> 16 | append[2] << 16;
      block2[0] = append[2] >> 16 | append[3] << 16;
      block2[1] = append[3] >> 16;
      break;

    case 23:
      block1[1] = block1[1]       | append[0] << 24;
      block1[2] = append[0] >>  8 | append[1] << 24;
      block1[3] = append[1] >>  8 | append[2] << 24;
      block2[0] = append[2] >>  8 | append[3] << 24;
      block2[1] = append[3] >>  8;
      break;

    case 24:
      block1[2] = append[0];
      block1[3] = append[1];
      block2[0] = append[2];
      block2[1] = append[3];
      break;

    case 25:
      block1[2] = block1[2]       | append[0] <<  8;
      block1[3] = append[0] >> 24 | append[1] <<  8;
      block2[0] = append[1] >> 24 | append[2] <<  8;
      block2[1] = append[2] >> 24 | append[3] <<  8;
      block2[2] = append[3] >> 24;
      break;

    case 26:
      block1[2] = block1[2]       | append[0] << 16;
      block1[3] = append[0] >> 16 | append[1] << 16;
      block2[0] = append[1] >> 16 | append[2] << 16;
      block2[1] = append[2] >> 16 | append[3] << 16;
      block2[2] = append[3] >> 16;
      break;

    case 27:
      block1[2] = block1[2]       | append[0] << 24;
      block1[3] = append[0] >>  8 | append[1] << 24;
      block2[0] = append[1] >>  8 | append[2] << 24;
      block2[1] = append[2] >>  8 | append[3] << 24;
      block2[2] = append[3] >>  8;
      break;

    case 28:
      block1[3] = append[0];
      block2[0] = append[1];
      block2[1] = append[2];
      block2[2] = append[3];
      break;

    case 29:
      block1[3] = block1[3]       | append[0] <<  8;
      block2[0] = append[0] >> 24 | append[1] <<  8;
      block2[1] = append[1] >> 24 | append[2] <<  8;
      block2[2] = append[2] >> 24 | append[3] <<  8;
      block2[3] = append[3] >> 24;
      break;

    case 30:
      block1[3] = block1[3]       | append[0] << 16;
      block2[0] = append[0] >> 16 | append[1] << 16;
      block2[1] = append[1] >> 16 | append[2] << 16;
      block2[2] = append[2] >> 16 | append[3] << 16;
      block2[3] = append[3] >> 16;
      break;

    case 31:
      block1[3] = block1[3]       | append[0] << 24;
      block2[0] = append[0] >>  8 | append[1] << 24;
      block2[1] = append[1] >>  8 | append[2] << 24;
      block2[2] = append[2] >>  8 | append[3] << 24;
      block2[3] = append[3] >>  8;
      break;

    case 32:
      block2[0] = append[0];
      block2[1] = append[1];
      block2[2] = append[2];
      block2[3] = append[3];
      break;

    case 33:
      block2[0] = block2[0]       | append[0] <<  8;
      block2[1] = append[0] >> 24 | append[1] <<  8;
      block2[2] = append[1] >> 24 | append[2] <<  8;
      block2[3] = append[2] >> 24 | append[3] <<  8;
      block3[0] = append[3] >> 24;
      break;

    case 34:
      block2[0] = block2[0]       | append[0] << 16;
      block2[1] = append[0] >> 16 | append[1] << 16;
      block2[2] = append[1] >> 16 | append[2] << 16;
      block2[3] = append[2] >> 16 | append[3] << 16;
      block3[0] = append[3] >> 16;
      break;

    case 35:
      block2[0] = block2[0]       | append[0] << 24;
      block2[1] = append[0] >>  8 | append[1] << 24;
      block2[2] = append[1] >>  8 | append[2] << 24;
      block2[3] = append[2] >>  8 | append[3] << 24;
      block3[0] = append[3] >>  8;
      break;

    case 36:
      block2[1] = append[0];
      block2[2] = append[1];
      block2[3] = append[2];
      block3[0] = append[3];
      break;

    case 37:
      block2[1] = block2[1]       | append[0] <<  8;
      block2[2] = append[0] >> 24 | append[1] <<  8;
      block2[3] = append[1] >> 24 | append[2] <<  8;
      block3[0] = append[2] >> 24 | append[3] <<  8;
      block3[1] = append[3] >> 24;
      break;

    case 38:
      block2[1] = block2[1]       | append[0] << 16;
      block2[2] = append[0] >> 16 | append[1] << 16;
      block2[3] = append[1] >> 16 | append[2] << 16;
      block3[0] = append[2] >> 16 | append[3] << 16;
      block3[1] = append[3] >> 16;
      break;

    case 39:
      block2[1] = block2[1]       | append[0] << 24;
      block2[2] = append[0] >>  8 | append[1] << 24;
      block2[3] = append[1] >>  8 | append[2] << 24;
      block3[0] = append[2] >>  8 | append[3] << 24;
      block3[1] = append[3] >>  8;
      break;

    case 40:
      block2[2] = append[0];
      block2[3] = append[1];
      block3[0] = append[2];
      block3[1] = append[3];
      break;

    case 41:
      block2[2] = block2[2]       | append[0] <<  8;
      block2[3] = append[0] >> 24 | append[1] <<  8;
      block3[0] = append[1] >> 24 | append[2] <<  8;
      block3[1] = append[2] >> 24 | append[3] <<  8;
      block3[2] = append[3] >> 24;
      break;

    case 42:
      block2[2] = block2[2]       | append[0] << 16;
      block2[3] = append[0] >> 16 | append[1] << 16;
      block3[0] = append[1] >> 16 | append[2] << 16;
      block3[1] = append[2] >> 16 | append[3] << 16;
      block3[2] = append[3] >> 16;
      break;

    case 43:
      block2[2] = block2[2]       | append[0] << 24;
      block2[3] = append[0] >>  8 | append[1] << 24;
      block3[0] = append[1] >>  8 | append[2] << 24;
      block3[1] = append[2] >>  8 | append[3] << 24;
      block3[2] = append[3] >>  8;
      break;

    case 44:
      block2[3] = append[0];
      block3[0] = append[1];
      block3[1] = append[2];
      block3[2] = append[3];
      break;

    case 45:
      block2[3] = block2[3]       | append[0] <<  8;
      block3[0] = append[0] >> 24 | append[1] <<  8;
      block3[1] = append[1] >> 24 | append[2] <<  8;
      block3[2] = append[2] >> 24 | append[3] <<  8;
      block3[3] = append[3] >> 24;
      break;

    case 46:
      block2[3] = block2[3]       | append[0] << 16;
      block3[0] = append[0] >> 16 | append[1] << 16;
      block3[1] = append[1] >> 16 | append[2] << 16;
      block3[2] = append[2] >> 16 | append[3] << 16;
      block3[3] = append[3] >> 16;
      break;

    case 47:
      block2[3] = block2[3]       | append[0] << 24;
      block3[0] = append[0] >>  8 | append[1] << 24;
      block3[1] = append[1] >>  8 | append[2] << 24;
      block3[2] = append[2] >>  8 | append[3] << 24;
      block3[3] = append[3] >>  8;
      break;

    case 48:
      block3[0] = append[0];
      block3[1] = append[1];
      block3[2] = append[2];
      block3[3] = append[3];
      break;

    case 49:
      block3[0] = block3[0]       | append[0] <<  8;
      block3[1] = append[0] >> 24 | append[1] <<  8;
      block3[2] = append[1] >> 24 | append[2] <<  8;
      block3[3] = append[2] >> 24 | append[3] <<  8;
      break;

    case 50:
      block3[0] = block3[0]       | append[0] << 16;
      block3[1] = append[0] >> 16 | append[1] << 16;
      block3[2] = append[1] >> 16 | append[2] << 16;
      block3[3] = append[2] >> 16 | append[3] << 16;
      break;

    case 51:
      block3[0] = block3[0]       | append[0] << 24;
      block3[1] = append[0] >>  8 | append[1] << 24;
      block3[2] = append[1] >>  8 | append[2] << 24;
      block3[3] = append[2] >>  8 | append[3] << 24;
      break;

    case 52:
      block3[1] = append[0];
      block3[2] = append[1];
      block3[3] = append[2];
      break;

    case 53:
      block3[1] = block3[1]       | append[0] <<  8;
      block3[2] = append[0] >> 24 | append[1] <<  8;
      block3[3] = append[1] >> 24 | append[2] <<  8;
      break;

    case 54:
      block3[1] = block3[1]       | append[0] << 16;
      block3[2] = append[0] >> 16 | append[1] << 16;
      block3[3] = append[1] >> 16 | append[2] << 16;
      break;

    case 55:
      block3[1] = block3[1]       | append[0] << 24;
      block3[2] = append[0] >>  8 | append[1] << 24;
      block3[3] = append[1] >>  8 | append[2] << 24;
      break;

    case 56:
      block3[2] = append[0];
      block3[3] = append[1];
      break;
  }
}

static void memcat16_x80 (u32x block0[4], u32x block1[4], u32x block2[4], u32x block3[4], const u32 block_len, const u32x append[4])
{
  switch (block_len)
  {
    case 0:
      block0[0] = append[0];
      block0[1] = append[1];
      block0[2] = append[2];
      block0[3] = append[3];
      block1[0] = 0x80;
      break;

    case 1:
      block0[0] = block0[0]       | append[0] <<  8;
      block0[1] = append[0] >> 24 | append[1] <<  8;
      block0[2] = append[1] >> 24 | append[2] <<  8;
      block0[3] = append[2] >> 24 | append[3] <<  8;
      block1[0] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 2:
      block0[0] = block0[0]       | append[0] << 16;
      block0[1] = append[0] >> 16 | append[1] << 16;
      block0[2] = append[1] >> 16 | append[2] << 16;
      block0[3] = append[2] >> 16 | append[3] << 16;
      block1[0] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 3:
      block0[0] = block0[0]       | append[0] << 24;
      block0[1] = append[0] >>  8 | append[1] << 24;
      block0[2] = append[1] >>  8 | append[2] << 24;
      block0[3] = append[2] >>  8 | append[3] << 24;
      block1[0] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 4:
      block0[1] = append[0];
      block0[2] = append[1];
      block0[3] = append[2];
      block1[0] = append[3];
      block1[1] = 0x80;
      break;

    case 5:
      block0[1] = block0[1]       | append[0] <<  8;
      block0[2] = append[0] >> 24 | append[1] <<  8;
      block0[3] = append[1] >> 24 | append[2] <<  8;
      block1[0] = append[2] >> 24 | append[3] <<  8;
      block1[1] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 6:
      block0[1] = block0[1]       | append[0] << 16;
      block0[2] = append[0] >> 16 | append[1] << 16;
      block0[3] = append[1] >> 16 | append[2] << 16;
      block1[0] = append[2] >> 16 | append[3] << 16;
      block1[1] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 7:
      block0[1] = block0[1]       | append[0] << 24;
      block0[2] = append[0] >>  8 | append[1] << 24;
      block0[3] = append[1] >>  8 | append[2] << 24;
      block1[0] = append[2] >>  8 | append[3] << 24;
      block1[1] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 8:
      block0[2] = append[0];
      block0[3] = append[1];
      block1[0] = append[2];
      block1[1] = append[3];
      block1[2] = 0x80;
      break;

    case 9:
      block0[2] = block0[2]       | append[0] <<  8;
      block0[3] = append[0] >> 24 | append[1] <<  8;
      block1[0] = append[1] >> 24 | append[2] <<  8;
      block1[1] = append[2] >> 24 | append[3] <<  8;
      block1[2] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 10:
      block0[2] = block0[2]       | append[0] << 16;
      block0[3] = append[0] >> 16 | append[1] << 16;
      block1[0] = append[1] >> 16 | append[2] << 16;
      block1[1] = append[2] >> 16 | append[3] << 16;
      block1[2] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 11:
      block0[2] = block0[2]       | append[0] << 24;
      block0[3] = append[0] >>  8 | append[1] << 24;
      block1[0] = append[1] >>  8 | append[2] << 24;
      block1[1] = append[2] >>  8 | append[3] << 24;
      block1[2] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 12:
      block0[3] = append[0];
      block1[0] = append[1];
      block1[1] = append[2];
      block1[2] = append[3];
      block1[3] = 0x80;
      break;

    case 13:
      block0[3] = block0[3]       | append[0] <<  8;
      block1[0] = append[0] >> 24 | append[1] <<  8;
      block1[1] = append[1] >> 24 | append[2] <<  8;
      block1[2] = append[2] >> 24 | append[3] <<  8;
      block1[3] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 14:
      block0[3] = block0[3]       | append[0] << 16;
      block1[0] = append[0] >> 16 | append[1] << 16;
      block1[1] = append[1] >> 16 | append[2] << 16;
      block1[2] = append[2] >> 16 | append[3] << 16;
      block1[3] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 15:
      block0[3] = block0[3]       | append[0] << 24;
      block1[0] = append[0] >>  8 | append[1] << 24;
      block1[1] = append[1] >>  8 | append[2] << 24;
      block1[2] = append[2] >>  8 | append[3] << 24;
      block1[3] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 16:
      block1[0] = append[0];
      block1[1] = append[1];
      block1[2] = append[2];
      block1[3] = append[3];
      block2[0] = 0x80;
      break;

    case 17:
      block1[0] = block1[0]       | append[0] <<  8;
      block1[1] = append[0] >> 24 | append[1] <<  8;
      block1[2] = append[1] >> 24 | append[2] <<  8;
      block1[3] = append[2] >> 24 | append[3] <<  8;
      block2[0] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 18:
      block1[0] = block1[0]       | append[0] << 16;
      block1[1] = append[0] >> 16 | append[1] << 16;
      block1[2] = append[1] >> 16 | append[2] << 16;
      block1[3] = append[2] >> 16 | append[3] << 16;
      block2[0] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 19:
      block1[0] = block1[0]       | append[0] << 24;
      block1[1] = append[0] >>  8 | append[1] << 24;
      block1[2] = append[1] >>  8 | append[2] << 24;
      block1[3] = append[2] >>  8 | append[3] << 24;
      block2[0] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 20:
      block1[1] = append[0];
      block1[2] = append[1];
      block1[3] = append[2];
      block2[0] = append[3];
      block2[1] = 0x80;
      break;

    case 21:
      block1[1] = block1[1]       | append[0] <<  8;
      block1[2] = append[0] >> 24 | append[1] <<  8;
      block1[3] = append[1] >> 24 | append[2] <<  8;
      block2[0] = append[2] >> 24 | append[3] <<  8;
      block2[1] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 22:
      block1[1] = block1[1]       | append[0] << 16;
      block1[2] = append[0] >> 16 | append[1] << 16;
      block1[3] = append[1] >> 16 | append[2] << 16;
      block2[0] = append[2] >> 16 | append[3] << 16;
      block2[1] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 23:
      block1[1] = block1[1]       | append[0] << 24;
      block1[2] = append[0] >>  8 | append[1] << 24;
      block1[3] = append[1] >>  8 | append[2] << 24;
      block2[0] = append[2] >>  8 | append[3] << 24;
      block2[1] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 24:
      block1[2] = append[0];
      block1[3] = append[1];
      block2[0] = append[2];
      block2[1] = append[3];
      block2[2] = 0x80;
      break;

    case 25:
      block1[2] = block1[2]       | append[0] <<  8;
      block1[3] = append[0] >> 24 | append[1] <<  8;
      block2[0] = append[1] >> 24 | append[2] <<  8;
      block2[1] = append[2] >> 24 | append[3] <<  8;
      block2[2] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 26:
      block1[2] = block1[2]       | append[0] << 16;
      block1[3] = append[0] >> 16 | append[1] << 16;
      block2[0] = append[1] >> 16 | append[2] << 16;
      block2[1] = append[2] >> 16 | append[3] << 16;
      block2[2] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 27:
      block1[2] = block1[2]       | append[0] << 24;
      block1[3] = append[0] >>  8 | append[1] << 24;
      block2[0] = append[1] >>  8 | append[2] << 24;
      block2[1] = append[2] >>  8 | append[3] << 24;
      block2[2] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 28:
      block1[3] = append[0];
      block2[0] = append[1];
      block2[1] = append[2];
      block2[2] = append[3];
      block2[3] = 0x80;
      break;

    case 29:
      block1[3] = block1[3]       | append[0] <<  8;
      block2[0] = append[0] >> 24 | append[1] <<  8;
      block2[1] = append[1] >> 24 | append[2] <<  8;
      block2[2] = append[2] >> 24 | append[3] <<  8;
      block2[3] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 30:
      block1[3] = block1[3]       | append[0] << 16;
      block2[0] = append[0] >> 16 | append[1] << 16;
      block2[1] = append[1] >> 16 | append[2] << 16;
      block2[2] = append[2] >> 16 | append[3] << 16;
      block2[3] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 31:
      block1[3] = block1[3]       | append[0] << 24;
      block2[0] = append[0] >>  8 | append[1] << 24;
      block2[1] = append[1] >>  8 | append[2] << 24;
      block2[2] = append[2] >>  8 | append[3] << 24;
      block2[3] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 32:
      block2[0] = append[0];
      block2[1] = append[1];
      block2[2] = append[2];
      block2[3] = append[3];
      block3[0] = 0x80;
      break;

    case 33:
      block2[0] = block2[0]       | append[0] <<  8;
      block2[1] = append[0] >> 24 | append[1] <<  8;
      block2[2] = append[1] >> 24 | append[2] <<  8;
      block2[3] = append[2] >> 24 | append[3] <<  8;
      block3[0] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 34:
      block2[0] = block2[0]       | append[0] << 16;
      block2[1] = append[0] >> 16 | append[1] << 16;
      block2[2] = append[1] >> 16 | append[2] << 16;
      block2[3] = append[2] >> 16 | append[3] << 16;
      block3[0] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 35:
      block2[0] = block2[0]       | append[0] << 24;
      block2[1] = append[0] >>  8 | append[1] << 24;
      block2[2] = append[1] >>  8 | append[2] << 24;
      block2[3] = append[2] >>  8 | append[3] << 24;
      block3[0] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 36:
      block2[1] = append[0];
      block2[2] = append[1];
      block2[3] = append[2];
      block3[0] = append[3];
      block3[1] = 0x80;
      break;

    case 37:
      block2[1] = block2[1]       | append[0] <<  8;
      block2[2] = append[0] >> 24 | append[1] <<  8;
      block2[3] = append[1] >> 24 | append[2] <<  8;
      block3[0] = append[2] >> 24 | append[3] <<  8;
      block3[1] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 38:
      block2[1] = block2[1]       | append[0] << 16;
      block2[2] = append[0] >> 16 | append[1] << 16;
      block2[3] = append[1] >> 16 | append[2] << 16;
      block3[0] = append[2] >> 16 | append[3] << 16;
      block3[1] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 39:
      block2[1] = block2[1]       | append[0] << 24;
      block2[2] = append[0] >>  8 | append[1] << 24;
      block2[3] = append[1] >>  8 | append[2] << 24;
      block3[0] = append[2] >>  8 | append[3] << 24;
      block3[1] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 40:
      block2[2] = append[0];
      block2[3] = append[1];
      block3[0] = append[2];
      block3[1] = append[3];
      block3[2] = 0x80;
      break;

    case 41:
      block2[2] = block2[2]       | append[0] <<  8;
      block2[3] = append[0] >> 24 | append[1] <<  8;
      block3[0] = append[1] >> 24 | append[2] <<  8;
      block3[1] = append[2] >> 24 | append[3] <<  8;
      block3[2] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 42:
      block2[2] = block2[2]       | append[0] << 16;
      block2[3] = append[0] >> 16 | append[1] << 16;
      block3[0] = append[1] >> 16 | append[2] << 16;
      block3[1] = append[2] >> 16 | append[3] << 16;
      block3[2] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 43:
      block2[2] = block2[2]       | append[0] << 24;
      block2[3] = append[0] >>  8 | append[1] << 24;
      block3[0] = append[1] >>  8 | append[2] << 24;
      block3[1] = append[2] >>  8 | append[3] << 24;
      block3[2] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 44:
      block2[3] = append[0];
      block3[0] = append[1];
      block3[1] = append[2];
      block3[2] = append[3];
      block3[3] = 0x80;
      break;

    case 45:
      block2[3] = block2[3]       | append[0] <<  8;
      block3[0] = append[0] >> 24 | append[1] <<  8;
      block3[1] = append[1] >> 24 | append[2] <<  8;
      block3[2] = append[2] >> 24 | append[3] <<  8;
      block3[3] = append[3] >> 24 | 0x80u     <<  8;
      break;

    case 46:
      block2[3] = block2[3]       | append[0] << 16;
      block3[0] = append[0] >> 16 | append[1] << 16;
      block3[1] = append[1] >> 16 | append[2] << 16;
      block3[2] = append[2] >> 16 | append[3] << 16;
      block3[3] = append[3] >> 16 | 0x80u     << 16;
      break;

    case 47:
      block2[3] = block2[3]       | append[0] << 24;
      block3[0] = append[0] >>  8 | append[1] << 24;
      block3[1] = append[1] >>  8 | append[2] << 24;
      block3[2] = append[2] >>  8 | append[3] << 24;
      block3[3] = append[3] >>  8 | 0x80u     << 24;
      break;

    case 48:
      block3[0] = append[0];
      block3[1] = append[1];
      block3[2] = append[2];
      block3[3] = append[3];
      break;

    case 49:
      block3[0] = block3[0]       | append[0] <<  8;
      block3[1] = append[0] >> 24 | append[1] <<  8;
      block3[2] = append[1] >> 24 | append[2] <<  8;
      block3[3] = append[2] >> 24 | append[3] <<  8;
      break;

    case 50:
      block3[0] = block3[0]       | append[0] << 16;
      block3[1] = append[0] >> 16 | append[1] << 16;
      block3[2] = append[1] >> 16 | append[2] << 16;
      block3[3] = append[2] >> 16 | append[3] << 16;
      break;

    case 51:
      block3[0] = block3[0]       | append[0] << 24;
      block3[1] = append[0] >>  8 | append[1] << 24;
      block3[2] = append[1] >>  8 | append[2] << 24;
      block3[3] = append[2] >>  8 | append[3] << 24;
      break;

    case 52:
      block3[1] = append[0];
      block3[2] = append[1];
      block3[3] = append[2];
      break;

    case 53:
      block3[1] = block3[1]       | append[0] <<  8;
      block3[2] = append[0] >> 24 | append[1] <<  8;
      block3[3] = append[1] >> 24 | append[2] <<  8;
      break;

    case 54:
      block3[1] = block3[1]       | append[0] << 16;
      block3[2] = append[0] >> 16 | append[1] << 16;
      block3[3] = append[1] >> 16 | append[2] << 16;
      break;

    case 55:
      block3[1] = block3[1]       | append[0] << 24;
      block3[2] = append[0] >>  8 | append[1] << 24;
      block3[3] = append[1] >>  8 | append[2] << 24;
      break;

    case 56:
      block3[2] = append[0];
      block3[3] = append[1];
      break;
  }
}

static void memcat8 (u32x block0[4], u32x block1[4], u32x block2[4], u32x block3[4], const u32 block_len, const u32 append[2])
{
  switch (block_len)
  {
    case 0:
      block0[0] = append[0];
      block0[1] = append[1];
      break;

    case 1:
      block0[0] = block0[0]       | append[0] <<  8;
      block0[1] = append[0] >> 24 | append[1] <<  8;
      block0[2] = append[1] >> 24;
      break;

    case 2:
      block0[0] = block0[0]       | append[0] << 16;
      block0[1] = append[0] >> 16 | append[1] << 16;
      block0[2] = append[1] >> 16;
      break;

    case 3:
      block0[0] = block0[0]       | append[0] << 24;
      block0[1] = append[0] >>  8 | append[1] << 24;
      block0[2] = append[1] >>  8;
      break;

    case 4:
      block0[1] = append[0];
      block0[2] = append[1];
      break;

    case 5:
      block0[1] = block0[1]       | append[0] <<  8;
      block0[2] = append[0] >> 24 | append[1] <<  8;
      block0[3] = append[1] >> 24;
      break;

    case 6:
      block0[1] = block0[1]       | append[0] << 16;
      block0[2] = append[0] >> 16 | append[1] << 16;
      block0[3] = append[1] >> 16;
      break;

    case 7:
      block0[1] = block0[1]       | append[0] << 24;
      block0[2] = append[0] >>  8 | append[1] << 24;
      block0[3] = append[1] >>  8;
      break;

    case 8:
      block0[2] = append[0];
      block0[3] = append[1];
      break;

    case 9:
      block0[2] = block0[2]       | append[0] <<  8;
      block0[3] = append[0] >> 24 | append[1] <<  8;
      block1[0] = append[1] >> 24;
      break;

    case 10:
      block0[2] = block0[2]       | append[0] << 16;
      block0[3] = append[0] >> 16 | append[1] << 16;
      block1[0] = append[1] >> 16;
      break;

    case 11:
      block0[2] = block0[2]       | append[0] << 24;
      block0[3] = append[0] >>  8 | append[1] << 24;
      block1[0] = append[1] >>  8;
      break;

    case 12:
      block0[3] = append[0];
      block1[0] = append[1];
      break;

    case 13:
      block0[3] = block0[3]       | append[0] <<  8;
      block1[0] = append[0] >> 24 | append[1] <<  8;
      block1[1] = append[1] >> 24;
      break;

    case 14:
      block0[3] = block0[3]       | append[0] << 16;
      block1[0] = append[0] >> 16 | append[1] << 16;
      block1[1] = append[1] >> 16;
      break;

    case 15:
      block0[3] = block0[3]       | append[0] << 24;
      block1[0] = append[0] >>  8 | append[1] << 24;
      block1[1] = append[1] >>  8;
      break;

    case 16:
      block1[0] = append[0];
      block1[1] = append[1];
      break;

    case 17:
      block1[0] = block1[0]       | append[0] <<  8;
      block1[1] = append[0] >> 24 | append[1] <<  8;
      block1[2] = append[1] >> 24;
      break;

    case 18:
      block1[0] = block1[0]       | append[0] << 16;
      block1[1] = append[0] >> 16 | append[1] << 16;
      block1[2] = append[1] >> 16;
      break;

    case 19:
      block1[0] = block1[0]       | append[0] << 24;
      block1[1] = append[0] >>  8 | append[1] << 24;
      block1[2] = append[1] >>  8;
      break;

    case 20:
      block1[1] = append[0];
      block1[2] = append[1];
      break;

    case 21:
      block1[1] = block1[1]       | append[0] <<  8;
      block1[2] = append[0] >> 24 | append[1] <<  8;
      block1[3] = append[1] >> 24;
      break;

    case 22:
      block1[1] = block1[1]       | append[0] << 16;
      block1[2] = append[0] >> 16 | append[1] << 16;
      block1[3] = append[1] >> 16;
      break;

    case 23:
      block1[1] = block1[1]       | append[0] << 24;
      block1[2] = append[0] >>  8 | append[1] << 24;
      block1[3] = append[1] >>  8;
      break;

    case 24:
      block1[2] = append[0];
      block1[3] = append[1];
      break;

    case 25:
      block1[2] = block1[2]       | append[0] <<  8;
      block1[3] = append[0] >> 24 | append[1] <<  8;
      block2[0] = append[1] >> 24;
      break;

    case 26:
      block1[2] = block1[2]       | append[0] << 16;
      block1[3] = append[0] >> 16 | append[1] << 16;
      block2[0] = append[1] >> 16;
      break;

    case 27:
      block1[2] = block1[2]       | append[0] << 24;
      block1[3] = append[0] >>  8 | append[1] << 24;
      block2[0] = append[1] >>  8;
      break;

    case 28:
      block1[3] = append[0];
      block2[0] = append[1];
      break;

    case 29:
      block1[3] = block1[3]       | append[0] <<  8;
      block2[0] = append[0] >> 24 | append[1] <<  8;
      block2[1] = append[1] >> 24;
      break;

    case 30:
      block1[3] = block1[3]       | append[0] << 16;
      block2[0] = append[0] >> 16 | append[1] << 16;
      block2[1] = append[1] >> 16;
      break;

    case 31:
      block1[3] = block1[3]       | append[0] << 24;
      block2[0] = append[0] >>  8 | append[1] << 24;
      block2[1] = append[1] >>  8;
      break;

    case 32:
      block2[0] = append[0];
      block2[1] = append[1];
      break;

    case 33:
      block2[0] = block2[0]       | append[0] <<  8;
      block2[1] = append[0] >> 24 | append[1] <<  8;
      block2[2] = append[1] >> 24;
      break;

    case 34:
      block2[0] = block2[0]       | append[0] << 16;
      block2[1] = append[0] >> 16 | append[1] << 16;
      block2[2] = append[1] >> 16;
      break;

    case 35:
      block2[0] = block2[0]       | append[0] << 24;
      block2[1] = append[0] >>  8 | append[1] << 24;
      block2[2] = append[1] >>  8;
      break;

    case 36:
      block2[1] = append[0];
      block2[2] = append[1];
      break;

    case 37:
      block2[1] = block2[1]       | append[0] <<  8;
      block2[2] = append[0] >> 24 | append[1] <<  8;
      block2[3] = append[1] >> 24;
      break;

    case 38:
      block2[1] = block2[1]       | append[0] << 16;
      block2[2] = append[0] >> 16 | append[1] << 16;
      block2[3] = append[1] >> 16;
      break;

    case 39:
      block2[1] = block2[1]       | append[0] << 24;
      block2[2] = append[0] >>  8 | append[1] << 24;
      block2[3] = append[1] >>  8;
      break;

    case 40:
      block2[2] = append[0];
      block2[3] = append[1];
      break;

    case 41:
      block2[2] = block2[2]       | append[0] <<  8;
      block2[3] = append[0] >> 24 | append[1] <<  8;
      block3[0] = append[1] >> 24;
      break;

    case 42:
      block2[2] = block2[2]       | append[0] << 16;
      block2[3] = append[0] >> 16 | append[1] << 16;
      block3[0] = append[1] >> 16;
      break;

    case 43:
      block2[2] = block2[2]       | append[0] << 24;
      block2[3] = append[0] >>  8 | append[1] << 24;
      block3[0] = append[1] >>  8;
      break;

    case 44:
      block2[3] = append[0];
      block3[0] = append[1];
      break;

    case 45:
      block2[3] = block2[3]       | append[0] <<  8;
      block3[0] = append[0] >> 24 | append[1] <<  8;
      block3[1] = append[1] >> 24;
      break;

    case 46:
      block2[3] = block2[3]       | append[0] << 16;
      block3[0] = append[0] >> 16 | append[1] << 16;
      block3[1] = append[1] >> 16;
      break;

    case 47:
      block2[3] = block2[3]       | append[0] << 24;
      block3[0] = append[0] >>  8 | append[1] << 24;
      block3[1] = append[1] >>  8;
      break;

    case 48:
      block3[0] = append[0];
      block3[1] = append[1];
      break;

    case 49:
      block3[0] = block3[0]       | append[0] <<  8;
      block3[1] = append[0] >> 24 | append[1] <<  8;
      block3[2] = append[1] >> 24;
      break;

    case 50:
      block3[0] = block3[0]       | append[0] << 16;
      block3[1] = append[0] >> 16 | append[1] << 16;
      block3[2] = append[1] >> 16;
      break;

    case 51:
      block3[0] = block3[0]       | append[0] << 24;
      block3[1] = append[0] >>  8 | append[1] << 24;
      block3[2] = append[1] >>  8;
      break;

    case 52:
      block3[1] = append[0];
      block3[2] = append[1];
      break;

    case 53:
      block3[1] = block3[1]       | append[0] <<  8;
      block3[2] = append[0] >> 24 | append[1] <<  8;
      block3[3] = append[1] >> 24;
      break;

    case 54:
      block3[1] = block3[1]       | append[0] << 16;
      block3[2] = append[0] >> 16 | append[1] << 16;
      block3[3] = append[1] >> 16;
      break;

    case 55:
      block3[1] = block3[1]       | append[0] << 24;
      block3[2] = append[0] >>  8 | append[1] << 24;
      block3[3] = append[1] >>  8;
      break;

    case 56:
      block3[2] = append[0];
      block3[3] = append[1];
      break;
  }
}

static void append_sign (u32x block0[4], u32x block1[4], const u32 block_len)
{
  switch (block_len)
  {
    case 0:
      block0[0] = md5apr1_magic0;
      block0[1] = md5apr1_magic1;
      break;

    case 1:
      block0[0] = block0[0]            | md5apr1_magic0 <<  8;
      block0[1] = md5apr1_magic0 >> 24 | md5apr1_magic1 <<  8;
      block0[2] = md5apr1_magic1 >> 24;
      break;

    case 2:
      block0[0] = block0[0]            | md5apr1_magic0 << 16;
      block0[1] = md5apr1_magic0 >> 16 | md5apr1_magic1 << 16;
      block0[2] = md5apr1_magic1 >> 16;
      break;

    case 3:
      block0[0] = block0[0]            | md5apr1_magic0 << 24;
      block0[1] = md5apr1_magic0 >>  8 | md5apr1_magic1 << 24;
      block0[2] = md5apr1_magic1 >>  8;
      break;

    case 4:
      block0[1] = md5apr1_magic0;
      block0[2] = md5apr1_magic1;
      break;

    case 5:
      block0[1] = block0[1]            | md5apr1_magic0 <<  8;
      block0[2] = md5apr1_magic0 >> 24 | md5apr1_magic1 <<  8;
      block0[3] = md5apr1_magic1 >> 24;
      break;

    case 6:
      block0[1] = block0[1]            | md5apr1_magic0 << 16;
      block0[2] = md5apr1_magic0 >> 16 | md5apr1_magic1 << 16;
      block0[3] = md5apr1_magic1 >> 16;
      break;

    case 7:
      block0[1] = block0[1]            | md5apr1_magic0 << 24;
      block0[2] = md5apr1_magic0 >>  8 | md5apr1_magic1 << 24;
      block0[3] = md5apr1_magic1 >>  8;
      break;

    case 8:
      block0[2] = md5apr1_magic0;
      block0[3] = md5apr1_magic1;
      break;

    case 9:
      block0[2] = block0[2]            | md5apr1_magic0 <<  8;
      block0[3] = md5apr1_magic0 >> 24 | md5apr1_magic1 <<  8;
      block1[0] = md5apr1_magic1 >> 24;
      break;

    case 10:
      block0[2] = block0[2]            | md5apr1_magic0 << 16;
      block0[3] = md5apr1_magic0 >> 16 | md5apr1_magic1 << 16;
      block1[0] = md5apr1_magic1 >> 16;
      break;

    case 11:
      block0[2] = block0[2]            | md5apr1_magic0 << 24;
      block0[3] = md5apr1_magic0 >>  8 | md5apr1_magic1 << 24;
      block1[0] = md5apr1_magic1 >>  8;
      break;

    case 12:
      block0[3] = md5apr1_magic0;
      block1[0] = md5apr1_magic1;
      break;

    case 13:
      block0[3] = block0[3]            | md5apr1_magic0 <<  8;
      block1[0] = md5apr1_magic0 >> 24 | md5apr1_magic1 <<  8;
      block1[1] = md5apr1_magic1 >> 24;
      break;

    case 14:
      block0[3] = block0[3]            | md5apr1_magic0 << 16;
      block1[0] = md5apr1_magic0 >> 16 | md5apr1_magic1 << 16;
      block1[1] = md5apr1_magic1 >> 16;
      break;

    case 15:
      block0[3] = block0[3]            | md5apr1_magic0 << 24;
      block1[0] = md5apr1_magic0 >>  8 | md5apr1_magic1 << 24;
      block1[1] = md5apr1_magic1 >>  8;
      break;
  }
}

static void append_1st (u32x block0[4], u32x block1[4], u32x block2[4], u32x block3[4], const u32 block_len, const u32x append)
{
  switch (block_len)
  {
    case 0:
      block0[0] = append;
      break;

    case 1:
      block0[0] = block0[0] | append <<  8;
      break;

    case 2:
      block0[0] = block0[0] | append << 16;
      break;

    case 3:
      block0[0] = block0[0] | append << 24;
      break;

    case 4:
      block0[1] = append;
      break;

    case 5:
      block0[1] = block0[1] | append <<  8;
      break;

    case 6:
      block0[1] = block0[1] | append << 16;
      break;

    case 7:
      block0[1] = block0[1] | append << 24;
      break;

    case 8:
      block0[2] = append;
      break;

    case 9:
      block0[2] = block0[2] | append <<  8;
      break;

    case 10:
      block0[2] = block0[2] | append << 16;
      break;

    case 11:
      block0[2] = block0[2] | append << 24;
      break;

    case 12:
      block0[3] = append;
      break;

    case 13:
      block0[3] = block0[3] | append <<  8;
      break;

    case 14:
      block0[3] = block0[3] | append << 16;
      break;

    case 15:
      block0[3] = block0[3] | append << 24;
      break;

    case 16:
      block1[0] = append;
      break;

    case 17:
      block1[0] = block1[0] | append <<  8;
      break;

    case 18:
      block1[0] = block1[0] | append << 16;
      break;

    case 19:
      block1[0] = block1[0] | append << 24;
      break;

    case 20:
      block1[1] = append;
      break;

    case 21:
      block1[1] = block1[1] | append <<  8;
      break;

    case 22:
      block1[1] = block1[1] | append << 16;
      break;

    case 23:
      block1[1] = block1[1] | append << 24;
      break;

    case 24:
      block1[2] = append;
      break;

    case 25:
      block1[2] = block1[2] | append <<  8;
      break;

    case 26:
      block1[2] = block1[2] | append << 16;
      break;

    case 27:
      block1[2] = block1[2] | append << 24;
      break;

    case 28:
      block1[3] = append;
      break;

    case 29:
      block1[3] = block1[3] | append <<  8;
      break;

    case 30:
      block1[3] = block1[3] | append << 16;
      break;

    case 31:
      block1[3] = block1[3] | append << 24;
      break;

    case 32:
      block2[0] = append;
      break;

    case 33:
      block2[0] = block2[0] | append <<  8;
      break;

    case 34:
      block2[0] = block2[0] | append << 16;
      break;

    case 35:
      block2[0] = block2[0] | append << 24;
      break;

    case 36:
      block2[1] = append;
      break;

    case 37:
      block2[1] = block2[1] | append <<  8;
      break;

    case 38:
      block2[1] = block2[1] | append << 16;
      break;

    case 39:
      block2[1] = block2[1] | append << 24;
      break;

    case 40:
      block2[2] = append;
      break;

    case 41:
      block2[2] = block2[2] | append <<  8;
      break;

    case 42:
      block2[2] = block2[2] | append << 16;
      break;

    case 43:
      block2[2] = block2[2] | append << 24;
      break;

    case 44:
      block2[3] = append;
      break;

    case 45:
      block2[3] = block2[3] | append <<  8;
      break;

    case 46:
      block2[3] = block2[3] | append << 16;
      break;

    case 47:
      block2[3] = block2[3] | append << 24;
      break;

    case 48:
      block3[0] = append;
      break;

    case 49:
      block3[0] = block3[0] | append <<  8;
      break;

    case 50:
      block3[0] = block3[0] | append << 16;
      break;

    case 51:
      block3[0] = block3[0] | append << 24;
      break;

    case 52:
      block3[1] = append;
      break;

    case 53:
      block3[1] = block3[1] | append <<  8;
      break;

    case 54:
      block3[1] = block3[1] | append << 16;
      break;

    case 55:
      block3[1] = block3[1] | append << 24;
      break;

    case 56:
      block3[2] = append;
      break;
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01600_init (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global md5crypt_tmp_t *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 rules_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  u32x w0[4];

  w0[0] = pws[gid].i[0];
  w0[1] = pws[gid].i[1];
  w0[2] = pws[gid].i[2];
  w0[3] = pws[gid].i[3];

  const u32 pw_len = pws[gid].pw_len;

  /**
   * salt
   */

  u32 salt_buf[2];

  salt_buf[0] = salt_bufs[salt_pos].salt_buf[0];
  salt_buf[1] = salt_bufs[salt_pos].salt_buf[1];

  const u32 salt_len = salt_bufs[salt_pos].salt_len;

  /**
   * init
   */

  //memcat16 (block0, block1, block2, block3, block_len, w0);
  //block_len += pw_len;

  u32 block_len = pw_len;

  u32x block0[4];

  block0[0] = w0[0];
  block0[1] = w0[1];
  block0[2] = w0[2];
  block0[3] = w0[3];

  u32x block1[4];

  block1[0] = 0;
  block1[1] = 0;
  block1[2] = 0;
  block1[3] = 0;

  u32x block2[4];

  block2[0] = 0;
  block2[1] = 0;
  block2[2] = 0;
  block2[3] = 0;

  u32x block3[4];

  block3[0] = 0;
  block3[1] = 0;
  block3[2] = 0;
  block3[3] = 0;

  memcat8 (block0, block1, block2, block3, block_len, salt_buf);

  block_len += salt_len;

  memcat16 (block0, block1, block2, block3, block_len, w0);

  block_len += pw_len;

  append_0x80_4 (block0, block1, block2, block3, block_len);

  block3[2] = block_len * 8;

  u32x digest[4];

  digest[0] = MD5M_A;
  digest[1] = MD5M_B;
  digest[2] = MD5M_C;
  digest[3] = MD5M_D;

  md5_transform (block0, block1, block2, block3, digest);

  /* The password first, since that is what is most unknown */
  /* Then our magic string */
  /* Then the raw salt */
  /* Then just as many characters of the MD5(pw,salt,pw) */

  //memcat16 (block0, block1, block2, block3, block_len, w);
  //block_len += pw_len;

  block_len = pw_len;

  block0[0] = w0[0];
  block0[1] = w0[1];
  block0[2] = w0[2];
  block0[3] = w0[3];

  block1[0] = 0;
  block1[1] = 0;
  block1[2] = 0;
  block1[3] = 0;

  block2[0] = 0;
  block2[1] = 0;
  block2[2] = 0;
  block2[3] = 0;

  block3[0] = 0;
  block3[1] = 0;
  block3[2] = 0;
  block3[3] = 0;

  append_sign (block0, block1, block_len);

  block_len += 6;

  memcat8 (block0, block1, block2, block3, block_len, salt_buf);

  block_len += salt_len;

  truncate_block (digest, pw_len);

  memcat16 (block0, block1, block2, block3, block_len, digest);

  block_len += pw_len;

  /* Then something really weird... */

  u32x append = block0[0] & 0xFF;

  for (u32 j = pw_len; j; j >>= 1)
  {
    if ((j & 1) == 0)
    {
      append_1st (block0, block1, block2, block3, block_len, append);
    }

    block_len++;
  }

  append_0x80_4 (block0, block1, block2, block3, block_len);

  block3[2] = block_len * 8;

  digest[0] = MD5M_A;
  digest[1] = MD5M_B;
  digest[2] = MD5M_C;
  digest[3] = MD5M_D;

  md5_transform (block0, block1, block2, block3, digest);

  tmps[gid].digest_buf[0] = digest[0];
  tmps[gid].digest_buf[1] = digest[1];
  tmps[gid].digest_buf[2] = digest[2];
  tmps[gid].digest_buf[3] = digest[3];
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01600_loop (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global md5crypt_tmp_t *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 rules_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * base
   */

  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  u32x w0[4];

  w0[0] = pws[gid].i[0];
  w0[1] = pws[gid].i[1];
  w0[2] = pws[gid].i[2];
  w0[3] = pws[gid].i[3];

  const u32 pw_len = pws[gid].pw_len;

  u32x w0_x80[4];

  w0_x80[0] = w0[0];
  w0_x80[1] = w0[1];
  w0_x80[2] = w0[2];
  w0_x80[3] = w0[3];

  append_0x80_1 (w0_x80, pw_len);

  /**
   * salt
   */

  u32 salt_buf[2];

  salt_buf[0] = salt_bufs[salt_pos].salt_buf[0];
  salt_buf[1] = salt_bufs[salt_pos].salt_buf[1];

  const u32 salt_len = salt_bufs[salt_pos].salt_len;

  /**
   * digest
   */

  u32x digest[4];

  digest[0] = tmps[gid].digest_buf[0];
  digest[1] = tmps[gid].digest_buf[1];
  digest[2] = tmps[gid].digest_buf[2];
  digest[3] = tmps[gid].digest_buf[3];

  /**
   * loop
   */

  /* and now, just to make sure things don't run too fast */

  u32 block_len;

  u32x block0[4];

  block0[0] = 0;
  block0[1] = 0;
  block0[2] = 0;
  block0[3] = 0;

  u32x block1[4];

  block1[0] = 0;
  block1[1] = 0;
  block1[2] = 0;
  block1[3] = 0;

  u32x block2[4];

  block2[0] = 0;
  block2[1] = 0;
  block2[2] = 0;
  block2[3] = 0;

  u32x block3[4];

  block3[0] = 0;
  block3[1] = 0;
  block3[2] = 0;
  block3[3] = 0;

  for (u32 i = 0, j = loop_pos; i < loop_cnt; i++, j++)
  {
    block1[0] = 0;
    block1[1] = 0;
    block1[2] = 0;
    block1[3] = 0;
    block2[0] = 0;
    block2[1] = 0;
    block2[2] = 0;
    block2[3] = 0;
    block3[0] = 0;
    block3[1] = 0;

    const u32 j1 = (j & 1) ? 1 : 0;
    const u32 j3 = (j % 3) ? 1 : 0;
    const u32 j7 = (j % 7) ? 1 : 0;

    if (j1)
    {
      block0[0] = w0[0];
      block0[1] = w0[1];
      block0[2] = w0[2];
      block0[3] = w0[3];

      block_len = pw_len;

      if (j3)
      {
        memcat8 (block0, block1, block2, block3, block_len, salt_buf);

        block_len += salt_len;
      }

      if (j7)
      {
        memcat16 (block0, block1, block2, block3, block_len, w0);

        block_len += pw_len;
      }

      memcat16_x80 (block0, block1, block2, block3, block_len, digest);

      block_len += 16;
    }
    else
    {
      block0[0] = digest[0];
      block0[1] = digest[1];
      block0[2] = digest[2];
      block0[3] = digest[3];

      block_len = 16;

      if (j3 && j7)
      {
        block1[0] = salt_buf[0];
        block1[1] = salt_buf[1];

        block_len += salt_len;

        memcat16 (block0, block1, block2, block3, block_len, w0);

        block_len += pw_len;
      }
      else if (j3)
      {
        block1[0] = salt_buf[0];
        block1[1] = salt_buf[1];

        block_len += salt_len;
      }
      else if (j7)
      {
        block1[0] = w0[0];
        block1[1] = w0[1];
        block1[2] = w0[2];
        block1[3] = w0[3];

        block_len += pw_len;
      }

      memcat16 (block0, block1, block2, block3, block_len, w0_x80);

      block_len += pw_len;
    }

    block3[2] = block_len * 8;

    digest[0] = MD5M_A;
    digest[1] = MD5M_B;
    digest[2] = MD5M_C;
    digest[3] = MD5M_D;

    md5_transform (block0, block1, block2, block3, digest);
  }

  tmps[gid].digest_buf[0] = digest[0];
  tmps[gid].digest_buf[1] = digest[1];
  tmps[gid].digest_buf[2] = digest[2];
  tmps[gid].digest_buf[3] = digest[3];
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01600_comp (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global md5crypt_tmp_t *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 rules_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  const u32 lid = get_local_id (0);

  /**
   * digest
   */

  const u32x r0 = tmps[gid].digest_buf[DGST_R0];
  const u32x r1 = tmps[gid].digest_buf[DGST_R1];
  const u32x r2 = tmps[gid].digest_buf[DGST_R2];
  const u32x r3 = tmps[gid].digest_buf[DGST_R3];

  #define il_pos 0

  #include VECT_COMPARE_M
}
