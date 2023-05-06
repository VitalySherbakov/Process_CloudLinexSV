/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#include <inttypes.h>
#include "common.h"
#include "types.h"
#include "modules.h"
#include "bitops.h"
#include "convert.h"
#include "shared.h"

static const u32   ATTACK_EXEC    = ATTACK_EXEC_OUTSIDE_KERNEL;
static const u32   DGST_POS0      = 0;
static const u32   DGST_POS1      = 1;
static const u32   DGST_POS2      = 2;
static const u32   DGST_POS3      = 3;
static const u32   DGST_SIZE      = DGST_SIZE_4_4;
static const u32   HASH_CATEGORY  = HASH_CATEGORY_CRYPTOCURRENCY_WALLET;
static const char *HASH_NAME      = "MultiBit Classic .wallet (scrypt)";
static const u64   KERN_TYPE      = 27700;
static const u32   OPTI_TYPE      = OPTI_TYPE_ZERO_BYTE;
static const u64   OPTS_TYPE      = OPTS_TYPE_PT_GENERATE_LE
                                  | OPTS_TYPE_MP_MULTI_DISABLE
                                  | OPTS_TYPE_NATIVE_THREADS
                                  | OPTS_TYPE_LOOP_PREPARE
                                  | OPTS_TYPE_SELF_TEST_DISABLE;
static const u32   SALT_TYPE      = SALT_TYPE_EMBEDDED;
static const char *ST_PASS        = "hashcat";
static const char *ST_HASH        = "$multibit$3*16384*8*1*7523cb5482e81b81*91780fd49b81a782ab840157a69ba7996d81270eaf456c850f314fc1787d9b0b";

u32         module_attack_exec    (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return ATTACK_EXEC;     }
u32         module_dgst_pos0      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return DGST_POS0;       }
u32         module_dgst_pos1      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return DGST_POS1;       }
u32         module_dgst_pos2      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return DGST_POS2;       }
u32         module_dgst_pos3      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return DGST_POS3;       }
u32         module_dgst_size      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return DGST_SIZE;       }
u32         module_hash_category  (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return HASH_CATEGORY;   }
const char *module_hash_name      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return HASH_NAME;       }
u64         module_kern_type      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return KERN_TYPE;       }
u32         module_opti_type      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return OPTI_TYPE;       }
u64         module_opts_type      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return OPTS_TYPE;       }
u32         module_salt_type      (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return SALT_TYPE;       }
const char *module_st_hash        (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return ST_HASH;         }
const char *module_st_pass        (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra) { return ST_PASS;         }

static const char *SIGNATURE_MULTIBIT = "$multibit$";

static const u64 SCRYPT_N = 16384;
static const u64 SCRYPT_R = 8;
static const u64 SCRYPT_P = 1;

bool module_unstable_warning (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra, MAYBE_UNUSED const hc_device_param_t *device_param)
{
  // AMD Radeon Pro W5700X Compute Engine; 1.2 (Apr 22 2021 21:54:44); 11.3.1; 20E241
  if ((device_param->opencl_platform_vendor_id == VENDOR_ID_APPLE) && (device_param->opencl_device_type & CL_DEVICE_TYPE_GPU))
  {
    return true;
  }

  // amdgpu-pro-20.50-1234664-ubuntu-20.04 (legacy)
  if ((device_param->opencl_device_vendor_id == VENDOR_ID_AMD) && (device_param->has_vperm == false))
  {
    return true;
  }

  return false;
}

u32 module_kernel_loops_min (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra)
{
  const u32 kernel_loops_min = 1024;

  return kernel_loops_min;
}

u32 module_kernel_loops_max (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra)
{
  const u32 kernel_loops_max = 1024;

  return kernel_loops_max;
}

u32 module_pw_max (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra)
{
  // this overrides the reductions of PW_MAX in case optimized kernel is selected
  // IOW, even in optimized kernel mode it support length 256

  const u32 pw_max = PW_MAX;

  return pw_max;
}

