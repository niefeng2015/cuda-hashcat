/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define _SHA384_

#include "include/constants.h"
#include "include/kernel_vendor.h"

#ifdef  VLIW1
#define VECT_SIZE1
#endif

#ifdef  VLIW2
#define VECT_SIZE1
#endif

#define DGST_R0 6
#define DGST_R1 7
#define DGST_R2 4
#define DGST_R3 5

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

__device__ __constant__ u64 k_sha384[80] =
{
  SHA384C00, SHA384C01, SHA384C02, SHA384C03,
  SHA384C04, SHA384C05, SHA384C06, SHA384C07,
  SHA384C08, SHA384C09, SHA384C0a, SHA384C0b,
  SHA384C0c, SHA384C0d, SHA384C0e, SHA384C0f,
  SHA384C10, SHA384C11, SHA384C12, SHA384C13,
  SHA384C14, SHA384C15, SHA384C16, SHA384C17,
  SHA384C18, SHA384C19, SHA384C1a, SHA384C1b,
  SHA384C1c, SHA384C1d, SHA384C1e, SHA384C1f,
  SHA384C20, SHA384C21, SHA384C22, SHA384C23,
  SHA384C24, SHA384C25, SHA384C26, SHA384C27,
  SHA384C28, SHA384C29, SHA384C2a, SHA384C2b,
  SHA384C2c, SHA384C2d, SHA384C2e, SHA384C2f,
  SHA384C30, SHA384C31, SHA384C32, SHA384C33,
  SHA384C34, SHA384C35, SHA384C36, SHA384C37,
  SHA384C38, SHA384C39, SHA384C3a, SHA384C3b,
  SHA384C3c, SHA384C3d, SHA384C3e, SHA384C3f,
  SHA384C40, SHA384C41, SHA384C42, SHA384C43,
  SHA384C44, SHA384C45, SHA384C46, SHA384C47,
  SHA384C48, SHA384C49, SHA384C4a, SHA384C4b,
  SHA384C4c, SHA384C4d, SHA384C4e, SHA384C4f,
};

