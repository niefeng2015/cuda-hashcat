/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define _LOTUS6_

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

__constant u32 lotus_magic_table[256] =
{
  0xbd, 0x56, 0xea, 0xf2, 0xa2, 0xf1, 0xac, 0x2a,
  0xb0, 0x93, 0xd1, 0x9c, 0x1b, 0x33, 0xfd, 0xd0,
  0x30, 0x04, 0xb6, 0xdc, 0x7d, 0xdf, 0x32, 0x4b,
  0xf7, 0xcb, 0x45, 0x9b, 0x31, 0xbb, 0x21, 0x5a,
  0x41, 0x9f, 0xe1, 0xd9, 0x4a, 0x4d, 0x9e, 0xda,
  0xa0, 0x68, 0x2c, 0xc3, 0x27, 0x5f, 0x80, 0x36,
  0x3e, 0xee, 0xfb, 0x95, 0x1a, 0xfe, 0xce, 0xa8,
  0x34, 0xa9, 0x13, 0xf0, 0xa6, 0x3f, 0xd8, 0x0c,
  0x78, 0x24, 0xaf, 0x23, 0x52, 0xc1, 0x67, 0x17,
  0xf5, 0x66, 0x90, 0xe7, 0xe8, 0x07, 0xb8, 0x60,
  0x48, 0xe6, 0x1e, 0x53, 0xf3, 0x92, 0xa4, 0x72,
  0x8c, 0x08, 0x15, 0x6e, 0x86, 0x00, 0x84, 0xfa,
  0xf4, 0x7f, 0x8a, 0x42, 0x19, 0xf6, 0xdb, 0xcd,
  0x14, 0x8d, 0x50, 0x12, 0xba, 0x3c, 0x06, 0x4e,
  0xec, 0xb3, 0x35, 0x11, 0xa1, 0x88, 0x8e, 0x2b,
  0x94, 0x99, 0xb7, 0x71, 0x74, 0xd3, 0xe4, 0xbf,
  0x3a, 0xde, 0x96, 0x0e, 0xbc, 0x0a, 0xed, 0x77,
  0xfc, 0x37, 0x6b, 0x03, 0x79, 0x89, 0x62, 0xc6,
  0xd7, 0xc0, 0xd2, 0x7c, 0x6a, 0x8b, 0x22, 0xa3,
  0x5b, 0x05, 0x5d, 0x02, 0x75, 0xd5, 0x61, 0xe3,
  0x18, 0x8f, 0x55, 0x51, 0xad, 0x1f, 0x0b, 0x5e,
  0x85, 0xe5, 0xc2, 0x57, 0x63, 0xca, 0x3d, 0x6c,
  0xb4, 0xc5, 0xcc, 0x70, 0xb2, 0x91, 0x59, 0x0d,
  0x47, 0x20, 0xc8, 0x4f, 0x58, 0xe0, 0x01, 0xe2,
  0x16, 0x38, 0xc4, 0x6f, 0x3b, 0x0f, 0x65, 0x46,
  0xbe, 0x7e, 0x2d, 0x7b, 0x82, 0xf9, 0x40, 0xb5,
  0x1d, 0x73, 0xf8, 0xeb, 0x26, 0xc7, 0x87, 0x97,
  0x25, 0x54, 0xb1, 0x28, 0xaa, 0x98, 0x9d, 0xa5,
  0x64, 0x6d, 0x7a, 0xd4, 0x10, 0x81, 0x44, 0xef,
  0x49, 0xd6, 0xae, 0x2e, 0xdd, 0x76, 0x5c, 0x2f,
  0xa7, 0x1c, 0xc9, 0x09, 0x69, 0x9a, 0x83, 0xcf,
  0x29, 0x39, 0xb9, 0xe9, 0x4c, 0xff, 0x43, 0xab,
};

#ifdef VECT_SIZE1
#define BOX(S,i) (u32x) ((S)[(i)])
#endif

#ifdef VECT_SIZE2
#define BOX(S,i) (u32x) ((S)[(i).s0], (S)[(i).s1])
#endif

#ifdef VECT_SIZE4
#define BOX(S,i) (u32x) ((S)[(i).s0], (S)[(i).s1], (S)[(i).s2], (S)[(i).s3])
#endif