u64 module_extra_buffer_size (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra, MAYBE_UNUSED const hashes_t *hashes, MAYBE_UNUSED const hc_device_param_t *device_param)
{
  // we need to set the self-test hash settings to pass the self-test
  // the decoder for the self-test is called after this function

  const u64 scrypt_N = (hashes->salts_buf[0].scrypt_N) ? hashes->salts_buf[0].scrypt_N : SCRYPT_N;
  const u64 scrypt_r = (hashes->salts_buf[0].scrypt_r) ? hashes->salts_buf[0].scrypt_r : SCRYPT_R;

  const u64 kernel_power_max = ((OPTS_TYPE & OPTS_TYPE_MP_MULTI_DISABLE) ? 1 : device_param->device_processors) * device_param->kernel_threads_max * device_param->kernel_accel_max;

  u64 tmto_start = 0;
  u64 tmto_stop  = 4;

  if (user_options->scrypt_tmto_chgd == true)
  {
    tmto_start = user_options->scrypt_tmto;
    tmto_stop  = user_options->scrypt_tmto;
  }

  // size_pws

  const u64 size_pws = kernel_power_max * sizeof (pw_t);

  const u64 size_pws_amp = size_pws;

  // size_pws_comp

  const u64 size_pws_comp = kernel_power_max * (sizeof (u32) * 64);

  // size_pws_idx

  const u64 size_pws_idx = (kernel_power_max + 1) * sizeof (pw_idx_t);

  // size_tmps

  const u64 size_tmps = kernel_power_max * hashconfig->tmp_size;

  // size_hooks

  const u64 size_hooks = kernel_power_max * hashconfig->hook_size;

  u64 size_pws_pre  = 4;
  u64 size_pws_base = 4;

  if (user_options->slow_candidates == true)
  {
    // size_pws_pre

    size_pws_pre = kernel_power_max * sizeof (pw_pre_t);

    // size_pws_base

    size_pws_base = kernel_power_max * sizeof (pw_pre_t);
  }

  // sometimes device_available_mem and device_maxmem_alloc reported back from the opencl runtime are a bit inaccurate.
  // let's add some extra space just to be sure.
  // now depends on the kernel-accel value (where scrypt and similar benefits), but also hard minimum 64mb and maximum 1024mb limit

  u64 EXTRA_SPACE = (1024ULL * 1024ULL) * device_param->kernel_accel_max;

  EXTRA_SPACE = MAX (EXTRA_SPACE, (  64ULL * 1024ULL * 1024ULL));
  EXTRA_SPACE = MIN (EXTRA_SPACE, (1024ULL * 1024ULL * 1024ULL));

  const u64 scrypt_extra_space
    = device_param->size_bfs
    + device_param->size_combs
    + device_param->size_digests
    + device_param->size_esalts
    + device_param->size_markov_css
    + device_param->size_plains
    + device_param->size_results
    + device_param->size_root_css
    + device_param->size_rules
    + device_param->size_rules_c
    + device_param->size_salts
    + device_param->size_shown
    + device_param->size_tm
    + device_param->size_st_digests
    + device_param->size_st_salts
    + device_param->size_st_esalts
    + size_pws
    + size_pws_amp
    + size_pws_comp
    + size_pws_idx
    + size_tmps
    + size_hooks
    + size_pws_pre
    + size_pws_base
    + EXTRA_SPACE;

  bool not_enough_memory = true;

  u64 size_scrypt = 0;

  u64 tmto;

  for (tmto = tmto_start; tmto <= tmto_stop; tmto++)
  {
    size_scrypt = (128ULL * scrypt_r) * scrypt_N;

    size_scrypt /= 1ull << tmto;

    size_scrypt *= kernel_power_max;

    if ((size_scrypt / 4) > device_param->device_maxmem_alloc) continue;

    if ((size_scrypt + scrypt_extra_space) > device_param->device_available_mem) continue;

    not_enough_memory = false;

    break;
  }

  if (not_enough_memory == true) return -1;

  return size_scrypt;
}

u64 module_tmp_size (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra)
{
  const u64 tmp_size = 0; // we'll add some later

  return tmp_size;
}