__device__ static void sha384_transform (const u32x w0[4], const u32x w1[4], const u32x w2[4], const u32x w3[4], u64x digest[8])
{
  u64x w0_t = hl32_to_64 (w0[0], w0[1]);
  u64x w1_t = hl32_to_64 (w0[2], w0[3]);
  u64x w2_t = hl32_to_64 (w1[0], w1[1]);
  u64x w3_t = hl32_to_64 (w1[2], w1[3]);
  u64x w4_t = hl32_to_64 (w2[0], w2[1]);
  u64x w5_t = hl32_to_64 (w2[2], w2[3]);
  u64x w6_t = hl32_to_64 (w3[0], w3[1]);
  u64x w7_t = 0;
  u64x w8_t = 0;
  u64x w9_t = 0;
  u64x wa_t = 0;
  u64x wb_t = 0;
  u64x wc_t = 0;
  u64x wd_t = 0;
  u64x we_t = 0;
  u64x wf_t = hl32_to_64 (w3[2], w3[3]);

  u64x a = digest[0];
  u64x b = digest[1];
  u64x c = digest[2];
  u64x d = digest[3];
  u64x e = digest[4];
  u64x f = digest[5];
  u64x g = digest[6];
  u64x h = digest[7];

  #define ROUND_EXPAND()                            \
  {                                                 \
    w0_t = SHA384_EXPAND (we_t, w9_t, w1_t, w0_t);  \
    w1_t = SHA384_EXPAND (wf_t, wa_t, w2_t, w1_t);  \
    w2_t = SHA384_EXPAND (w0_t, wb_t, w3_t, w2_t);  \
    w3_t = SHA384_EXPAND (w1_t, wc_t, w4_t, w3_t);  \
    w4_t = SHA384_EXPAND (w2_t, wd_t, w5_t, w4_t);  \
    w5_t = SHA384_EXPAND (w3_t, we_t, w6_t, w5_t);  \
    w6_t = SHA384_EXPAND (w4_t, wf_t, w7_t, w6_t);  \
    w7_t = SHA384_EXPAND (w5_t, w0_t, w8_t, w7_t);  \
    w8_t = SHA384_EXPAND (w6_t, w1_t, w9_t, w8_t);  \
    w9_t = SHA384_EXPAND (w7_t, w2_t, wa_t, w9_t);  \
    wa_t = SHA384_EXPAND (w8_t, w3_t, wb_t, wa_t);  \
    wb_t = SHA384_EXPAND (w9_t, w4_t, wc_t, wb_t);  \
    wc_t = SHA384_EXPAND (wa_t, w5_t, wd_t, wc_t);  \
    wd_t = SHA384_EXPAND (wb_t, w6_t, we_t, wd_t);  \
    we_t = SHA384_EXPAND (wc_t, w7_t, wf_t, we_t);  \
    wf_t = SHA384_EXPAND (wd_t, w8_t, w0_t, wf_t);  \
  }

  #define ROUND_STEP(i)                                                                   \
  {                                                                                       \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, a, b, c, d, e, f, g, h, w0_t, k_sha384[i +  0]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, h, a, b, c, d, e, f, g, w1_t, k_sha384[i +  1]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, g, h, a, b, c, d, e, f, w2_t, k_sha384[i +  2]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, f, g, h, a, b, c, d, e, w3_t, k_sha384[i +  3]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, e, f, g, h, a, b, c, d, w4_t, k_sha384[i +  4]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, d, e, f, g, h, a, b, c, w5_t, k_sha384[i +  5]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, c, d, e, f, g, h, a, b, w6_t, k_sha384[i +  6]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, b, c, d, e, f, g, h, a, w7_t, k_sha384[i +  7]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, a, b, c, d, e, f, g, h, w8_t, k_sha384[i +  8]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, h, a, b, c, d, e, f, g, w9_t, k_sha384[i +  9]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, g, h, a, b, c, d, e, f, wa_t, k_sha384[i + 10]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, f, g, h, a, b, c, d, e, wb_t, k_sha384[i + 11]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, e, f, g, h, a, b, c, d, wc_t, k_sha384[i + 12]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, d, e, f, g, h, a, b, c, wd_t, k_sha384[i + 13]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, c, d, e, f, g, h, a, b, we_t, k_sha384[i + 14]); \
    SHA384_STEP (SHA384_F0o, SHA384_F1o, b, c, d, e, f, g, h, a, wf_t, k_sha384[i + 15]); \
  }

  ROUND_STEP (0);

  for (int i = 16; i < 80; i += 16)
  {
    ROUND_EXPAND (); ROUND_STEP (i);
  }

  /* rev
  digest[0] += a;
  digest[1] += b;
  digest[2] += c;
  digest[3] += d;
  digest[4] += e;
  digest[5] += f;
  digest[6] += g;
  digest[7] += h;
  */

  digest[0] = a;
  digest[1] = b;
  digest[2] = c;
  digest[3] = d;
  digest[4] = e;
  digest[5] = f;
  digest[6] = 0;
  digest[7] = 0;
}

