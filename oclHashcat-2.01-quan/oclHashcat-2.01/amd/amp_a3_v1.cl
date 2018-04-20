/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#define VECT_SIZE1

#include "include/constants.h"
#include "include/kernel_vendor.h"
#include "types_amd.c"

__kernel void __attribute__((reqd_work_group_size (64, 1, 1))) amp (__global pw_t *pws, __global pw_t *pws_amp, __global gpu_rule_t *rules_buf, __global comb_t *combs_buf, __global bf_t *bfs_buf, const u32 combs_mode, const u32 gid_max)
{
  const u32 gid = get_global_id (0);

  if (gid >= gid_max) return;

  const u32 pw_len = pws[gid].pw_len;

  u32x w0[4];
  u32x w1[4];
  u32x w2[4];
  u32x w3[4];

  w0[0] = pws[gid].i[ 0];
  w0[1] = pws[gid].i[ 1];
  w0[2] = pws[gid].i[ 2];
  w0[3] = pws[gid].i[ 3];
  w1[0] = pws[gid].i[ 4];
  w1[1] = pws[gid].i[ 5];
  w1[2] = pws[gid].i[ 6];
  w1[3] = pws[gid].i[ 7];
  w2[0] = pws[gid].i[ 8];
  w2[1] = pws[gid].i[ 9];
  w2[2] = pws[gid].i[10];
  w2[3] = pws[gid].i[11];
  w3[0] = pws[gid].i[12];
  w3[1] = pws[gid].i[13];
  w3[2] = pws[gid].i[14];
  w3[3] = pws[gid].i[15];

  const u32 w0r = bfs_buf[0].i;

  pws_amp[gid].i[ 0] = w0[0] | w0r;
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