u64 module_extra_tmp_size (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra, MAYBE_UNUSED const hashes_t *hashes)
{
  const u64 scrypt_N = (hashes->salts_buf[0].scrypt_N) ? hashes->salts_buf[0].scrypt_N : SCRYPT_N;
  const u64 scrypt_r = (hashes->salts_buf[0].scrypt_r) ? hashes->salts_buf[0].scrypt_r : SCRYPT_R;
  const u64 scrypt_p = (hashes->salts_buf[0].scrypt_p) ? hashes->salts_buf[0].scrypt_p : SCRYPT_P;

  // we need to check that all hashes have the same scrypt settings

  for (u32 i = 1; i < hashes->salts_cnt; i++)
  {
    if ((hashes->salts_buf[i].scrypt_N != scrypt_N)
     || (hashes->salts_buf[i].scrypt_r != scrypt_r)
     || (hashes->salts_buf[i].scrypt_p != scrypt_p))
    {
      return -1;
    }
  }

  const u64 tmp_size = 128ULL * scrypt_r * scrypt_p;

  return tmp_size;
}

bool module_warmup_disable (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra)
{
  return true;
}

char *module_jit_build_options (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra, MAYBE_UNUSED const hashes_t *hashes, MAYBE_UNUSED const hc_device_param_t *device_param)
{
  const u64 scrypt_N = (hashes->salts_buf[0].scrypt_N) ? hashes->salts_buf[0].scrypt_N : SCRYPT_N;
  const u64 scrypt_r = (hashes->salts_buf[0].scrypt_r) ? hashes->salts_buf[0].scrypt_r : SCRYPT_R;
  const u64 scrypt_p = (hashes->salts_buf[0].scrypt_p) ? hashes->salts_buf[0].scrypt_p : SCRYPT_P;

  const u64 extra_buffer_size = device_param->extra_buffer_size;

  const u64 kernel_power_max = ((OPTS_TYPE & OPTS_TYPE_MP_MULTI_DISABLE) ? 1 : device_param->device_processors) * device_param->kernel_threads_max * device_param->kernel_accel_max;

  const u64 size_scrypt = 128ULL * scrypt_r * scrypt_N;

  const u64 scrypt_tmto_final = (kernel_power_max * size_scrypt) / extra_buffer_size;

  const u64 tmp_size = 128ULL * scrypt_r * scrypt_p;

  char *jit_build_options = NULL;

  hc_asprintf (&jit_build_options, "-DSCRYPT_N=%u -DSCRYPT_R=%u -DSCRYPT_P=%u -DSCRYPT_TMTO=%" PRIu64 " -DSCRYPT_TMP_ELEM=%" PRIu64,
    hashes->salts_buf[0].scrypt_N,
    hashes->salts_buf[0].scrypt_r,
    hashes->salts_buf[0].scrypt_p,
    scrypt_tmto_final,
    tmp_size / 16);

  return jit_build_options;
}

