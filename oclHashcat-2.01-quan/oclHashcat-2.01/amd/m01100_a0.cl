/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define _MD4_

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
#define DGST_R1 3
#define DGST_R2 2
#define DGST_R3 1

#include "include/kernel_functions.c"
#include "types_amd.c"
#include "common_amd.c"
#include "include/rp_gpu.h"
#include "rp_amd.c"

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

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01100_m04 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 rules_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = get_local_id (0);

  /**
   * base
   */

  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  u32x pw_buf0[4];

  pw_buf0[0] = pws[gid].i[ 0];
  pw_buf0[1] = pws[gid].i[ 1];
  pw_buf0[2] = pws[gid].i[ 2];
  pw_buf0[3] = pws[gid].i[ 3];

  u32x pw_buf1[4];

  pw_buf1[0] = pws[gid].i[ 4];
  pw_buf1[1] = pws[gid].i[ 5];
  pw_buf1[2] = pws[gid].i[ 6];
  pw_buf1[3] = pws[gid].i[ 7];

  const u32 pw_len = pws[gid].pw_len;

  /**
   * salt
   */

  u32 salt_buf0[4];

  salt_buf0[0] = salt_bufs[salt_pos].salt_buf[ 0];
  salt_buf0[1] = salt_bufs[salt_pos].salt_buf[ 1];
  salt_buf0[2] = salt_bufs[salt_pos].salt_buf[ 2];
  salt_buf0[3] = salt_bufs[salt_pos].salt_buf[ 3];

  u32 salt_buf1[4];

  salt_buf1[0] = salt_bufs[salt_pos].salt_buf[ 4];
  salt_buf1[1] = salt_bufs[salt_pos].salt_buf[ 5];
  salt_buf1[2] = salt_bufs[salt_pos].salt_buf[ 6];
  salt_buf1[3] = salt_bufs[salt_pos].salt_buf[ 7];

  u32 salt_buf2[4];

  salt_buf2[0] = salt_bufs[salt_pos].salt_buf[ 8];
  salt_buf2[1] = salt_bufs[salt_pos].salt_buf[ 9];
  salt_buf2[2] = 0;
  salt_buf2[3] = 0;

  const u32 salt_len = salt_bufs[salt_pos].salt_len;

  /**
   * loop
   */

  for (u32 il_pos = 0; il_pos < rules_cnt; il_pos++)
  {
    u32x w0[4];

    w0[0] = pw_buf0[0];
    w0[1] = pw_buf0[1];
    w0[2] = pw_buf0[2];
    w0[3] = pw_buf0[3];

    u32x w1[4];

    w1[0] = pw_buf1[0];
    w1[1] = pw_buf1[1];
    w1[2] = pw_buf1[2];
    w1[3] = pw_buf1[3];

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

    const u32 out_len = apply_rules (rules_buf[il_pos].cmds, w0, w1, pw_len);

    append_0x80_2 (w0, w1, out_len);

    u32x w0_t[4];
    u32x w1_t[4];
    u32x w2_t[4];
    u32x w3_t[4];

    make_unicode (w0, w0_t, w1_t);
    make_unicode (w1, w2_t, w3_t);

    w3_t[2] = out_len * 8 * 2;

    u32x a = MD4M_A;
    u32x b = MD4M_B;
    u32x c = MD4M_C;
    u32x d = MD4M_D;

    MD4_STEP (MD4_Fo, a, b, c, d, w0_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w0_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w0_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w0_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w1_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w1_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w1_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w1_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w2_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w2_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w2_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w2_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w3_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w3_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w3_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w3_t[3], MD4C00, MD4S03);

    MD4_STEP (MD4_Go, a, b, c, d, w0_t[0], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[0], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[0], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[0], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[1], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[1], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[1], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[1], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[2], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[2], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[2], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[2], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[3], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[3], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[3], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[3], MD4C01, MD4S13);

    MD4_STEP (MD4_H , a, b, c, d, w0_t[0], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[0], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[0], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[0], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[2], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[2], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[2], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[2], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[1], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[1], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[1], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[1], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[3], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[3], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[3], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[3], MD4C02, MD4S23);

    a += MD4M_A;
    b += MD4M_B;
    c += MD4M_C;
    d += MD4M_D;

    w0_t[0] = a;
    w0_t[1] = b;
    w0_t[2] = c;
    w0_t[3] = d;
    w1_t[0] = salt_buf0[0];
    w1_t[1] = salt_buf0[1];
    w1_t[2] = salt_buf0[2];
    w1_t[3] = salt_buf0[3];
    w2_t[0] = salt_buf1[0];
    w2_t[1] = salt_buf1[1];
    w2_t[2] = salt_buf1[2];
    w2_t[3] = salt_buf1[3];
    w3_t[0] = salt_buf2[0];
    w3_t[1] = salt_buf2[1];
    w3_t[2] = (16 + salt_len) * 8;
    w3_t[3] = 0;

    a = MD4M_A;
    b = MD4M_B;
    c = MD4M_C;
    d = MD4M_D;

    MD4_STEP (MD4_Fo, a, b, c, d, w0_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w0_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w0_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w0_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w1_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w1_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w1_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w1_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w2_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w2_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w2_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w2_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w3_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w3_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w3_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w3_t[3], MD4C00, MD4S03);

    MD4_STEP (MD4_Go, a, b, c, d, w0_t[0], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[0], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[0], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[0], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[1], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[1], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[1], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[1], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[2], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[2], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[2], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[2], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[3], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[3], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[3], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[3], MD4C01, MD4S13);

    MD4_STEP (MD4_H , a, b, c, d, w0_t[0], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[0], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[0], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[0], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[2], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[2], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[2], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[2], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[1], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[1], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[1], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[1], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[3], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[3], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[3], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[3], MD4C02, MD4S23);

    const u32x r0 = a;
    const u32x r1 = d;
    const u32x r2 = c;
    const u32x r3 = b;

    #include VECT_COMPARE_M
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01100_m08 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01100_m16 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01100_s04 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 rules_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = get_local_id (0);

  /**
   * base
   */

  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  u32x pw_buf0[4];

  pw_buf0[0] = pws[gid].i[ 0];
  pw_buf0[1] = pws[gid].i[ 1];
  pw_buf0[2] = pws[gid].i[ 2];
  pw_buf0[3] = pws[gid].i[ 3];

  u32x pw_buf1[4];

  pw_buf1[0] = pws[gid].i[ 4];
  pw_buf1[1] = pws[gid].i[ 5];
  pw_buf1[2] = pws[gid].i[ 6];
  pw_buf1[3] = pws[gid].i[ 7];

  const u32 pw_len = pws[gid].pw_len;

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

  u32 salt_buf0[4];

  salt_buf0[0] = salt_bufs[salt_pos].salt_buf[ 0];
  salt_buf0[1] = salt_bufs[salt_pos].salt_buf[ 1];
  salt_buf0[2] = salt_bufs[salt_pos].salt_buf[ 2];
  salt_buf0[3] = salt_bufs[salt_pos].salt_buf[ 3];

  u32 salt_buf1[4];

  salt_buf1[0] = salt_bufs[salt_pos].salt_buf[ 4];
  salt_buf1[1] = salt_bufs[salt_pos].salt_buf[ 5];
  salt_buf1[2] = salt_bufs[salt_pos].salt_buf[ 6];
  salt_buf1[3] = salt_bufs[salt_pos].salt_buf[ 7];

  u32 salt_buf2[4];

  salt_buf2[0] = salt_bufs[salt_pos].salt_buf[ 8];
  salt_buf2[1] = salt_bufs[salt_pos].salt_buf[ 9];
  salt_buf2[2] = 0;
  salt_buf2[3] = 0;

  const u32 salt_len = salt_bufs[salt_pos].salt_len;

  /**
   * loop
   */

  for (u32 il_pos = 0; il_pos < rules_cnt; il_pos++)
  {
    u32x w0[4];

    w0[0] = pw_buf0[0];
    w0[1] = pw_buf0[1];
    w0[2] = pw_buf0[2];
    w0[3] = pw_buf0[3];

    u32x w1[4];

    w1[0] = pw_buf1[0];
    w1[1] = pw_buf1[1];
    w1[2] = pw_buf1[2];
    w1[3] = pw_buf1[3];

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

    const u32 out_len = apply_rules (rules_buf[il_pos].cmds, w0, w1, pw_len);

    append_0x80_2 (w0, w1, out_len);

    u32x w0_t[4];
    u32x w1_t[4];
    u32x w2_t[4];
    u32x w3_t[4];

    make_unicode (w0, w0_t, w1_t);
    make_unicode (w1, w2_t, w3_t);

    w3_t[2] = out_len * 8 * 2;

    u32x a = MD4M_A;
    u32x b = MD4M_B;
    u32x c = MD4M_C;
    u32x d = MD4M_D;

    MD4_STEP (MD4_Fo, a, b, c, d, w0_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w0_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w0_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w0_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w1_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w1_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w1_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w1_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w2_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w2_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w2_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w2_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w3_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w3_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w3_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w3_t[3], MD4C00, MD4S03);

    MD4_STEP (MD4_Go, a, b, c, d, w0_t[0], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[0], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[0], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[0], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[1], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[1], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[1], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[1], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[2], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[2], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[2], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[2], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[3], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[3], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[3], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[3], MD4C01, MD4S13);

    MD4_STEP (MD4_H , a, b, c, d, w0_t[0], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[0], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[0], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[0], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[2], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[2], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[2], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[2], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[1], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[1], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[1], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[1], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[3], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[3], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[3], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[3], MD4C02, MD4S23);

    a += MD4M_A;
    b += MD4M_B;
    c += MD4M_C;
    d += MD4M_D;

    w0_t[0] = a;
    w0_t[1] = b;
    w0_t[2] = c;
    w0_t[3] = d;
    w1_t[0] = salt_buf0[0];
    w1_t[1] = salt_buf0[1];
    w1_t[2] = salt_buf0[2];
    w1_t[3] = salt_buf0[3];
    w2_t[0] = salt_buf1[0];
    w2_t[1] = salt_buf1[1];
    w2_t[2] = salt_buf1[2];
    w2_t[3] = salt_buf1[3];
    w3_t[0] = salt_buf2[0];
    w3_t[1] = salt_buf2[1];
    w3_t[2] = (16 + salt_len) * 8;
    w3_t[3] = 0;

    a = MD4M_A;
    b = MD4M_B;
    c = MD4M_C;
    d = MD4M_D;

    MD4_STEP (MD4_Fo, a, b, c, d, w0_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w0_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w0_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w0_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w1_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w1_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w1_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w1_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w2_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w2_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w2_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w2_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w3_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w3_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w3_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w3_t[3], MD4C00, MD4S03);

    MD4_STEP (MD4_Go, a, b, c, d, w0_t[0], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[0], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[0], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[0], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[1], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[1], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[1], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[1], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[2], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[2], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[2], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[2], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[3], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[3], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[3], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[3], MD4C01, MD4S13);

    MD4_STEP (MD4_H , a, b, c, d, w0_t[0], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[0], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[0], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[0], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[2], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[2], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[2], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[2], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[1], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[1], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[1], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[1], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[3], MD4C02, MD4S20);

    bool q_cond = allx (search[0] != a);

    if (q_cond) continue;

    MD4_STEP (MD4_H , d, a, b, c, w2_t[3], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[3], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[3], MD4C02, MD4S23);

    const u32x r0 = a;
    const u32x r1 = d;
    const u32x r2 = c;
    const u32x r3 = b;

    #include VECT_COMPARE_S
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01100_s08 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m01100_s16 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}