#ifdef VECT_SIZE1
#define uint_to_hex_upper8(i) (u32x) (l_bin2asc[(i)])
#endif

#ifdef VECT_SIZE2
#define uint_to_hex_upper8(i) (u32x) (l_bin2asc[(i).s0], l_bin2asc[(i).s1])
#endif

#ifdef VECT_SIZE4
#define uint_to_hex_upper8(i) (u32x) (l_bin2asc[(i).s0], l_bin2asc[(i).s1], l_bin2asc[(i).s2], l_bin2asc[(i).s3])
#endif

static void lotus_mix (u32x *in, __local u32 s_lotus_magic_table[256])
{
  u32x p = 0;

  for (int i = 0; i < 18; i++)
  {
    u32 s = 48;

    #pragma unroll
    for (int j = 0; j < 12; j++)
    {
      u32x tmp_in = in[j];
      u32x tmp_out = 0;

      p = (p + s--) & 0xff; p = ((tmp_in >>  0) & 0xff) ^ BOX (s_lotus_magic_table, p); tmp_out |= p <<  0;
      p = (p + s--) & 0xff; p = ((tmp_in >>  8) & 0xff) ^ BOX (s_lotus_magic_table, p); tmp_out |= p <<  8;
      p = (p + s--) & 0xff; p = ((tmp_in >> 16) & 0xff) ^ BOX (s_lotus_magic_table, p); tmp_out |= p << 16;
      p = (p + s--) & 0xff; p = ((tmp_in >> 24) & 0xff) ^ BOX (s_lotus_magic_table, p); tmp_out |= p << 24;

      in[j] = tmp_out;
    }
  }
}

static void lotus_transform_password (u32x *in, u32x *out, __local u32 s_lotus_magic_table[256])
{
  u32x t = out[3] >> 24;

  u32x c;

  //#pragma unroll // kernel fails if used
  for (int i = 0; i < 4; i++)
  {
    t ^= (in[i] >>  0) & 0xff; c = BOX (s_lotus_magic_table, t); out[i] ^= c <<  0; t = ((out[i] >>  0) & 0xff);
    t ^= (in[i] >>  8) & 0xff; c = BOX (s_lotus_magic_table, t); out[i] ^= c <<  8; t = ((out[i] >>  8) & 0xff);
    t ^= (in[i] >> 16) & 0xff; c = BOX (s_lotus_magic_table, t); out[i] ^= c << 16; t = ((out[i] >> 16) & 0xff);
    t ^= (in[i] >> 24) & 0xff; c = BOX (s_lotus_magic_table, t); out[i] ^= c << 24; t = ((out[i] >> 24) & 0xff);
  }
}

static void pad (u32x w[4], const u32 len)
{
  const u32 val = 16 - len;

  const u32 mask1 = val << 24;

  const u32 mask2 = val << 16
                   | val << 24;

  const u32 mask3 = val <<  8
                   | val << 16
                   | val << 24;

  const u32 mask4 = val <<  0
                   | val <<  8
                   | val << 16
                   | val << 24;

  switch (len)
  {
    case  0:  w[0]  = mask4;
              w[1]  = mask4;
              w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  1:  w[0] |= mask3;
              w[1]  = mask4;
              w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  2:  w[0] |= mask2;
              w[1]  = mask4;
              w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  3:  w[0] |= mask1;
              w[1]  = mask4;
              w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  4:  w[1]  = mask4;
              w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  5:  w[1] |= mask3;
              w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  6:  w[1] |= mask2;
              w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  7:  w[1] |= mask1;
              w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  8:  w[2]  = mask4;
              w[3]  = mask4;
              break;
    case  9:  w[2] |= mask3;
              w[3]  = mask4;
              break;
    case 10:  w[2] |= mask2;
              w[3]  = mask4;
              break;
    case 11:  w[2] |= mask1;
              w[3]  = mask4;
              break;
    case 12:  w[3]  = mask4;
              break;
    case 13:  w[3] |= mask3;
              break;
    case 14:  w[3] |= mask2;
              break;
    case 15:  w[3] |= mask1;
              break;
  }
}