int module_hash_decode (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED void *digest_buf, MAYBE_UNUSED salt_t *salt, MAYBE_UNUSED void *esalt_buf, MAYBE_UNUSED void *hook_salt_buf, MAYBE_UNUSED hashinfo_t *hash_info, const char *line_buf, MAYBE_UNUSED const int line_len)
{
  u32 *digest = (u32 *) digest_buf;

  hc_token_t token;

  token.token_cnt  = 7;

  token.signatures_cnt    = 1;
  token.signatures_buf[0] = SIGNATURE_MULTIBIT;

  token.len[0]     = 10;
  token.attr[0]    = TOKEN_ATTR_FIXED_LENGTH
                   | TOKEN_ATTR_VERIFY_SIGNATURE;

  token.len_min[1] = 1;
  token.len_max[1] = 1;
  token.sep[1]     = '*';
  token.attr[1]    = TOKEN_ATTR_VERIFY_LENGTH
                   | TOKEN_ATTR_VERIFY_DIGIT;

  token.len_min[2] = 5;
  token.len_max[2] = 5;
  token.sep[2]     = '*';
  token.attr[2]    = TOKEN_ATTR_VERIFY_LENGTH
                   | TOKEN_ATTR_VERIFY_DIGIT;

  token.len_min[3] = 1;
  token.len_max[3] = 1;
  token.sep[3]     = '*';
  token.attr[3]    = TOKEN_ATTR_VERIFY_LENGTH
                   | TOKEN_ATTR_VERIFY_DIGIT;

  token.len_min[4] = 1;
  token.len_max[4] = 1;
  token.sep[4]     = '*';
  token.attr[4]    = TOKEN_ATTR_VERIFY_LENGTH
                   | TOKEN_ATTR_VERIFY_DIGIT;

  token.len_min[5] = 16;
  token.len_max[5] = 16;
  token.sep[5]     = '*';
  token.attr[5]    = TOKEN_ATTR_VERIFY_LENGTH
                   | TOKEN_ATTR_VERIFY_HEX;

  token.len[6]     = 64;
  token.attr[6]    = TOKEN_ATTR_FIXED_LENGTH
                   | TOKEN_ATTR_VERIFY_HEX;

  const int rc_tokenizer = input_tokenizer ((const u8 *) line_buf, line_len, &token);

  if (rc_tokenizer != PARSER_OK) return (rc_tokenizer);

  // version

  const u8 *version_pos = token.buf[1];

  if (version_pos[0] != (u8) '3') return (PARSER_SIGNATURE_UNMATCHED);

  // scrypt settings

  const u8 *scrypt_n_pos = token.buf[2];

  const u32 scrypt_n = hc_strtoul ((const char *) scrypt_n_pos, NULL, 10);

  if (scrypt_n != SCRYPT_N) return (PARSER_SALT_VALUE);

  const u8 *scrypt_r_pos = token.buf[3];

  const u32 scrypt_r = hc_strtoul ((const char *) scrypt_r_pos, NULL, 10);

  if (scrypt_r != SCRYPT_R) return (PARSER_SALT_VALUE);

  const u8 *scrypt_p_pos = token.buf[4];

  const u32 scrypt_p = hc_strtoul ((const char *) scrypt_p_pos, NULL, 10);

  if (scrypt_p != SCRYPT_P) return (PARSER_SALT_VALUE);

  salt->scrypt_N = SCRYPT_N;
  salt->scrypt_r = SCRYPT_R;
  salt->scrypt_p = SCRYPT_P;

  salt->salt_iter    = salt->scrypt_N;
  salt->salt_repeats = salt->scrypt_p - 1;

  // salt

  const u8 *salt_pos = token.buf[5];

  salt->salt_buf[0] = hex_to_u32 (salt_pos +  0);
  salt->salt_buf[1] = hex_to_u32 (salt_pos +  8);

  salt->salt_buf[0] = byte_swap_32 (salt->salt_buf[0]); // swap the salt (only the salt)
  salt->salt_buf[1] = byte_swap_32 (salt->salt_buf[1]);

  // IV

  const u8 *blob_pos = token.buf[6];

  salt->salt_buf[2] = hex_to_u32 (blob_pos +  0);
  salt->salt_buf[3] = hex_to_u32 (blob_pos +  8);
  salt->salt_buf[4] = hex_to_u32 (blob_pos + 16);
  salt->salt_buf[5] = hex_to_u32 (blob_pos + 24);

  // data

  salt->salt_buf[6] = hex_to_u32 (blob_pos + 32);
  salt->salt_buf[7] = hex_to_u32 (blob_pos + 40);
  salt->salt_buf[8] = hex_to_u32 (blob_pos + 48);
  salt->salt_buf[9] = hex_to_u32 (blob_pos + 56);

  salt->salt_len = 40;

  // fake digest:

  digest[0] = salt->salt_buf[4];
  digest[1] = salt->salt_buf[5];
  digest[2] = salt->salt_buf[6];
  digest[3] = salt->salt_buf[7];

  return (PARSER_OK);
}

int module_hash_encode (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const void *digest_buf, MAYBE_UNUSED const salt_t *salt, MAYBE_UNUSED const void *esalt_buf, MAYBE_UNUSED const void *hook_salt_buf, MAYBE_UNUSED const hashinfo_t *hash_info, char *line_buf, MAYBE_UNUSED const int line_size)
{
  const int line_len = snprintf (line_buf, line_size, "%s%u*%" PRIu64 "*%" PRIu64 "*%" PRIu64 "*%08x%08x*%08x%08x%08x%08x%08x%08x%08x%08x",
    SIGNATURE_MULTIBIT,
    3,
    SCRYPT_N,
    SCRYPT_R,
    SCRYPT_P,
    salt->salt_buf[0],
    salt->salt_buf[1],
    byte_swap_32 (salt->salt_buf[2]),
    byte_swap_32 (salt->salt_buf[3]),
    byte_swap_32 (salt->salt_buf[4]),
    byte_swap_32 (salt->salt_buf[5]),
    byte_swap_32 (salt->salt_buf[6]),
    byte_swap_32 (salt->salt_buf[7]),
    byte_swap_32 (salt->salt_buf[8]),
    byte_swap_32 (salt->salt_buf[9]));

  return line_len;
}

