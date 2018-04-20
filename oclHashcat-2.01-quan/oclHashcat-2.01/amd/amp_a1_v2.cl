/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define VECT_SIZE2

#include "include/constants.h"
#include "include/kernel_vendor.h"
#include "types_amd.c"

static void switch_buffer_by_offset (u32x w0[4], u32x w1[4], u32x w2[4], u32x w3[4], const u32 offset)
{
  const int offset_mod_4 = offset & 3;

  const int offset_minus_4 = 4 - offset;

  switch (offset / 4)
  {
    case 0:
      w3[2] = amd_bytealign (    0, w3[1], offset_minus_4);
      w3[1] = amd_bytealign (w3[1], w3[0], offset_minus_4);
      w3[0] = amd_bytealign (w3[0], w2[3], offset_minus_4);
      w2[3] = amd_bytealign (w2[3], w2[2], offset_minus_4);
      w2[2] = amd_bytealign (w2[2], w2[1], offset_minus_4);
      w2[1] = amd_bytealign (w2[1], w2[0], offset_minus_4);
      w2[0] = amd_bytealign (w2[0], w1[3], offset_minus_4);
      w1[3] = amd_bytealign (w1[3], w1[2], offset_minus_4);
      w1[2] = amd_bytealign (w1[2], w1[1], offset_minus_4);
      w1[1] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w1[0] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w0[3] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w0[2] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w0[1] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w0[0] = amd_bytealign (w0[0],     0, offset_minus_4);

      if (offset_mod_4 == 0)
      {
        w0[0] = w0[1];
        w0[1] = w0[2];
        w0[2] = w0[3];
        w0[3] = w1[0];
        w1[0] = w1[1];
        w1[1] = w1[2];
        w1[2] = w1[3];
        w1[3] = w2[0];
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 1:
      w3[2] = amd_bytealign (    0, w3[0], offset_minus_4);
      w3[1] = amd_bytealign (w3[0], w2[3], offset_minus_4);
      w3[0] = amd_bytealign (w2[3], w2[2], offset_minus_4);
      w2[3] = amd_bytealign (w2[2], w2[1], offset_minus_4);
      w2[2] = amd_bytealign (w2[1], w2[0], offset_minus_4);
      w2[1] = amd_bytealign (w2[0], w1[3], offset_minus_4);
      w2[0] = amd_bytealign (w1[3], w1[2], offset_minus_4);
      w1[3] = amd_bytealign (w1[2], w1[1], offset_minus_4);
      w1[2] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w1[1] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w1[0] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w0[3] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w0[2] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w0[1] = amd_bytealign (w0[0],     0, offset_minus_4);
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w0[1] = w0[2];
        w0[2] = w0[3];
        w0[3] = w1[0];
        w1[0] = w1[1];
        w1[1] = w1[2];
        w1[2] = w1[3];
        w1[3] = w2[0];
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 2:
      w3[2] = amd_bytealign (    0, w2[3], offset_minus_4);
      w3[1] = amd_bytealign (w2[3], w2[2], offset_minus_4);
      w3[0] = amd_bytealign (w2[2], w2[1], offset_minus_4);
      w2[3] = amd_bytealign (w2[1], w2[0], offset_minus_4);
      w2[2] = amd_bytealign (w2[0], w1[3], offset_minus_4);
      w2[1] = amd_bytealign (w1[3], w1[2], offset_minus_4);
      w2[0] = amd_bytealign (w1[2], w1[1], offset_minus_4);
      w1[3] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w1[2] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w1[1] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w1[0] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w0[3] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w0[2] = amd_bytealign (w0[0],     0, offset_minus_4);
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w0[2] = w0[3];
        w0[3] = w1[0];
        w1[0] = w1[1];
        w1[1] = w1[2];
        w1[2] = w1[3];
        w1[3] = w2[0];
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 3:
      w3[2] = amd_bytealign (    0, w2[2], offset_minus_4);
      w3[1] = amd_bytealign (w2[2], w2[1], offset_minus_4);
      w3[0] = amd_bytealign (w2[1], w2[0], offset_minus_4);
      w2[3] = amd_bytealign (w2[0], w1[3], offset_minus_4);
      w2[2] = amd_bytealign (w1[3], w1[2], offset_minus_4);
      w2[1] = amd_bytealign (w1[2], w1[1], offset_minus_4);
      w2[0] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w1[3] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w1[2] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w1[1] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w1[0] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w0[3] = amd_bytealign (w0[0],     0, offset_minus_4);
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w0[3] = w1[0];
        w1[0] = w1[1];
        w1[1] = w1[2];
        w1[2] = w1[3];
        w1[3] = w2[0];
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 4:
      w3[2] = amd_bytealign (    0, w2[1], offset_minus_4);
      w3[1] = amd_bytealign (w2[1], w2[0], offset_minus_4);
      w3[0] = amd_bytealign (w2[0], w1[3], offset_minus_4);
      w2[3] = amd_bytealign (w1[3], w1[2], offset_minus_4);
      w2[2] = amd_bytealign (w1[2], w1[1], offset_minus_4);
      w2[1] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w2[0] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w1[3] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w1[2] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w1[1] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w1[0] = amd_bytealign (w0[0],     0, offset_minus_4);
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w1[0] = w1[1];
        w1[1] = w1[2];
        w1[2] = w1[3];
        w1[3] = w2[0];
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 5:
      w3[2] = amd_bytealign (    0, w2[0], offset_minus_4);
      w3[1] = amd_bytealign (w2[0], w1[3], offset_minus_4);
      w3[0] = amd_bytealign (w1[3], w1[2], offset_minus_4);
      w2[3] = amd_bytealign (w1[2], w1[1], offset_minus_4);
      w2[2] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w2[1] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w2[0] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w1[3] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w1[2] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w1[1] = amd_bytealign (w0[0],     0, offset_minus_4);
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w1[1] = w1[2];
        w1[2] = w1[3];
        w1[3] = w2[0];
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 6:
      w3[2] = amd_bytealign (    0, w1[3], offset_minus_4);
      w3[1] = amd_bytealign (w1[3], w1[2], offset_minus_4);
      w3[0] = amd_bytealign (w1[2], w1[1], offset_minus_4);
      w2[3] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w2[2] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w2[1] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w2[0] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w1[3] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w1[2] = amd_bytealign (w0[0],     0, offset_minus_4);
      w1[1] = 0;
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w1[2] = w1[3];
        w1[3] = w2[0];
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 7:
      w3[2] = amd_bytealign (    0, w1[2], offset_minus_4);
      w3[1] = amd_bytealign (w1[2], w1[1], offset_minus_4);
      w3[0] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w2[3] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w2[2] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w2[1] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w2[0] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w1[3] = amd_bytealign (w0[0],     0, offset_minus_4);
      w1[2] = 0;
      w1[1] = 0;
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w1[3] = w2[0];
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 8:
      w3[2] = amd_bytealign (    0, w1[1], offset_minus_4);
      w3[1] = amd_bytealign (w1[1], w1[0], offset_minus_4);
      w3[0] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w2[3] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w2[2] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w2[1] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w2[0] = amd_bytealign (w0[0],     0, offset_minus_4);
      w1[3] = 0;
      w1[2] = 0;
      w1[1] = 0;
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w2[0] = w2[1];
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 9:
      w3[2] = amd_bytealign (    0, w1[0], offset_minus_4);
      w3[1] = amd_bytealign (w1[0], w0[3], offset_minus_4);
      w3[0] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w2[3] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w2[2] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w2[1] = amd_bytealign (w0[0],     0, offset_minus_4);
      w2[0] = 0;
      w1[3] = 0;
      w1[2] = 0;
      w1[1] = 0;
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w2[1] = w2[2];
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 10:
      w3[2] = amd_bytealign (    0, w0[3], offset_minus_4);
      w3[1] = amd_bytealign (w0[3], w0[2], offset_minus_4);
      w3[0] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w2[3] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w2[2] = amd_bytealign (w0[0],     0, offset_minus_4);
      w2[1] = 0;
      w2[0] = 0;
      w1[3] = 0;
      w1[2] = 0;
      w1[1] = 0;
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w2[2] = w2[3];
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 11:
      w3[2] = amd_bytealign (    0, w0[2], offset_minus_4);
      w3[1] = amd_bytealign (w0[2], w0[1], offset_minus_4);
      w3[0] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w2[3] = amd_bytealign (w0[0],     0, offset_minus_4);
      w2[2] = 0;
      w2[1] = 0;
      w2[0] = 0;
      w1[3] = 0;
      w1[2] = 0;
      w1[1] = 0;
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w2[3] = w3[0];
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 12:
      w3[2] = amd_bytealign (    0, w0[1], offset_minus_4);
      w3[1] = amd_bytealign (w0[1], w0[0], offset_minus_4);
      w3[0] = amd_bytealign (w0[0],     0, offset_minus_4);
      w2[3] = 0;
      w2[2] = 0;
      w2[1] = 0;
      w2[0] = 0;
      w1[3] = 0;
      w1[2] = 0;
      w1[1] = 0;
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w3[0] = w3[1];
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;

    case 13:
      w3[2] = amd_bytealign (    0, w0[0], offset_minus_4);
      w3[1] = amd_bytealign (w0[0],     0, offset_minus_4);
      w3[0] = 0;
      w2[3] = 0;
      w2[2] = 0;
      w2[1] = 0;
      w2[0] = 0;
      w1[3] = 0;
      w1[2] = 0;
      w1[1] = 0;
      w1[0] = 0;
      w0[3] = 0;
      w0[2] = 0;
      w0[1] = 0;
      w0[0] = 0;

      if (offset_mod_4 == 0)
      {
        w3[1] = w3[2];
        w3[2] = 0;
      }

      break;
  }
}

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) amp (__global pw_t *pws, __global pw_t *pws_amp, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, const u32 combs_mode, const u32 gid_max)
{
  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  const u32 pw_l_len = pws[gid].pw_len;

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

  const u32 pw_r_len = combs_buf[0].pw_len;

  u32x wordr0[4];

  wordr0[0] = combs_buf[0].i[0];
  wordr0[1] = combs_buf[0].i[1];
  wordr0[2] = combs_buf[0].i[2];
  wordr0[3] = combs_buf[0].i[3];

  u32x wordr1[4];

  wordr1[0] = combs_buf[0].i[4];
  wordr1[1] = combs_buf[0].i[5];
  wordr1[2] = combs_buf[0].i[6];
  wordr1[3] = combs_buf[0].i[7];

  u32x wordr2[4];

  wordr2[0] = 0;
  wordr2[1] = 0;
  wordr2[2] = 0;
  wordr2[3] = 0;

  u32x wordr3[4];

  wordr3[0] = 0;
  wordr3[1] = 0;
  wordr3[2] = 0;
  wordr3[3] = 0;

  if (combs_mode == COMBINATOR_MODE_BASE_LEFT)
  {
    switch_buffer_by_offset (wordr0, wordr1, wordr2, wordr3, pw_l_len);
  }

  if (combs_mode == COMBINATOR_MODE_BASE_RIGHT)
  {
    switch_buffer_by_offset (wordl0, wordl1, wordl2, wordl3, pw_r_len);
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

  const u32 pw_len = pw_l_len + pw_r_len;

  pws_amp[gid].i[ 0] = w0[0];
  pws_amp[gid].i[ 1] = w0[1];
  pws_amp[gid].i[ 2] = w0[2];
  pws_amp[gid].i[ 3] = w0[3];
  pws_amp[gid].i[ 4] = w1[0];
  pws_amp[gid].i[ 5] = w1[1];
  pws_amp[gid].i[ 6] = w1[2];
  pws_amp[gid].i[ 7] = w1[3];
  pws_amp[gid].i[ 8] = w2[0];
  pws_amp[gid].i[ 9] = w2[1];
  pws_amp[gid].i[10] = w2[2];
  pws_amp[gid].i[11] = w2[3];
  pws_amp[gid].i[12] = w3[0];
  pws_amp[gid].i[13] = w3[1];
  pws_amp[gid].i[14] = w3[2];
  pws_amp[gid].i[15] = w3[3];

  pws_amp[gid].pw_len = pw_len;
}