extern "C" __global__ void __launch_bounds__ (256, 1) m10800_m04 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = threadIdx.x;

  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  if (gid >= gid_max) return;

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
    append_0x80_2 (wordl0, wordl1, pw_l_len);

    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, c_combs[0].pw_len);
  }

  /**
   * loop
   */

  for (u32 il_pos = 0; il_pos < combs_cnt; il_pos++)
  {
    const u32 pw_r_len = c_combs[il_pos].pw_len;

    const u32 pw_len = pw_l_len + pw_r_len;

    u32 wordr0[4];

    wordr0[0] = c_combs[il_pos].i[0];
    wordr0[1] = c_combs[il_pos].i[1];
    wordr0[2] = c_combs[il_pos].i[2];
    wordr0[3] = c_combs[il_pos].i[3];

    u32 wordr1[4];

    wordr1[0] = c_combs[il_pos].i[4];
    wordr1[1] = c_combs[il_pos].i[5];
    wordr1[2] = c_combs[il_pos].i[6];
    wordr1[3] = c_combs[il_pos].i[7];

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
      append_0x80_2 (wordr0, wordr1, pw_r_len);

      switch_buffer_by_offset (wordr0, wordr1, wordr2, wordr3, pw_l_len);
    }

    u32x w0[4];
    u32x w1[4];
    u32x w2[4];
    u32x w3[4];

    w0[0] = wordl0[0] | wordr0[0];
    w0[1] = wordl0[1] | wordr0[1];
    w0[2] = wordl0[2] | wordr0[2];
    w0[3] = wordl0[3] | wordr0[3];
    w1[0] = wordl1[0] | wordr1[0];
    w1[1] = wordl1[1] | wordr1[1];
    w1[2] = wordl1[2] | wordr1[2];
    w1[3] = wordl1[3] | wordr1[3];
    w2[0] = wordl2[0] | wordr2[0];
    w2[1] = wordl2[1] | wordr2[1];
    w2[2] = wordl2[2] | wordr2[2];
    w2[3] = wordl2[3] | wordr2[3];
    w3[0] = wordl3[0] | wordr3[0];
    w3[1] = wordl3[1] | wordr3[1];
    w3[2] = wordl3[2] | wordr3[2];
    w3[3] = wordl3[3] | wordr3[3];

    /**
     * SHA384
     */

    u32x w0_t[4];
    u32x w1_t[4];
    u32x w2_t[4];
    u32x w3_t[4];

    w0_t[0] = swap_workaround (w0[0]);
    w0_t[1] = swap_workaround (w0[1]);
    w0_t[2] = swap_workaround (w0[2]);
    w0_t[3] = swap_workaround (w0[3]);
    w1_t[0] = swap_workaround (w1[0]);
    w1_t[1] = swap_workaround (w1[1]);
    w1_t[2] = swap_workaround (w1[2]);
    w1_t[3] = swap_workaround (w1[3]);
    w2_t[0] = swap_workaround (w2[0]);
    w2_t[1] = swap_workaround (w2[1]);
    w2_t[2] = swap_workaround (w2[2]);
    w2_t[3] = swap_workaround (w2[3]);
    w3_t[0] = swap_workaround (w3[0]);
    w3_t[1] = swap_workaround (w3[1]);
    w3_t[2] = 0;
    w3_t[3] = pw_len * 8;

    u64x digest[8];

    digest[0] = SHA384M_A;
    digest[1] = SHA384M_B;
    digest[2] = SHA384M_C;
    digest[3] = SHA384M_D;
    digest[4] = SHA384M_E;
    digest[5] = SHA384M_F;
    digest[6] = SHA384M_G;
    digest[7] = SHA384M_H;

    sha384_transform (w0_t, w1_t, w2_t, w3_t, digest);

    const u32x r0 = l32_from_64 (digest[3]);
    const u32x r1 = h32_from_64 (digest[3]);
    const u32x r2 = l32_from_64 (digest[2]);
    const u32x r3 = h32_from_64 (digest[2]);

    #include VECT_COMPARE_M
  }
}

extern "C" __global__ void __launch_bounds__ (256, 1) m10800_m08 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

extern "C" __global__ void __launch_bounds__ (256, 1) m10800_m16 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