/*

Find the right -n value for your GPU:
=====================================

1. For example, to find the value for 27700, first create a valid hash for 27700 as follows:

$ ./hashcat --example-hashes -m 27700 | grep Example.Hash | grep -v Format | cut -b 25- > tmp.hash.27700

2. Now let it iterate through all -n values to a certain point. In this case, I'm using 200, but in general it's a value that is at least twice that of the multiprocessor. If you don't mind you can just leave it as it is, it just runs a little longer.

$ export i=1; while [ $i -ne 201 ]; do echo $i; ./hashcat --quiet tmp.hash.27700 --keep-guessing --self-test-disable --markov-disable --restore-disable --outfile-autohex-disable --wordlist-autohex-disable --potfile-disable --logfile-disable --hwmon-disable --status --status-timer 1 --runtime 28 --machine-readable --optimized-kernel-enable --workload-profile 3 --hash-type 27700 --attack-mode 3 ?b?b?b?b?b?b?b --backend-devices 1 --force -n $i; i=$(($i+1)); done | tee x

3. Determine the highest measured H/s speed. But don't just use the highest value. Instead, use the number that seems most stable, usually at the beginning.

$ grep "$(printf 'STATUS\t3')" x | cut -f4 -d$'\t' | sort -n | tail

4. To match the speed you have chosen to the correct value in the 'x' file, simply search for it in it. Then go up a little on the block where you found him. The value -n is the single value that begins before the block start. If you have multiple blocks at the same speed, choose the lowest value for -n

*/

const char *module_extra_tuningdb_block (MAYBE_UNUSED const hashconfig_t *hashconfig, MAYBE_UNUSED const user_options_t *user_options, MAYBE_UNUSED const user_options_extra_t *user_options_extra)
{
  const char *extra_tuningdb_block =
    "DEVICE_TYPE_CPU                                 *       27700   1       N       A\n"
    "DEVICE_TYPE_GPU                                 *       27700   1       N       A\n"
    "GeForce_GTX_980                                 *       27700   1      29       A\n"
    "GeForce_GTX_1080                                *       27700   1      15       A\n"
    "GeForce_RTX_2080_Ti                             *       27700   1      68       A\n"
    "GeForce_RTX_3060_Ti                             *       27700   1      51       A\n"
    "GeForce_RTX_3070                                *       27700   1      46       A\n"
    "GeForce_RTX_3090                                *       27700   1      82       A\n"
    "ALIAS_AMD_RX480                                 *       27700   1      15       A\n"
    "ALIAS_AMD_Vega64                                *       27700   1      28       A\n"
    "ALIAS_AMD_MI100                                 *       27700   1      79       A\n"
    "ALIAS_AMD_RX6900XT                              *       27700   1      59       A\n"
  ;

  return extra_tuningdb_block;
}