static void mdtransform_norecalc (u32x state[4], u32x block[4], __local u32 s_lotus_magic_table[256])
{
	u32x x[12];

  x[ 0] = state[0];
  x[ 1] = state[1];
  x[ 2] = state[2];
  x[ 3] = state[3];
  x[ 4] = block[0];
  x[ 5] = block[1];
  x[ 6] = block[2];
  x[ 7] = block[3];
  x[ 8] = state[0] ^ block[0];
  x[ 9] = state[1] ^ block[1];
  x[10] = state[2] ^ block[2];
  x[11] = state[3] ^ block[3];

  lotus_mix (x, s_lotus_magic_table);

  state[0] = x[0];
  state[1] = x[1];
  state[2] = x[2];
  state[3] = x[3];
}

static void mdtransform (u32x state[4], u32x checksum[4], u32x block[4], __local u32 s_lotus_magic_table[256])
{
  mdtransform_norecalc (state, block, s_lotus_magic_table);

  lotus_transform_password (block, checksum, s_lotus_magic_table);
}

static void domino_big_md (const u32x saved_key[16], const u32 size, u32x state[4], __local u32 s_lotus_magic_table[256])
{
	u32x checksum[4];

  checksum[0] = 0;
  checksum[1] = 0;
  checksum[2] = 0;
  checksum[3] = 0;

	u32x block[4];

  block[0] = 0;
  block[1] = 0;
  block[2] = 0;
  block[3] = 0;

  u32 curpos;
  u32 idx;

  for (curpos = 0, idx = 0; curpos + 16 < size; curpos += 16, idx += 4)
  {
    block[0] = saved_key[idx + 0];
    block[1] = saved_key[idx + 1];
    block[2] = saved_key[idx + 2];
    block[3] = saved_key[idx + 3];

    mdtransform (state, checksum, block, s_lotus_magic_table);
  }

  block[0] = saved_key[idx + 0];
  block[1] = saved_key[idx + 1];
  block[2] = saved_key[idx + 2];
  block[3] = saved_key[idx + 3];

  mdtransform (state, checksum, block, s_lotus_magic_table);

	mdtransform_norecalc (state, checksum, s_lotus_magic_table);
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08700_m04 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 rules_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = get_local_id (0);

  /**
   * base
   */

  const u32 gid = get_global_id (0);

  /**
   * bin2asc table
   */

  __local u32 l_bin2asc[256];

  const u32 lid4 = lid * 4;

  const u32 lid40 = lid4 + 0;
  const u32 lid41 = lid4 + 1;
  const u32 lid42 = lid4 + 2;
  const u32 lid43 = lid4 + 3;

  const u32 v400 = (lid40 >> 0) & 15;
  const u32 v401 = (lid40 >> 4) & 15;
  const u32 v410 = (lid41 >> 0) & 15;
  const u32 v411 = (lid41 >> 4) & 15;
  const u32 v420 = (lid42 >> 0) & 15;
  const u32 v421 = (lid42 >> 4) & 15;
  const u32 v430 = (lid43 >> 0) & 15;
  const u32 v431 = (lid43 >> 4) & 15;

  l_bin2asc[lid40] = ((v400 < 10) ? '0' + v400 : 'A' - 10 + v400) << 8
                   | ((v401 < 10) ? '0' + v401 : 'A' - 10 + v401) << 0;
  l_bin2asc[lid41] = ((v410 < 10) ? '0' + v410 : 'A' - 10 + v410) << 8
                   | ((v411 < 10) ? '0' + v411 : 'A' - 10 + v411) << 0;
  l_bin2asc[lid42] = ((v420 < 10) ? '0' + v420 : 'A' - 10 + v420) << 8
                   | ((v421 < 10) ? '0' + v421 : 'A' - 10 + v421) << 0;
  l_bin2asc[lid43] = ((v430 < 10) ? '0' + v430 : 'A' - 10 + v430) << 8
                   | ((v431 < 10) ? '0' + v431 : 'A' - 10 + v431) << 0;

  barrier (CLK_LOCAL_MEM_FENCE);

  /**
   * sbox
   */

  __local u32 s_lotus_magic_table[256];

  s_lotus_magic_table[lid4 + 0] = lotus_magic_table[lid4 + 0];
  s_lotus_magic_table[lid4 + 1] = lotus_magic_table[lid4 + 1];
  s_lotus_magic_table[lid4 + 2] = lotus_magic_table[lid4 + 2];
  s_lotus_magic_table[lid4 + 3] = lotus_magic_table[lid4 + 3];

  barrier (CLK_LOCAL_MEM_FENCE);

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

  const u32 salt0 = salt_bufs[salt_pos].salt_buf[0];
  const u32 salt1 = salt_bufs[salt_pos].salt_buf[1] & 0xff | '(' << 8;

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

    u32x w[16];

    w[ 0] = w0[0];
    w[ 1] = w0[1];
    w[ 2] = w0[2];
    w[ 3] = w0[3];
    w[ 4] = w1[0];
    w[ 5] = w1[1];
    w[ 6] = w1[2];
    w[ 7] = w1[3];
    w[ 8] = w2[0];
    w[ 9] = w2[1];
    w[10] = w2[2];
    w[11] = w2[3];
    w[12] = w3[0];
    w[13] = w3[1];
    w[14] = w3[2];
    w[15] = w3[3];

    u32x state[4];

    state[0] = 0;
    state[1] = 0;
    state[2] = 0;
    state[3] = 0;

    /**
     * padding
     */

    if (pw_len < 16)
    {
      pad (&w[ 0], pw_len & 0xf);
    }
    else if (pw_len < 32)
    {
      pad (&w[ 4], pw_len & 0xf);
    }
    else if (pw_len < 48)
    {
      pad (&w[ 8], pw_len & 0xf);
    }
    else if (pw_len < 64)
    {
      pad (&w[12], pw_len & 0xf);
    }

    domino_big_md (w, pw_len, state, s_lotus_magic_table);

    const u32x w0_t = uint_to_hex_upper8 ((state[0] >>  0) & 255) <<  0
                     | uint_to_hex_upper8 ((state[0] >>  8) & 255) << 16;
    const u32x w1_t = uint_to_hex_upper8 ((state[0] >> 16) & 255) <<  0
                     | uint_to_hex_upper8 ((state[0] >> 24) & 255) << 16;
    const u32x w2_t = uint_to_hex_upper8 ((state[1] >>  0) & 255) <<  0
                     | uint_to_hex_upper8 ((state[1] >>  8) & 255) << 16;
    const u32x w3_t = uint_to_hex_upper8 ((state[1] >> 16) & 255) <<  0
                     | uint_to_hex_upper8 ((state[1] >> 24) & 255) << 16;
    const u32x w4_t = uint_to_hex_upper8 ((state[2] >>  0) & 255) <<  0
                     | uint_to_hex_upper8 ((state[2] >>  8) & 255) << 16;
    const u32x w5_t = uint_to_hex_upper8 ((state[2] >> 16) & 255) <<  0
                     | uint_to_hex_upper8 ((state[2] >> 24) & 255) << 16;
    const u32x w6_t = uint_to_hex_upper8 ((state[3] >>  0) & 255) <<  0
                     | uint_to_hex_upper8 ((state[3] >>  8) & 255) << 16;
    //const u32x w7_t = uint_to_hex_upper8 ((state[3] >> 16) & 255) <<  0
    //                 | uint_to_hex_upper8 ((state[3] >> 24) & 255) << 16;

    const u32 pade = 0x0e0e0e0e;

    w[ 0] = salt0;
    w[ 1] = salt1      | w0_t << 16;
    w[ 2] = w0_t >> 16 | w1_t << 16;
    w[ 3] = w1_t >> 16 | w2_t << 16;
    w[ 4] = w2_t >> 16 | w3_t << 16;
    w[ 5] = w3_t >> 16 | w4_t << 16;
    w[ 6] = w4_t >> 16 | w5_t << 16;
    w[ 7] = w5_t >> 16 | w6_t << 16;
    w[ 8] = w6_t >> 16 | pade << 16; // | w7_t <<  8;
    w[ 9] = pade;
    w[10] = pade;
    w[11] = pade;
    w[12] = 0;
    w[13] = 0;
    w[14] = 0;
    w[15] = 0;

    state[0] = 0;
    state[1] = 0;
    state[2] = 0;
    state[3] = 0;

    domino_big_md (w, 34, state, s_lotus_magic_table);

    u32x a = state[0] & 0xffffffff;
    u32x b = state[1] & 0xffffffff;
    u32x c = state[2] & 0x000000ff;
    u32x d = state[3] & 0x00000000;

    const u32x r0 = a;
    const u32x r1 = b;
    const u32x r2 = c;
    const u32x r3 = d;

    #include VECT_COMPARE_M
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08700_m08 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08700_m16 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08700_s04 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 rules_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
  /**
   * modifier
   */

  const u32 lid = get_local_id (0);

  /**
   * base
   */

  const u32 gid = get_global_id (0);

  /**
   * bin2asc table
   */

  __local u32 l_bin2asc[256];

  const u32 lid4 = lid * 4;

  const u32 lid40 = lid4 + 0;
  const u32 lid41 = lid4 + 1;
  const u32 lid42 = lid4 + 2;
  const u32 lid43 = lid4 + 3;

  const u32 v400 = (lid40 >> 0) & 15;
  const u32 v401 = (lid40 >> 4) & 15;
  const u32 v410 = (lid41 >> 0) & 15;
  const u32 v411 = (lid41 >> 4) & 15;
  const u32 v420 = (lid42 >> 0) & 15;
  const u32 v421 = (lid42 >> 4) & 15;
  const u32 v430 = (lid43 >> 0) & 15;
  const u32 v431 = (lid43 >> 4) & 15;

  l_bin2asc[lid40] = ((v400 < 10) ? '0' + v400 : 'A' - 10 + v400) << 8
                   | ((v401 < 10) ? '0' + v401 : 'A' - 10 + v401) << 0;
  l_bin2asc[lid41] = ((v410 < 10) ? '0' + v410 : 'A' - 10 + v410) << 8
                   | ((v411 < 10) ? '0' + v411 : 'A' - 10 + v411) << 0;
  l_bin2asc[lid42] = ((v420 < 10) ? '0' + v420 : 'A' - 10 + v420) << 8
                   | ((v421 < 10) ? '0' + v421 : 'A' - 10 + v421) << 0;
  l_bin2asc[lid43] = ((v430 < 10) ? '0' + v430 : 'A' - 10 + v430) << 8
                   | ((v431 < 10) ? '0' + v431 : 'A' - 10 + v431) << 0;

  barrier (CLK_LOCAL_MEM_FENCE);

  /**
   * sbox
   */

  __local u32 s_lotus_magic_table[256];

  s_lotus_magic_table[lid4 + 0] = lotus_magic_table[lid4 + 0];
  s_lotus_magic_table[lid4 + 1] = lotus_magic_table[lid4 + 1];
  s_lotus_magic_table[lid4 + 2] = lotus_magic_table[lid4 + 2];
  s_lotus_magic_table[lid4 + 3] = lotus_magic_table[lid4 + 3];

  barrier (CLK_LOCAL_MEM_FENCE);

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

  const u32 salt0 = salt_bufs[salt_pos].salt_buf[0];
  const u32 salt1 = salt_bufs[salt_pos].salt_buf[1] & 0xff | '(' << 8;

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

    u32x w[16];

    w[ 0] = w0[0];
    w[ 1] = w0[1];
    w[ 2] = w0[2];
    w[ 3] = w0[3];
    w[ 4] = w1[0];
    w[ 5] = w1[1];
    w[ 6] = w1[2];
    w[ 7] = w1[3];
    w[ 8] = w2[0];
    w[ 9] = w2[1];
    w[10] = w2[2];
    w[11] = w2[3];
    w[12] = w3[0];
    w[13] = w3[1];
    w[14] = w3[2];
    w[15] = w3[3];

    u32x state[4];

    state[0] = 0;
    state[1] = 0;
    state[2] = 0;
    state[3] = 0;

    /**
     * padding
     */

    if (pw_len < 16)
    {
      pad (&w[ 0], pw_len & 0xf);
    }
    else if (pw_len < 32)
    {
      pad (&w[ 4], pw_len & 0xf);
    }
    else if (pw_len < 48)
    {
      pad (&w[ 8], pw_len & 0xf);
    }
    else if (pw_len < 64)
    {
      pad (&w[12], pw_len & 0xf);
    }

    domino_big_md (w, pw_len, state, s_lotus_magic_table);

    const u32x w0_t = uint_to_hex_upper8 ((state[0] >>  0) & 255) <<  0
                     | uint_to_hex_upper8 ((state[0] >>  8) & 255) << 16;
    const u32x w1_t = uint_to_hex_upper8 ((state[0] >> 16) & 255) <<  0
                     | uint_to_hex_upper8 ((state[0] >> 24) & 255) << 16;
    const u32x w2_t = uint_to_hex_upper8 ((state[1] >>  0) & 255) <<  0
                     | uint_to_hex_upper8 ((state[1] >>  8) & 255) << 16;
    const u32x w3_t = uint_to_hex_upper8 ((state[1] >> 16) & 255) <<  0
                     | uint_to_hex_upper8 ((state[1] >> 24) & 255) << 16;
    const u32x w4_t = uint_to_hex_upper8 ((state[2] >>  0) & 255) <<  0
                     | uint_to_hex_upper8 ((state[2] >>  8) & 255) << 16;
    const u32x w5_t = uint_to_hex_upper8 ((state[2] >> 16) & 255) <<  0
                     | uint_to_hex_upper8 ((state[2] >> 24) & 255) << 16;
    const u32x w6_t = uint_to_hex_upper8 ((state[3] >>  0) & 255) <<  0
                     | uint_to_hex_upper8 ((state[3] >>  8) & 255) << 16;
    //const u32x w7_t = uint_to_hex_upper8 ((state[3] >> 16) & 255) <<  0
    //                 | uint_to_hex_upper8 ((state[3] >> 24) & 255) << 16;

    const u32 pade = 0x0e0e0e0e;

    w[ 0] = salt0;
    w[ 1] = salt1      | w0_t << 16;
    w[ 2] = w0_t >> 16 | w1_t << 16;
    w[ 3] = w1_t >> 16 | w2_t << 16;
    w[ 4] = w2_t >> 16 | w3_t << 16;
    w[ 5] = w3_t >> 16 | w4_t << 16;
    w[ 6] = w4_t >> 16 | w5_t << 16;
    w[ 7] = w5_t >> 16 | w6_t << 16;
    w[ 8] = w6_t >> 16 | pade << 16; // | w7_t <<  8;
    w[ 9] = pade;
    w[10] = pade;
    w[11] = pade;
    w[12] = 0;
    w[13] = 0;
    w[14] = 0;
    w[15] = 0;

    state[0] = 0;
    state[1] = 0;
    state[2] = 0;
    state[3] = 0;

    domino_big_md (w, 34, state, s_lotus_magic_table);

    u32x a = state[0] & 0xffffffff;
    u32x b = state[1] & 0xffffffff;
    u32x c = state[2] & 0x000000ff;
    u32x d = state[3] & 0x00000000;

    const u32x r0 = a;
    const u32x r1 = b;
    const u32x r2 = c;
    const u32x r3 = d;

    #include VECT_COMPARE_S
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08700_s08 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) m08700_s16 (__global pw_t *pws, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, __global void *tmps, __global void *hooks, __global u32 *bitmaps_buf_s1_a, __global u32 *bitmaps_buf_s1_b, __global u32 *bitmaps_buf_s1_c, __global u32 *bitmaps_buf_s1_d, __global u32 *bitmaps_buf_s2_a, __global u32 *bitmaps_buf_s2_b, __global u32 *bitmaps_buf_s2_c, __global u32 *bitmaps_buf_s2_d, __global plain_t *plains_buf, __global digest_t *digests_buf, __global u32 *hashes_shown, __global salt_t *salt_bufs, __global void *esalt_bufs, __global u32 *d_return_buf, __global u32 *d_scryptV_buf, const u32 bitmap_mask, const u32 bitmap_shift1, const u32 bitmap_shift2, const u32 salt_pos, const u32 loop_pos, const u32 loop_cnt, const u32 combs_cnt, const u32 digests_cnt, const u32 digests_offset, const u32 combs_mode, const u32 gid_max)
{
}