extern "C" __global__ void __launch_bounds__ (256, 1) m10800_s04 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = threadIdx.x;

  /**
   * base
   */

  const u32 gid = (blockIdx.x * blockDim.x) + threadIdx.x;

  if (gid >= gid_max) return;

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
    append_0x80_2 (wordl0, wordl1, pw_l_len);

    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, c_combs[0].pw_len);
  }

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
    const u32 pw_r_len = c_combs[il_pos].pw_len;

    const u32 pw_len = pw_l_len + pw_r_len;

    u32 wordr0[4];

    wordr0[0] = c_combs[il_pos].i[0];
    wordr0[1] = c_combs[il_pos].i[1];
    wordr0[2] = c_combs[il_pos].i[2];
    wordr0[3] = c_combs[il_pos].i[3];

    u32 wordr1[4];

    wordr1[0] = c_combs[il_pos].i[4];
    wordr1[1] = c_combs[il_pos].i[5];
    wordr1[2] = c_combs[il_pos].i[6];
    wordr1[3] = c_combs[il_pos].i[7];

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
      append_0x80_2 (wordr0, wordr1, pw_r_len);

      switch_buffer_by_offset (wordr0, wordr1, wordr2, wordr3, pw_l_len);
    }

    u32x w0[4];
    u32x w1[4];
    u32x w2[4];
    u32x w3[4];

    w0[0] = wordl0[0] | wordr0[0];
    w0[1] = wordl0[1] | wordr0[1];
    w0[2] = wordl0[2] | wordr0[2];
    w0[3] = wordl0[3] | wordr0[3];
    w1[0] = wordl1[0] | wordr1[0];
    w1[1] = wordl1[1] | wordr1[1];
    w1[2] = wordl1[2] | wordr1[2];
    w1[3] = wordl1[3] | wordr1[3];
    w2[0] = wordl2[0] | wordr2[0];
    w2[1] = wordl2[1] | wordr2[1];
    w2[2] = wordl2[2] | wordr2[2];
    w2[3] = wordl2[3] | wordr2[3];
    w3[0] = wordl3[0] | wordr3[0];
    w3[1] = wordl3[1] | wordr3[1];
    w3[2] = wordl3[2] | wordr3[2];
    w3[3] = wordl3[3] | wordr3[3];

    /**
     * SHA384
     */

    u32x w0_t[4];
    u32x w1_t[4];
    u32x w2_t[4];
    u32x w3_t[4];

    w0_t[0] = swap_workaround (w0[0]);
    w0_t[1] = swap_workaround (w0[1]);
    w0_t[2] = swap_workaround (w0[2]);
    w0_t[3] = swap_workaround (w0[3]);
    w1_t[0] = swap_workaround (w1[0]);
    w1_t[1] = swap_workaround (w1[1]);
    w1_t[2] = swap_workaround (w1[2]);
    w1_t[3] = swap_workaround (w1[3]);
    w2_t[0] = swap_workaround (w2[0]);
    w2_t[1] = swap_workaround (w2[1]);
    w2_t[2] = swap_workaround (w2[2]);
    w2_t[3] = swap_workaround (w2[3]);
    w3_t[0] = swap_workaround (w3[0]);
    w3_t[1] = swap_workaround (w3[1]);
    w3_t[2] = 0;
    w3_t[3] = pw_len * 8;

    u64x digest[8];

    digest[0] = SHA384M_A;
    digest[1] = SHA384M_B;
    digest[2] = SHA384M_C;
    digest[3] = SHA384M_D;
    digest[4] = SHA384M_E;
    digest[5] = SHA384M_F;
    digest[6] = SHA384M_G;
    digest[7] = SHA384M_H;

    sha384_transform (w0_t, w1_t, w2_t, w3_t, digest);

    const u32x r0 = l32_from_64 (digest[3]);
    const u32x r1 = h32_from_64 (digest[3]);
    const u32x r2 = l32_from_64 (digest[2]);
    const u32x r3 = h32_from_64 (digest[2]);

    #include VECT_COMPARE_S
  }
}

extern "C" __global__ void __launch_bounds__ (256, 1) m10800_s08 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

extern "C" __global__ void __launch_bounds__ (256, 1) m10800_s16 (const pw_t *pws, const gpu_rule_t *rules_buf, const comb_t *combs_buf, const bf_t *bfs_buf, const void *tmps, void *hooks, const u32 *bitmaps_buf_s1_a, const u32 *bitmaps_buf_s1_b, const u32 *bitmaps_buf_s1_c, const u32 *bitmaps_buf_s1_d, const u32 *bitmaps_buf_s2_a, const u32 *bitmaps_buf_s2_b, const u32 *bitmaps_buf_s2_c, const u32 *bitmaps_buf_s2_d, plain_t *plains_buf, const digest_t *digests_buf, u32 *hashes_shown, const salt_t *salt_bufs, const void *esalt_bufs, u32 *d_return_buf, u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}