void module_init (module_ctx_t *module_ctx)
{
  module_ctx->module_context_size             = MODULE_CONTEXT_SIZE_CURRENT;
  module_ctx->module_interface_version        = MODULE_INTERFACE_VERSION_CURRENT;

  module_ctx->module_attack_exec              = module_attack_exec;
  module_ctx->module_benchmark_esalt          = MODULE_DEFAULT;
  module_ctx->module_benchmark_hook_salt      = MODULE_DEFAULT;
  module_ctx->module_benchmark_mask           = MODULE_DEFAULT;
  module_ctx->module_benchmark_salt           = MODULE_DEFAULT;
  module_ctx->module_build_plain_postprocess  = MODULE_DEFAULT;
  module_ctx->module_deep_comp_kernel         = MODULE_DEFAULT;
  module_ctx->module_deprecated_notice        = MODULE_DEFAULT;
  module_ctx->module_dgst_pos0                = module_dgst_pos0;
  module_ctx->module_dgst_pos1                = module_dgst_pos1;
  module_ctx->module_dgst_pos2                = module_dgst_pos2;
  module_ctx->module_dgst_pos3                = module_dgst_pos3;
  module_ctx->module_dgst_size                = module_dgst_size;
  module_ctx->module_dictstat_disable         = MODULE_DEFAULT;
  module_ctx->module_esalt_size               = MODULE_DEFAULT;
  module_ctx->module_extra_buffer_size        = module_extra_buffer_size;
  module_ctx->module_extra_tmp_size           = module_extra_tmp_size;
  module_ctx->module_extra_tuningdb_block     = module_extra_tuningdb_block;
  module_ctx->module_forced_outfile_format    = MODULE_DEFAULT;
  module_ctx->module_hash_binary_count        = MODULE_DEFAULT;
  module_ctx->module_hash_binary_parse        = MODULE_DEFAULT;
  module_ctx->module_hash_binary_save         = MODULE_DEFAULT;
  module_ctx->module_hash_decode_postprocess  = MODULE_DEFAULT;
  module_ctx->module_hash_decode_potfile      = MODULE_DEFAULT;
  module_ctx->module_hash_decode_zero_hash    = MODULE_DEFAULT;
  module_ctx->module_hash_decode              = module_hash_decode;
  module_ctx->module_hash_encode_status       = MODULE_DEFAULT;
  module_ctx->module_hash_encode_potfile      = MODULE_DEFAULT;
  module_ctx->module_hash_encode              = module_hash_encode;
  module_ctx->module_hash_init_selftest       = MODULE_DEFAULT;
  module_ctx->module_hash_mode                = MODULE_DEFAULT;
  module_ctx->module_hash_category            = module_hash_category;
  module_ctx->module_hash_name                = module_hash_name;
  module_ctx->module_hashes_count_min         = MODULE_DEFAULT;
  module_ctx->module_hashes_count_max         = MODULE_DEFAULT;
  module_ctx->module_hlfmt_disable            = MODULE_DEFAULT;
  module_ctx->module_hook_extra_param_size    = MODULE_DEFAULT;
  module_ctx->module_hook_extra_param_init    = MODULE_DEFAULT;
  module_ctx->module_hook_extra_param_term    = MODULE_DEFAULT;
  module_ctx->module_hook12                   = MODULE_DEFAULT;
  module_ctx->module_hook23                   = MODULE_DEFAULT;
  module_ctx->module_hook_salt_size           = MODULE_DEFAULT;
  module_ctx->module_hook_size                = MODULE_DEFAULT;
  module_ctx->module_jit_build_options        = module_jit_build_options;
  module_ctx->module_jit_cache_disable        = MODULE_DEFAULT;
  module_ctx->module_kernel_accel_max         = MODULE_DEFAULT;
  module_ctx->module_kernel_accel_min         = MODULE_DEFAULT;
  module_ctx->module_kernel_loops_max         = module_kernel_loops_max;
  module_ctx->module_kernel_loops_min         = module_kernel_loops_min;
  module_ctx->module_kernel_threads_max       = MODULE_DEFAULT;
  module_ctx->module_kernel_threads_min       = MODULE_DEFAULT;
  module_ctx->module_kern_type                = module_kern_type;
  module_ctx->module_kern_type_dynamic        = MODULE_DEFAULT;
  module_ctx->module_opti_type                = module_opti_type;
  module_ctx->module_opts_type                = module_opts_type;
  module_ctx->module_outfile_check_disable    = MODULE_DEFAULT;
  module_ctx->module_outfile_check_nocomp     = MODULE_DEFAULT;
  module_ctx->module_potfile_custom_check     = MODULE_DEFAULT;
  module_ctx->module_potfile_disable          = MODULE_DEFAULT;
  module_ctx->module_potfile_keep_all_hashes  = MODULE_DEFAULT;
  module_ctx->module_pwdump_column            = MODULE_DEFAULT;
  module_ctx->module_pw_max                   = module_pw_max;
  module_ctx->module_pw_min                   = MODULE_DEFAULT;
  module_ctx->module_salt_max                 = MODULE_DEFAULT;
  module_ctx->module_salt_min                 = MODULE_DEFAULT;
  module_ctx->module_salt_type                = module_salt_type;
  module_ctx->module_separator                = MODULE_DEFAULT;
  module_ctx->module_st_hash                  = module_st_hash;
  module_ctx->module_st_pass                  = module_st_pass;
  module_ctx->module_tmp_size                 = module_tmp_size;
  module_ctx->module_unstable_warning         = module_unstable_warning;
  module_ctx->module_warmup_disable           = module_warmup_disable;
}