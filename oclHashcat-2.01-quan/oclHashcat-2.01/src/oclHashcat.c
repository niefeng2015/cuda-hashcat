/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#include <common.h>
#include <shared.h>
#include <rp_gpu_on_cpu.h>

#include <getopt.h>

#ifdef _CUDA
const char *PROGNAME          = "cudaHashcat";
#elif _OCL
const char *PROGNAME          = "oclHashcat";
#endif

const char *VERSION_TXT       = "2.01";
const uint  VERSION_BIN       = 201;
const uint  RESTORE_MIN       = 200;

#define INCR_RULES            10000
#define INCR_SALTS            100000
#define INCR_MASKS            1000
#define INCR_POT              1000

// comment-out for kernel source mode

#define BINARY_KERNEL

#define USAGE                   0
#define VERSION                 0
#define QUIET                   0
#define MARKOV_THRESHOLD        0
#define MARKOV_DISABLE          0
#define MARKOV_CLASSIC          0
#define BENCHMARK               0
#define BENCHMARK_MODE          1
#define RESTORE                 0
#define RESTORE_TIMER           60
#define RESTORE_DISABLE         0
#define STATUS                  0
#define STATUS_TIMER            10
#define STATUS_AUTOMAT          0
#define LOOPBACK                0
#define WEAK_HASH_THRESHOLD     100
#define SHOW                    0
#define LEFT                    0
#define USERNAME                0
#define REMOVE                  0
#define REMOVE_TIMER            60
#define SKIP                    0
#define LIMIT                   0
#define KEYSPACE                0
#define POTFILE_DISABLE         0
#define DEBUG_MODE              0
#define RP_GEN                  0
#define RP_GEN_FUNC_MIN         1
#define RP_GEN_FUNC_MAX         4
#define RP_GEN_SEED             0
#define RULE_BUF_L              ":"
#define RULE_BUF_R              ":"
#define FORCE                   0
#define RUNTIME                 0
#define HEX_CHARSET             0
#define HEX_SALT                0
#define HEX_WORDLIST            0
#define OUTFILE_FORMAT          3
#define OUTFILE_AUTOHEX         1
#define OUTFILE_CHECK_TIMER     5
#define ATTACK_MODE             0
#define HASH_MODE               0
#define SEGMENT_SIZE            32
#define INCREMENT               0
#define INCREMENT_MIN           1
#define INCREMENT_MAX           PW_MAX
#define SEPARATOR               ':'
#define BITMAP_MIN              16
#define BITMAP_MAX              24
#define GPU_ASYNC               0
#define GPU_TEMP_DISABLE        0
#define GPU_TEMP_ABORT          90
#define GPU_TEMP_RETAIN         80
#define WORKLOAD_PROFILE        2
#define GPU_ACCEL               0
#define GPU_LOOPS               0
#define GPU_RULES               1024
#define GPU_COMBS               1024
#define GPU_BFS                 1024
#define GPU_THREADS_AMD         64
#define GPU_THREADS_NV          256
#define POWERTUNE_ENABLE        0
#define LOGFILE_DISABLE         0
#define SCRYPT_TMTO             0

#define VECT_SIZE_1             1
#define VECT_SIZE_2             2
#define VECT_SIZE_4             4
#define VECT_SIZE_8             8

#define WL_MODE_STDIN           1
#define WL_MODE_FILE            2
#define WL_MODE_MASK            3

#define HL_MODE_FILE            4
#define HL_MODE_ARG             5

#define HLFMT_HASHCAT           0
#define HLFMT_PWDUMP            1
#define HLFMT_PASSWD            2
#define HLFMT_SHADOW            3
#define HLFMT_DCC               4
#define HLFMT_DCC2              5
#define HLFMT_NETNTLM1          7
#define HLFMT_NETNTLM2          8
#define HLFMT_NSLDAP            9
#define HLFMT_NSLDAPS           10
#define HLFMTS_CNT              11

#define ATTACK_MODE_STRAIGHT    0
#define ATTACK_MODE_COMBI       1
#define ATTACK_MODE_TOGGLE      2
#define ATTACK_MODE_BF          3
#define ATTACK_MODE_PERM        4
#define ATTACK_MODE_TABLE       5
#define ATTACK_MODE_HYBRID1     6
#define ATTACK_MODE_HYBRID2     7
#define ATTACK_MODE_NONE        100

#define ATTACK_KERN_STRAIGHT    0
#define ATTACK_KERN_COMBI       1
#define ATTACK_KERN_BF          3
#define ATTACK_KERN_NONE        100

#define ATTACK_EXEC_ON_CPU      10
#define ATTACK_EXEC_ON_GPU      11

#define COMBINATOR_MODE_BASE_LEFT  10001
#define COMBINATOR_MODE_BASE_RIGHT 10002

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

#define MAX_CUT_TRIES           4

#define MAX_DICTSTAT            10000

#define NUM_DEFAULT_BENCHMARK_ALGORITHMS 128

#define global_free(attr)       \
{                               \
  myfree ((void *) data.attr);  \
                                \
  data.attr = NULL;             \
}

#define local_free(attr)  \
{                         \
  myfree ((void *) attr); \
                          \
  attr = NULL;            \
}

static uint default_benchmark_algorithms[NUM_DEFAULT_BENCHMARK_ALGORITHMS] =
{
  900,
  0,
  5100,
  100,
  1400,
  10800,
  1700,
  5000,
  10100,
  6000,
  6100,
  6900,
  11700,
  11800,
  400,
  8900,
  11900,
  12000,
  10900,
  12100,
  23,
  2500,
  5300,
  5400,
  5500,
  5600,
  7300,
  7500,
  8300,
  11100,
  11200,
  11400,
  121,
  2611,
  2711,
  2811,
  8400,
  11,
  2612,
  7900,
  21,
  11000,
  124,
  10000,
  3711,
  7600,
  12,
  131,
  132,
  1731,
  200,
  300,
  3100,
  112,
  12300,
  8000,
  141,
  1441,
  1600,
  12600,
  1421,
  101,
  111,
  1711,
  3000,
  1000,
  1100,
  2100,
 12800,
  1500,
  12400,
  500,
  3200,
  7400,
  1800,
  122,
  1722,
  7100,
  6300,
  6700,
  6400,
  6500,
  2400,
  2410,
  5700,
  9200,
  9300,
  22,
  501,
  5800,
  8100,
  8500,
  7200,
  9900,
  7700,
  7800,
  10300,
  8600,
  8700,
  9100,
  133,
  11600,
  12500,
  6211,
  6221,
  6231,
  6241,
  8800,
  12200,
  9700,
  9710,
  9800,
  9810,
  9400,
  9500,
  9600,
  10400,
  10410,
  10500,
  10600,
  10700,
  9000,
  5200,
  6800,
  6600,
  8200,
  11300,
  12700
};

/**
 * types
 */

static void (*get_next_word_func) (char *, uint32_t, uint32_t *, uint32_t *);

/**
 * globals
 */

static unsigned int full01 = 0x01010101;
static unsigned int full80 = 0x80808080;

int SUPPRESS_OUTPUT = 0;

hc_thread_mutex_t mux_adl;
hc_thread_mutex_t mux_counter;
hc_thread_mutex_t mux_dispatcher;
hc_thread_mutex_t mux_display;

hc_global_data_t data;

const char *PROMPT = "[s]tatus [p]ause [r]esume [b]ypass [q]uit => ";

const char *USAGE_MINI[] =
{
  "Usage: %s [options]... hash|hashfile|hccapfile [dictionary|mask|directory]...",
  "",
  "Try --help for more help.",
  NULL
};

const char *USAGE_BIG[] =
{
  "%s, advanced password recovery",
  "",
  "Usage: %s [options]... hash|hashfile|hccapfile [dictionary|mask|directory]...",
  "",
  "=======",
  "Options",
  "=======",
  "",
  "* General:",
  "",
  "  -m,  --hash-type=NUM               Hash-type, see references below",
  "  -a,  --attack-mode=NUM             Attack-mode, see references below",
  "  -V,  --version                     Print version",
  "  -h,  --help                        Print help",
  "       --quiet                       Suppress output",
  "",
  "* Benchmark:",
  "",
  "  -b,  --benchmark                   Run benchmark",
  "       --benchmark-mode=NUM          Benchmark-mode, see references below",
  "",
  "* Misc:",
  "",
  "       --hex-charset                 Assume charset is given in hex",
  "       --hex-salt                    Assume salt is given in hex",
  "       --hex-wordlist                Assume words in wordlist is given in hex",
  "       --force                       Ignore warnings",
  "       --status                      Enable automatic update of the status-screen",
  "       --status-timer=NUM            Seconds between status-screen update",
  "       --status-automat              Display the status view in a machine readable format",
  "       --loopback                    Add new plains to induct directory",
  "       --weak-hash-threshold=NUM     Threshold when to stop checking for weak hashes, default is 100 salts",
  "",
  "* Markov:",
  "",
  "       --markov-hcstat=FILE          Specify hcstat file to use, default is hashcat.hcstat",
  "       --markov-disable              Disables markov-chains, emulates classic brute-force",
  "       --markov-classic              Enables classic markov-chains, no per-position enhancement",
  "  -t,  --markov-threshold=NUM        Threshold when to stop accepting new markov-chains",
  "",
  "* Session:",
  "",
  "       --runtime=NUM                 Abort session after NUM seconds of runtime",
  "       --session=STR                 Define specific session name",
  "       --restore                     Restore session from --session",
  "       --restore-disable             Do not write restore file",
  "",
  "* Files:",
  "",
  "  -o,  --outfile=FILE                Define outfile for recovered hash",
  "       --outfile-format=NUM          Define outfile-format for recovered hash, see references below",
  "       --outfile-autohex-disable     Disable the use of $HEX[] in output plains",
  "       --outfile-check-timer=NUM     Seconds between outfile checks",
  "  -p,  --separator=CHAR              Separator char for hashlists and outfile",
  "       --show                        Show cracked passwords only",
  "       --left                        Show un-cracked passwords only",
  "       --username                    Enable ignoring of usernames in hashfile (recommended: also use --show)",
  "       --remove                      Enable remove of hash once it is cracked",
  "       --remove-timer=NUM            Update input hash file each NUM seconds",
  "       --potfile-disable             Do not write potfile",
  "       --debug-mode=NUM              Defines the debug mode (hybrid only by using rules), see references below",
  "       --debug-file=FILE             Output file for debugging rules (see also --debug-mode)",
  "       --induction-dir=FOLDER        Specify induction directory to use, default is $session.induct",
  "       --outfile-check-dir=FOLDER    Specify the outfile directory which should be monitored, default is $session.outfiles",
  "       --logfile-disable             Disable the logfile",
  "       --truecrypt-keyfiles=FILE     Keyfiles used, seperate with comma",
  "",
  "* Resources:",
  "",
  "  -c,  --segment-size=NUM            Size in MB to cache from the wordfile",
  "       --bitmap-min=NUM              Minimum number of bits allowed for bitmaps",
  "       --bitmap-max=NUM              Maximum number of bits allowed for bitmaps",
  "       --cpu-affinity=STR            Locks to CPU devices, seperate with comma",
  "       --gpu-async                   Use non-blocking async calls (NV only)",
  "  -d,  --gpu-devices=STR             Devices to use, separate with comma",
  "  -w,  --workload-profile=NUM        Enable a specific workload profile, see references below",
  "  -n,  --gpu-accel=NUM               Workload tuning: 1, 8, 40, 80, 160",
  "  -u,  --gpu-loops=NUM               Workload fine-tuning: 8 - 1024",
  "       --gpu-temp-disable            Disable temperature and fanspeed readings and triggers",
  "       --gpu-temp-abort=NUM          Abort session if GPU temperature reaches NUM degrees celsius",
  "       --gpu-temp-retain=NUM         Try to retain GPU temperature at NUM degrees celsius (AMD only)",
  "       --powertune-enable            Enable automatic power tuning option (AMD OverDrive 6 only)",
  "       --scrypt-tmto=NUM             Manually override automatically calculated TMTO value for scrypt",
  "",
  "* Distributed:",
  "",
  "  -s,  --skip=NUM                    Skip number of words",
  "  -l,  --limit=NUM                   Limit number of words",
  "       --keyspace                    Show keyspace base:mod values and quit",
  "",
  "* Rules:",
  "",
  "  -j,  --rule-left=RULE              Single rule applied to each word from left dict",
  "  -k,  --rule-right=RULE             Single rule applied to each word from right dict",
  "  -r,  --rules-file=FILE             Rules-file, multi use: -r 1.rule -r 2.rule",
  "  -g,  --generate-rules=NUM          Generate NUM random rules",
  "       --generate-rules-func-min=NUM Force NUM functions per random rule min",
  "       --generate-rules-func-max=NUM Force NUM functions per random rule max",
  "       --generate-rules-seed=NUM     Force RNG seed to NUM",
  "",
  "* Custom charsets:",
  "",
  "  -1,  --custom-charset1=CS          User-defined charsets",
  "  -2,  --custom-charset2=CS          Example:",
  "  -3,  --custom-charset3=CS          --custom-charset1=?dabcdef : sets charset ?1 to 0123456789abcdef",
  "  -4,  --custom-charset4=CS          -2 mycharset.hcchr : sets charset ?2 to chars contained in file",
  "",
  "* Increment:",
  "",
  "  -i,  --increment                   Enable increment mode",
  "       --increment-min=NUM           Start incrementing at NUM",
  "       --increment-max=NUM           Stop incrementing at NUM",
  "",
  "==========",
  "References",
  "==========",
  "",
  "* Workload Profile:",
  "",
  "    1 = Reduced performance profile (low latency desktop)",
  "    2 = Default performance profile",
  "    3 = Tuned   performance profile (high latency desktop)",
  "",
  "* Benchmark Settings:",
  "",
  "    0 = Manual Tuning",
  "    1 = Performance Tuning, default",
  "",
  "* Outfile Formats:",
  "",
  "    1 = hash[:salt]",
  "    2 = plain",
  "    3 = hash[:salt]:plain",
  "    4 = hex_plain",
  "    5 = hash[:salt]:hex_plain",
  "    6 = plain:hex_plain",
  "    7 = hash[:salt]:plain:hex_plain",
  "    8 = crackpos",
  "    9 = hash[:salt]:crackpos",
  "   10 = plain:crackpos",
  "   11 = hash[:salt]:plain:crackpos",
  "   12 = hex_plain:crackpos",
  "   13 = hash[:salt]:hex_plain:crackpos",
  "   14 = plain:hex_plain:crackpos",
  "   15 = hash[:salt]:plain:hex_plain:crackpos",
  "",
  "* Debug mode output formats (for hybrid mode only, by using rules):",
  "",
  "    1 = save finding rule",
  "    2 = save original word",
  "    3 = save original word and finding rule",
  "    4 = save original word, finding rule and modified plain",
  "",
  "* Built-in charsets:",
  "",
  "   ?l = abcdefghijklmnopqrstuvwxyz",
  "   ?u = ABCDEFGHIJKLMNOPQRSTUVWXYZ",
  "   ?d = 0123456789",
  "   ?s =  !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~",
  "   ?a = ?l?u?d?s",
  "   ?b = 0x00 - 0xff",
  "",
  "* Attack modes:",
  "",
  "    0 = Straight",
  "    1 = Combination",
  "    3 = Brute-force",
  "    6 = Hybrid dict + mask",
  "    7 = Hybrid mask + dict",
  "",
  "* Hash types:",
  "",
  "[[ Roll-your-own: Raw Hashes ]]",
  "",
  "    900 = MD4",
  "      0 = MD5",
  "   5100 = Half MD5",
  "    100 = SHA1",
  "  10800 = SHA-384",
  "   1400 = SHA-256",
  "   1700 = SHA-512",
  "   5000 = SHA-3(Keccak)",
  "  10100 = SipHash",
  "   6000 = RipeMD160",
  "   6100 = Whirlpool",
  "   6900 = GOST R 34.11-94",
  "  11700 = GOST R 34.11-2012 (Streebog) 256-bit",
  "  11800 = GOST R 34.11-2012 (Streebog) 512-bit",
  "",
  "[[ Roll-your-own: Iterated and / or Salted Hashes ]]",
  "",
  "     10 = md5($pass.$salt)",
  "     20 = md5($salt.$pass)",
  "     30 = md5(unicode($pass).$salt)",
  "     40 = md5($salt.unicode($pass))",
  "   3800 = md5($salt.$pass.$salt)",
  "   3710 = md5($salt.md5($pass))",
  "   2600 = md5(md5($pass)",
  "   4300 = md5(strtoupper(md5($pass)))",
  "   4400 = md5(sha1($pass))",
  "    110 = sha1($pass.$salt)",
  "    120 = sha1($salt.$pass)",
  "    130 = sha1(unicode($pass).$salt)",
  "    140 = sha1($salt.unicode($pass))",
  "   4500 = sha1(sha1($pass)",
  "   4700 = sha1(md5($pass))",
  "   4900 = sha1($salt.$pass.$salt)",
  "   1410 = sha256($pass.$salt)",
  "   1420 = sha256($salt.$pass)",
  "   1430 = sha256(unicode($pass).$salt)",
  "   1440 = sha256($salt.unicode($pass))",
  "   1710 = sha512($pass.$salt)",
  "   1720 = sha512($salt.$pass)",
  "   1730 = sha512(unicode($pass).$salt)",
  "   1740 = sha512($salt.unicode($pass))",
  "",
  "[[ Roll-your-own: Authenticated Hashes ]]",
  "",
  "     50 = HMAC-MD5 (key = $pass)",
  "     60 = HMAC-MD5 (key = $salt)",
  "    150 = HMAC-SHA1 (key = $pass)",
  "    160 = HMAC-SHA1 (key = $salt)",
  "   1450 = HMAC-SHA256 (key = $pass)",
  "   1460 = HMAC-SHA256 (key = $salt)",
  "   1750 = HMAC-SHA512 (key = $pass)",
  "   1760 = HMAC-SHA512 (key = $salt)",
  "",
  "[[ Generic KDF ]]",
  "",
  "    400 = phpass",
  "   8900 = scrypt",
  "  11900 = PBKDF2-HMAC-MD5",
  "  12000 = PBKDF2-HMAC-SHA1",
  "  10900 = PBKDF2-HMAC-SHA256",
  "  12100 = PBKDF2-HMAC-SHA512",
  "",
  "[[ Network protocols, Challenge-Response ]]",
  "",
  "     23 = Skype",
  "   2500 = WPA/WPA2",
  "   4800 = iSCSI CHAP authentication, MD5(Chap)",
  "   5300 = IKE-PSK MD5",
  "   5400 = IKE-PSK SHA1",
  "   5500 = NetNTLMv1",
  "   5500 = NetNTLMv1 + ESS",
  "   5600 = NetNTLMv2",
  "   7300 = IPMI2 RAKP HMAC-SHA1",
  "   7500 = Kerberos 5 AS-REQ Pre-Auth etype 23",
  "   8300 = DNSSEC (NSEC3)",
  "  10200 = Cram MD5",
  "  11100 = PostgreSQL Challenge-Response Authentication (MD5)",
  "  11200 = MySQL Challenge-Response Authentication (SHA1)",
  "  11400 = SIP digest authentication (MD5)",
  "",
  "[[ Forums, CMS, E-Commerce, Frameworks, Middleware, Wiki, Management ]]",
  "",
  "    121 = SMF (Simple Machines Forum)",
  "    400 = phpBB3",
  "   2611 = vBulletin < v3.8.5",
  "   2711 = vBulletin > v3.8.5",
  "   2811 = MyBB",
  "   2811 = IPB (Invison Power Board)",
  "   8400 = WBB3 (Woltlab Burning Board)",
  "     11 = Joomla < 2.5.18",
  "    400 = Joomla > 2.5.18",
  "    400 = Wordpress",
  "   2612 = PHPS",
  "   7900 = Drupal7",
  "     21 = osCommerce",
  "     21 = xt:Commerce",
  "  11000 = PrestaShop",
  "    124 = Django (SHA-1)",
  "  10000 = Django (PBKDF2-SHA256)",
  "   3711 = Mediawiki B type",
  "   7600 = Redmine",
  "",
  "[[ Database Server ]]",
  "",
  "     12 = PostgreSQL",
  "    131 = MSSQL(2000)",
  "    132 = MSSQL(2005)",
  "   1731 = MSSQL(2012)",
  "   1731 = MSSQL(2014)",
  "    200 = MySQL323",
  "    300 = MySQL4.1/MySQL5",
  "   3100 = Oracle H: Type (Oracle 7+)",
  "    112 = Oracle S: Type (Oracle 11+)",
  "  12300 = Oracle T: Type (Oracle 12+)",
  "   8000 = Sybase ASE",
  "",
  "[[ HTTP, SMTP, LDAP Server]]",
  "",
  "    141 = EPiServer 6.x < v4",
  "   1441 = EPiServer 6.x > v4",
  "   1600 = Apache $apr1$",
  "  12600 = ColdFusion 10+",
  "   1421 = hMailServer",
  "    101 = nsldap, SHA-1(Base64), Netscape LDAP SHA",
  "    111 = nsldaps, SSHA-1(Base64), Netscape LDAP SSHA",
  "   1711 = SSHA-512(Base64), LDAP {SSHA512}",
  "",
  "[[ Checksums ]]",
  "",
  "  11500 = CRC32",
  "",
  "[[ Operating-Systems ]]",
  "",
  "   3000 = LM",
  "   1000 = NTLM",
  "   1100 = Domain Cached Credentials (DCC), MS Cache",
  "   2100 = Domain Cached Credentials 2 (DCC2), MS Cache 2",
  "  12800 = MS-AzureSync PBKDF2-HMAC-SHA256",
  "   1500 = descrypt, DES(Unix), Traditional DES",
  "  12400 = BSDiCrypt, Extended DES",
  "    500 = md5crypt $1$, MD5(Unix)",
  "   3200 = bcrypt $2*$, Blowfish(Unix)",
  "   7400 = sha256crypt $5$, SHA256(Unix)",
  "   1800 = sha512crypt $6$, SHA512(Unix)",
  "    122 = OSX v10.4",
  "    122 = OSX v10.5",
  "    122 = OSX v10.6",
  "   1722 = OSX v10.7",
  "   7100 = OSX v10.8",
  "   7100 = OSX v10.9",
  "   7100 = OSX v10.10",
  "   6300 = AIX {smd5}",
  "   6700 = AIX {ssha1}",
  "   6400 = AIX {ssha256}",
  "   6500 = AIX {ssha512}",
  "   2400 = Cisco-PIX",
  "   2410 = Cisco-ASA",
  "    500 = Cisco-IOS $1$",
  "   5700 = Cisco-IOS $4$",
  "   9200 = Cisco-IOS $8$",
  "   9300 = Cisco-IOS $9$",
  "     22 = Juniper Netscreen/SSG (ScreenOS)",
  "    501 = Juniper IVE",
  "   5800 = Android PIN",
  "   8100 = Citrix Netscaler",
  "   8500 = RACF",
  "   7200 = GRUB 2",
  "   9900 = Radmin2",
  "",
  "[[ Enterprise Application Software (EAS) ]]",
  "",
  "   7700 = SAP CODVN B (BCODE)",
  "   7800 = SAP CODVN F/G (PASSCODE)",
  "  10300 = SAP CODVN H (PWDSALTEDHASH) iSSHA-1",
  "   8600 = Lotus Notes/Domino 5",
  "   8700 = Lotus Notes/Domino 6",
  "   9100 = Lotus Notes/Domino 8",
  "    133 = PeopleSoft",
  "",
  "[[ Archives ]]",
  "",
  "  11600 = 7-Zip",
  "  12500 = RAR3-hp",
  "",
  "[[ Full-Disk encryptions (FDE) ]]",
  "",
  "   62XY = TrueCrypt 5.0+",
  "     X  = 1 = PBKDF2-HMAC-RipeMD160",
  "     X  = 2 = PBKDF2-HMAC-SHA512",
  "     X  = 3 = PBKDF2-HMAC-Whirlpool",
  "     X  = 4 = PBKDF2-HMAC-RipeMD160 + boot-mode",
  "      Y = 1 = XTS  512 bit (Ciphers: AES or Serpent or Twofish)",
  "      Y = 2 = XTS 1024 bit (Ciphers: AES or Serpent or Twofish or AES-Twofish or Serpent-AES or Twofish-Serpent)",
  "      Y = 3 = XTS 1536 bit (Ciphers: All)",
  "   8800 = Android FDE < v4.3",
  "  12200 = eCryptfs",
  "",
  "[[ Documents ]]",
  "",
  "   9700 = MS Office <= 2003 MD5 + RC4, oldoffice$0, oldoffice$1",
  "   9710 = MS Office <= 2003 MD5 + RC4, collider-mode #1",
  "   9720 = MS Office <= 2003 MD5 + RC4, collider-mode #2",
  "   9800 = MS Office <= 2003 SHA1 + RC4, oldoffice$3, oldoffice$4",
  "   9810 = MS Office <= 2003 SHA1 + RC4, collider-mode #1",
  "   9820 = MS Office <= 2003 SHA1 + RC4, collider-mode #2",
  "   9400 = MS Office 2007",
  "   9500 = MS Office 2010",
  "   9600 = MS Office 2013",
  "  10400 = PDF 1.1 - 1.3 (Acrobat 2 - 4)",
  "  10410 = PDF 1.1 - 1.3 (Acrobat 2 - 4) + collider-mode #1",
  "  10420 = PDF 1.1 - 1.3 (Acrobat 2 - 4) + collider-mode #2",
  "  10500 = PDF 1.4 - 1.6 (Acrobat 5 - 8)",
  "  10600 = PDF 1.7 Level 3 (Acrobat 9)",
  "  10700 = PDF 1.7 Level 8 (Acrobat 10 - 11)",
  "",
  "[[ Password Managers ]]",
  "",
  "   9000 = Password Safe v2",
  "   5200 = Password Safe v3",
  "   6800 = Lastpass",
  "   6600 = 1Password, agilekeychain",
  "   8200 = 1Password, cloudkeychain",
  "  11300 = Bitcoin/Litecoin wallet.dat",
  "  12700 = Blockchain, My Wallet",
  "",
  NULL
};

/**
 * oclHashcat specific functions
 */

void status_display_automat ()
{
  FILE *out = stdout;

  fprintf (out, "STATUS\t%u\t", data.devices_status);

  /**
   * speed new
   */

  fprintf (out, "SPEED\t");

  for (uint device_id = 0; device_id < data.devices_cnt; device_id++)
  {
    hc_device_param_t *device_param = &data.devices_param[device_id];

    uint64_t  speed_cnt  = 0;
    float     speed_ms   = 0;

    for (int i = 0; i < SPEED_CACHE; i++)
    {
      float rec_ms;

      hc_timer_get (device_param->speed_rec[i], rec_ms);

      if (rec_ms > SPEED_MAXAGE) continue;

      speed_cnt  += device_param->speed_cnt[i];
      speed_ms   += device_param->speed_ms[i];
    }

    speed_cnt  /= SPEED_CACHE;
    speed_ms   /= SPEED_CACHE;

    fprintf (out, "%llu\t%f\t", (unsigned long long int) speed_cnt, speed_ms);
  }

  /**
   * words_cur
   */

  uint64_t words_cur = get_lowest_words_done ();

  fprintf (out, "CURKU\t%llu\t", (unsigned long long int) words_cur);

  /**
   * counter
   */

  uint salts_left = data.salts_cnt - data.salts_done;

  if (salts_left == 0) salts_left = 1;

  uint64_t progress_total = data.words_cnt * salts_left;

  uint64_t all_done     = 0;
  uint64_t all_rejected = 0;
  uint64_t all_restored = 0;

  for (uint salt_pos = 0; salt_pos < data.salts_cnt; salt_pos++)
  {
    if (salts_left > 1)
    {
      // otherwise the final cracked status shows 0/XXX progress

      if (data.salts_shown[salt_pos] == 1) continue;
    }

    all_done     += data.words_progress_done[salt_pos];
    all_rejected += data.words_progress_rejected[salt_pos];
    all_restored += data.words_progress_restored[salt_pos];
  }

  uint64_t progress_cur = all_restored + all_done + all_rejected;
  uint64_t progress_end = progress_total;

  uint64_t progress_skip = 0;

  if (data.skip)
  {
    progress_skip = MIN (data.skip, data.words_base) * salts_left;

    if      (data.attack_kern == ATTACK_KERN_STRAIGHT) progress_skip *= data.gpu_rules_cnt;
    else if (data.attack_kern == ATTACK_KERN_COMBI)    progress_skip *= data.combs_cnt;
    else if (data.attack_kern == ATTACK_KERN_BF)       progress_skip *= data.bfs_cnt;
  }

  if (data.limit)
  {
    progress_end = MIN (data.limit, data.words_base) * salts_left;

    if      (data.attack_kern == ATTACK_KERN_STRAIGHT) progress_end  *= data.gpu_rules_cnt;
    else if (data.attack_kern == ATTACK_KERN_COMBI)    progress_end  *= data.combs_cnt;
    else if (data.attack_kern == ATTACK_KERN_BF)       progress_end  *= data.bfs_cnt;
  }

  uint64_t progress_cur_relative_skip = progress_cur - progress_skip;
  uint64_t progress_end_relative_skip = progress_end - progress_skip;

  fprintf (out, "PROGRESS\t%llu\t%llu\t", (unsigned long long int) progress_cur_relative_skip, (unsigned long long int) progress_end_relative_skip);

  /**
   * cracks
   */

  fprintf (out, "RECHASH\t%u\t%u\t", data.digests_done, data.digests_cnt);
  fprintf (out, "RECSALT\t%u\t%u\t", data.salts_done,   data.salts_cnt);

  /**
   * temperature
   */

  if (data.gpu_temp_disable == 0)
  {
    fprintf (out, "TEMP\t");

    hc_thread_mutex_lock (mux_adl);

    for (uint i = 0; i < data.devices_cnt; i++)
    {
      int temp = hm_get_temperature_with_device_id (i);

      fprintf (out, "%d\t", temp);
    }

    hc_thread_mutex_unlock (mux_adl);
  }

  #ifdef _WIN
  fputc ('\r', out);
  fputc ('\n', out);
  #endif

  #ifdef _POSIX
  fputc ('\n', out);
  #endif

  fflush (out);
}

void status_display ()
{
  if (data.devices_status == STATUS_INIT)     return;
  if (data.devices_status == STATUS_STARTING) return;
  if (data.devices_status == STATUS_BYPASS)   return;

  if (data.status_automat == 1)
  {
    status_display_automat ();

    return;
  }

  char tmp_buf[1000];

  uint tmp_len = 0;

  log_info ("Session.Name...: %s", data.session);

  char *status_type = strstatus (data.devices_status);

  uint hash_mode = data.hash_mode;

  char *hash_type = strhashtype (hash_mode); // not a bug

  log_info ("Status.........: %s", status_type);

  /**
   * show rules
   */

  if (data.rp_files_cnt)
  {
    uint i;

    for (i = 0, tmp_len = 0; i < data.rp_files_cnt - 1 && tmp_len < sizeof (tmp_buf); i++)
    {
      tmp_len += snprintf (tmp_buf + tmp_len, sizeof (tmp_buf) - tmp_len, "File (%s), ", data.rp_files[i]);
    }

    snprintf (tmp_buf + tmp_len, sizeof (tmp_buf) - tmp_len, "File (%s)", data.rp_files[i]);

    log_info ("Rules.Type.....: %s", tmp_buf);

    tmp_len = 0;
  }

  if (data.rp_gen)
  {
    log_info ("Rules.Type.....: Generated (%u)", data.rp_gen);

    if (data.rp_gen_seed)
    {
      log_info ("Rules.Seed.....: %u", data.rp_gen_seed);
    }
  }

  /**
   * show input
   */

  if (data.attack_mode == ATTACK_MODE_STRAIGHT)
  {
    if (data.wordlist_mode == WL_MODE_FILE)
    {
      if (data.dictfile != NULL) log_info ("Input.Mode.....: File (%s)", data.dictfile);
    }
    else if (data.wordlist_mode == WL_MODE_STDIN)
    {
      log_info ("Input.Mode.....: Pipe");
    }
  }
  else if (data.attack_mode == ATTACK_MODE_COMBI)
  {
    if (data.dictfile  != NULL) log_info ("Input.Left.....: File (%s)", data.dictfile);
    if (data.dictfile2 != NULL) log_info ("Input.Right....: File (%s)", data.dictfile2);
  }
  else if (data.attack_mode == ATTACK_MODE_BF)
  {
    char *mask = data.mask;

    if (mask != NULL)
    {
      uint mask_len = data.css_cnt;

      tmp_len += snprintf (tmp_buf + tmp_len, sizeof (tmp_buf) - tmp_len, "Mask (%s)", mask);

      if (mask_len > 0)
      {
        if (data.opti_type & OPTI_TYPE_SINGLE_HASH)
        {
          if (data.opti_type & OPTI_TYPE_APPENDED_SALT)
          {
            mask_len -= data.salts_buf[0].salt_len;
          }
        }

        if (data.opts_type & OPTS_TYPE_PT_UNICODE) mask_len /= 2;

        tmp_len += snprintf (tmp_buf + tmp_len, sizeof (tmp_buf) - tmp_len, " [%i]", mask_len);
      }

      if (data.maskcnt > 1)
      {
        float mask_percentage = (float) data.maskpos / (float) data.maskcnt;

        tmp_len += snprintf (tmp_buf + tmp_len, sizeof (tmp_buf) - tmp_len, " (%.02f%%)", mask_percentage * 100);
      }

      log_info ("Input.Mode.....: %s", tmp_buf);
    }

    tmp_len = 0;
  }
  else if (data.attack_mode == ATTACK_MODE_HYBRID1)
  {
    if (data.dictfile != NULL) log_info ("Input.Left.....: File (%s)", data.dictfile);
    if (data.mask     != NULL) log_info ("Input.Right....: Mask (%s) [%i]", data.mask, data.css_cnt);
  }
  else if (data.attack_mode == ATTACK_MODE_HYBRID2)
  {
    if (data.mask     != NULL) log_info ("Input.Left.....: Mask (%s) [%i]", data.mask, data.css_cnt);
    if (data.dictfile != NULL) log_info ("Input.Right....: File (%s)", data.dictfile);
  }

  if (data.digests_cnt == 1)
  {
    if (data.hash_mode == 2500)
    {
      wpa_t *wpa = (wpa_t *) data.esalts_buf;

      uint pke[25];

      char *pke_ptr = (char *) pke;

      for (uint i = 0; i < 25; i++)
      {
        pke[i] = byte_swap_32 (wpa->pke[i]);
      }

      char mac1[6];
      char mac2[6];

      memcpy (mac1, pke_ptr + 23, 6);
      memcpy (mac2, pke_ptr + 29, 6);

      log_info ("Hash.Target....: %s (%02x:%02x:%02x:%02x:%02x:%02x <-> %02x:%02x:%02x:%02x:%02x:%02x)",
                (char *) data.salts_buf[0].salt_buf,
                mac1[0] & 0xff,
                mac1[1] & 0xff,
                mac1[2] & 0xff,
                mac1[3] & 0xff,
                mac1[4] & 0xff,
                mac1[5] & 0xff,
                mac2[0] & 0xff,
                mac2[1] & 0xff,
                mac2[2] & 0xff,
                mac2[3] & 0xff,
                mac2[4] & 0xff,
                mac2[5] & 0xff);
    }
    else if (data.hash_mode == 5200)
    {
      log_info ("Hash.Target....: File (%s)", data.hashfile);
    }
    else if (data.hash_mode == 9000)
    {
      log_info ("Hash.Target....: File (%s)", data.hashfile);
    }
    else if ((data.hash_mode >= 6200) && (data.hash_mode <= 6299))
    {
      log_info ("Hash.Target....: File (%s)", data.hashfile);
    }
    else
    {
      char out_buf[4096];

      ascii_digest (out_buf, 0, 0);

      // limit length
      if (strlen (out_buf) > 40)
      {
        out_buf[41] = '.';
        out_buf[42] = '.';
        out_buf[43] = '.';
        out_buf[44] = 0;
      }

      log_info ("Hash.Target....: %s", out_buf);
    }
  }
  else
  {
    if (data.hash_mode == 3000)
    {
      char out_buf1[4096];
      char out_buf2[4096];

      ascii_digest (out_buf1, 0, 0);
      ascii_digest (out_buf2, 0, 1);

      log_info ("Hash.Target....: %s, %s", out_buf1, out_buf2);
    }
    else
    {
      log_info ("Hash.Target....: File (%s)", data.hashfile);
    }
  }

  log_info ("Hash.Type......: %s", hash_type);

  /**
   * speed new
   */

  uint64_t speed_cnt[DEVICES_MAX];
  float    speed_ms[DEVICES_MAX];

  for (uint device_id = 0; device_id < data.devices_cnt; device_id++)
  {
    hc_device_param_t *device_param = &data.devices_param[device_id];

    // we need to clear values (set to 0) because in case the gpu does
    // not get new candidates it idles around but speed display would
    // show it as working.
    // if we instantly set it to 0 after reading it happens that the
    // speed can be shown as zero if the users refreshs to fast.
    // therefore, we add a timestamp when a stat was recorded and if its
    // to old we will not use it

    speed_cnt[device_id] = 0;
    speed_ms[device_id]  = 0;

    for (int i = 0; i < SPEED_CACHE; i++)
    {
      float rec_ms;

      hc_timer_get (device_param->speed_rec[i], rec_ms);

      if (rec_ms > SPEED_MAXAGE) continue;

      speed_cnt[device_id] += device_param->speed_cnt[i];
      speed_ms[device_id]  += device_param->speed_ms[i];
    }

    speed_cnt[device_id] /= SPEED_CACHE;
    speed_ms[device_id]  /= SPEED_CACHE;
  }

  float hashes_all_ms = 0;

  float hashes_dev_ms[DEVICES_MAX];

  for (uint device_id = 0; device_id < data.devices_cnt; device_id++)
  {
    hashes_dev_ms[device_id] = 0;

    if (speed_ms[device_id])
    {
      hashes_dev_ms[device_id] = speed_cnt[device_id] / speed_ms[device_id];

      hashes_all_ms += hashes_dev_ms[device_id];
    }
  }

  /**
   * timers
   */

  float ms_running = 0;

  hc_timer_get (data.timer_running, ms_running);

  float ms_paused = data.ms_paused;

  if (data.devices_status == STATUS_PAUSED)
  {
    float ms_paused_tmp = 0;

    hc_timer_get (data.timer_paused, ms_paused_tmp);

    ms_paused += ms_paused_tmp;
  }

  #ifdef WIN

  __time64_t sec_run = ms_running / 1000;

  #else

  time_t sec_run = ms_running / 1000;

  #endif

  if (sec_run)
  {
    char display_run[32];

    struct tm tm_run;

    struct tm *tmp;

    #ifdef WIN

    tmp = _gmtime64 (&sec_run);

    #else

    tmp = gmtime (&sec_run);

    #endif

    if (tmp != NULL)
    {
      memcpy (&tm_run, tmp, sizeof (struct tm));

      format_timer_display (&tm_run, display_run, sizeof (tm_run));

      char *start = ctime (&data.proc_start);

      size_t start_len = strlen (start);

      if (start[start_len - 1] == '\n') start[start_len - 1] = 0;
      if (start[start_len - 2] == '\r') start[start_len - 2] = 0;

      log_info ("Time.Started...: %s (%s)", start, display_run);
    }
  }
  else
  {
    log_info ("Time.Started...: 0 secs");
  }

  /**
   * counters
   */

  uint salts_left = data.salts_cnt - data.salts_done;

  if (salts_left == 0) salts_left = 1;

  uint64_t progress_total = data.words_cnt * salts_left;

  uint64_t all_done     = 0;
  uint64_t all_rejected = 0;
  uint64_t all_restored = 0;

  for (uint salt_pos = 0; salt_pos < data.salts_cnt; salt_pos++)
  {
    if (salts_left > 1)
    {
      // otherwise the final cracked status shows 0/XXX progress

      if (data.salts_shown[salt_pos] == 1) continue;
    }

    all_done     += data.words_progress_done[salt_pos];
    all_rejected += data.words_progress_rejected[salt_pos];
    all_restored += data.words_progress_restored[salt_pos];
  }

  uint64_t progress_cur = all_restored + all_done + all_rejected;
  uint64_t progress_end = progress_total;

  uint64_t progress_skip = 0;

  if (data.skip)
  {
    progress_skip = MIN (data.skip, data.words_base) * salts_left;

    if      (data.attack_kern == ATTACK_KERN_STRAIGHT) progress_skip *= data.gpu_rules_cnt;
    else if (data.attack_kern == ATTACK_KERN_COMBI)    progress_skip *= data.combs_cnt;
    else if (data.attack_kern == ATTACK_KERN_BF)       progress_skip *= data.bfs_cnt;
  }

  if (data.limit)
  {
    progress_end = MIN (data.limit, data.words_base) * salts_left;

    if      (data.attack_kern == ATTACK_KERN_STRAIGHT) progress_end  *= data.gpu_rules_cnt;
    else if (data.attack_kern == ATTACK_KERN_COMBI)    progress_end  *= data.combs_cnt;
    else if (data.attack_kern == ATTACK_KERN_BF)       progress_end  *= data.bfs_cnt;
  }

  uint64_t progress_cur_relative_skip = progress_cur - progress_skip;
  uint64_t progress_end_relative_skip = progress_end - progress_skip;

  float    speed_ms_real     = ms_running - ms_paused;
  uint64_t speed_plains_real = all_done;

  if ((data.wordlist_mode == WL_MODE_FILE) || (data.wordlist_mode == WL_MODE_MASK))
  {
    if (data.devices_status != STATUS_CRACKED)
    {
      uint64_t words_per_ms = 0;

      if (speed_plains_real && speed_ms_real)
      {
        words_per_ms = speed_plains_real / speed_ms_real;
      }

      #ifdef WIN
      __time64_t sec_etc = 0;
      #else
      time_t sec_etc = 0;
      #endif

      if (words_per_ms)
      {
        uint64_t progress_left_relative_skip = progress_end_relative_skip - progress_cur_relative_skip;

        uint64_t ms_left = progress_left_relative_skip / words_per_ms;

        sec_etc = ms_left / 1000;
      }

      if (sec_etc == 0)
      {
        log_info ("Time.Estimated.: 0 secs");
      }
      else if ((uint64_t) sec_etc > ETC_MAX)
      {
        log_info ("Time.Estimated.: > 10 Years");
      }
      else
      {
        char display_etc[32];

        struct tm tm_etc;

        struct tm *tmp;

        #ifdef WIN

        tmp = _gmtime64 (&sec_etc);

        #else

        tmp = gmtime (&sec_etc);

        #endif

        if (tmp != NULL)
        {
          memcpy (&tm_etc, tmp, sizeof (tm_etc));

          format_timer_display (&tm_etc, display_etc, sizeof (display_etc));

          time_t now;

          time (&now);

          now += sec_etc;

          char *etc = ctime (&now);

          size_t etc_len = strlen (etc);

          if (etc[etc_len - 1] == '\n') etc[etc_len - 1] = 0;
          if (etc[etc_len - 2] == '\r') etc[etc_len - 2] = 0;

          log_info ("Time.Estimated.: %s (%s)", etc, display_etc);
        }
      }
    }
  }

  for (uint device_id = 0; device_id < data.devices_cnt; device_id++)
  {
    char display_dev_cur[16];

    memset (display_dev_cur, 0, sizeof (display_dev_cur));

    strncpy (display_dev_cur, "0.00", 4);

    format_speed_display (hashes_dev_ms[device_id] * 1000, display_dev_cur, sizeof (display_dev_cur));

    log_info ("Speed.GPU.#%d...: %9sH/s", device_id + 1, display_dev_cur);
  }

  char display_all_cur[16];

  memset (display_all_cur, 0, sizeof (display_all_cur));

  strncpy (display_all_cur, "0.00", 4);

  format_speed_display (hashes_all_ms * 1000, display_all_cur, sizeof (display_all_cur));

  if (data.devices_cnt > 1) log_info ("Speed.GPU.#*...: %9sH/s", display_all_cur);

  const float digests_percent = (float) data.digests_done / data.digests_cnt;
  const float salts_percent   = (float) data.salts_done   / data.salts_cnt;

  log_info ("Recovered......: %u/%u (%.2f%%) Digests, %u/%u (%.2f%%) Salts", data.digests_done, data.digests_cnt, digests_percent * 100, data.salts_done, data.salts_cnt, salts_percent * 100);

  // crack-per-time

  if (data.digests_cnt > 100)
  {
    time_t now = time (NULL);

    int cpt_cur_min  = 0;
    int cpt_cur_hour = 0;
    int cpt_cur_day  = 0;

    for (int i = 0; i < CPT_BUF; i++)
    {
      const uint   cracked   = data.cpt_buf[i].cracked;
      const time_t timestamp = data.cpt_buf[i].timestamp;

      if ((timestamp + 60) > now)
      {
        cpt_cur_min  += cracked;
      }

      if ((timestamp + 3600) > now)
      {
        cpt_cur_hour += cracked;
      }

      if ((timestamp + 86400) > now)
      {
        cpt_cur_day  += cracked;
      }
    }

    float cpt_avg_min  = (float) data.cpt_total / ((speed_ms_real / 1000) / 60);
    float cpt_avg_hour = (float) data.cpt_total / ((speed_ms_real / 1000) / 3600);
    float cpt_avg_day  = (float) data.cpt_total / ((speed_ms_real / 1000) / 86400);

    if ((data.cpt_start + 86400) < now)
    {
      log_info ("Recovered/Time.: CUR:%llu,%llu,%llu AVG:%0.2f,%0.2f,%0.2f (Min,Hour,Day)",
        cpt_cur_min,
        cpt_cur_hour,
        cpt_cur_day,
        cpt_avg_min,
        cpt_avg_hour,
        cpt_avg_day);
    }
    else if ((data.cpt_start + 3600) < now)
    {
      log_info ("Recovered/Time.: CUR:%llu,%llu,N/A AVG:%0.2f,%0.2f,%0.2f (Min,Hour,Day)",
        cpt_cur_min,
        cpt_cur_hour,
        cpt_avg_min,
        cpt_avg_hour,
        cpt_avg_day);
    }
    else if ((data.cpt_start + 60) < now)
    {
      log_info ("Recovered/Time.: CUR:%llu,N/A,N/A AVG:%0.2f,%0.2f,%0.2f (Min,Hour,Day)",
        cpt_cur_min,
        cpt_avg_min,
        cpt_avg_hour,
        cpt_avg_day);
    }
    else
    {
      log_info ("Recovered/Time.: CUR:N/A,N/A,N/A AVG:%0.2f,%0.2f,%0.2f (Min,Hour,Day)",
        cpt_avg_min,
        cpt_avg_hour,
        cpt_avg_day);
    }
  }

  // Restore point

  uint64_t restore_point = get_lowest_words_done ();

  uint64_t restore_total = data.words_base;

  float percent_restore = 0;

  if (restore_total != 0) percent_restore = (float) restore_point / (float) restore_total;

  if (progress_end_relative_skip)
  {
    if ((data.wordlist_mode == WL_MODE_FILE) || (data.wordlist_mode == WL_MODE_MASK))
    {
      float percent_finished = (float) progress_cur_relative_skip / (float) progress_end_relative_skip;
      float percent_rejected = 0.0;

      if (progress_cur)
      {
        percent_rejected = (float) (all_rejected) / (float) progress_cur;
      }

      log_info ("Progress.......: %llu/%llu (%.02f%%)", (unsigned long long int) progress_cur_relative_skip, (unsigned long long int) progress_end_relative_skip, percent_finished * 100);
      log_info ("Rejected.......: %llu/%llu (%.02f%%)", (unsigned long long int) all_rejected,               (unsigned long long int) progress_cur_relative_skip, percent_rejected * 100);

      if (data.restore_disable == 0)
      {
        if (percent_finished != 1)
        {
          log_info ("Restore.Point..: %llu/%llu (%.02f%%)", (unsigned long long int) restore_point, (unsigned long long int) restore_total, percent_restore * 100);
        }
      }
    }
  }
  else
  {
    if ((data.wordlist_mode == WL_MODE_FILE) || (data.wordlist_mode == WL_MODE_MASK))
    {
      log_info ("Progress.......: %llu/%llu (%.02f%%)", (uint64_t) 0, (uint64_t) 0, (float) 100);
      log_info ("Rejected.......: %llu/%llu (%.02f%%)", (uint64_t) 0, (uint64_t) 0, (float) 100);

      if (data.restore_disable == 0)
      {
        log_info ("Restore point..: %llu/%llu (%.02f%%)", (uint64_t) 0, (uint64_t) 0, (float) 100);
      }
    }
    else
    {
      log_info ("Progress.......: %llu", (unsigned long long int) progress_cur_relative_skip);
      log_info ("Rejected.......: %llu", (unsigned long long int) all_rejected);

      // --restore not allowed if stdin is used -- really? why?

      //if (data.restore_disable == 0)
      //{
      //  log_info ("Restore point..: %llu", (unsigned long long int) restore_point);
      //}
    }
  }

  if (data.gpu_temp_disable == 0)
  {
    hc_thread_mutex_lock (mux_adl);

    for (uint i = 0; i < data.devices_cnt; i++)
    {
      if (data.hm_device[i].fan_supported == 1)
      {
        const int temperature = hm_get_temperature_with_device_id (i);
        const int utilization = hm_get_utilization_with_device_id (i);
        const int fanspeed    = hm_get_fanspeed_with_device_id (i);

        #ifdef _OCL
        log_info ("HWMon.GPU.#%d...: %2d%% Util, %2dc Temp, %2d%% Fan", i + 1, utilization, temperature, fanspeed);
        #else
        #ifdef LINUX
        log_info ("HWMon.GPU.#%d...: %2d%% Util, %2dc Temp, %2d%% Fan", i + 1, utilization, temperature, fanspeed);
        #else
        log_info ("HWMon.GPU.#%d...: %2d%% Util, %2dc Temp, %2drpm Fan", i + 1, utilization, temperature, fanspeed);
        #endif
        #endif
      }
      else
      {
        const int temperature = hm_get_temperature_with_device_id (i);
        const int utilization = hm_get_utilization_with_device_id (i);

        log_info ("HWMon.GPU.#%d...: %2d%% Util, %2dc Temp, N/A Fan", i + 1, utilization, temperature);
      }
    }

    hc_thread_mutex_unlock (mux_adl);
  }
}

static void status_benchmark ()
{
  if (data.devices_status == STATUS_INIT) return;
  if (data.devices_status == STATUS_STARTING) return;

  if (data.words_cnt == 0) return;

  uint64_t speed_cnt[DEVICES_MAX];
  float    speed_ms[DEVICES_MAX];

  uint device_id;

  for (device_id = 0; device_id < data.devices_cnt; device_id++)
  {
    hc_device_param_t *device_param = &data.devices_param[device_id];

    speed_cnt[device_id] = 0;
    speed_ms[device_id]  = 0;

    for (int i = 0; i < SPEED_CACHE; i++)
    {
      speed_cnt[device_id] += device_param->speed_cnt[i];
      speed_ms[device_id]  += device_param->speed_ms[i];
    }

    speed_cnt[device_id] /= SPEED_CACHE;
    speed_ms[device_id]  /= SPEED_CACHE;
  }

  float hashes_all_ms = 0;

  float hashes_dev_ms[DEVICES_MAX];

  for (uint device_id = 0; device_id < data.devices_cnt; device_id++)
  {
    hashes_dev_ms[device_id] = 0;

    if (speed_ms[device_id])
    {
      hashes_dev_ms[device_id] = speed_cnt[device_id] / speed_ms[device_id];

      hashes_all_ms += hashes_dev_ms[device_id];
    }
  }

  for (uint device_id = 0; device_id < data.devices_cnt; device_id++)
  {
    char display_dev_cur[16];

    memset (display_dev_cur, 0, sizeof (display_dev_cur));

    strncpy (display_dev_cur, "0.00", 4);

    format_speed_display (hashes_dev_ms[device_id] * 1000, display_dev_cur, sizeof (display_dev_cur));

    log_info ("Speed.GPU.#%d.: %9sH/s", device_id + 1, display_dev_cur);
  }

  char display_all_cur[16];

  memset (display_all_cur, 0, sizeof (display_all_cur));

  strncpy (display_all_cur, "0.00", 4);

  format_speed_display (hashes_all_ms * 1000, display_all_cur, sizeof (display_all_cur));

  if (data.devices_cnt > 1) log_info ("Speed.GPU.#*.: %9sH/s", display_all_cur);
}

/**
 * oclHashcat -only- functions
 */

static uint convert_from_hex (char *line_buf, const uint line_len)
{
  if (line_len & 1) return (line_len); // not in hex

  if (data.hex_wordlist == 1)
  {
    uint i;
    uint j;

    for (i = 0, j = 0; j < line_len; i += 1, j += 2)
    {
      line_buf[i] = hex_to_char (&line_buf[j]);
    }

    memset (line_buf + i, 0, line_len - i);

    return (i);
  }
  else if (line_len >= 6) // $HEX[] = 6
  {
    if (line_buf[0]            != '$') return (line_len);
    if (line_buf[1]            != 'H') return (line_len);
    if (line_buf[2]            != 'E') return (line_len);
    if (line_buf[3]            != 'X') return (line_len);
    if (line_buf[4]            != '[') return (line_len);
    if (line_buf[line_len - 1] != ']') return (line_len);

    uint i;
    uint j;

    for (i = 0, j = 5; j < line_len - 1; i += 1, j += 2)
    {
      line_buf[i] = hex_to_char (&line_buf[j]);
    }

    memset (line_buf + i, 0, line_len - i);

    return (i);
  }

  return (line_len);
}

static uint count_lines (FILE *fd)
{
  uint cnt = 0;

  char *buf = (char *) mymalloc (BUFSIZ);

  size_t nread_tmp = 0;

  char *ptr = buf;

  while (!feof (fd))
  {
    size_t nread = fread (buf, sizeof (char), BUFSIZ, fd);
    nread_tmp    = nread;

    if (nread < 1) continue;

    ptr = buf;

    do
    {
      if (*ptr++ == '\n') cnt++;

    } while (nread--);
  }

  // special case (if last line did not contain a newline char ... at the very end of the file)

  if (nread_tmp > 3)
  {
    ptr -= 2;

    if (*ptr != '\n')
    {
      ptr--;

      if (*ptr != '\n') // needed ? different on windows systems?
      {
        cnt++;
      }
    }
  }

  myfree (buf);

  return cnt;
}

static void clear_prompt ()
{
  fputc ('\r', stdout);

  for (size_t i = 0; i < strlen (PROMPT); i++)
  {
    fputc (' ', stdout);
  }

  fputc ('\r', stdout);

  fflush (stdout);
}

static void gidd_to_pw_t (hc_device_param_t *device_param, const uint64_t gidd, pw_t *pw)
{
  #ifdef _CUDA
  hc_cuCtxPushCurrent (device_param->context);

  hc_cuMemcpyDtoH (pw, device_param->d_pws_buf + (gidd * sizeof (pw_t)), sizeof (pw_t));

  hc_cuCtxPopCurrent (&device_param->context);

  #elif _OCL
  hc_clEnqueueReadBuffer (device_param->command_queue, device_param->d_pws_buf, CL_TRUE, gidd * sizeof (pw_t), sizeof (pw_t), pw, 0, NULL, NULL);

  #endif
}

static void check_hash (hc_device_param_t *device_param, const uint salt_pos, const uint digest_pos)
{
  char *outfile    = data.outfile;
  uint  quiet      = data.quiet;
  FILE *pot_fp     = data.pot_fp;
  uint  loopback   = data.loopback;
  uint  debug_mode = data.debug_mode;
  char *debug_file = data.debug_file;

  char debug_rule_buf[BLOCK_SIZE];
  int  debug_rule_len  = 0; // -1 error
  uint debug_plain_len = 0;

  unsigned char debug_plain_ptr[BLOCK_SIZE];

  // hash

  char out_buf[4096]; memset (out_buf, 0, sizeof (out_buf));

  ascii_digest (out_buf, salt_pos, digest_pos);

  uint idx = data.salts_buf[salt_pos].digests_offset + digest_pos;

  // plain

  plain_t plain;

  #ifdef _CUDA
  hc_cuCtxPushCurrent (device_param->context);

  hc_cuMemcpyDtoH (&plain, device_param->d_plain_bufs + (idx * sizeof (plain_t)), sizeof (plain_t));

  hc_cuCtxPopCurrent (&device_param->context);
  #elif _OCL
  hc_clEnqueueReadBuffer (device_param->command_queue, device_param->d_plain_bufs, CL_TRUE, idx * sizeof (plain_t), sizeof (plain_t), &plain, 0, NULL, NULL);
  #endif

  uint gidvid = plain.gidvid;
  uint il_pos = plain.il_pos;

  uint64_t crackpos = device_param->words_off;

  uint plain_buf[16];

  unsigned char *plain_ptr = (unsigned char *) plain_buf;
  unsigned int   plain_len = 0;

  if (data.attack_mode == ATTACK_MODE_STRAIGHT)
  {
    uint64_t gidd = gidvid / device_param->gpu_vector_width;
    uint64_t gidm = gidvid % device_param->gpu_vector_width;

    pw_t pw;

    gidd_to_pw_t (device_param, gidd, &pw);

    for (int i = 0, j = gidm; i < 16; i++, j += device_param->gpu_vector_width)
    {
      plain_buf[i] = pw.hi1[0][j];
    }

    plain_len = pw.pw_len;

    const uint off = device_param->innerloop_pos + il_pos;

    if (debug_mode > 0)
    {
      debug_rule_len = 0;

      // save rule
      if ((debug_mode == 1) || (debug_mode == 3) || (debug_mode == 4))
      {
        memset (debug_rule_buf, 0, sizeof (debug_rule_buf));

        debug_rule_len = gpu_rule_to_cpu_rule (debug_rule_buf, &data.gpu_rules_buf[off]);
      }

      // save plain
      if ((debug_mode == 2) || (debug_mode == 3) || (debug_mode == 4))
      {
        memset (debug_plain_ptr, 0, sizeof (debug_plain_ptr));

        memcpy (debug_plain_ptr, plain_ptr, plain_len);

        debug_plain_len = plain_len;
      }
    }

    plain_len = apply_rules (data.gpu_rules_buf[off].cmds, &plain_buf[0], &plain_buf[4], plain_len);

    crackpos += gidvid;
    crackpos *= data.gpu_rules_cnt;
    crackpos += device_param->innerloop_pos + il_pos;

    if (plain_len > data.pw_max) plain_len = data.pw_max;
  }
  else if (data.attack_mode == ATTACK_MODE_COMBI)
  {
    uint64_t gidd = gidvid / device_param->gpu_vector_width;
    uint64_t gidm = gidvid % device_param->gpu_vector_width;

    pw_t pw;

    gidd_to_pw_t (device_param, gidd, &pw);

    for (int i = 0, j = gidm; i < 16; i++, j += device_param->gpu_vector_width)
    {
      plain_buf[i] = pw.hi1[0][j];
    }

    plain_len = pw.pw_len;

    char *comb_buf = (char *) device_param->combs_buf[il_pos].i;
    uint  comb_len =          device_param->combs_buf[il_pos].pw_len;

    if (data.combs_mode == COMBINATOR_MODE_BASE_LEFT)
    {
      memcpy (plain_ptr + plain_len, comb_buf, comb_len);
    }
    else
    {
      memmove (plain_ptr + comb_len, plain_ptr, plain_len);

      memcpy (plain_ptr, comb_buf, comb_len);
    }

    plain_len += comb_len;

    crackpos += gidvid;
    crackpos *= data.combs_cnt;
    crackpos += device_param->innerloop_pos + il_pos;

    if (data.pw_max != PW_DICTMAX1)
    {
      if (plain_len > data.pw_max) plain_len = data.pw_max;
    }
  }
  else if (data.attack_mode == ATTACK_MODE_BF)
  {
    uint64_t l_off = device_param->kernel_params_mp_l_buf64[3] + gidvid;
    uint64_t r_off = device_param->kernel_params_mp_r_buf64[3] + il_pos;

    uint l_start = device_param->kernel_params_mp_l_buf32[5];
    uint r_start = device_param->kernel_params_mp_r_buf32[5];

    uint l_stop = device_param->kernel_params_mp_l_buf32[4];
    uint r_stop = device_param->kernel_params_mp_r_buf32[4];

    sp_exec (l_off, (char *) plain_ptr + l_start, data.root_css_buf, data.markov_css_buf, l_start, l_start + l_stop);
    sp_exec (r_off, (char *) plain_ptr + r_start, data.root_css_buf, data.markov_css_buf, r_start, r_start + r_stop);

    plain_len = data.css_cnt;

    crackpos += gidvid;
    crackpos *= data.bfs_cnt;
    crackpos += device_param->innerloop_pos + il_pos;
  }
  else if (data.attack_mode == ATTACK_MODE_HYBRID1)
  {
    uint64_t gidd = gidvid / device_param->gpu_vector_width;
    uint64_t gidm = gidvid % device_param->gpu_vector_width;

    pw_t pw;

    gidd_to_pw_t (device_param, gidd, &pw);

    for (int i = 0, j = gidm; i < 16; i++, j += device_param->gpu_vector_width)
    {
      plain_buf[i] = pw.hi1[0][j];
    }

    plain_len = pw.pw_len;

    uint64_t off = device_param->kernel_params_mp_buf64[3] + il_pos;

    uint start = 0;
    uint stop  = device_param->kernel_params_mp_buf32[4];

    sp_exec (off, (char *) plain_ptr + plain_len, data.root_css_buf, data.markov_css_buf, start, start + stop);

    plain_len += start + stop;

    crackpos += gidvid;
    crackpos *= data.combs_cnt;
    crackpos += device_param->innerloop_pos + il_pos;

    if (data.pw_max != PW_DICTMAX1)
    {
      if (plain_len > data.pw_max) plain_len = data.pw_max;
    }
  }
  else if (data.attack_mode == ATTACK_MODE_HYBRID2)
  {
    uint64_t gidd = gidvid / device_param->gpu_vector_width;
    uint64_t gidm = gidvid % device_param->gpu_vector_width;

    pw_t pw;

    gidd_to_pw_t (device_param, gidd, &pw);

    for (int i = 0, j = gidm; i < 16; i++, j += device_param->gpu_vector_width)
    {
      plain_buf[i] = pw.hi1[0][j];
    }

    plain_len = pw.pw_len;

    uint64_t off = device_param->kernel_params_mp_buf64[3] + il_pos;

    uint start = 0;
    uint stop  = device_param->kernel_params_mp_buf32[4];

    memmove (plain_ptr + stop, plain_ptr, plain_len);

    sp_exec (off, (char *) plain_ptr, data.root_css_buf, data.markov_css_buf, start, start + stop);

    plain_len += start + stop;

    crackpos += gidvid;
    crackpos *= data.combs_cnt;
    crackpos += device_param->innerloop_pos + il_pos;

    if (data.pw_max != PW_DICTMAX1)
    {
      if (plain_len > data.pw_max) plain_len = data.pw_max;
    }
  }

  if (data.opti_type & OPTI_TYPE_BRUTE_FORCE) // lots of optimizations can happen here
  {
    if (data.opti_type & OPTI_TYPE_SINGLE_HASH)
    {
      if (data.opti_type & OPTI_TYPE_APPENDED_SALT)
      {
        plain_len = plain_len - data.salts_buf[0].salt_len;
      }
    }

    if (data.opts_type & OPTS_TYPE_PT_UNICODE)
    {
      for (uint i = 0, j = 0; i < plain_len; i += 2, j += 1)
      {
        plain_ptr[j] = plain_ptr[i];
      }

      plain_len = plain_len / 2;
    }
  }

  // if enabled, update also the potfile

  if (pot_fp)
  {
    fprintf (pot_fp, "%s:", out_buf);

    format_plain (pot_fp, plain_ptr, plain_len, 1);

    fputc ('\n', pot_fp);

    fflush (pot_fp);
  }

  // outfile

  FILE *out_fp = NULL;

  if (outfile != NULL)
  {
    if ((out_fp = fopen (outfile, "ab")) == NULL)
    {
      log_error ("ERROR: %s: %s", outfile, strerror (errno));

      out_fp = stdout;
    }
  }
  else
  {
    out_fp = stdout;

    if (quiet == 0) clear_prompt ();
  }

  format_output (out_fp, out_buf, plain_ptr, plain_len, crackpos, NULL, 0);

  if (outfile != NULL)
  {
    if (out_fp != stdout)
    {
      fclose (out_fp);
    }
  }
  else
  {
    if ((data.wordlist_mode == WL_MODE_FILE) || (data.wordlist_mode == WL_MODE_MASK))
    {
      if ((data.devices_status != STATUS_CRACKED) && (data.status != 1))
      {
        if (quiet == 0) fprintf (stdout, "%s", PROMPT);
        if (quiet == 0) fflush (stdout);
      }
    }
  }

  // loopback

  if (loopback)
  {
    char *loopback_file = data.loopback_file;

    FILE *fb_fp = NULL;

    if ((fb_fp = fopen (loopback_file, "ab")) != NULL)
    {
      format_plain (fb_fp, plain_ptr, plain_len, 1);

      fputc ('\n', fb_fp);

      fclose (fb_fp);
    }
  }

  // (rule) debug mode

  // the next check implies that:
  // - (data.attack_mode == ATTACK_MODE_STRAIGHT)
  // - debug_mode > 0

  if ((debug_plain_len > 0) || (debug_rule_len > 0))
  {
    if (debug_rule_len < 0) debug_rule_len = 0;

    if ((quiet == 0) && (debug_file == NULL)) clear_prompt ();

    format_debug (debug_file, debug_mode, debug_plain_ptr, debug_plain_len, plain_ptr, plain_len, debug_rule_buf, debug_rule_len);

    if ((quiet == 0) && (debug_file == NULL))
    {
      fprintf (stdout, "%s", PROMPT);
      fflush (stdout);
    }
  }
}

static void check_cracked (hc_device_param_t *device_param, const uint salt_pos)
{
  salt_t *salt_buf = &data.salts_buf[salt_pos];

  int found = 0;

  #ifdef _CUDA

  hc_cuCtxPushCurrent (device_param->context);

  hc_cuMemcpyDtoH (device_param->result, device_param->d_result, device_param->size_results);

  hc_cuCtxPopCurrent (&device_param->context);

  for (uint i = 0; i < GPU_THREADS_NV; i++) if (device_param->result[i] == 1) found = 1;

  #elif _OCL

  hc_clEnqueueReadBuffer (device_param->command_queue, device_param->d_result, CL_TRUE, 0, device_param->size_results, device_param->result, 0, NULL, NULL);

  for (uint i = 0; i < GPU_THREADS_AMD; i++) if (device_param->result[i] == 1) found = 1;

  #endif

  if (found == 1)
  {
    // display hack (for weak hashes etc, it could be that there is still something to clear on the current line)

    log_info_nn ("");

    #ifdef _CUDA

    hc_cuCtxPushCurrent (device_param->context);

    hc_cuMemcpyDtoH (&data.digests_shown_tmp[salt_buf->digests_offset], device_param->d_digests_shown + (salt_buf->digests_offset * sizeof (uint)), salt_buf->digests_cnt * sizeof (uint));

    hc_cuCtxPopCurrent (&device_param->context);

    #elif _OCL

    hc_clEnqueueReadBuffer (device_param->command_queue, device_param->d_digests_shown, CL_TRUE, salt_buf->digests_offset * sizeof (uint), salt_buf->digests_cnt * sizeof (uint), &data.digests_shown_tmp[salt_buf->digests_offset], 0, NULL, NULL);

    #endif

    uint cpt_cracked = 0;

    for (uint digest_pos = 0; digest_pos < salt_buf->digests_cnt; digest_pos++)
    {
      uint idx = salt_buf->digests_offset + digest_pos;

      if (data.digests_shown_tmp[idx] == 0) continue;

      if (data.digests_shown[idx] == 1) continue;

      if ((data.opts_type & OPTS_TYPE_PT_NEVERCRACK) == 0)
      {
        data.digests_shown[idx] = 1;

        data.digests_done++;

        cpt_cracked++;

        salt_buf->digests_done++;

        if (salt_buf->digests_done == salt_buf->digests_cnt)
        {
          data.salts_shown[salt_pos] = 1;

          data.salts_done++;
        }
      }

      if (data.salts_done == data.salts_cnt) data.devices_status = STATUS_CRACKED;

      check_hash (device_param, salt_pos, digest_pos);
    }

    if (cpt_cracked > 0)
    {
      data.cpt_buf[data.cpt_pos].timestamp = time (NULL);
      data.cpt_buf[data.cpt_pos].cracked   = cpt_cracked;

      data.cpt_pos++;

      data.cpt_total += cpt_cracked;

      if (data.cpt_pos == CPT_BUF) data.cpt_pos = 0;
    }

    if (data.opts_type & OPTS_TYPE_PT_NEVERCRACK)
    {
      // we need to reset cracked state on the gpu
      // otherwise host thinks again and again the hash was cracked
      // and returns invalid password each time

      memset (data.digests_shown_tmp, 0, salt_buf->digests_cnt * sizeof (uint));

      #ifdef _CUDA

      hc_cuCtxPushCurrent (device_param->context);

      hc_cuMemsetD8 (device_param->d_digests_shown + (salt_buf->digests_offset * sizeof (uint)), 0, salt_buf->digests_cnt * sizeof (uint));

      hc_cuCtxPopCurrent (&device_param->context);

      #elif _OCL

      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_digests_shown, CL_TRUE, salt_buf->digests_offset * sizeof (uint), salt_buf->digests_cnt * sizeof (uint), &data.digests_shown_tmp[salt_buf->digests_offset], 0, NULL, NULL);

      #endif
    }

    #ifdef _CUDA

    hc_cuCtxPushCurrent (device_param->context);

    hc_cuMemsetD8 (device_param->d_result, 0, device_param->size_results);

    hc_cuCtxPopCurrent (&device_param->context);

    #elif _OCL

    memset (device_param->result, 0, device_param->size_results);

    hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_result, CL_TRUE, 0, device_param->size_results, device_param->result, 0, NULL, NULL);

    #endif
  }
}

static void save_hash ()
{
  char *hashfile = data.hashfile;

  char new_hashfile[256];
  char old_hashfile[256];

  memset (new_hashfile, 0, sizeof (new_hashfile));
  memset (old_hashfile, 0, sizeof (old_hashfile));

  snprintf (new_hashfile, 255, "%s.new", hashfile);
  snprintf (old_hashfile, 255, "%s.old", hashfile);

  unlink (new_hashfile);

  char separator = data.separator;

  FILE *fp = fopen (new_hashfile, "wb");

  if (fp == NULL)
  {
    log_error ("ERROR: %s: %s", new_hashfile, strerror (errno));

    exit (-1);
  }

  for (uint salt_pos = 0; salt_pos < data.salts_cnt; salt_pos++)
  {
    if (data.salts_shown[salt_pos] == 1) continue;

    salt_t *salt_buf = &data.salts_buf[salt_pos];

    for (uint digest_pos = 0; digest_pos < salt_buf->digests_cnt; digest_pos++)
    {
      uint idx = salt_buf->digests_offset + digest_pos;

      if (data.digests_shown[idx] == 1) continue;

      if (data.hash_mode != 2500)
      {
        char out_buf[4096];

        memset (out_buf, 0, sizeof (out_buf));

        if (data.username == 1)
        {
          user_t *user = data.hash_info[idx]->user;

          uint i;

          for (i = 0; i < user->user_len; i++) fputc (user->user_name[i], fp);

          fputc (separator, fp);
        }

        ascii_digest (out_buf, salt_pos, digest_pos);

        fputs (out_buf, fp);

        log_out (fp, "");
      }
      else
      {
        hccap_t hccap;

        to_hccap_t (&hccap, salt_pos, digest_pos);

        fwrite (&hccap, sizeof (hccap_t), 1, fp);
      }
    }
  }

  fflush (fp);

  fclose (fp);

  unlink (old_hashfile);

  if (rename (hashfile, old_hashfile) != 0)
  {
    log_error ("ERROR: Rename file '%s' to '%s': %s", hashfile, old_hashfile, strerror (errno));

    exit (-1);
  }

  unlink (hashfile);

  if (rename (new_hashfile, hashfile) != 0)
  {
    log_error ("ERROR: Rename file '%s' to '%s': %s", new_hashfile, hashfile, strerror (errno));

    exit (-1);
  }

  unlink (old_hashfile);
}

static float find_gpu_blocks_div (const uint64_t total_left, const uint gpu_blocks_all)
{
  // function called only in case gpu_blocks_all > words_left)

  float gpu_blocks_div = (float) (total_left) / gpu_blocks_all;

  gpu_blocks_div += gpu_blocks_div / 100;

  uint32_t gpu_blocks_new = (uint32_t) (gpu_blocks_all * gpu_blocks_div);

  while (gpu_blocks_new < total_left)
  {
    gpu_blocks_div += gpu_blocks_div / 100;

    gpu_blocks_new = (uint32_t) (gpu_blocks_all * gpu_blocks_div);
  }

  if (data.quiet == 0)
  {
    clear_prompt ();

    log_info ("");

    log_info ("INFO: approaching final keyspace, workload adjusted");

    log_info ("");

    fprintf (stdout, "%s", PROMPT);

    fflush (stdout);
  }

  if ((gpu_blocks_all * gpu_blocks_div) < 8) return 1;

  return gpu_blocks_div;
}

static void run_kernel (const uint kern_run, hc_device_param_t *device_param, const uint num)
{
  // uint gpu_vector_width = device_param->gpu_vector_width;

  // uint num_elements = mydivc32 (num, gpu_vector_width);

  uint num_elements = num;

  device_param->kernel_params_buf32[30] = data.combs_mode;
  device_param->kernel_params_buf32[31] = num;

  uint gpu_threads = device_param->gpu_threads;

  while (num_elements % gpu_threads) num_elements++;

  #ifdef _CUDA
  CUfunction function = NULL;

  switch (kern_run)
  {
    case KERN_RUN_1:    function = device_param->function1;     break;
    case KERN_RUN_12:   function = device_param->function12;    break;
    case KERN_RUN_2:    function = device_param->function2;     break;
    case KERN_RUN_23:   function = device_param->function23;    break;
    case KERN_RUN_3:    function = device_param->function3;     break;
  }

  num_elements /= gpu_threads;

  hc_cuCtxPushCurrent (device_param->context);

  hc_cuLaunchKernel (function, num_elements, 1, 1, gpu_threads, 1, 1, 0, device_param->stream, device_param->kernel_params, NULL);

  hc_cuStreamSynchronize (device_param->stream);

  hc_cuCtxPopCurrent (&device_param->context);

  #elif _OCL

  cl_kernel kernel = NULL;

  switch (kern_run)
  {
    case KERN_RUN_1:    kernel = device_param->kernel1;     break;
    case KERN_RUN_12:   kernel = device_param->kernel12;    break;
    case KERN_RUN_2:    kernel = device_param->kernel2;     break;
    case KERN_RUN_23:   kernel = device_param->kernel23;    break;
    case KERN_RUN_3:    kernel = device_param->kernel3;     break;
  }

  hc_clSetKernelArg (kernel, 21, sizeof (cl_uint), device_param->kernel_params[21]);
  hc_clSetKernelArg (kernel, 22, sizeof (cl_uint), device_param->kernel_params[22]);
  hc_clSetKernelArg (kernel, 23, sizeof (cl_uint), device_param->kernel_params[23]);
  hc_clSetKernelArg (kernel, 24, sizeof (cl_uint), device_param->kernel_params[24]);
  hc_clSetKernelArg (kernel, 25, sizeof (cl_uint), device_param->kernel_params[25]);
  hc_clSetKernelArg (kernel, 26, sizeof (cl_uint), device_param->kernel_params[26]);
  hc_clSetKernelArg (kernel, 27, sizeof (cl_uint), device_param->kernel_params[27]);
  hc_clSetKernelArg (kernel, 28, sizeof (cl_uint), device_param->kernel_params[28]);
  hc_clSetKernelArg (kernel, 29, sizeof (cl_uint), device_param->kernel_params[29]);
  hc_clSetKernelArg (kernel, 30, sizeof (cl_uint), device_param->kernel_params[30]);
  hc_clSetKernelArg (kernel, 31, sizeof (cl_uint), device_param->kernel_params[31]);

  if ((data.opts_type & OPTS_TYPE_PT_BITSLICE) && (data.attack_mode == ATTACK_MODE_BF))
  {
    const size_t global_work_size[3] = { num_elements,      32, 1 };
    const size_t local_work_size[3]  = { gpu_threads / 32,  32, 1 };

    hc_clEnqueueNDRangeKernel (device_param->command_queue, kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
  }
  else
  {
    const size_t global_work_size[3] = { num_elements, 1, 1 };
    const size_t local_work_size[3]  = { gpu_threads,  1, 1 };

    hc_clEnqueueNDRangeKernel (device_param->command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
  }

  hc_clFlush (device_param->command_queue);

  hc_clFinish (device_param->command_queue);

  #endif
}

static void run_kernel_mp (const uint kern_run, hc_device_param_t *device_param, const uint num)
{
  // uint gpu_vector_width = device_param->gpu_vector_width;

  // uint num_elements = mydivc32 (num, gpu_vector_width);

  uint num_elements = num;

  switch (kern_run)
  {
    case KERN_RUN_MP:   device_param->kernel_params_mp_buf32[8]   = num; break;
    case KERN_RUN_MP_R: device_param->kernel_params_mp_r_buf32[8] = num; break;
    case KERN_RUN_MP_L: device_param->kernel_params_mp_l_buf32[9] = num; break;
  }

  // causes problems with special threads like in bcrypt
  // const uint gpu_threads = device_param->gpu_threads;

  #ifdef _CUDA

  const uint gpu_threads = GPU_THREADS_NV;

  while (num_elements % gpu_threads) num_elements++;

  CUfunction function = NULL;

  switch (kern_run)
  {
    case KERN_RUN_MP:    function = device_param->function_mp;    break;
    case KERN_RUN_MP_R:  function = device_param->function_mp_r;  break;
    case KERN_RUN_MP_L:  function = device_param->function_mp_l;  break;
  }

  void **kernel_params = NULL;

  switch (kern_run)
  {
    case KERN_RUN_MP:    kernel_params = device_param->kernel_params_mp;   break;
    case KERN_RUN_MP_R:  kernel_params = device_param->kernel_params_mp_r; break;
    case KERN_RUN_MP_L:  kernel_params = device_param->kernel_params_mp_l; break;
  }

  num_elements /= gpu_threads;

  hc_cuCtxPushCurrent (device_param->context);

  hc_cuLaunchKernel (function, num_elements, 1, 1, gpu_threads, 1, 1, 0, device_param->stream, kernel_params, NULL);

  hc_cuStreamSynchronize (device_param->stream);

  hc_cuCtxPopCurrent (&device_param->context);

  #elif _OCL

  const uint gpu_threads = GPU_THREADS_AMD;

  while (num_elements % gpu_threads) num_elements++;

  cl_kernel kernel = NULL;

  switch (kern_run)
  {
    case KERN_RUN_MP:   kernel = device_param->kernel_mp;   break;
    case KERN_RUN_MP_R: kernel = device_param->kernel_mp_r; break;
    case KERN_RUN_MP_L: kernel = device_param->kernel_mp_l; break;
  }

  switch (kern_run)
  {
    case KERN_RUN_MP:   hc_clSetKernelArg (kernel, 3, sizeof (cl_ulong), device_param->kernel_params_mp[3]);
                        hc_clSetKernelArg (kernel, 4, sizeof (cl_uint),  device_param->kernel_params_mp[4]);
                        hc_clSetKernelArg (kernel, 5, sizeof (cl_uint),  device_param->kernel_params_mp[5]);
                        hc_clSetKernelArg (kernel, 6, sizeof (cl_uint),  device_param->kernel_params_mp[6]);
                        hc_clSetKernelArg (kernel, 7, sizeof (cl_uint),  device_param->kernel_params_mp[7]);
                        hc_clSetKernelArg (kernel, 8, sizeof (cl_uint),  device_param->kernel_params_mp[8]);
                        break;
    case KERN_RUN_MP_R: hc_clSetKernelArg (kernel, 3, sizeof (cl_ulong), device_param->kernel_params_mp_r[3]);
                        hc_clSetKernelArg (kernel, 4, sizeof (cl_uint),  device_param->kernel_params_mp_r[4]);
                        hc_clSetKernelArg (kernel, 5, sizeof (cl_uint),  device_param->kernel_params_mp_r[5]);
                        hc_clSetKernelArg (kernel, 6, sizeof (cl_uint),  device_param->kernel_params_mp_r[6]);
                        hc_clSetKernelArg (kernel, 7, sizeof (cl_uint),  device_param->kernel_params_mp_r[7]);
                        hc_clSetKernelArg (kernel, 8, sizeof (cl_uint),  device_param->kernel_params_mp_r[8]);
                        break;
    case KERN_RUN_MP_L: hc_clSetKernelArg (kernel, 3, sizeof (cl_ulong), device_param->kernel_params_mp_l[3]);
                        hc_clSetKernelArg (kernel, 4, sizeof (cl_uint),  device_param->kernel_params_mp_l[4]);
                        hc_clSetKernelArg (kernel, 5, sizeof (cl_uint),  device_param->kernel_params_mp_l[5]);
                        hc_clSetKernelArg (kernel, 6, sizeof (cl_uint),  device_param->kernel_params_mp_l[6]);
                        hc_clSetKernelArg (kernel, 7, sizeof (cl_uint),  device_param->kernel_params_mp_l[7]);
                        hc_clSetKernelArg (kernel, 8, sizeof (cl_uint),  device_param->kernel_params_mp_l[8]);
                        hc_clSetKernelArg (kernel, 9, sizeof (cl_uint),  device_param->kernel_params_mp_l[9]);
                        break;
  }

  const size_t global_work_size[3] = { num_elements, 1, 1 };
  const size_t local_work_size[3]  = { gpu_threads,  1, 1 };

  hc_clEnqueueNDRangeKernel (device_param->command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

  hc_clFlush (device_param->command_queue);

  hc_clFinish (device_param->command_queue);

  #endif
}

static void run_kernel_tb (hc_device_param_t *device_param, const uint num)
{
  uint num_elements = num;

  uint gpu_threads = device_param->gpu_threads;

  while (num_elements % gpu_threads) num_elements++;

  #ifdef _CUDA

  CUfunction function = device_param->function_tb;

  void **kernel_params = device_param->kernel_params_tb;

  hc_cuCtxPushCurrent (device_param->context);

  hc_cuLaunchKernel (function, num_elements / gpu_threads, 1, 1, gpu_threads, 1, 1, 0, device_param->stream, kernel_params, NULL);

  hc_cuStreamSynchronize (device_param->stream);

  hc_cuCtxPopCurrent (&device_param->context);

  #elif _OCL

  cl_kernel kernel = device_param->kernel_tb;

  const size_t global_work_size[3] = { num_elements, 1, 1 };
  const size_t local_work_size[3]  = { gpu_threads,  1, 1 };

  hc_clEnqueueNDRangeKernel (device_param->command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

  hc_clFlush (device_param->command_queue);

  hc_clFinish (device_param->command_queue);

  #endif
}

static void run_kernel_tm (hc_device_param_t *device_param)
{
  const uint num_elements = 1024; // fixed

  const uint gpu_threads = 32;

  #ifdef _CUDA

  CUfunction function = device_param->function_tm;

  void **kernel_params = device_param->kernel_params_tm;

  hc_cuCtxPushCurrent (device_param->context);

  hc_cuLaunchKernel (function, num_elements / gpu_threads, 1, 1, gpu_threads, 1, 1, 0, device_param->stream, kernel_params, NULL);

  hc_cuStreamSynchronize (device_param->stream);

  hc_cuCtxPopCurrent (&device_param->context);

  #elif _OCL

  cl_kernel kernel = device_param->kernel_tm;

  const size_t global_work_size[3] = { num_elements, 1, 1 };
  const size_t local_work_size[3]  = { gpu_threads,  1, 1 };

  hc_clEnqueueNDRangeKernel (device_param->command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

  hc_clFlush (device_param->command_queue);

  hc_clFinish (device_param->command_queue);

  #endif
}

static void run_kernel_amp (hc_device_param_t *device_param, const uint num)
{
  // uint gpu_vector_width = device_param->gpu_vector_width;

  // uint num_elements = mydivc32 (num, gpu_vector_width);

  uint num_elements = num;

  device_param->kernel_params_amp_buf32[5] = data.combs_mode;
  device_param->kernel_params_amp_buf32[6] = num_elements;

  // causes problems with special threads like in bcrypt
  // const uint gpu_threads = device_param->gpu_threads;

  #ifdef _CUDA

  const uint gpu_threads = GPU_THREADS_NV;

  while (num_elements % gpu_threads) num_elements++;

  CUfunction function = device_param->function_amp;

  void **kernel_params = device_param->kernel_params_amp;

  num_elements /= gpu_threads;

  hc_cuCtxPushCurrent (device_param->context);

  hc_cuLaunchKernel (function, num_elements, 1, 1, gpu_threads, 1, 1, 0, device_param->stream, kernel_params, NULL);

  hc_cuStreamSynchronize (device_param->stream);

  hc_cuCtxPopCurrent (&device_param->context);

  #elif _OCL

  const uint gpu_threads = GPU_THREADS_AMD;

  while (num_elements % gpu_threads) num_elements++;

  cl_kernel kernel = device_param->kernel_amp;

  hc_clSetKernelArg (kernel, 5, sizeof (cl_uint), device_param->kernel_params_amp[5]);
  hc_clSetKernelArg (kernel, 6, sizeof (cl_uint), device_param->kernel_params_amp[6]);

  const size_t global_work_size[3] = { num_elements, 1, 1 };
  const size_t local_work_size[3]  = { gpu_threads,  1, 1 };

  hc_clEnqueueNDRangeKernel (device_param->command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

  hc_clFlush (device_param->command_queue);

  hc_clFinish (device_param->command_queue);

  #endif
}

#ifdef _OCL
static void run_kernel_bzero (hc_device_param_t *device_param, cl_mem buf, const uint size)
{
  const cl_uchar zero = 0;

  hc_clEnqueueFillBuffer (device_param->command_queue, buf, &zero, sizeof (cl_uchar), 0, size, 0, NULL, NULL);
}
#elif _CUDA
static void run_kernel_bzero (hc_device_param_t *device_param, CUdeviceptr buf, const uint size)
{
  hc_cuCtxPushCurrent (device_param->context);

  hc_cuMemsetD8 (buf, 0, size);

  hc_cuCtxPopCurrent (&device_param->context);
}
#endif

static int run_rule_engine (const int rule_len, const char *rule_buf)
{
  if (rule_len == 0)
  {
    return 0;
  }
  else if (rule_len == 1)
  {
    if (rule_buf[0] == RULE_OP_MANGLE_NOOP) return 0;
  }

  return 1;
}

static void run_copy (hc_device_param_t *device_param, const uint pws_cnt)
{
  #ifdef _CUDA
  hc_cuCtxPushCurrent (device_param->context);
  #endif

  // clear some leftovers from previous run (maskfiles, etc)

  #ifdef _CUDA
  if (device_param->c_bfs != 0) // should be only true in this specific case: if (data.attack_kern == ATTACK_KERN_BF)
  {
    hc_cuMemsetD8 (device_param->c_bfs, 0, device_param->c_bytes);
  }
  #endif

  if (data.attack_kern == ATTACK_KERN_STRAIGHT)
  {
    #ifdef _CUDA
    hc_cuMemcpyHtoD (device_param->d_pws_buf, device_param->pws_buf, pws_cnt * sizeof (pw_t));
    #elif _OCL
    hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_pws_buf, CL_TRUE, 0, pws_cnt * sizeof (pw_t), device_param->pws_buf, 0, NULL, NULL);
    #endif
  }
  else if (data.attack_kern == ATTACK_KERN_COMBI)
  {
    #ifdef _CUDA
    hc_cuMemcpyHtoD (device_param->d_pws_buf, device_param->pws_buf, pws_cnt * sizeof (pw_t));
    #elif _OCL
    hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_pws_buf, CL_TRUE, 0, pws_cnt * sizeof (pw_t), device_param->pws_buf, 0, NULL, NULL);
    #endif
  }
  else if (data.attack_kern == ATTACK_KERN_BF)
  {
    const uint64_t off = device_param->words_off;

    device_param->kernel_params_mp_l_buf64[3] = off;

    run_kernel_mp (KERN_RUN_MP_L, device_param, pws_cnt);
  }

  #ifdef _CUDA
  hc_cuCtxPopCurrent (&device_param->context);
  #endif
}

static void run_cracker (hc_device_param_t *device_param, const uint pw_cnt, const uint pws_cnt)
{
  const uint gpu_loops = data.gpu_loops;

  // init speed timer

  uint speed_pos = device_param->speed_pos;

  #ifdef _POSIX
  if (device_param->timer_speed.tv_sec == 0)
  {
    hc_timer_set (&device_param->timer_speed);
  }
  #endif

  #ifdef _WIN
  if (device_param->timer_speed.QuadPart == 0)
  {
    hc_timer_set (&device_param->timer_speed);
  }
  #endif

  // find higest password length, this is for optimization stuff

  uint highest_pw_len = 0;

  if (data.attack_kern == ATTACK_KERN_STRAIGHT)
  {
  }
  else if (data.attack_kern == ATTACK_KERN_COMBI)
  {
  }
  else if (data.attack_kern == ATTACK_KERN_BF)
  {
    highest_pw_len = device_param->kernel_params_mp_l_buf32[4]
                   + device_param->kernel_params_mp_l_buf32[5];
  }

  // bitslice optimization stuff

  if (data.attack_mode == ATTACK_MODE_BF)
  {
    if (data.opts_type & OPTS_TYPE_PT_BITSLICE)
    {
      run_kernel_tb (device_param, pws_cnt);
    }
  }

  // iteration type

  uint innerloop_step = 0;
  uint innerloop_cnt  = 0;

  if      (data.attack_exec == ATTACK_EXEC_ON_GPU)   innerloop_step = gpu_loops;
  else                                               innerloop_step = 1;

  if      (data.attack_kern == ATTACK_KERN_STRAIGHT) innerloop_cnt  = data.gpu_rules_cnt;
  else if (data.attack_kern == ATTACK_KERN_COMBI)    innerloop_cnt  = data.combs_cnt;
  else if (data.attack_kern == ATTACK_KERN_BF)       innerloop_cnt  = data.bfs_cnt;

  // loop start: most outer loop = salt iteration, then innerloops (if multi)

  for (uint salt_pos = 0; salt_pos < data.salts_cnt; salt_pos++)
  {
    while (data.devices_status == STATUS_PAUSED) hc_sleep (1);

    if (data.devices_status == STATUS_CRACKED) break;
    if (data.devices_status == STATUS_ABORTED) break;
    if (data.devices_status == STATUS_QUIT)    break;
    if (data.devices_status == STATUS_BYPASS)  break;

    if (data.salts_shown[salt_pos] == 1) continue;

    salt_t *salt_buf = &data.salts_buf[salt_pos];

    device_param->kernel_params_buf32[24] = salt_pos;
    device_param->kernel_params_buf32[28] = salt_buf->digests_cnt;
    device_param->kernel_params_buf32[29] = salt_buf->digests_offset;

    FILE *combs_fp = device_param->combs_fp;

    if (data.attack_mode == ATTACK_MODE_COMBI)
    {
      rewind (combs_fp);
    }

    // innerloops

    for (uint innerloop_pos = 0; innerloop_pos < innerloop_cnt; innerloop_pos += innerloop_step)
    {
      while (data.devices_status == STATUS_PAUSED) hc_sleep (1);

      if (data.devices_status == STATUS_CRACKED) break;
      if (data.devices_status == STATUS_ABORTED) break;
      if (data.devices_status == STATUS_QUIT)    break;
      if (data.devices_status == STATUS_BYPASS)  break;

      uint innerloop_left = innerloop_cnt - innerloop_pos;

      if (innerloop_left > innerloop_step) innerloop_left = innerloop_step;

      device_param->innerloop_pos  = innerloop_pos;
      device_param->innerloop_left = innerloop_left;

      device_param->kernel_params_buf32[27] = innerloop_left;

      if (innerloop_left == 0) continue;

      // initialize amplifiers

      if (data.attack_mode == ATTACK_MODE_COMBI)
      {
        char line_buf[BUFSIZ];

        uint i = 0;

        while (i < innerloop_left)
        {
          if (feof (combs_fp)) break;

          int line_len = fgetl (combs_fp, line_buf);

          if (line_len >= PW_MAX1) continue;

          line_len = convert_from_hex (line_buf, line_len);

          char *line_buf_new = line_buf;

          if (run_rule_engine (data.rule_len_r, data.rule_buf_r))
          {
            char rule_buf_out[BLOCK_SIZE];

            memset (rule_buf_out, 0, sizeof (rule_buf_out));

            int rule_len_out = _old_apply_rule (data.rule_buf_r, data.rule_len_r, line_buf, line_len, rule_buf_out);

            if (rule_len_out < 0)
            {
              data.words_progress_rejected[salt_pos] += pw_cnt;

              continue;
            }

            line_len = rule_len_out;

            line_buf_new = rule_buf_out;
          }

          line_len = MIN (line_len, PW_DICTMAX);

          char *ptr = (char *) device_param->combs_buf[i].i;

          memcpy (ptr, line_buf_new, line_len);

          memset (ptr + line_len, 0, PW_DICTMAX1 - line_len);

          if (data.opts_type & OPTS_TYPE_PT_UPPER)
          {
            uppercase (ptr, line_len);
          }

          if (data.combs_mode == COMBINATOR_MODE_BASE_LEFT)
          {
            if (data.opts_type & OPTS_TYPE_PT_ADD80)
            {
              ptr[line_len] = 0x80;
            }

            if (data.opts_type & OPTS_TYPE_PT_ADD01)
            {
              ptr[line_len] = 0x01;
            }
          }

          device_param->combs_buf[i].pw_len = line_len;

          i++;
        }

        for (uint j = i; j < innerloop_left; j++)
        {
          device_param->combs_buf[j].i[0] = 0;
          device_param->combs_buf[j].i[1] = 0;
          device_param->combs_buf[j].i[2] = 0;
          device_param->combs_buf[j].i[3] = 0;
          device_param->combs_buf[j].i[4] = 0;
          device_param->combs_buf[j].i[5] = 0;
          device_param->combs_buf[j].i[6] = 0;
          device_param->combs_buf[j].i[7] = 0;

          device_param->combs_buf[j].pw_len = 0;
        }

        innerloop_left = i;
      }
      else if (data.attack_mode == ATTACK_MODE_BF)
      {
        uint64_t off = innerloop_pos;

        device_param->kernel_params_mp_r_buf64[3] = off;

        const uint gpu_vector_width = device_param->gpu_vector_width;

        const uint innerloop_left_d = mydivc32 (innerloop_left, gpu_vector_width);

        run_kernel_mp (KERN_RUN_MP_R, device_param, innerloop_left_d);
      }
      else if (data.attack_mode == ATTACK_MODE_HYBRID1)
      {
        uint64_t off = innerloop_pos;

        device_param->kernel_params_mp_buf64[3] = off;

        const uint gpu_vector_width = device_param->gpu_vector_width;

        const uint innerloop_left_d = mydivc32 (innerloop_left, gpu_vector_width);

        run_kernel_mp (KERN_RUN_MP, device_param, innerloop_left_d);
      }
      else if (data.attack_mode == ATTACK_MODE_HYBRID2)
      {
        uint64_t off = innerloop_pos;

        device_param->kernel_params_mp_buf64[3] = off;

        const uint gpu_vector_width = device_param->gpu_vector_width;

        const uint innerloop_left_d = mydivc32 (innerloop_left, gpu_vector_width);

        run_kernel_mp (KERN_RUN_MP, device_param, innerloop_left_d);
      }

      // copy amplifiers

      #ifdef _CUDA
      hc_cuCtxPushCurrent (device_param->context);

      if (data.attack_mode == ATTACK_MODE_STRAIGHT)
      {
        hc_cuMemcpyDtoD (device_param->c_rules, device_param->d_rules + (innerloop_pos * sizeof (gpu_rule_t)), innerloop_left * sizeof (gpu_rule_t));
      }
      else if (data.attack_mode == ATTACK_MODE_COMBI)
      {
        hc_cuMemcpyHtoD (device_param->c_combs, device_param->combs_buf, innerloop_left * sizeof (comb_t));
      }
      else if (data.attack_mode == ATTACK_MODE_BF)
      {
        hc_cuMemcpyDtoD (device_param->c_bfs, device_param->d_bfs, innerloop_left * sizeof (bf_t));
      }
      else if (data.attack_mode == ATTACK_MODE_HYBRID1)
      {
        hc_cuMemcpyDtoD (device_param->c_combs, device_param->d_combs, innerloop_left * sizeof (comb_t));
      }
      else if (data.attack_mode == ATTACK_MODE_HYBRID2)
      {
        hc_cuMemcpyDtoD (device_param->c_combs, device_param->d_combs, innerloop_left * sizeof (comb_t));
      }

      hc_cuCtxPopCurrent (&device_param->context);

      #elif _OCL
      if (data.attack_mode == ATTACK_MODE_STRAIGHT)
      {
        hc_clEnqueueCopyBuffer (device_param->command_queue, device_param->d_rules, device_param->d_rules_c, innerloop_pos * sizeof (gpu_rule_t), 0, innerloop_left * sizeof (gpu_rule_t), 0, NULL, NULL);
      }
      else if (data.attack_mode == ATTACK_MODE_COMBI)
      {
        hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_combs_c, CL_TRUE, 0, innerloop_left * sizeof (comb_t), device_param->combs_buf, 0, NULL, NULL);
      }
      else if (data.attack_mode == ATTACK_MODE_BF)
      {
        hc_clEnqueueCopyBuffer (device_param->command_queue, device_param->d_bfs, device_param->d_bfs_c, 0, 0, innerloop_left * sizeof (bf_t), 0, NULL, NULL);
      }
      else if (data.attack_mode == ATTACK_MODE_HYBRID1)
      {
        hc_clEnqueueCopyBuffer (device_param->command_queue, device_param->d_combs, device_param->d_combs_c, 0, 0, innerloop_left * sizeof (comb_t), 0, NULL, NULL);
      }
      else if (data.attack_mode == ATTACK_MODE_HYBRID2)
      {
        hc_clEnqueueCopyBuffer (device_param->command_queue, device_param->d_combs, device_param->d_combs_c, 0, 0, innerloop_left * sizeof (comb_t), 0, NULL, NULL);
      }

      #endif

      if (data.attack_exec == ATTACK_EXEC_ON_GPU)
      {
        if (data.attack_mode == ATTACK_MODE_BF)
        {
          if (data.opts_type & OPTS_TYPE_PT_BITSLICE)
          {
            const uint size_tm = 32 * sizeof (bs_word_t);

            #ifdef _CUDA
            run_kernel_bzero (device_param, device_param->d_tm, size_tm);
            #elif _OCL
            run_kernel_bzero (device_param, device_param->d_tm_c, size_tm);
            #endif

            run_kernel_tm (device_param);

            #ifdef _CUDA
            hc_cuCtxPushCurrent (device_param->context);

            hc_cuMemcpyDtoD (device_param->c_tm, device_param->d_tm, size_tm);

            hc_cuCtxPopCurrent (&device_param->context);
            #elif _OCL
            hc_clEnqueueCopyBuffer (device_param->command_queue, device_param->d_tm_c, device_param->d_bfs_c, 0, 0, size_tm, 0, NULL, NULL);
            #endif
          }
        }

        if (highest_pw_len < 16)
        {
          run_kernel (KERN_RUN_1, device_param, pws_cnt);
        }
        else if (highest_pw_len < 32)
        {
          run_kernel (KERN_RUN_2, device_param, pws_cnt);
        }
        else
        {
          run_kernel (KERN_RUN_3, device_param, pws_cnt);
        }
      }
      else
      {
        run_kernel_amp (device_param, pws_cnt);

        run_kernel (KERN_RUN_1, device_param, pws_cnt);

        if (data.opts_type & OPTS_TYPE_HOOK12)
        {
          run_kernel (KERN_RUN_12, device_param, pws_cnt);
        }

        uint iter = salt_buf->salt_iter;

        for (uint loop_pos = 0; loop_pos < iter; loop_pos += gpu_loops)
        {
          uint loop_left = iter - loop_pos;

          loop_left = MIN (loop_left, gpu_loops);

          device_param->kernel_params_buf32[25] = loop_pos;
          device_param->kernel_params_buf32[26] = loop_left;

          run_kernel (KERN_RUN_2, device_param, pws_cnt);

          if (data.devices_status == STATUS_CRACKED) break;
          if (data.devices_status == STATUS_ABORTED) break;
          if (data.devices_status == STATUS_QUIT)    break;
        }

        if (data.opts_type & OPTS_TYPE_HOOK23)
        {
          run_kernel (KERN_RUN_23, device_param, pws_cnt);

          #ifdef _CUDA
          hc_cuCtxPushCurrent (device_param->context);

          hc_cuMemcpyDtoH (device_param->hooks_buf, device_param->d_hooks, device_param->size_hooks);

          hc_cuCtxPopCurrent (&device_param->context);
          #elif _OCL
          hc_clEnqueueReadBuffer (device_param->command_queue, device_param->d_hooks, CL_TRUE, 0, device_param->size_hooks, device_param->hooks_buf, 0, NULL, NULL);
          #endif

          // do something with data


          #ifdef _CUDA
          hc_cuCtxPushCurrent (device_param->context);

          hc_cuMemcpyHtoD (device_param->d_hooks, device_param->hooks_buf, device_param->size_hooks);

          hc_cuCtxPopCurrent (&device_param->context);
          #elif _OCL
          hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_hooks, CL_TRUE, 0, device_param->size_hooks, device_param->hooks_buf, 0, NULL, NULL);
          #endif
        }

        run_kernel (KERN_RUN_3, device_param, pws_cnt);
      }

      if (data.devices_status == STATUS_CRACKED) break;
      if (data.devices_status == STATUS_ABORTED) break;
      if (data.devices_status == STATUS_QUIT)    break;

      /**
       * result
       */

      hc_thread_mutex_lock (mux_display);

      check_cracked (device_param, salt_pos);

      hc_thread_mutex_unlock (mux_display);

      /**
       * progress
       */

      uint64_t perf_sum_all = (uint64_t) pw_cnt * (uint64_t) innerloop_left;

      hc_thread_mutex_lock (mux_counter);

      data.words_progress_done[salt_pos] += perf_sum_all;

      hc_thread_mutex_unlock (mux_counter);

      /**
       * speed
       */

      float speed_ms;

      hc_timer_get (device_param->timer_speed, speed_ms);

      hc_timer_set (&device_param->timer_speed);

      hc_thread_mutex_lock (mux_display);

      device_param->speed_cnt[speed_pos] = perf_sum_all;

      device_param->speed_ms[speed_pos] = speed_ms;

      device_param->speed_rec[speed_pos] = device_param->timer_speed;

      hc_thread_mutex_unlock (mux_display);

      speed_pos++;

      if (speed_pos == SPEED_CACHE)
      {
        speed_pos = 0;
      }
    }
  }

  device_param->speed_pos = speed_pos;
}

static void load_segment (wl_data_t *wl_data, FILE *fd)
{
  // NOTE: use (never changing) ->incr here instead of ->avail otherwise the buffer gets bigger and bigger

  wl_data->pos = 0;

  wl_data->cnt = fread (wl_data->buf, 1, wl_data->incr - 1000, fd);

  wl_data->buf[wl_data->cnt] = 0;

  if (wl_data->cnt == 0) return;

  if (wl_data->buf[wl_data->cnt - 1] == '\n') return;

  while (!feof (fd))
  {
    if (wl_data->cnt == wl_data->avail)
    {
      wl_data->buf = (char *) myrealloc (wl_data->buf, wl_data->avail, wl_data->incr);

      wl_data->avail += wl_data->incr;
    }

    const int c = fgetc (fd);

    if (c == EOF) break;

    wl_data->buf[wl_data->cnt] = (char) c;

    wl_data->cnt++;

    if (c == '\n') break;
  }

  // ensure stream ends with a newline

  if (wl_data->buf[wl_data->cnt - 1] != '\n')
  {
    wl_data->cnt++;

    wl_data->buf[wl_data->cnt - 1] = '\n';
  }

  return;
}

static void get_next_word_lm (char *buf, uint32_t sz, uint32_t *len, uint32_t *off)
{
  char *ptr = buf;

  for (uint32_t i = 0; i < sz; i++, ptr++)
  {
    if (*ptr >= 'a' && *ptr <= 'z') *ptr -= 0x20;

    if (i == 7)
    {
      *off = i;
      *len = i;

      return;
    }

    if (*ptr != '\n') continue;

    *off = i + 1;

    if ((i > 0) && (buf[i - 1] == '\r')) i--;

    *len = i;

    return;
  }

  *off = sz;
  *len = sz;
}

static void get_next_word_uc (char *buf, uint32_t sz, uint32_t *len, uint32_t *off)
{
  char *ptr = buf;

  for (uint32_t i = 0; i < sz; i++, ptr++)
  {
    if (*ptr >= 'a' && *ptr <= 'z') *ptr -= 0x20;

    if (*ptr != '\n') continue;

    *off = i + 1;

    if ((i > 0) && (buf[i - 1] == '\r')) i--;

    *len = i;

    return;
  }

  *off = sz;
  *len = sz;
}

static void get_next_word_std (char *buf, uint32_t sz, uint32_t *len, uint32_t *off)
{
  char *ptr = buf;

  for (uint32_t i = 0; i < sz; i++, ptr++)
  {
    if (*ptr != '\n') continue;

    *off = i + 1;

    if ((i > 0) && (buf[i - 1] == '\r')) i--;

    *len = i;

    return;
  }

  *off = sz;
  *len = sz;
}

static void get_next_word (wl_data_t *wl_data, FILE *fd, char **out_buf, uint *out_len)
{
  while (wl_data->pos < wl_data->cnt)
  {
    uint off;
    uint len;

    char *ptr = wl_data->buf + wl_data->pos;

    get_next_word_func (ptr, wl_data->cnt - wl_data->pos, &len, &off);

    wl_data->pos += off;

    if (run_rule_engine (data.rule_len_l, data.rule_buf_l))
    {
      char rule_buf_out[BLOCK_SIZE];

      memset (rule_buf_out, 0, sizeof (rule_buf_out));

      int rule_len_out = -1;

      if (len < BLOCK_SIZE)
      {
        rule_len_out = _old_apply_rule (data.rule_buf_l, data.rule_len_l, ptr, len, rule_buf_out);
      }

      if (rule_len_out < 0)
      {
        continue;
      }

      if (rule_len_out > PW_MAX)
      {
        continue;
      }
    }
    else
    {
      if (len > PW_MAX)
      {
        continue;
      }
    }

    *out_buf = ptr;
    *out_len = len;

    return;
  }

  if (feof (fd))
  {
    fprintf (stderr, "bug!!\n");

    return;
  }

  load_segment (wl_data, fd);

  get_next_word (wl_data, fd, out_buf, out_len);
}

#ifdef _POSIX
static uint64_t count_words (wl_data_t *wl_data, FILE *fd, char *dictfile, dictstat_t *dictstat_base, size_t *dictstat_nmemb)
#endif

#ifdef _WIN
static uint64_t count_words (wl_data_t *wl_data, FILE *fd, char *dictfile, dictstat_t *dictstat_base, uint *dictstat_nmemb)
#endif
{
  hc_signal (NULL);

  dictstat_t d;

  d.cnt = 0;

  #ifdef _POSIX
  fstat (fileno (fd), &d.stat);
  #endif

  #ifdef _WIN
  _fstat64 (fileno (fd), &d.stat);
  #endif

  d.stat.st_mode    = 0;
  d.stat.st_nlink   = 0;
  d.stat.st_uid     = 0;
  d.stat.st_gid     = 0;
  d.stat.st_rdev    = 0;
  d.stat.st_atime   = 0;

  #ifdef _POSIX
  d.stat.st_blksize = 0;
  d.stat.st_blocks  = 0;
  #endif

  if (d.stat.st_size == 0) return 0;

  dictstat_t *d_cache = (dictstat_t *) lfind (&d, dictstat_base, dictstat_nmemb, sizeof (dictstat_t), sort_by_dictstat);

  if (run_rule_engine (data.rule_len_l, data.rule_buf_l) == 0)
  {
    if (d_cache)
    {
      uint64_t cnt = d_cache->cnt;

      uint64_t keyspace = cnt;

      if (data.attack_kern == ATTACK_KERN_STRAIGHT)
      {
        keyspace *= data.gpu_rules_cnt;
      }
      else if (data.attack_kern == ATTACK_KERN_COMBI)
      {
        keyspace *= data.combs_cnt;
      }

      if (data.quiet == 0) log_info ("Cache-hit dictionary stats %s: %llu bytes, %llu words, %llu keyspace", dictfile, (unsigned long long int) d.stat.st_size, (unsigned long long int) cnt, (unsigned long long int) keyspace);
      if (data.quiet == 0) log_info ("");

      hc_signal (sigHandler_default);

      return (keyspace);
    }
  }

  time_t now  = 0;
  time_t prev = 0;

  uint64_t comp = 0;
  uint64_t cnt  = 0;
  uint64_t cnt2 = 0;

  while (!feof (fd))
  {
    load_segment (wl_data, fd);

    comp += wl_data->cnt;

    uint32_t i = 0;

    while (i < wl_data->cnt)
    {
      uint32_t len;
      uint32_t off;

      get_next_word_func (wl_data->buf + i, wl_data->cnt - i, &len, &off);

      if (run_rule_engine (data.rule_len_l, data.rule_buf_l))
      {
        char rule_buf_out[BLOCK_SIZE];

        memset (rule_buf_out, 0, sizeof (rule_buf_out));

        int rule_len_out = -1;

        if (len < BLOCK_SIZE)
        {
          rule_len_out = _old_apply_rule (data.rule_buf_l, data.rule_len_l, wl_data->buf + i, len, rule_buf_out);
        }

        if (rule_len_out < 0)
        {
          len = PW_MAX1;
        }
        else
        {
          len = rule_len_out;
        }
      }

      if (len < PW_MAX1)
      {
        if (data.attack_kern == ATTACK_KERN_STRAIGHT)
        {
          cnt += data.gpu_rules_cnt;
        }
        else if (data.attack_kern == ATTACK_KERN_COMBI)
        {
          cnt += data.combs_cnt;
        }

        d.cnt++;
      }

      i += off;

      cnt2++;
    }

    time (&now);

    if ((now - prev) == 0) continue;

    float percent = (float) comp / (float) d.stat.st_size;

    if (data.quiet == 0) log_info_nn ("Generating dictionary stats for %s: %llu bytes (%.2f%%), %llu words, %llu keyspace", dictfile, (unsigned long long int) comp, percent * 100, (unsigned long long int) cnt2, (unsigned long long int) cnt);

    time (&prev);
  }

  if (data.quiet == 0) log_info ("Generated dictionary stats for %s: %llu bytes, %llu words, %llu keyspace", dictfile, (unsigned long long int) comp, (unsigned long long int) cnt2, (unsigned long long int) cnt);
  if (data.quiet == 0) log_info ("");

  lsearch (&d, dictstat_base, dictstat_nmemb, sizeof (dictstat_t), sort_by_dictstat);

  hc_signal (sigHandler_default);

  return (cnt);
}

static uint get_gpu_vector_width (const uint hash_mode, const uint attack_mode, const uint attack_exec, const uint opti_type, const uint vliw)
{
  uint gpu_vector_width = 0;

  if ((attack_mode == ATTACK_MODE_BF) && (attack_exec == ATTACK_EXEC_ON_GPU) && (opti_type & OPTI_TYPE_SCALAR_MODE))
  {
    return VECT_SIZE_1;
  }

  #ifdef _CUDA
  if ((attack_mode == ATTACK_MODE_STRAIGHT) && (attack_exec == ATTACK_EXEC_ON_GPU))
  {
    return VECT_SIZE_1;
  }

  if (vliw == 1)
  {
    switch (hash_mode)
    {
      default:    gpu_vector_width = VECT_SIZE_1; break;
    }
  }
  else if (vliw == 2)
  {
    switch (hash_mode)
    {
      case     0: gpu_vector_width = VECT_SIZE_4; break;
      case    10: gpu_vector_width = VECT_SIZE_4; break;
      case    11: gpu_vector_width = VECT_SIZE_4; break;
      case    12: gpu_vector_width = VECT_SIZE_4; break;
      case    20: gpu_vector_width = VECT_SIZE_4; break;
      case    21: gpu_vector_width = VECT_SIZE_4; break;
      case    22: gpu_vector_width = VECT_SIZE_4; break;
      case    23: gpu_vector_width = VECT_SIZE_4; break;
      case    30: gpu_vector_width = VECT_SIZE_4; break;
      case    40: gpu_vector_width = VECT_SIZE_4; break;
      case    50: gpu_vector_width = VECT_SIZE_4; break;
      case    60: gpu_vector_width = VECT_SIZE_4; break;
      case   100: gpu_vector_width = VECT_SIZE_4; break;
      case   101: gpu_vector_width = VECT_SIZE_4; break;
      case   110: gpu_vector_width = VECT_SIZE_4; break;
      case   111: gpu_vector_width = VECT_SIZE_4; break;
      case   112: gpu_vector_width = VECT_SIZE_4; break;
      case   120: gpu_vector_width = VECT_SIZE_4; break;
      case   121: gpu_vector_width = VECT_SIZE_4; break;
      case   122: gpu_vector_width = VECT_SIZE_4; break;
      case   124: gpu_vector_width = VECT_SIZE_4; break;
      case   130: gpu_vector_width = VECT_SIZE_4; break;
      case   131: gpu_vector_width = VECT_SIZE_4; break;
      case   132: gpu_vector_width = VECT_SIZE_4; break;
      case   133: gpu_vector_width = VECT_SIZE_4; break;
      case   140: gpu_vector_width = VECT_SIZE_4; break;
      case   141: gpu_vector_width = VECT_SIZE_4; break;
      case   150: gpu_vector_width = VECT_SIZE_4; break;
      case   160: gpu_vector_width = VECT_SIZE_4; break;
      case   190: gpu_vector_width = VECT_SIZE_4; break;
      case   200: gpu_vector_width = VECT_SIZE_4; break;
      case   400: gpu_vector_width = VECT_SIZE_2; break;
      case   500: gpu_vector_width = VECT_SIZE_2; break;
      case   501: gpu_vector_width = VECT_SIZE_2; break;
      case   900: gpu_vector_width = VECT_SIZE_4; break;
      case  1000: gpu_vector_width = VECT_SIZE_4; break;
      case  1100: gpu_vector_width = VECT_SIZE_4; break;
      case  2400: gpu_vector_width = VECT_SIZE_4; break;
      case  2410: gpu_vector_width = VECT_SIZE_4; break;
      case  2600: gpu_vector_width = VECT_SIZE_4; break;
      case  2611: gpu_vector_width = VECT_SIZE_4; break;
      case  2612: gpu_vector_width = VECT_SIZE_4; break;
      case  2711: gpu_vector_width = VECT_SIZE_4; break;
      case  2811: gpu_vector_width = VECT_SIZE_4; break;
      case  3710: gpu_vector_width = VECT_SIZE_4; break;
      case  3800: gpu_vector_width = VECT_SIZE_4; break;
      case  3711: gpu_vector_width = VECT_SIZE_4; break;
      case  4300: gpu_vector_width = VECT_SIZE_4; break;
      case  4800: gpu_vector_width = VECT_SIZE_4; break;
      case  4900: gpu_vector_width = VECT_SIZE_4; break;
      case  5100: gpu_vector_width = VECT_SIZE_4; break;
      case  9900: gpu_vector_width = VECT_SIZE_4; break;
      case 10200: gpu_vector_width = VECT_SIZE_4; break;
      case 11000: gpu_vector_width = VECT_SIZE_4; break;
      case 11500: gpu_vector_width = VECT_SIZE_4; break;

      default:    gpu_vector_width = VECT_SIZE_1; break;
    }
  }
  #endif

  #ifdef _OCL
  if (vliw == 1)
  {
    switch (hash_mode)
    {
      default:    gpu_vector_width = VECT_SIZE_1; break;
    }
  }
  else if (vliw == 4)
  {
    switch (hash_mode)
    {
      case   150: gpu_vector_width = VECT_SIZE_2; break;
      case   160: gpu_vector_width = VECT_SIZE_2; break;
      case   300: gpu_vector_width = VECT_SIZE_2; break;
      case  1400: gpu_vector_width = VECT_SIZE_2; break;
      case  1410: gpu_vector_width = VECT_SIZE_2; break;
      case  1420: gpu_vector_width = VECT_SIZE_2; break;
      case  1421: gpu_vector_width = VECT_SIZE_2; break;
      case  1430: gpu_vector_width = VECT_SIZE_2; break;
      case  1440: gpu_vector_width = VECT_SIZE_2; break;
      case  1441: gpu_vector_width = VECT_SIZE_2; break;
      case  1450: gpu_vector_width = VECT_SIZE_1; break;
      case  1460: gpu_vector_width = VECT_SIZE_2; break;
      case  1500: gpu_vector_width = VECT_SIZE_1; break;
      case  1700: gpu_vector_width = VECT_SIZE_1; break;
      case  1710: gpu_vector_width = VECT_SIZE_1; break;
      case  1711: gpu_vector_width = VECT_SIZE_1; break;
      case  1720: gpu_vector_width = VECT_SIZE_1; break;
      case  1722: gpu_vector_width = VECT_SIZE_1; break;
      case  1730: gpu_vector_width = VECT_SIZE_1; break;
      case  1731: gpu_vector_width = VECT_SIZE_1; break;
      case  1740: gpu_vector_width = VECT_SIZE_1; break;
      case  1750: gpu_vector_width = VECT_SIZE_1; break;
      case  1760: gpu_vector_width = VECT_SIZE_1; break;
      case  1800: gpu_vector_width = VECT_SIZE_1; break;
      case  2100: gpu_vector_width = VECT_SIZE_2; break;
      case  2500: gpu_vector_width = VECT_SIZE_2; break;
      case  3000: gpu_vector_width = VECT_SIZE_1; break;
      case  3100: gpu_vector_width = VECT_SIZE_2; break;
      case  3200: gpu_vector_width = VECT_SIZE_1; break;
      case  5000: gpu_vector_width = VECT_SIZE_1; break;
      case  5200: gpu_vector_width = VECT_SIZE_2; break;
      case  5600: gpu_vector_width = VECT_SIZE_2; break;
      case  5700: gpu_vector_width = VECT_SIZE_2; break;
      case  6100: gpu_vector_width = VECT_SIZE_2; break;
      case  6211:
      case  6212:
      case  6213:
      case  6221:
      case  6222:
      case  6223:
      case  6231:
      case  6232:
      case  6233:
      case  6241:
      case  6242:
      case  6243: gpu_vector_width = VECT_SIZE_1; break;
      case  6400: gpu_vector_width = VECT_SIZE_1; break;
      case  6500: gpu_vector_width = VECT_SIZE_1; break;
      case  6600: gpu_vector_width = VECT_SIZE_1; break;
      case  6700: gpu_vector_width = VECT_SIZE_2; break;
      case  6800: gpu_vector_width = VECT_SIZE_1; break;
      case  6900: gpu_vector_width = VECT_SIZE_1; break;
      case  7100: gpu_vector_width = VECT_SIZE_1; break;
      case  7200: gpu_vector_width = VECT_SIZE_1; break;
      case  7300: gpu_vector_width = VECT_SIZE_1; break;
      case  7400: gpu_vector_width = VECT_SIZE_1; break;
      case  7500: gpu_vector_width = VECT_SIZE_1; break;
      case  7700: gpu_vector_width = VECT_SIZE_1; break;
      case  7800: gpu_vector_width = VECT_SIZE_1; break;
      case  7900: gpu_vector_width = VECT_SIZE_1; break;
      case  8000: gpu_vector_width = VECT_SIZE_2; break;
      case  8200: gpu_vector_width = VECT_SIZE_1; break;
      case  8500: gpu_vector_width = VECT_SIZE_2; break;
      case  8700: gpu_vector_width = VECT_SIZE_2; break;
      case  8800: gpu_vector_width = VECT_SIZE_1; break;
      case  8900: gpu_vector_width = VECT_SIZE_1; break;
      case  9000: gpu_vector_width = VECT_SIZE_1; break;
      case  9100: gpu_vector_width = VECT_SIZE_1; break;
      case  9200: gpu_vector_width = VECT_SIZE_1; break;
      case  9300: gpu_vector_width = VECT_SIZE_1; break;
      case  9400: gpu_vector_width = VECT_SIZE_1; break;
      case  9500: gpu_vector_width = VECT_SIZE_1; break;
      case  9600: gpu_vector_width = VECT_SIZE_1; break;
      case  9700: gpu_vector_width = VECT_SIZE_1; break;
      case  9710: gpu_vector_width = VECT_SIZE_1; break;
      case  9720: gpu_vector_width = VECT_SIZE_2; break;
      case  9800: gpu_vector_width = VECT_SIZE_1; break;
      case  9810: gpu_vector_width = VECT_SIZE_1; break;
      case  9820: gpu_vector_width = VECT_SIZE_2; break;
      case 10000: gpu_vector_width = VECT_SIZE_1; break;
      case 10100: gpu_vector_width = VECT_SIZE_1; break;
      case 10400: gpu_vector_width = VECT_SIZE_1; break;
      case 10410: gpu_vector_width = VECT_SIZE_1; break;
      case 10420: gpu_vector_width = VECT_SIZE_2; break;
      case 10500: gpu_vector_width = VECT_SIZE_1; break;
      case 10600: gpu_vector_width = VECT_SIZE_2; break;
      case 10700: gpu_vector_width = VECT_SIZE_1; break;
      case 10800: gpu_vector_width = VECT_SIZE_1; break;
      case 10900: gpu_vector_width = VECT_SIZE_1; break;
      case 11100: gpu_vector_width = VECT_SIZE_2; break;
      case 11200: gpu_vector_width = VECT_SIZE_2; break;
      case 11300: gpu_vector_width = VECT_SIZE_1; break;
      case 11400: gpu_vector_width = VECT_SIZE_1; break;
      case 11600: gpu_vector_width = VECT_SIZE_1; break;
      case 11700: gpu_vector_width = VECT_SIZE_1; break;
      case 11800: gpu_vector_width = VECT_SIZE_1; break;
      case 11900: gpu_vector_width = VECT_SIZE_1; break;
      case 12000: gpu_vector_width = VECT_SIZE_1; break;
      case 12100: gpu_vector_width = VECT_SIZE_1; break;
      case 12200: gpu_vector_width = VECT_SIZE_1; break;
      case 12300: gpu_vector_width = VECT_SIZE_1; break;
      case 12500: gpu_vector_width = VECT_SIZE_1; break;
      case 12700: gpu_vector_width = VECT_SIZE_1; break;
      case 12800: gpu_vector_width = VECT_SIZE_1; break;

      default:    gpu_vector_width = VECT_SIZE_4; break;
    }
  }
  else if (vliw == 5)
  {
    switch (hash_mode)
    {
      case   150: gpu_vector_width = VECT_SIZE_2; break;
      case   160: gpu_vector_width = VECT_SIZE_2; break;
      case   300: gpu_vector_width = VECT_SIZE_2; break;
      case  1400: gpu_vector_width = VECT_SIZE_2; break;
      case  1410: gpu_vector_width = VECT_SIZE_2; break;
      case  1420: gpu_vector_width = VECT_SIZE_2; break;
      case  1421: gpu_vector_width = VECT_SIZE_2; break;
      case  1430: gpu_vector_width = VECT_SIZE_2; break;
      case  1440: gpu_vector_width = VECT_SIZE_2; break;
      case  1441: gpu_vector_width = VECT_SIZE_2; break;
      case  1450: gpu_vector_width = VECT_SIZE_1; break;
      case  1460: gpu_vector_width = VECT_SIZE_2; break;
      case  1500: gpu_vector_width = VECT_SIZE_1; break;
      case  1700: gpu_vector_width = VECT_SIZE_1; break;
      case  1710: gpu_vector_width = VECT_SIZE_1; break;
      case  1711: gpu_vector_width = VECT_SIZE_1; break;
      case  1720: gpu_vector_width = VECT_SIZE_1; break;
      case  1722: gpu_vector_width = VECT_SIZE_1; break;
      case  1730: gpu_vector_width = VECT_SIZE_1; break;
      case  1731: gpu_vector_width = VECT_SIZE_1; break;
      case  1740: gpu_vector_width = VECT_SIZE_1; break;
      case  1750: gpu_vector_width = VECT_SIZE_1; break;
      case  1760: gpu_vector_width = VECT_SIZE_1; break;
      case  1800: gpu_vector_width = VECT_SIZE_1; break;
      case  2100: gpu_vector_width = VECT_SIZE_2; break;
      case  2500: gpu_vector_width = VECT_SIZE_2; break;
      case  3000: gpu_vector_width = VECT_SIZE_1; break;
      case  3100: gpu_vector_width = VECT_SIZE_2; break;
      case  3200: gpu_vector_width = VECT_SIZE_1; break;
      case  5000: gpu_vector_width = VECT_SIZE_1; break;
      case  5200: gpu_vector_width = VECT_SIZE_2; break;
      case  5400: gpu_vector_width = VECT_SIZE_2; break;
      case  5600: gpu_vector_width = VECT_SIZE_2; break;
      case  5700: gpu_vector_width = VECT_SIZE_2; break;
      case  6100: gpu_vector_width = VECT_SIZE_2; break;
      case  6211:
      case  6212:
      case  6213:
      case  6221:
      case  6222:
      case  6223:
      case  6231:
      case  6232:
      case  6233:
      case  6241:
      case  6242:
      case  6243: gpu_vector_width = VECT_SIZE_1; break;
      case  6400: gpu_vector_width = VECT_SIZE_1; break;
      case  6500: gpu_vector_width = VECT_SIZE_1; break;
      case  6600: gpu_vector_width = VECT_SIZE_1; break;
      case  6700: gpu_vector_width = VECT_SIZE_2; break;
      case  6800: gpu_vector_width = VECT_SIZE_1; break;
      case  6900: gpu_vector_width = VECT_SIZE_1; break;
      case  7100: gpu_vector_width = VECT_SIZE_1; break;
      case  7200: gpu_vector_width = VECT_SIZE_1; break;
      case  7300: gpu_vector_width = VECT_SIZE_1; break;
      case  7400: gpu_vector_width = VECT_SIZE_1; break;
      case  7500: gpu_vector_width = VECT_SIZE_1; break;
      case  7700: gpu_vector_width = VECT_SIZE_1; break;
      case  7800: gpu_vector_width = VECT_SIZE_1; break;
      case  7900: gpu_vector_width = VECT_SIZE_1; break;
      case  8000: gpu_vector_width = VECT_SIZE_2; break;
      case  8200: gpu_vector_width = VECT_SIZE_1; break;
      case  8300: gpu_vector_width = VECT_SIZE_2; break;
      case  8400: gpu_vector_width = VECT_SIZE_2; break;
      case  8500: gpu_vector_width = VECT_SIZE_2; break;
      case  8700: gpu_vector_width = VECT_SIZE_2; break;
      case  8800: gpu_vector_width = VECT_SIZE_1; break;
      case  8900: gpu_vector_width = VECT_SIZE_1; break;
      case  9000: gpu_vector_width = VECT_SIZE_1; break;
      case  9100: gpu_vector_width = VECT_SIZE_1; break;
      case  9200: gpu_vector_width = VECT_SIZE_1; break;
      case  9300: gpu_vector_width = VECT_SIZE_1; break;
      case  9400: gpu_vector_width = VECT_SIZE_1; break;
      case  9500: gpu_vector_width = VECT_SIZE_1; break;
      case  9600: gpu_vector_width = VECT_SIZE_1; break;
      case  9700: gpu_vector_width = VECT_SIZE_1; break;
      case  9710: gpu_vector_width = VECT_SIZE_1; break;
      case  9720: gpu_vector_width = VECT_SIZE_2; break;
      case  9800: gpu_vector_width = VECT_SIZE_1; break;
      case  9810: gpu_vector_width = VECT_SIZE_1; break;
      case  9820: gpu_vector_width = VECT_SIZE_2; break;
      case 10000: gpu_vector_width = VECT_SIZE_1; break;
      case 10100: gpu_vector_width = VECT_SIZE_1; break;
      case 10400: gpu_vector_width = VECT_SIZE_1; break;
      case 10410: gpu_vector_width = VECT_SIZE_1; break;
      case 10420: gpu_vector_width = VECT_SIZE_2; break;
      case 10500: gpu_vector_width = VECT_SIZE_1; break;
      case 10600: gpu_vector_width = VECT_SIZE_2; break;
      case 10700: gpu_vector_width = VECT_SIZE_1; break;
      case 10800: gpu_vector_width = VECT_SIZE_1; break;
      case 10900: gpu_vector_width = VECT_SIZE_1; break;
      case 11100: gpu_vector_width = VECT_SIZE_2; break;
      case 11200: gpu_vector_width = VECT_SIZE_2; break;
      case 11300: gpu_vector_width = VECT_SIZE_1; break;
      case 11400: gpu_vector_width = VECT_SIZE_1; break;
      case 11600: gpu_vector_width = VECT_SIZE_1; break;
      case 11700: gpu_vector_width = VECT_SIZE_1; break;
      case 11800: gpu_vector_width = VECT_SIZE_1; break;
      case 11900: gpu_vector_width = VECT_SIZE_1; break;
      case 12000: gpu_vector_width = VECT_SIZE_1; break;
      case 12100: gpu_vector_width = VECT_SIZE_1; break;
      case 12200: gpu_vector_width = VECT_SIZE_1; break;
      case 12300: gpu_vector_width = VECT_SIZE_1; break;
      case 12500: gpu_vector_width = VECT_SIZE_1; break;
      case 12700: gpu_vector_width = VECT_SIZE_1; break;
      case 12800: gpu_vector_width = VECT_SIZE_1; break;

      default:    gpu_vector_width = VECT_SIZE_4; break;
    }
  }
  #endif

  return gpu_vector_width;
}

static void pw_transpose_to_hi1 (const pw_t *p1, pw_t *p2)
{
  memcpy (p2->hi1, p1->hi1, 64 * sizeof (uint));
}

static void pw_transpose_to_hi2 (const pw_t *p1, pw_t *p2)
{
  p2->hi2[0][ 0] = p1->hi2[0][ 0];
  p2->hi2[0][ 2] = p1->hi2[0][ 1];
  p2->hi2[0][ 4] = p1->hi2[0][ 2];
  p2->hi2[0][ 6] = p1->hi2[0][ 3];
  p2->hi2[0][ 8] = p1->hi2[0][ 4];
  p2->hi2[0][10] = p1->hi2[0][ 5];
  p2->hi2[0][12] = p1->hi2[0][ 6];
  p2->hi2[0][14] = p1->hi2[0][ 7];
  p2->hi2[0][16] = p1->hi2[0][ 8];
  p2->hi2[0][18] = p1->hi2[0][ 9];
  p2->hi2[0][20] = p1->hi2[0][10];
  p2->hi2[0][22] = p1->hi2[0][11];
  p2->hi2[0][24] = p1->hi2[0][12];
  p2->hi2[0][26] = p1->hi2[0][13];
  p2->hi2[0][28] = p1->hi2[0][14];
  p2->hi2[0][30] = p1->hi2[0][15];
  p2->hi2[1][ 0] = p1->hi2[0][16];
  p2->hi2[1][ 2] = p1->hi2[0][17];
  p2->hi2[1][ 4] = p1->hi2[0][18];
  p2->hi2[1][ 6] = p1->hi2[0][19];
  p2->hi2[1][ 8] = p1->hi2[0][20];
  p2->hi2[1][10] = p1->hi2[0][21];
  p2->hi2[1][12] = p1->hi2[0][22];
  p2->hi2[1][14] = p1->hi2[0][23];
  p2->hi2[1][16] = p1->hi2[0][24];
  p2->hi2[1][18] = p1->hi2[0][25];
  p2->hi2[1][20] = p1->hi2[0][26];
  p2->hi2[1][22] = p1->hi2[0][27];
  p2->hi2[1][24] = p1->hi2[0][28];
  p2->hi2[1][26] = p1->hi2[0][29];
  p2->hi2[1][28] = p1->hi2[0][30];
  p2->hi2[1][30] = p1->hi2[0][31];

  p2->hi2[0][ 1] = p1->hi2[1][ 0];
  p2->hi2[0][ 3] = p1->hi2[1][ 1];
  p2->hi2[0][ 5] = p1->hi2[1][ 2];
  p2->hi2[0][ 7] = p1->hi2[1][ 3];
  p2->hi2[0][ 9] = p1->hi2[1][ 4];
  p2->hi2[0][11] = p1->hi2[1][ 5];
  p2->hi2[0][13] = p1->hi2[1][ 6];
  p2->hi2[0][15] = p1->hi2[1][ 7];
  p2->hi2[0][17] = p1->hi2[1][ 8];
  p2->hi2[0][19] = p1->hi2[1][ 9];
  p2->hi2[0][21] = p1->hi2[1][10];
  p2->hi2[0][23] = p1->hi2[1][11];
  p2->hi2[0][25] = p1->hi2[1][12];
  p2->hi2[0][27] = p1->hi2[1][13];
  p2->hi2[0][29] = p1->hi2[1][14];
  p2->hi2[0][31] = p1->hi2[1][15];
  p2->hi2[1][ 1] = p1->hi2[1][16];
  p2->hi2[1][ 3] = p1->hi2[1][17];
  p2->hi2[1][ 5] = p1->hi2[1][18];
  p2->hi2[1][ 7] = p1->hi2[1][19];
  p2->hi2[1][ 9] = p1->hi2[1][20];
  p2->hi2[1][11] = p1->hi2[1][21];
  p2->hi2[1][13] = p1->hi2[1][22];
  p2->hi2[1][15] = p1->hi2[1][23];
  p2->hi2[1][17] = p1->hi2[1][24];
  p2->hi2[1][19] = p1->hi2[1][25];
  p2->hi2[1][21] = p1->hi2[1][26];
  p2->hi2[1][23] = p1->hi2[1][27];
  p2->hi2[1][25] = p1->hi2[1][28];
  p2->hi2[1][27] = p1->hi2[1][29];
  p2->hi2[1][29] = p1->hi2[1][30];
  p2->hi2[1][31] = p1->hi2[1][31];
}

static void pw_transpose_to_hi4 (const pw_t *p1, pw_t *p2)
{
  p2->hi4[0][ 0] = p1->hi4[0][ 0];
  p2->hi4[0][ 4] = p1->hi4[0][ 1];
  p2->hi4[0][ 8] = p1->hi4[0][ 2];
  p2->hi4[0][12] = p1->hi4[0][ 3];
  p2->hi4[1][ 0] = p1->hi4[0][ 4];
  p2->hi4[1][ 4] = p1->hi4[0][ 5];
  p2->hi4[1][ 8] = p1->hi4[0][ 6];
  p2->hi4[1][12] = p1->hi4[0][ 7];
  p2->hi4[2][ 0] = p1->hi4[0][ 8];
  p2->hi4[2][ 4] = p1->hi4[0][ 9];
  p2->hi4[2][ 8] = p1->hi4[0][10];
  p2->hi4[2][12] = p1->hi4[0][11];
  p2->hi4[3][ 0] = p1->hi4[0][12];
  p2->hi4[3][ 4] = p1->hi4[0][13];
  p2->hi4[3][ 8] = p1->hi4[0][14];
  p2->hi4[3][12] = p1->hi4[0][15];

  p2->hi4[0][ 1] = p1->hi4[1][ 0];
  p2->hi4[0][ 5] = p1->hi4[1][ 1];
  p2->hi4[0][ 9] = p1->hi4[1][ 2];
  p2->hi4[0][13] = p1->hi4[1][ 3];
  p2->hi4[1][ 1] = p1->hi4[1][ 4];
  p2->hi4[1][ 5] = p1->hi4[1][ 5];
  p2->hi4[1][ 9] = p1->hi4[1][ 6];
  p2->hi4[1][13] = p1->hi4[1][ 7];
  p2->hi4[2][ 1] = p1->hi4[1][ 8];
  p2->hi4[2][ 5] = p1->hi4[1][ 9];
  p2->hi4[2][ 9] = p1->hi4[1][10];
  p2->hi4[2][13] = p1->hi4[1][11];
  p2->hi4[3][ 1] = p1->hi4[1][12];
  p2->hi4[3][ 5] = p1->hi4[1][13];
  p2->hi4[3][ 9] = p1->hi4[1][14];
  p2->hi4[3][13] = p1->hi4[1][15];

  p2->hi4[0][ 2] = p1->hi4[2][ 0];
  p2->hi4[0][ 6] = p1->hi4[2][ 1];
  p2->hi4[0][10] = p1->hi4[2][ 2];
  p2->hi4[0][14] = p1->hi4[2][ 3];
  p2->hi4[1][ 2] = p1->hi4[2][ 4];
  p2->hi4[1][ 6] = p1->hi4[2][ 5];
  p2->hi4[1][10] = p1->hi4[2][ 6];
  p2->hi4[1][14] = p1->hi4[2][ 7];
  p2->hi4[2][ 2] = p1->hi4[2][ 8];
  p2->hi4[2][ 6] = p1->hi4[2][ 9];
  p2->hi4[2][10] = p1->hi4[2][10];
  p2->hi4[2][14] = p1->hi4[2][11];
  p2->hi4[3][ 2] = p1->hi4[2][12];
  p2->hi4[3][ 6] = p1->hi4[2][13];
  p2->hi4[3][10] = p1->hi4[2][14];
  p2->hi4[3][14] = p1->hi4[2][15];

  p2->hi4[0][ 3] = p1->hi4[3][ 0];
  p2->hi4[0][ 7] = p1->hi4[3][ 1];
  p2->hi4[0][11] = p1->hi4[3][ 2];
  p2->hi4[0][15] = p1->hi4[3][ 3];
  p2->hi4[1][ 3] = p1->hi4[3][ 4];
  p2->hi4[1][ 7] = p1->hi4[3][ 5];
  p2->hi4[1][11] = p1->hi4[3][ 6];
  p2->hi4[1][15] = p1->hi4[3][ 7];
  p2->hi4[2][ 3] = p1->hi4[3][ 8];
  p2->hi4[2][ 7] = p1->hi4[3][ 9];
  p2->hi4[2][11] = p1->hi4[3][10];
  p2->hi4[2][15] = p1->hi4[3][11];
  p2->hi4[3][ 3] = p1->hi4[3][12];
  p2->hi4[3][ 7] = p1->hi4[3][13];
  p2->hi4[3][11] = p1->hi4[3][14];
  p2->hi4[3][15] = p1->hi4[3][15];
}

static uint pw_add_to_hc1 (hc_device_param_t *device_param, const uint8_t *pw_buf, const uint pw_len)
{
  if (data.devices_status == STATUS_BYPASS) return 0;

  pw_cache_t *pw_cache = device_param->pw_caches + pw_len;

  uint cache_cnt = pw_cache->cnt;

  uint8_t *pw_hc1 = pw_cache->pw_buf.hc1[cache_cnt];

  memcpy (pw_hc1, pw_buf, pw_len);

  memset (pw_hc1 + pw_len, 0, 256 - pw_len);

  uint pws_cnt = device_param->pws_cnt;

  cache_cnt++;

  if (cache_cnt == VECT_SIZE_1)
  {
    pw_t *pw = device_param->pws_buf + pws_cnt;

    device_param->pw_transpose (&pw_cache->pw_buf, pw);

    pw->pw_len = pw_len;

    pws_cnt++;

    device_param->pws_cnt = pws_cnt;
    device_param->pw_cnt  = pws_cnt * 1;

    cache_cnt = 0;
  }

  pw_cache->cnt = cache_cnt;

  return pws_cnt;
}

static uint pw_add_to_hc2 (hc_device_param_t *device_param, const uint8_t *pw_buf, const uint pw_len)
{
  if (data.devices_status == STATUS_BYPASS) return 0;

  pw_cache_t *pw_cache = device_param->pw_caches + pw_len;

  uint cache_cnt = pw_cache->cnt;

  uint8_t *pw_hc2 = pw_cache->pw_buf.hc2[cache_cnt];

  memcpy (pw_hc2, pw_buf, pw_len);

  memset (pw_hc2 + pw_len, 0, 128 - pw_len);

  uint pws_cnt = device_param->pws_cnt;

  cache_cnt++;

  if (cache_cnt == VECT_SIZE_2)
  {
    pw_t *pw = device_param->pws_buf + pws_cnt;

    device_param->pw_transpose (&pw_cache->pw_buf, pw);

    pw->pw_len = pw_len;

    pws_cnt++;

    device_param->pws_cnt = pws_cnt;
    device_param->pw_cnt  = pws_cnt * 2;

    cache_cnt = 0;
  }

  pw_cache->cnt = cache_cnt;

  return pws_cnt;
}

static uint pw_add_to_hc4 (hc_device_param_t *device_param, const uint8_t *pw_buf, const uint pw_len)
{
  if (data.devices_status == STATUS_BYPASS) return 0;

  pw_cache_t *pw_cache = device_param->pw_caches + pw_len;

  uint cache_cnt = pw_cache->cnt;

  uint8_t *pw_hc4 = pw_cache->pw_buf.hc4[cache_cnt];

  memcpy (pw_hc4, pw_buf, pw_len);

  memset (pw_hc4 + pw_len, 0, 64 - pw_len);

  uint pws_cnt = device_param->pws_cnt;

  cache_cnt++;

  if (cache_cnt == VECT_SIZE_4)
  {
    pw_t *pw = device_param->pws_buf + pws_cnt;

    device_param->pw_transpose (&pw_cache->pw_buf, pw);

    pw->pw_len = pw_len;

    pws_cnt++;

    device_param->pws_cnt = pws_cnt;
    device_param->pw_cnt  = pws_cnt * 4;

    cache_cnt = 0;
  }

  pw_cache->cnt = cache_cnt;

  return pws_cnt;
}

static void *thread_monitor (void *p)
{
  uint runtime_check = 0;
  uint remove_check  = 0;
  uint status_check  = 0;
  uint hwmon_check   = 0;
  uint restore_check = 0;

  uint restore_left = data.restore_timer;
  uint remove_left  = data.remove_timer;
  uint status_left  = data.status_timer;

  #ifdef _OCL
  #ifndef OSX

  int *fan_speed_chgd = (int *) mycalloc (data.devices_cnt, sizeof (int));

  // temperature controller "loopback" values

  int *temp_diff_old = (int *) mycalloc (data.devices_cnt, sizeof (int));
  int *temp_diff_sum = (int *) mycalloc (data.devices_cnt, sizeof (int));

  int temp_threshold = 1; // degrees celcius

  int fan_speed_min =  15; // in percentage
  int fan_speed_max = 100;

  time_t last_temp_check_time;

  #endif
  #endif

  uint sleep_time = 1;

  if (data.runtime)
    runtime_check = 1;

  if (data.restore_timer)
    restore_check = 1;

  if ((data.remove == 1) && (data.hashlist_mode == HL_MODE_FILE))
    remove_check = 1;

  if (data.status == 1)
    status_check = 1;

  if (data.gpu_temp_disable == 0)
  {
    #ifdef _OCL
    #ifndef OSX
    time (&last_temp_check_time);
    #endif
    #endif

    hwmon_check = 1;
  }

  if ((runtime_check == 0) && (remove_check == 0) && (status_check == 0) && (hwmon_check == 0) && (restore_check == 0))
  {
    return (p);
  }

  while ((data.devices_status != STATUS_EXHAUSTED) && (data.devices_status != STATUS_CRACKED) && (data.devices_status != STATUS_ABORTED) && (data.devices_status != STATUS_QUIT))
  {
    hc_sleep (sleep_time);

    if (data.devices_status != STATUS_RUNNING) continue;

    if (hwmon_check == 1)
    {
      hc_thread_mutex_lock (mux_adl);

      #ifdef _OCL
      #ifndef OSX

      time_t temp_check_time;

      time (&temp_check_time);

      uint Ta = temp_check_time - last_temp_check_time; // set Ta = sleep_time; is not good enough (see --remove etc)

      if (Ta == 0) Ta = 1;

      #endif
      #endif

      for (uint i = 0; i < data.devices_cnt; i++)
      {
        const int temperature = hm_get_temperature_with_device_id (i);

        if (temperature > (int) data.gpu_temp_abort)
        {
          log_error ("ERROR: Temperature limit on GPU %d reached, aborting...", i + 1);

          if (data.devices_status != STATUS_QUIT) myabort ();

          break;
        }

        #ifdef _OCL
        #ifndef OSX

        const int gpu_temp_retain = data.gpu_temp_retain;

        if (gpu_temp_retain)
        {
          if (data.hm_device[i].fan_supported == 1)
          {
            int temp_cur = temperature;

            int temp_diff_new = gpu_temp_retain - temp_cur;

            temp_diff_sum[i] = temp_diff_sum[i] + temp_diff_new;

            // calculate Ta value (time difference in seconds between the last check and this check)

            last_temp_check_time = temp_check_time;

            float Kp = 1.8;
            float Ki = 0.005;
            float Kd = 6;

            // PID controller (3-term controller: proportional - Kp, integral - Ki, derivative - Kd)

            int fan_diff_required = (int) (Kp * (float)temp_diff_new + Ki * Ta * (float)temp_diff_sum[i] + Kd * ((float)(temp_diff_new - temp_diff_old[i])) / Ta);

            if (abs (fan_diff_required) >= temp_threshold)
            {
              const int fan_speed_cur = hm_get_fanspeed_with_device_id (i);

              int fan_speed_level = fan_speed_cur;

              if (fan_speed_chgd[i] == 0) fan_speed_level = temp_cur;

              int fan_speed_new = fan_speed_level - fan_diff_required;

              if (fan_speed_new > fan_speed_max) fan_speed_new = fan_speed_max;
              if (fan_speed_new < fan_speed_min) fan_speed_new = fan_speed_min;

              if (fan_speed_new != fan_speed_cur)
              {
                int freely_change_fan_speed = (fan_speed_chgd[i] == 1);
                int fan_speed_must_change = (fan_speed_new > fan_speed_cur);

                if ((freely_change_fan_speed == 1) || (fan_speed_must_change == 1))
                {
                  hm_set_fanspeed_with_device_id (i, fan_speed_new);

                  fan_speed_chgd[i] = 1;
                }

                temp_diff_old[i] = temp_diff_new;
              }
            }
          }
        }

        #endif
        #endif
      }

      hc_thread_mutex_unlock (mux_adl);
    }

    if (restore_check == 1)
    {
      restore_left--;

      if (restore_left == 0)
      {
        if (data.restore_disable == 0) cycle_restore ();

        restore_left = data.restore_timer;
      }
    }

    if ((runtime_check == 1) && (data.runtime_start > 0))
    {
      time_t runtime_cur;

      time (&runtime_cur);

      int runtime_left = data.runtime_start + data.runtime - runtime_cur;

      if (runtime_left <= 0)
      {
        if (data.benchmark == 0)
        {
          if (data.quiet == 0) log_info ("\nNOTE: Runtime limit reached, aborting...\n");
        }

        if (data.devices_status != STATUS_QUIT) myabort ();
      }
    }

    if (remove_check == 1)
    {
      remove_left--;

      if (remove_left == 0)
      {
        if (data.digests_saved != data.digests_done)
        {
          data.digests_saved = data.digests_done;

          save_hash ();
        }

        remove_left = data.remove_timer;
      }
    }

    if (status_check == 1)
    {
      status_left--;

      if (status_left == 0)
      {
        hc_thread_mutex_lock (mux_display);

        if (data.quiet == 0) clear_prompt ();

        if (data.quiet == 0) log_info ("");

        status_display ();

        if (data.quiet == 0) log_info ("");

        hc_thread_mutex_unlock (mux_display);

        status_left = data.status_timer;
      }
    }
  }

  #ifdef _OCL
  #ifndef OSX
  myfree (fan_speed_chgd);

  myfree (temp_diff_old);
  myfree (temp_diff_sum);
  #endif
  #endif

  p = NULL;

  return (p);
}

static void *thread_outfile_remove (void *p)
{
  // some hash-dependent constants
  char *outfile_dir = data.outfile_check_directory;
  uint dgst_size    = data.dgst_size;
  uint isSalted     = data.isSalted;
  uint esalt_size   = data.esalt_size;
  uint hash_mode    = data.hash_mode;

  uint outfile_check_timer = data.outfile_check_timer;

  char separator = data.separator;

  // some hash-dependent functions
  int (*sort_by_digest) (const void *, const void *) = data.sort_by_digest;
  int (*parse_func) (char *, uint, hash_t *)         = data.parse_func;

  // buffers
  hash_t hash_buf;

  memset (&hash_buf, 0, sizeof (hash_buf));

  hash_buf.digest = mymalloc (dgst_size);

  if (isSalted)   hash_buf.salt =  (salt_t *) mymalloc (sizeof (salt_t));

  if (esalt_size) hash_buf.esalt = (void   *) mymalloc (esalt_size);

  uint digest_buf[64];

  outfile_data_t *out_info = NULL;

  char **out_files = NULL;

  time_t folder_mtime = 0;

  int  out_cnt = 0;

  uint check_left = outfile_check_timer; // or 1 if we want to check it at startup

  while ((data.devices_status != STATUS_EXHAUSTED) && (data.devices_status != STATUS_CRACKED) && (data.devices_status != STATUS_ABORTED) && (data.devices_status != STATUS_QUIT))
  {
    hc_sleep (1);

    if (data.devices_status != STATUS_RUNNING) continue;

    check_left--;

    if (check_left == 0)
    {
      struct stat outfile_check_stat;

      if (stat (outfile_dir, &outfile_check_stat) == 0)
      {
        uint is_dir = S_ISDIR (outfile_check_stat.st_mode);

        if (is_dir == 1)
        {
          if (outfile_check_stat.st_mtime > folder_mtime)
          {
            char **out_files_new = scan_directory (outfile_dir);

            int out_cnt_new = count_dictionaries (out_files_new);

            outfile_data_t *out_info_new = NULL;

            if (out_cnt_new > 0)
            {
              out_info_new = (outfile_data_t *) mycalloc (out_cnt_new, sizeof (outfile_data_t));

              for (int i = 0; i < out_cnt_new; i++)
              {
                out_info_new[i].file_name = out_files_new[i];

                // check if there are files that we have seen/checked before (and not changed)

                for (int j = 0; j < out_cnt; j++)
                {
                  if (strcmp (out_info[j].file_name, out_info_new[i].file_name) == 0)
                  {
                    struct stat outfile_stat;

                    if (stat (out_info_new[i].file_name, &outfile_stat) == 0)
                    {
                      if (outfile_stat.st_ctime == out_info[j].ctime)
                      {
                        out_info_new[i].ctime = out_info[j].ctime;
                        out_info_new[i].seek  = out_info[j].seek;
                      }
                    }
                  }
                }
              }
            }

            local_free (out_info);
            local_free (out_files);

            out_files = out_files_new;
            out_cnt   = out_cnt_new;
            out_info  = out_info_new;

            folder_mtime = outfile_check_stat.st_mtime;
          }

          for (int j = 0; j < out_cnt; j++)
          {
            FILE *fp = fopen (out_info[j].file_name, "rb");

            if (fp != NULL)
            {
              //hc_thread_mutex_lock (mux_display);

              #ifdef _POSIX
              struct stat outfile_stat;

              fstat (fileno (fp), &outfile_stat);
              #endif

              #ifdef _WIN
              struct stat64 outfile_stat;

              _fstat64 (fileno (fp), &outfile_stat);
              #endif

              if (outfile_stat.st_ctime > out_info[j].ctime)
              {
                out_info[j].ctime = outfile_stat.st_ctime;
                out_info[j].seek  = 0;
              }

              fseek (fp, out_info[j].seek, SEEK_SET);

              while (!feof (fp))
              {
                char line_buf[BUFSIZ];

                memset (line_buf, 0, BUFSIZ);

                char *ptr = fgets (line_buf, BUFSIZ - 1, fp);

                if (ptr == NULL) break;

                int line_len = strlen (line_buf);

                if (line_len <= 0) continue;

                int iter = MAX_CUT_TRIES;

                for (uint i = line_len - 1; i && iter; i--, line_len--)
                {
                  if (line_buf[i] != separator) continue;

                  int parser_status = PARSER_OK;

                  if ((hash_mode != 2500) && (hash_mode != 6800))
                  {
                    parser_status = parse_func (line_buf, line_len - 1, &hash_buf);
                  }

                  uint found = 0;

                  if (parser_status == PARSER_OK)
                  {
                    for (uint salt_pos = 0; (found == 0) && (salt_pos < data.salts_cnt); salt_pos++)
                    {
                      if (data.salts_shown[salt_pos] == 1) continue;

                      salt_t *salt_buf = &data.salts_buf[salt_pos];

                      for (uint digest_pos = 0; (found == 0) && (digest_pos < salt_buf->digests_cnt); digest_pos++)
                      {
                        uint idx = salt_buf->digests_offset + digest_pos;

                        if (data.digests_shown[idx] == 1) continue;

                        uint cracked = 0;

                        if (hash_mode == 6800)
                        {
                          if (i == salt_buf->salt_len)
                          {
                            cracked = (memcmp (line_buf, salt_buf->salt_buf, salt_buf->salt_len) == 0);
                          }
                        }
                        else if (hash_mode == 2500)
                        {
                          // BSSID : MAC1 : MAC2 (:plain)
                          if (i == (salt_buf->salt_len + 1 + 12 + 1 + 12))
                          {
                            cracked = (memcmp (line_buf, salt_buf->salt_buf, salt_buf->salt_len) == 0);

                            if (!cracked) continue;

                            // now compare MAC1 and MAC2 too, since we have this additional info
                            char *mac1_pos = line_buf + salt_buf->salt_len + 1;
                            char *mac2_pos = mac1_pos + 12 + 1;

                            wpa_t *wpas = (wpa_t *) data.esalts_buf;
                            wpa_t *wpa  = &wpas[salt_pos];

                            uint pke[25];

                            char *pke_ptr = (char *) pke;

                            for (uint i = 0; i < 25; i++)
                            {
                              pke[i] = byte_swap_32 (wpa->pke[i]);
                            }

                            unsigned char mac1[6];
                            unsigned char mac2[6];

                            memcpy (mac1, pke_ptr + 23, 6);
                            memcpy (mac2, pke_ptr + 29, 6);

                            // compare hex string(s) vs binary MAC address(es)

                            for (uint i = 0, j = 0; i < 6; i++, j += 2)
                            {
                              if (mac1[i] != (unsigned char) hex_to_char (&mac1_pos[j]))
                              {
                                cracked = 0;
                                break;
                              }
                            }

                            // early skip ;)
                            if (!cracked) continue;

                            for (uint i = 0, j = 0; i < 6; i++, j += 2)
                            {
                              if (mac2[i] != (unsigned char) hex_to_char (&mac2_pos[j]))
                              {
                                cracked = 0;
                                break;
                              }
                            }
                          }
                        }
                        else
                        {
                          char *digests_buf_ptr = (char *) data.digests_buf;

                          memcpy (digest_buf, digests_buf_ptr + (data.salts_buf[salt_pos].digests_offset * dgst_size) + (digest_pos * dgst_size), dgst_size);

                          cracked = (sort_by_digest (digest_buf, hash_buf.digest) == 0);
                        }

                        if (cracked == 1)
                        {
                          found = 1;

                          data.digests_shown[idx] = 1;

                          data.digests_done++;

                          salt_buf->digests_done++;

                          if (salt_buf->digests_done == salt_buf->digests_cnt)
                          {
                            data.salts_shown[salt_pos] = 1;

                            data.salts_done++;

                            if (data.salts_done == data.salts_cnt) data.devices_status = STATUS_CRACKED;
                          }
                        }
                      }

                      if (data.devices_status == STATUS_CRACKED) break;
                    }
                  }

                  if (found) break;

                  if (data.devices_status == STATUS_CRACKED) break;

                  iter--;
                }

                if (data.devices_status == STATUS_CRACKED) break;
              }

              out_info[j].seek = ftell (fp);

              //hc_thread_mutex_unlock (mux_display);

              fclose (fp);
            }
          }
        }
      }

      check_left = outfile_check_timer;
    }
  }

  if (esalt_size) local_free (hash_buf.esalt);

  if (isSalted)   local_free (hash_buf.salt);

  local_free (hash_buf.digest);

  local_free (out_info);

  local_free (out_files);

  p = NULL;

  return (p);
}

static uint get_work (hc_device_param_t *device_param, const uint64_t max)
{
  hc_thread_mutex_lock (mux_dispatcher);

  const uint64_t words_cur  = data.words_cur;
  const uint64_t words_base = (data.limit == 0) ? data.words_base : data.limit;

  device_param->words_off = words_cur;

  const uint64_t words_left = words_base - words_cur;

  if (data.gpu_blocks_all > words_left)
  {
    if (data.gpu_blocks_div == 0)
    {
      data.gpu_blocks_div = find_gpu_blocks_div (words_left, data.gpu_blocks_all);
    }
  }

  if (data.gpu_blocks_div)
  {
    if (device_param->gpu_blocks == device_param->gpu_blocks_user)
    {
      const uint32_t gpu_blocks_new = (float) device_param->gpu_blocks * data.gpu_blocks_div;
      const uint32_t gpu_power_new  = gpu_blocks_new / device_param->gpu_vector_width;

      if (gpu_blocks_new < device_param->gpu_blocks)
      {
        device_param->gpu_blocks  = gpu_blocks_new;
        device_param->gpu_power   = gpu_power_new;
      }
    }
  }

  const uint gpu_blocks = device_param->gpu_blocks;

  uint work = MIN (words_left, gpu_blocks);

  work = MIN (work, max);

  data.words_cur += work;

  hc_thread_mutex_unlock (mux_dispatcher);

  return work;
}

static void *thread_calc_stdin (void *p)
{
  hc_device_param_t *device_param = (hc_device_param_t *) p;

  const uint attack_kern = data.attack_kern;

  const uint gpu_blocks = device_param->gpu_blocks;

  while ((data.devices_status != STATUS_EXHAUSTED) && (data.devices_status != STATUS_CRACKED) && (data.devices_status != STATUS_ABORTED) && (data.devices_status != STATUS_QUIT))
  {
    hc_thread_mutex_lock (mux_dispatcher);

    if (feof (stdin) != 0)
    {
      hc_thread_mutex_unlock (mux_dispatcher);

      break;
    }

    uint words_cur = 0;

    while (words_cur < gpu_blocks)
    {
      char buf[BUFSIZ];

      char *line_buf = fgets (buf, sizeof (buf), stdin);

      if (line_buf == NULL) break;

      uint line_len = in_superchop (line_buf);

      line_len = convert_from_hex (line_buf, line_len);

      // post-process rule engine

      if (run_rule_engine (data.rule_len_l, data.rule_buf_l))
      {
        char rule_buf_out[BLOCK_SIZE];

        memset (rule_buf_out, 0, sizeof (rule_buf_out));

        int rule_len_out = -1;

        if (line_len < BLOCK_SIZE)
        {
          rule_len_out = _old_apply_rule (data.rule_buf_l, data.rule_len_l, line_buf, line_len, rule_buf_out);
        }

        if (rule_len_out < 0) continue;

        line_buf = rule_buf_out;
        line_len = rule_len_out;
      }

      if (line_len > PW_MAX)
      {
        continue;
      }

      if (attack_kern == ATTACK_KERN_STRAIGHT)
      {
        if ((line_len < data.pw_min) || (line_len > data.pw_max))
        {
          hc_thread_mutex_lock (mux_counter);

          for (uint salt_pos = 0; salt_pos < data.salts_cnt; salt_pos++)
          {
            data.words_progress_rejected[salt_pos] += data.gpu_rules_cnt;
          }

          hc_thread_mutex_unlock (mux_counter);

          continue;
        }
      }
      else if (attack_kern == ATTACK_KERN_COMBI)
      {
        // do not check if minimum restriction is satisfied (line_len >= data.pw_min) here
        // since we still need to combine the plains

        if (line_len > data.pw_max)
        {
          hc_thread_mutex_lock (mux_counter);

          for (uint salt_pos = 0; salt_pos < data.salts_cnt; salt_pos++)
          {
            data.words_progress_rejected[salt_pos] += data.combs_cnt;
          }

          hc_thread_mutex_unlock (mux_counter);

          continue;
        }
      }

      device_param->pw_add (device_param, (uint8_t *) line_buf, line_len);

      words_cur++;

      if (data.devices_status == STATUS_CRACKED) break;
      if (data.devices_status == STATUS_ABORTED) break;
      if (data.devices_status == STATUS_QUIT)    break;
      if (data.devices_status == STATUS_BYPASS)  break;
    }

    hc_thread_mutex_unlock (mux_dispatcher);

    if (data.devices_status == STATUS_CRACKED) break;
    if (data.devices_status == STATUS_ABORTED) break;
    if (data.devices_status == STATUS_QUIT)    break;
    if (data.devices_status == STATUS_BYPASS)  break;

    // we need 2 flushing because we have two independant caches and it can occur
    // that one buffer is already at threshold plus for that length also exists
    // more data in the 2nd buffer so it would overflow

    // flush session 1

    {
      for (int pw_len = 0; pw_len < PW_MAX1; pw_len++)
      {
        pw_cache_t *pw_cache = &device_param->pw_caches[pw_len];

        const uint pw_cache_cnt = pw_cache->cnt;

        if (pw_cache_cnt == 0) continue;

        pw_cache->cnt = 0;

        uint pws_cnt = device_param->pws_cnt;

        pw_t *pw = device_param->pws_buf + pws_cnt;

        device_param->pw_transpose (&pw_cache->pw_buf, pw);

        pw->pw_len = pw_len;

        uint pw_cnt = device_param->pw_cnt;

        pw_cnt += pw_cache_cnt;

        device_param->pw_cnt  = pw_cnt;

        pws_cnt++;

        device_param->pws_cnt = pws_cnt;

        if (pws_cnt == device_param->gpu_power_user) break;
      }

      const uint pw_cnt  = device_param->pw_cnt;
      const uint pws_cnt = device_param->pws_cnt;

      if (pws_cnt)
      {
        run_copy (device_param, pws_cnt);

        run_cracker (device_param, pw_cnt, pws_cnt);

        device_param->pw_cnt  = 0;
        device_param->pws_cnt = 0;
      }
    }

    // flush session 2

    {
      for (int pw_len = 0; pw_len < PW_MAX1; pw_len++)
      {
        pw_cache_t *pw_cache = &device_param->pw_caches[pw_len];

        const uint pw_cache_cnt = pw_cache->cnt;

        if (pw_cache_cnt == 0) continue;

        pw_cache->cnt = 0;

        uint pws_cnt = device_param->pws_cnt;

        pw_t *pw = device_param->pws_buf + pws_cnt;

        device_param->pw_transpose (&pw_cache->pw_buf, pw);

        pw->pw_len = pw_len;

        uint pw_cnt = device_param->pw_cnt;

        pw_cnt += pw_cache_cnt;

        device_param->pw_cnt  = pw_cnt;

        pws_cnt++;

        device_param->pws_cnt = pws_cnt;
      }

      const uint pw_cnt  = device_param->pw_cnt;
      const uint pws_cnt = device_param->pws_cnt;

      if (pws_cnt)
      {
        run_copy (device_param, pws_cnt);

        run_cracker (device_param, pw_cnt, pws_cnt);

        device_param->pw_cnt  = 0;
        device_param->pws_cnt = 0;
      }
    }
  }

  return NULL;
}

static void *thread_calc (void *p)
{
  hc_device_param_t *device_param = (hc_device_param_t *) p;

  const uint attack_mode = data.attack_mode;
  const uint attack_kern = data.attack_kern;

  if (attack_mode == ATTACK_MODE_BF)
  {
    while ((data.devices_status != STATUS_EXHAUSTED) && (data.devices_status != STATUS_CRACKED) && (data.devices_status != STATUS_ABORTED) && (data.devices_status != STATUS_QUIT))
    {
      const uint work = get_work (device_param, -1);

      if (work == 0) break;

      const uint64_t words_off = device_param->words_off;
      const uint64_t words_fin = words_off + work;

      const uint gpu_vector_width = device_param->gpu_vector_width;

      const uint pw_cnt  = work;
      const uint pws_cnt = mydivc32 (work, gpu_vector_width);

      device_param->pw_cnt  = pw_cnt;
      device_param->pws_cnt = pws_cnt;

      if (pws_cnt)
      {
        run_copy (device_param, pws_cnt);

        run_cracker (device_param, pw_cnt, pws_cnt);

        device_param->pw_cnt  = 0;
        device_param->pws_cnt = 0;
      }

      if (data.devices_status == STATUS_CRACKED) break;
      if (data.devices_status == STATUS_ABORTED) break;
      if (data.devices_status == STATUS_QUIT)    break;
      if (data.devices_status == STATUS_BYPASS)  break;

      device_param->words_done = words_fin;
    }
  }
  else
  {
    const uint segment_size = data.segment_size;

    char *dictfile = data.dictfile;

    if (attack_mode == ATTACK_MODE_COMBI)
    {
      if (data.combs_mode == COMBINATOR_MODE_BASE_RIGHT)
      {
        dictfile = data.dictfile2;
      }
    }

    FILE *fd = fopen (dictfile, "rb");

    if (fd == NULL)
    {
      log_error ("ERROR: %s: %s", dictfile, strerror (errno));

      return NULL;
    }

    if (attack_mode == ATTACK_MODE_COMBI)
    {
      const uint combs_mode = data.combs_mode;

      if (combs_mode == COMBINATOR_MODE_BASE_LEFT)
      {
        const char *dictfilec = data.dictfile2;

        FILE *combs_fp = fopen (dictfilec, "rb");

        if (combs_fp == NULL)
        {
          log_error ("ERROR: %s: %s", dictfilec, strerror (errno));

          fclose (fd);

          return NULL;
        }

        device_param->combs_fp = combs_fp;
      }
      else if (combs_mode == COMBINATOR_MODE_BASE_RIGHT)
      {
        const char *dictfilec = data.dictfile;

        FILE *combs_fp = fopen (dictfilec, "rb");

        if (combs_fp == NULL)
        {
          log_error ("ERROR: %s: %s", dictfilec, strerror (errno));

          fclose (fd);

          return NULL;
        }

        device_param->combs_fp = combs_fp;
      }
    }

    wl_data_t *wl_data = (wl_data_t *) mymalloc (sizeof (wl_data_t));

    wl_data->buf   = (char *) mymalloc (segment_size);
    wl_data->avail = segment_size;
    wl_data->incr  = segment_size;
    wl_data->cnt   = 0;
    wl_data->pos   = 0;

    uint64_t words_cur = 0;

    while ((data.devices_status != STATUS_EXHAUSTED) && (data.devices_status != STATUS_CRACKED) && (data.devices_status != STATUS_ABORTED) && (data.devices_status != STATUS_QUIT))
    {
      uint64_t words_off = 0;
      uint64_t words_fin = 0;

      uint64_t max = -1;

      while (max)
      {
        const uint work = get_work (device_param, max);

        if (work == 0) break;

        words_off = device_param->words_off;
        words_fin = words_off + work;

        char *line_buf;
        uint  line_len;

        for ( ; words_cur < words_off; words_cur++) get_next_word (wl_data, fd, &line_buf, &line_len);

        max = 0;

        for ( ; words_cur < words_fin; words_cur++)
        {
          get_next_word (wl_data, fd, &line_buf, &line_len);

          line_len = convert_from_hex (line_buf, line_len);

          // post-process rule engine

          if (run_rule_engine (data.rule_len_l, data.rule_buf_l))
          {
            char rule_buf_out[BLOCK_SIZE];

            memset (rule_buf_out, 0, sizeof (rule_buf_out));

            int rule_len_out = -1;

            if (line_len < BLOCK_SIZE)
            {
              rule_len_out = _old_apply_rule (data.rule_buf_l, data.rule_len_l, line_buf, line_len, rule_buf_out);
            }

            if (rule_len_out < 0) continue;

            line_buf = rule_buf_out;
            line_len = rule_len_out;
          }

          if (attack_kern == ATTACK_KERN_STRAIGHT)
          {
            if ((line_len < data.pw_min) || (line_len > data.pw_max))
            {
              max++;

              hc_thread_mutex_lock (mux_counter);

              for (uint salt_pos = 0; salt_pos < data.salts_cnt; salt_pos++)
              {
                data.words_progress_rejected[salt_pos] += data.gpu_rules_cnt;
              }

              hc_thread_mutex_unlock (mux_counter);

              continue;
            }
          }
          else if (attack_kern == ATTACK_KERN_COMBI)
          {
            // do not check if minimum restriction is satisfied (line_len >= data.pw_min) here
            // since we still need to combine the plains

            if (line_len > data.pw_max)
            {
              max++;

              hc_thread_mutex_lock (mux_counter);

              for (uint salt_pos = 0; salt_pos < data.salts_cnt; salt_pos++)
              {
                data.words_progress_rejected[salt_pos] += data.combs_cnt;
              }

              hc_thread_mutex_unlock (mux_counter);

              continue;
            }
          }

          device_param->pw_add (device_param, (uint8_t *) line_buf, line_len);

          if (data.devices_status == STATUS_CRACKED) break;
          if (data.devices_status == STATUS_ABORTED) break;
          if (data.devices_status == STATUS_QUIT)    break;
          if (data.devices_status == STATUS_BYPASS)  break;
        }

        if (data.devices_status == STATUS_CRACKED) break;
        if (data.devices_status == STATUS_ABORTED) break;
        if (data.devices_status == STATUS_QUIT)    break;
        if (data.devices_status == STATUS_BYPASS)  break;
      }

      if (data.devices_status == STATUS_CRACKED) break;
      if (data.devices_status == STATUS_ABORTED) break;
      if (data.devices_status == STATUS_QUIT)    break;
      if (data.devices_status == STATUS_BYPASS)  break;

      // we need 2 flushing because we have two independant caches and it can occur
      // that one buffer is already at threshold plus for that length also exists
      // more data in the 2nd buffer so it would overflow

      //
      // flush session 1
      //

      {
        for (int pw_len = 0; pw_len < PW_MAX1; pw_len++)
        {
          pw_cache_t *pw_cache = &device_param->pw_caches[pw_len];

          const uint pw_cache_cnt = pw_cache->cnt;

          if (pw_cache_cnt == 0) continue;

          pw_cache->cnt = 0;

          uint pws_cnt = device_param->pws_cnt;

          pw_t *pw = device_param->pws_buf + pws_cnt;

          device_param->pw_transpose (&pw_cache->pw_buf, pw);

          pw->pw_len = pw_len;

          uint pw_cnt = device_param->pw_cnt;

          pw_cnt += pw_cache_cnt;

          device_param->pw_cnt  = pw_cnt;

          pws_cnt++;

          device_param->pws_cnt = pws_cnt;

          if (pws_cnt == device_param->gpu_power_user) break;
        }

        const uint pw_cnt  = device_param->pw_cnt;
        const uint pws_cnt = device_param->pws_cnt;

        if (pws_cnt)
        {
          run_copy (device_param, pws_cnt);

          run_cracker (device_param, pw_cnt, pws_cnt);

          device_param->pw_cnt  = 0;
          device_param->pws_cnt = 0;
        }

        if (data.devices_status == STATUS_CRACKED) break;
        if (data.devices_status == STATUS_ABORTED) break;
        if (data.devices_status == STATUS_QUIT)    break;
        if (data.devices_status == STATUS_BYPASS)  break;
      }

      //
      // flush session 2
      //

      {
        for (int pw_len = 0; pw_len < PW_MAX1; pw_len++)
        {
          pw_cache_t *pw_cache = &device_param->pw_caches[pw_len];

          const uint pw_cache_cnt = pw_cache->cnt;

          if (pw_cache_cnt == 0) continue;

          pw_cache->cnt = 0;

          uint pws_cnt = device_param->pws_cnt;

          pw_t *pw = device_param->pws_buf + pws_cnt;

          device_param->pw_transpose (&pw_cache->pw_buf, pw);

          pw->pw_len = pw_len;

          uint pw_cnt = device_param->pw_cnt;

          pw_cnt += pw_cache_cnt;

          device_param->pw_cnt  = pw_cnt;

          pws_cnt++;

          device_param->pws_cnt = pws_cnt;
        }

        const uint pw_cnt  = device_param->pw_cnt;
        const uint pws_cnt = device_param->pws_cnt;

        if (pws_cnt)
        {
          run_copy (device_param, pws_cnt);

          run_cracker (device_param, pw_cnt, pws_cnt);

          device_param->pw_cnt  = 0;
          device_param->pws_cnt = 0;
        }

        if (data.devices_status == STATUS_CRACKED) break;
        if (data.devices_status == STATUS_ABORTED) break;
        if (data.devices_status == STATUS_QUIT)    break;
        if (data.devices_status == STATUS_BYPASS)  break;
      }

      if (words_fin == 0) break;

      device_param->words_done = words_fin;
    }

    if (attack_mode == ATTACK_MODE_COMBI)
    {
      fclose (device_param->combs_fp);
    }

    free (wl_data->buf);
    free (wl_data);

    fclose (fd);
  }

  return NULL;
}

static void weak_hash_check (hc_device_param_t *device_param, const uint salt_pos, const uint gpu_loops)
{
  #ifdef _CUDA
  hc_cuCtxPushCurrent (device_param->context);
  #endif

  salt_t *salt_buf = &data.salts_buf[salt_pos];

  device_param->kernel_params_buf32[24] = salt_pos;
  device_param->kernel_params_buf32[27] = 1;
  device_param->kernel_params_buf32[28] = salt_buf->digests_cnt;
  device_param->kernel_params_buf32[29] = salt_buf->digests_offset;
  device_param->kernel_params_buf32[30] = 0;
  device_param->kernel_params_buf32[31] = 1;

  char *dictfile_old  = data.dictfile;
  char *dictfile2_old = data.dictfile2;
  char *mask_old      = data.mask;

  const char *weak_hash_check = "weak-hash-check";

  data.dictfile  = (char *) weak_hash_check;
  data.dictfile2 = (char *) weak_hash_check;
  data.mask      = (char *) weak_hash_check;

  /**
   * run the kernel
   */

  if (data.attack_exec == ATTACK_EXEC_ON_GPU)
  {
    run_kernel (KERN_RUN_1, device_param, 1);
  }
  else
  {
    run_kernel (KERN_RUN_1, device_param, 1);

    const uint iter = salt_buf->salt_iter;

    for (uint loop_pos = 0; loop_pos < iter; loop_pos += gpu_loops)
    {
      uint loop_left = iter - loop_pos;

      loop_left = MIN (loop_left, gpu_loops);

      device_param->kernel_params_buf32[25] = loop_pos;
      device_param->kernel_params_buf32[26] = loop_left;

      run_kernel (KERN_RUN_2, device_param, 1);
    }

    run_kernel (KERN_RUN_3, device_param, 1);
  }

  /**
   * result
   */

  check_cracked (device_param, salt_pos);

  /**
   * cleanup
   */

  device_param->kernel_params_buf32[24] = 0;
  device_param->kernel_params_buf32[25] = 0;
  device_param->kernel_params_buf32[26] = 0;
  device_param->kernel_params_buf32[27] = 0;
  device_param->kernel_params_buf32[28] = 0;
  device_param->kernel_params_buf32[29] = 0;
  device_param->kernel_params_buf32[30] = 0;
  device_param->kernel_params_buf32[31] = 0;

  data.dictfile  = dictfile_old;
  data.dictfile2 = dictfile2_old;
  data.mask      = mask_old;

  #ifdef _CUDA
  hc_cuCtxPopCurrent (&device_param->context);
  #endif
}

// hlfmt hashcat

static void hlfmt_hash_hashcat (char line_buf[BUFSIZ], int line_len, char **hashbuf_pos, int *hashbuf_len)
{
  if (data.username == 0)
  {
    *hashbuf_pos = line_buf;
    *hashbuf_len = line_len;
  }
  else
  {
    char *pos = line_buf;
    int   len = line_len;

    for (int i = 0; i < line_len; i++, pos++, len--)
    {
      if (line_buf[i] == data.separator)
      {
        pos++;

        len--;

        break;
      }
    }

    *hashbuf_pos = pos;
    *hashbuf_len = len;
  }
}

static void hlfmt_user_hashcat (char line_buf[BUFSIZ], int line_len, char **userbuf_pos, int *userbuf_len)
{
  char *pos = NULL;
  int   len = 0;

  int sep_cnt = 0;

  for (int i = 0; i < line_len; i++)
  {
    if (line_buf[i] == data.separator)
    {
      sep_cnt++;

      continue;
    }

    if (sep_cnt == 0)
    {
      if (pos == NULL) pos = line_buf + i;

      len++;
    }
  }

  *userbuf_pos = pos;
  *userbuf_len = len;
}

// hlfmt pwdump

static int hlfmt_detect_pwdump (char line_buf[BUFSIZ], int line_len)
{
  int sep_cnt = 0;

  int sep2_len = 0;
  int sep3_len = 0;

  for (int i = 0; i < line_len; i++)
  {
    if (line_buf[i] == ':')
    {
      sep_cnt++;

      continue;
    }

    if (sep_cnt == 2) sep2_len++;
    if (sep_cnt == 3) sep3_len++;
  }

  if ((sep_cnt == 6) && ((sep2_len == 32) || (sep3_len == 32))) return 1;

  return 0;
}

static void hlfmt_hash_pwdump (char line_buf[BUFSIZ], int line_len, char **hashbuf_pos, int *hashbuf_len)
{
  char *pos = NULL;
  int   len = 0;

  int sep_cnt = 0;

  for (int i = 0; i < line_len; i++)
  {
    if (line_buf[i] == ':')
    {
      sep_cnt++;

      continue;
    }

    if (data.hash_mode == 1000)
    {
      if (sep_cnt == 3)
      {
        if (pos == NULL) pos = line_buf + i;

        len++;
      }
    }
    else if (data.hash_mode == 3000)
    {
      if (sep_cnt == 2)
      {
        if (pos == NULL) pos = line_buf + i;

        len++;
      }
    }
  }

  *hashbuf_pos = pos;
  *hashbuf_len = len;
}

static void hlfmt_user_pwdump (char line_buf[BUFSIZ], int line_len, char **userbuf_pos, int *userbuf_len)
{
  char *pos = NULL;
  int   len = 0;

  int sep_cnt = 0;

  for (int i = 0; i < line_len; i++)
  {
    if (line_buf[i] == ':')
    {
      sep_cnt++;

      continue;
    }

    if (sep_cnt == 0)
    {
      if (pos == NULL) pos = line_buf + i;

      len++;
    }
  }

  *userbuf_pos = pos;
  *userbuf_len = len;
}

// hlfmt passwd

static int hlfmt_detect_passwd (char line_buf[BUFSIZ], int line_len)
{
  int sep_cnt = 0;

  char sep5_first = 0;
  char sep6_first = 0;

  for (int i = 0; i < line_len; i++)
  {
    if (line_buf[i] == ':')
    {
      sep_cnt++;

      continue;
    }

    if (sep_cnt == 5) if (sep5_first == 0) sep5_first = line_buf[i];
    if (sep_cnt == 6) if (sep6_first == 0) sep6_first = line_buf[i];
  }

  if ((sep_cnt == 6) && ((sep5_first == '/') || (sep6_first == '/'))) return 1;

  return 0;
}

static void hlfmt_hash_passwd (char line_buf[BUFSIZ], int line_len, char **hashbuf_pos, int *hashbuf_len)
{
  char *pos = NULL;
  int   len = 0;

  int sep_cnt = 0;

  for (int i = 0; i < line_len; i++)
  {
    if (line_buf[i] == ':')
    {
      sep_cnt++;

      continue;
    }

    if (sep_cnt == 1)
    {
      if (pos == NULL) pos = line_buf + i;

      len++;
    }
  }

  *hashbuf_pos = pos;
  *hashbuf_len = len;
}

static void hlfmt_user_passwd (char line_buf[BUFSIZ], int line_len, char **userbuf_pos, int *userbuf_len)
{
  char *pos = NULL;
  int   len = 0;

  int sep_cnt = 0;

  for (int i = 0; i < line_len; i++)
  {
    if (line_buf[i] == ':')
    {
      sep_cnt++;

      continue;
    }

    if (sep_cnt == 0)
    {
      if (pos == NULL) pos = line_buf + i;

      len++;
    }
  }

  *userbuf_pos = pos;
  *userbuf_len = len;
}

// hlfmt shadow

static int hlfmt_detect_shadow (char line_buf[BUFSIZ], int line_len)
{
  int sep_cnt = 0;

  for (int i = 0; i < line_len; i++)
  {
    if (line_buf[i] == ':') sep_cnt++;
  }

  if (sep_cnt == 8) return 1;

  return 0;
}

static void hlfmt_hash_shadow (char line_buf[BUFSIZ], int line_len, char **hashbuf_pos, int *hashbuf_len)
{
  hlfmt_hash_passwd (line_buf, line_len, hashbuf_pos, hashbuf_len);
}

static void hlfmt_user_shadow (char line_buf[BUFSIZ], int line_len, char **userbuf_pos, int *userbuf_len)
{
  hlfmt_user_passwd (line_buf, line_len, userbuf_pos, userbuf_len);
}

// hlfmt main

static void hlfmt_hash (uint hashfile_format, char line_buf[BUFSIZ], int line_len, char **hashbuf_pos, int *hashbuf_len)
{
  switch (hashfile_format)
  {
    case HLFMT_HASHCAT: hlfmt_hash_hashcat (line_buf, line_len, hashbuf_pos, hashbuf_len); break;
    case HLFMT_PWDUMP:  hlfmt_hash_pwdump  (line_buf, line_len, hashbuf_pos, hashbuf_len); break;
    case HLFMT_PASSWD:  hlfmt_hash_passwd  (line_buf, line_len, hashbuf_pos, hashbuf_len); break;
    case HLFMT_SHADOW:  hlfmt_hash_shadow  (line_buf, line_len, hashbuf_pos, hashbuf_len); break;
  }
}

static void hlfmt_user (uint hashfile_format, char line_buf[BUFSIZ], int line_len, char **userbuf_pos, int *userbuf_len)
{
  switch (hashfile_format)
  {
    case HLFMT_HASHCAT: hlfmt_user_hashcat (line_buf, line_len, userbuf_pos, userbuf_len); break;
    case HLFMT_PWDUMP:  hlfmt_user_pwdump  (line_buf, line_len, userbuf_pos, userbuf_len); break;
    case HLFMT_PASSWD:  hlfmt_user_passwd  (line_buf, line_len, userbuf_pos, userbuf_len); break;
    case HLFMT_SHADOW:  hlfmt_user_shadow  (line_buf, line_len, userbuf_pos, userbuf_len); break;
  }
}

static uint hlfmt_detect (FILE *fp, uint max_check)
{
  // Exception: those formats are wrongly detected as HLFMT_SHADOW, prevent it

  if (data.hash_mode == 5300) return HLFMT_HASHCAT;
  if (data.hash_mode == 5400) return HLFMT_HASHCAT;

  uint *formats_cnt = (uint *) mycalloc (HLFMTS_CNT, sizeof (uint));

  uint num_check = 0;

  while (!feof (fp))
  {
    char line_buf[BUFSIZ];

    int line_len = fgetl (fp, line_buf);

    if (line_len == 0) continue;

    if (hlfmt_detect_pwdump (line_buf, line_len)) formats_cnt[HLFMT_PWDUMP]++;
    if (hlfmt_detect_passwd (line_buf, line_len)) formats_cnt[HLFMT_PASSWD]++;
    if (hlfmt_detect_shadow (line_buf, line_len)) formats_cnt[HLFMT_SHADOW]++;

    if (num_check == max_check) break;

    num_check++;
  }

  uint hashlist_format = HLFMT_HASHCAT;

  for (int i = 1; i < HLFMTS_CNT; i++)
  {
    if (formats_cnt[i - 1] >= formats_cnt[i]) continue;

    hashlist_format = i;
  }

  free (formats_cnt);

  return hashlist_format;
}

/**
 * main
 */

#ifdef _OCL
void *__stdcall ADL_Main_Memory_Alloc (const int iSize)
{
  return mymalloc (iSize);
}
#endif

static uint generate_bitmaps (const uint digests_cnt, const uint dgst_size, const uint dgst_shifts, char *digests_buf_ptr, const uint bitmap_mask, const uint bitmap_size, uint *bitmap_a, uint *bitmap_b, uint *bitmap_c, uint *bitmap_d, const uint64_t collisions_max)
{
  uint64_t collisions = 0;

  const uint dgst_pos0 = data.dgst_pos0;
  const uint dgst_pos1 = data.dgst_pos1;
  const uint dgst_pos2 = data.dgst_pos2;
  const uint dgst_pos3 = data.dgst_pos3;

  memset (bitmap_a, 0, bitmap_size);
  memset (bitmap_b, 0, bitmap_size);
  memset (bitmap_c, 0, bitmap_size);
  memset (bitmap_d, 0, bitmap_size);

  for (uint i = 0; i < digests_cnt; i++)
  {
    uint *digest_ptr = (uint *) digests_buf_ptr;

    digests_buf_ptr += dgst_size;

    const uint val0 = 1 << (digest_ptr[dgst_pos0] & 0x1f);
    const uint val1 = 1 << (digest_ptr[dgst_pos1] & 0x1f);
    const uint val2 = 1 << (digest_ptr[dgst_pos2] & 0x1f);
    const uint val3 = 1 << (digest_ptr[dgst_pos3] & 0x1f);

    const uint idx0 = (digest_ptr[dgst_pos0] >> dgst_shifts) & bitmap_mask;
    const uint idx1 = (digest_ptr[dgst_pos1] >> dgst_shifts) & bitmap_mask;
    const uint idx2 = (digest_ptr[dgst_pos2] >> dgst_shifts) & bitmap_mask;
    const uint idx3 = (digest_ptr[dgst_pos3] >> dgst_shifts) & bitmap_mask;

    if (bitmap_a[idx0] & val0) collisions++;
    if (bitmap_b[idx1] & val1) collisions++;
    if (bitmap_c[idx2] & val2) collisions++;
    if (bitmap_d[idx3] & val3) collisions++;

    bitmap_a[idx0] |= val0;
    bitmap_b[idx1] |= val1;
    bitmap_c[idx2] |= val2;
    bitmap_d[idx3] |= val3;

    if (collisions >= collisions_max) return 0x7fffffff;
  }

  return collisions;
}

int main (int argc, char **argv)
{
  /**
   * To help users a bit
   */

  char *compute = getenv ("COMPUTE");

  if (compute)
  {
    char display[100];

    snprintf (display, sizeof (display) - 1, "DISPLAY=%s", compute);

    putenv (display);
  }
  else
  {
    if (getenv ("DISPLAY") == NULL)
      putenv ((char *) "DISPLAY=:0");
  }

  /*
  if (getenv ("GPU_MAX_ALLOC_PERCENT") == NULL)
    putenv ((char *) "GPU_MAX_ALLOC_PERCENT=100");

  if (getenv ("GPU_USE_SYNC_OBJECTS") == NULL)
    putenv ((char *) "GPU_USE_SYNC_OBJECTS=1");
  */

  /**
   * Real init
   */

  memset (&data, 0, sizeof (hc_global_data_t));

  time_t proc_start;

  time (&proc_start);

  data.proc_start = proc_start;

  int    myargc = argc;
  char **myargv = argv;

  hc_thread_mutex_init (mux_dispatcher);
  hc_thread_mutex_init (mux_counter);
  hc_thread_mutex_init (mux_display);
  hc_thread_mutex_init (mux_adl);

  /**
   * commandline parameters
   */

  uint  usage             = USAGE;
  uint  version           = VERSION;
  uint  quiet             = QUIET;
  uint  benchmark         = BENCHMARK;
  uint  benchmark_mode    = BENCHMARK_MODE;
  uint  show              = SHOW;
  uint  left              = LEFT;
  uint  username          = USERNAME;
  uint  remove            = REMOVE;
  uint  remove_timer      = REMOVE_TIMER;
  uint64_t  skip          = SKIP;
  uint64_t  limit         = LIMIT;
  uint  keyspace          = KEYSPACE;
  uint  potfile_disable   = POTFILE_DISABLE;
  uint  debug_mode        = DEBUG_MODE;
  char *debug_file        = NULL;
  char *induction_dir     = NULL;
  char *outfile_check_dir = NULL;
  uint  force             = FORCE;
  uint  runtime           = RUNTIME;
  uint  hash_mode         = HASH_MODE;
  uint  attack_mode       = ATTACK_MODE;
  uint  markov_disable    = MARKOV_DISABLE;
  uint  markov_classic    = MARKOV_CLASSIC;
  uint  markov_threshold  = MARKOV_THRESHOLD;
  char *markov_hcstat     = NULL;
  char *outfile           = NULL;
  uint  outfile_format    = OUTFILE_FORMAT;
  uint  outfile_autohex   = OUTFILE_AUTOHEX;
  uint  outfile_check_timer = OUTFILE_CHECK_TIMER;
  uint  restore           = RESTORE;
  uint  restore_timer     = RESTORE_TIMER;
  uint  restore_disable   = RESTORE_DISABLE;
  uint  status            = STATUS;
  uint  status_timer      = STATUS_TIMER;
  uint  status_automat    = STATUS_AUTOMAT;
  uint  loopback          = LOOPBACK;
  uint  weak_hash_threshold = WEAK_HASH_THRESHOLD;
  char *session           = NULL;
  uint  hex_charset       = HEX_CHARSET;
  uint  hex_salt          = HEX_SALT;
  uint  hex_wordlist      = HEX_WORDLIST;
  uint  rp_gen            = RP_GEN;
  uint  rp_gen_func_min   = RP_GEN_FUNC_MIN;
  uint  rp_gen_func_max   = RP_GEN_FUNC_MAX;
  uint  rp_gen_seed       = RP_GEN_SEED;
  char *rule_buf_l        = (char *) RULE_BUF_L;
  char *rule_buf_r        = (char *) RULE_BUF_R;
  uint  increment         = INCREMENT;
  uint  increment_min     = INCREMENT_MIN;
  uint  increment_max     = INCREMENT_MAX;
  char *cpu_affinity      = NULL;
  uint  gpu_async         = GPU_ASYNC;
  char *gpu_devices       = NULL;
  char *truecrypt_keyfiles = NULL;
  uint  workload_profile  = WORKLOAD_PROFILE;
  uint  gpu_accel         = GPU_ACCEL;
  uint  gpu_loops         = GPU_LOOPS;
  uint  gpu_temp_disable  = GPU_TEMP_DISABLE;
  uint  gpu_temp_abort    = GPU_TEMP_ABORT;
  uint  gpu_temp_retain   = GPU_TEMP_RETAIN;
  uint  powertune_enable  = POWERTUNE_ENABLE;
  uint  logfile_disable   = LOGFILE_DISABLE;
  uint  segment_size      = SEGMENT_SIZE;
  uint  scrypt_tmto       = SCRYPT_TMTO;
  char  separator         = SEPARATOR;
  uint  bitmap_min        = BITMAP_MIN;
  uint  bitmap_max        = BITMAP_MAX;
  char *custom_charset_1  = NULL;
  char *custom_charset_2  = NULL;
  char *custom_charset_3  = NULL;
  char *custom_charset_4  = NULL;

  #define IDX_HELP              'h'
  #define IDX_VERSION           'V'
  #define IDX_VERSION_LOWER     'v'
  #define IDX_QUIET             0xff02
  #define IDX_SHOW              0xff03
  #define IDX_LEFT              0xff04
  #define IDX_REMOVE            0xff05
  #define IDX_REMOVE_TIMER      0xff37
  #define IDX_SKIP              's'
  #define IDX_LIMIT             'l'
  #define IDX_KEYSPACE          0xff35
  #define IDX_POTFILE_DISABLE   0xff06
  #define IDX_DEBUG_MODE        0xff43
  #define IDX_DEBUG_FILE        0xff44
  #define IDX_INDUCTION_DIR     0xff46
  #define IDX_OUTFILE_CHECK_DIR 0xff47
  #define IDX_USERNAME          0xff07
  #define IDX_FORCE             0xff08
  #define IDX_RUNTIME           0xff09
  #define IDX_BENCHMARK         'b'
  #define IDX_BENCHMARK_MODE    0xff32
  #define IDX_HASH_MODE         'm'
  #define IDX_ATTACK_MODE       'a'
  #define IDX_RP_FILE           'r'
  #define IDX_RP_GEN            'g'
  #define IDX_RP_GEN_FUNC_MIN   0xff10
  #define IDX_RP_GEN_FUNC_MAX   0xff11
  #define IDX_RP_GEN_SEED       0xff34
  #define IDX_RULE_BUF_L        'j'
  #define IDX_RULE_BUF_R        'k'
  #define IDX_INCREMENT         'i'
  #define IDX_INCREMENT_MIN     0xff12
  #define IDX_INCREMENT_MAX     0xff13
  #define IDX_OUTFILE           'o'
  #define IDX_OUTFILE_FORMAT    0xff14
  #define IDX_OUTFILE_AUTOHEX_DISABLE 0xff39
  #define IDX_OUTFILE_CHECK_TIMER 0xff45
  #define IDX_RESTORE           0xff15
  #define IDX_RESTORE_DISABLE   0xff27
  #define IDX_STATUS            0xff17
  #define IDX_STATUS_TIMER      0xff18
  #define IDX_STATUS_AUTOMAT    0xff50
  #define IDX_LOOPBACK          0xff38
  #define IDX_WEAK_HASH_THRESHOLD 0xff42
  #define IDX_SESSION           0xff19
  #define IDX_HEX_CHARSET       0xff20
  #define IDX_HEX_SALT          0xff21
  #define IDX_HEX_WORDLIST      0xff40
  #define IDX_MARKOV_DISABLE    0xff22
  #define IDX_MARKOV_CLASSIC    0xff23
  #define IDX_MARKOV_THRESHOLD  't'
  #define IDX_MARKOV_HCSTAT     0xff24
  #define IDX_CPU_AFFINITY      0xff25
  #define IDX_GPU_ASYNC         0xff26
  #define IDX_GPU_DEVICES       'd'
  #define IDX_WORKLOAD_PROFILE  'w'
  #define IDX_GPU_ACCEL         'n'
  #define IDX_GPU_LOOPS         'u'
  #define IDX_GPU_TEMP_DISABLE  0xff29
  #define IDX_GPU_TEMP_ABORT    0xff30
  #define IDX_GPU_TEMP_RETAIN   0xff31
  #define IDX_POWERTUNE_ENABLE  0xff41
  #define IDX_LOGFILE_DISABLE   0xff51
  #define IDX_TRUECRYPT_KEYFILES 0xff52
  #define IDX_SCRYPT_TMTO       0xff61
  #define IDX_SEGMENT_SIZE      'c'
  #define IDX_SEPARATOR         'p'
  #define IDX_BITMAP_MIN        0xff70
  #define IDX_BITMAP_MAX        0xff71
  #define IDX_CUSTOM_CHARSET_1  '1'
  #define IDX_CUSTOM_CHARSET_2  '2'
  #define IDX_CUSTOM_CHARSET_3  '3'
  #define IDX_CUSTOM_CHARSET_4  '4'

  char short_options[] = "hVvm:a:r:j:k:g:o:t:d:n:u:c:p:s:l:1:2:3:4:ibw:";

  struct option long_options[] =
  {
    {"help",              no_argument,       0, IDX_HELP},
    {"version",           no_argument,       0, IDX_VERSION},
    {"quiet",             no_argument,       0, IDX_QUIET},
    {"show",              no_argument,       0, IDX_SHOW},
    {"left",              no_argument,       0, IDX_LEFT},
    {"username",          no_argument,       0, IDX_USERNAME},
    {"remove",            no_argument,       0, IDX_REMOVE},
    {"remove-timer",      required_argument, 0, IDX_REMOVE_TIMER},
    {"skip",              required_argument, 0, IDX_SKIP},
    {"limit",             required_argument, 0, IDX_LIMIT},
    {"keyspace",          no_argument,       0, IDX_KEYSPACE},
    {"potfile-disable",   no_argument,       0, IDX_POTFILE_DISABLE},
    {"debug-mode",        required_argument, 0, IDX_DEBUG_MODE},
    {"debug-file",        required_argument, 0, IDX_DEBUG_FILE},
    {"induction-dir",     required_argument, 0, IDX_INDUCTION_DIR},
    {"outfile-check-dir", required_argument, 0, IDX_OUTFILE_CHECK_DIR},
    {"force",             no_argument,       0, IDX_FORCE},
    {"benchmark",         no_argument,       0, IDX_BENCHMARK},
    {"benchmark-mode",    required_argument, 0, IDX_BENCHMARK_MODE},
    {"restore",           no_argument,       0, IDX_RESTORE},
    {"restore-disable",   no_argument,       0, IDX_RESTORE_DISABLE},
    {"status",            no_argument,       0, IDX_STATUS},
    {"status-timer",      required_argument, 0, IDX_STATUS_TIMER},
    {"status-automat",    no_argument,       0, IDX_STATUS_AUTOMAT},
    {"loopback",          no_argument,       0, IDX_LOOPBACK},
    {"weak-hash-threshold",
                          required_argument, 0, IDX_WEAK_HASH_THRESHOLD},
    {"session",           required_argument, 0, IDX_SESSION},
    {"runtime",           required_argument, 0, IDX_RUNTIME},
    {"generate-rules",    required_argument, 0, IDX_RP_GEN},
    {"generate-rules-func-min",
                          required_argument, 0, IDX_RP_GEN_FUNC_MIN},
    {"generate-rules-func-max",
                          required_argument, 0, IDX_RP_GEN_FUNC_MAX},
    {"generate-rules-seed",
                          required_argument, 0, IDX_RP_GEN_SEED},
    {"rule-left",         required_argument, 0, IDX_RULE_BUF_L},
    {"rule-right",        required_argument, 0, IDX_RULE_BUF_R},
    {"hash-type",         required_argument, 0, IDX_HASH_MODE},
    {"attack-mode",       required_argument, 0, IDX_ATTACK_MODE},
    {"rules-file",        required_argument, 0, IDX_RP_FILE},
    {"outfile",           required_argument, 0, IDX_OUTFILE},
    {"outfile-format",    required_argument, 0, IDX_OUTFILE_FORMAT},
    {"outfile-autohex-disable",
                          no_argument,       0, IDX_OUTFILE_AUTOHEX_DISABLE},
    {"outfile-check-timer",
                          required_argument, 0, IDX_OUTFILE_CHECK_TIMER},
    {"hex-charset",       no_argument,       0, IDX_HEX_CHARSET},
    {"hex-salt",          no_argument,       0, IDX_HEX_SALT},
    {"hex-wordlist",      no_argument,       0, IDX_HEX_WORDLIST},
    {"markov-disable",    no_argument,       0, IDX_MARKOV_DISABLE},
    {"markov-classic",    no_argument,       0, IDX_MARKOV_CLASSIC},
    {"markov-threshold",  required_argument, 0, IDX_MARKOV_THRESHOLD},
    {"markov-hcstat",     required_argument, 0, IDX_MARKOV_HCSTAT},
    {"cpu-affinity",      required_argument, 0, IDX_CPU_AFFINITY},
    {"gpu-async",         no_argument,       0, IDX_GPU_ASYNC},
    {"gpu-devices",       required_argument, 0, IDX_GPU_DEVICES},
    {"workload-profile",  required_argument, 0, IDX_WORKLOAD_PROFILE},
    {"gpu-accel",         required_argument, 0, IDX_GPU_ACCEL},
    {"gpu-loops",         required_argument, 0, IDX_GPU_LOOPS},
    {"gpu-temp-disable",  no_argument,       0, IDX_GPU_TEMP_DISABLE},
    {"gpu-temp-abort",    required_argument, 0, IDX_GPU_TEMP_ABORT},
    {"gpu-temp-retain",   required_argument, 0, IDX_GPU_TEMP_RETAIN},
    {"powertune-enable",  no_argument,       0, IDX_POWERTUNE_ENABLE},
    {"logfile-disable",   no_argument,       0, IDX_LOGFILE_DISABLE},
    {"truecrypt-keyfiles", required_argument, 0, IDX_TRUECRYPT_KEYFILES},
    {"segment-size",      required_argument, 0, IDX_SEGMENT_SIZE},
    {"scrypt-tmto",       required_argument, 0, IDX_SCRYPT_TMTO},
    // deprecated
    {"seperator",         required_argument, 0, IDX_SEPARATOR},
    {"separator",         required_argument, 0, IDX_SEPARATOR},
    {"bitmap-min",        required_argument, 0, IDX_BITMAP_MIN},
    {"bitmap-max",        required_argument, 0, IDX_BITMAP_MAX},
    {"increment",         no_argument,       0, IDX_INCREMENT},
    {"increment-min",     required_argument, 0, IDX_INCREMENT_MIN},
    {"increment-max",     required_argument, 0, IDX_INCREMENT_MAX},
    {"custom-charset1",   required_argument, 0, IDX_CUSTOM_CHARSET_1},
    {"custom-charset2",   required_argument, 0, IDX_CUSTOM_CHARSET_2},
    {"custom-charset3",   required_argument, 0, IDX_CUSTOM_CHARSET_3},
    {"custom-charset4",   required_argument, 0, IDX_CUSTOM_CHARSET_4},

    {0, 0, 0, 0}
  };

  uint rp_files_cnt = 0;

  char **rp_files = (char **) mycalloc (argc, sizeof (char *));

  int option_index;
  int c;

  optind = 1;
  optopt = 0;
  option_index = 0;

  while (((c = getopt_long (argc, argv, short_options, long_options, &option_index)) != -1) && optopt == 0)
  {
    switch (c)
    {
      case IDX_HELP:          usage   = 1;      break;
      case IDX_VERSION:
      case IDX_VERSION_LOWER: version = 1;      break;
      case IDX_RESTORE:       restore = 1;      break;
      case IDX_SESSION:       session = optarg; break;
      case IDX_SHOW:          show    = 1;      break;
      case IDX_LEFT:          left    = 1;      break;
      case '?':               return (-1);
    }
  }

  if (optopt != 0)
  {
    log_error ("ERROR: Invalid argument specified");

    return (-1);
  }

  /**
   * exit functions
   */

  if (version)
  {
    log_info (VERSION_TXT);

    return (0);
  }

  if (usage)
  {
    usage_big_print (PROGNAME);

    return (0);
  }

  /**
   * session
   */

  if (session == NULL) session = (char *) PROGNAME;

  size_t session_size = strlen (session) + 32;

  data.session = session;

  char *eff_restore_file = (char *) mymalloc (session_size);
  char *new_restore_file = (char *) mymalloc (session_size);

  snprintf (eff_restore_file, session_size - 1, "%s.restore", session);
  snprintf (new_restore_file, session_size - 1, "%s.restore.new", session);

  data.eff_restore_file = eff_restore_file;
  data.new_restore_file = new_restore_file;

  if (((show == 1) || (left == 1)) && (restore == 1))
  {
    if (show == 1) log_error ("ERROR: Mixing --restore parameter and --show is not supported");
    else           log_error ("ERROR: Mixing --restore parameter and --left is not supported");

    return (-1);
  }

  // this allows the user to use --show and --left while cracking (i.e. while another instance of oclHashcat is running)
  if ((show == 1) || (left == 1))
  {
    restore_disable = 1;

    restore = 0;
  }

  data.restore_disable = restore_disable;

  restore_data_t *rd = init_restore (argc, argv);

  data.rd = rd;

  /**
   * restore file
   */

  if (restore == 1)
  {
    read_restore (eff_restore_file, rd);

    if (rd->version_bin < RESTORE_MIN)
    {
      log_error ("ERROR: Incompatible restore-file version");

      return (-1);
    }

    myargc = rd->argc;
    myargv = rd->argv;

    #ifdef _POSIX
    rd->pid = getpid ();
    #elif _WIN
    rd->pid = GetCurrentProcessId ();
    #endif
  }

  uint hash_mode_chgd       = 0;
  uint runtime_chgd         = 0;
  uint gpu_loops_chgd       = 0;
  uint gpu_accel_chgd       = 0;
  uint attack_mode_chgd     = 0;
  uint outfile_format_chgd  = 0;
  uint rp_gen_seed_chgd     = 0;
  uint remove_timer_chgd    = 0;
  uint increment_min_chgd   = 0;
  uint increment_max_chgd   = 0;

  #if _OCL
  uint gpu_temp_abort_chgd  = 0;
  uint gpu_temp_retain_chgd = 0;
  #endif

  optind = 1;
  optopt = 0;
  option_index = 0;

  while (((c = getopt_long (myargc, myargv, short_options, long_options, &option_index)) != -1) && optopt == 0)
  {
    switch (c)
    {
    //case IDX_HELP:              usage             = 1;               break;
    //case IDX_VERSION:           version           = 1;               break;
    //case IDX_RESTORE:           restore           = 1;               break;
      case IDX_QUIET:             quiet             = 1;               break;
    //case IDX_SHOW:              show              = 1;               break;
      case IDX_SHOW:                                                   break;
    //case IDX_LEFT:              left              = 1;               break;
      case IDX_LEFT:                                                   break;
      case IDX_USERNAME:          username          = 1;               break;
      case IDX_REMOVE:            remove            = 1;               break;
      case IDX_REMOVE_TIMER:      remove_timer      = atoi (optarg);
                                  remove_timer_chgd = 1;               break;
      case IDX_POTFILE_DISABLE:   potfile_disable   = 1;               break;
      case IDX_DEBUG_MODE:        debug_mode        = atoi (optarg);   break;
      case IDX_DEBUG_FILE:        debug_file        = optarg;          break;
      case IDX_INDUCTION_DIR:     induction_dir     = optarg;          break;
      case IDX_OUTFILE_CHECK_DIR: outfile_check_dir = optarg;          break;
      case IDX_FORCE:             force             = 1;               break;
      case IDX_SKIP:              skip              = atoll (optarg);  break;
      case IDX_LIMIT:             limit             = atoll (optarg);  break;
      case IDX_KEYSPACE:          keyspace          = 1;               break;
      case IDX_BENCHMARK:         benchmark         = 1;               break;
      case IDX_BENCHMARK_MODE:    benchmark_mode    = atoi (optarg);   break;
      case IDX_RESTORE:                                                break;
      case IDX_RESTORE_DISABLE:   restore_disable   = 1;               break;
      case IDX_STATUS:            status            = 1;               break;
      case IDX_STATUS_TIMER:      status_timer      = atoi (optarg);   break;
      case IDX_STATUS_AUTOMAT:    status_automat    = 1;               break;
      case IDX_LOOPBACK:          loopback          = 1;               break;
      case IDX_WEAK_HASH_THRESHOLD:
                                  weak_hash_threshold = atoi (optarg); break;
    //case IDX_SESSION:           session           = optarg;          break;
      case IDX_SESSION:                                                break;
      case IDX_HASH_MODE:         hash_mode         = atoi (optarg);
                                  hash_mode_chgd    = 1;               break;
      case IDX_RUNTIME:           runtime           = atoi (optarg);
                                  runtime_chgd      = 1;               break;
      case IDX_ATTACK_MODE:       attack_mode       = atoi (optarg);
                                  attack_mode_chgd  = 1;               break;
      case IDX_RP_FILE:           rp_files[rp_files_cnt++] = optarg;   break;
      case IDX_RP_GEN:            rp_gen            = atoi (optarg);   break;
      case IDX_RP_GEN_FUNC_MIN:   rp_gen_func_min   = atoi (optarg);   break;
      case IDX_RP_GEN_FUNC_MAX:   rp_gen_func_max   = atoi (optarg);   break;
      case IDX_RP_GEN_SEED:       rp_gen_seed       = atoi (optarg);
                                  rp_gen_seed_chgd  = 1;               break;
      case IDX_RULE_BUF_L:        rule_buf_l        = optarg;          break;
      case IDX_RULE_BUF_R:        rule_buf_r        = optarg;          break;
      case IDX_MARKOV_DISABLE:    markov_disable    = 1;               break;
      case IDX_MARKOV_CLASSIC:    markov_classic    = 1;               break;
      case IDX_MARKOV_THRESHOLD:  markov_threshold  = atoi (optarg);   break;
      case IDX_MARKOV_HCSTAT:     markov_hcstat     = optarg;          break;
      case IDX_OUTFILE:           outfile           = optarg;          break;
      case IDX_OUTFILE_FORMAT:    outfile_format    = atoi (optarg);
                                  outfile_format_chgd = 1;             break;
      case IDX_OUTFILE_AUTOHEX_DISABLE:
                                  outfile_autohex   = 0;               break;
      case IDX_OUTFILE_CHECK_TIMER:
                                  outfile_check_timer = atoi (optarg); break;
      case IDX_HEX_CHARSET:       hex_charset       = 1;               break;
      case IDX_HEX_SALT:          hex_salt          = 1;               break;
      case IDX_HEX_WORDLIST:      hex_wordlist      = 1;               break;
      case IDX_CPU_AFFINITY:      cpu_affinity      = optarg;          break;
      case IDX_GPU_ASYNC:         gpu_async         = 1;               break;
      case IDX_GPU_DEVICES:       gpu_devices       = optarg;          break;
      case IDX_WORKLOAD_PROFILE:  workload_profile  = atoi (optarg);   break;
      case IDX_GPU_ACCEL:         gpu_accel         = atoi (optarg);
                                  gpu_accel_chgd    = 1;               break;
      case IDX_GPU_LOOPS:         gpu_loops         = atoi (optarg);
                                  gpu_loops_chgd    = 1;               break;
      case IDX_GPU_TEMP_DISABLE:  gpu_temp_disable  = 1;               break;
      case IDX_GPU_TEMP_ABORT:
                                  #if _OCL
                                  gpu_temp_abort_chgd = 1;
                                  #endif
                                  gpu_temp_abort    = atoi (optarg);   break;
      case IDX_GPU_TEMP_RETAIN:
                                  #if _OCL
                                  gpu_temp_retain_chgd = 1;
                                  #endif
                                  gpu_temp_retain   = atoi (optarg);   break;
      case IDX_POWERTUNE_ENABLE:  powertune_enable  = 1;               break;
      case IDX_LOGFILE_DISABLE:   logfile_disable   = 1;               break;
      case IDX_TRUECRYPT_KEYFILES: truecrypt_keyfiles = optarg;        break;
      case IDX_SEGMENT_SIZE:      segment_size      = atoi (optarg);   break;
      case IDX_SCRYPT_TMTO:       scrypt_tmto       = atoi (optarg);   break;
      case IDX_SEPARATOR:         separator         = optarg[0];       break;
      case IDX_BITMAP_MIN:        bitmap_min        = atoi (optarg);   break;
      case IDX_BITMAP_MAX:        bitmap_max        = atoi (optarg);   break;
      case IDX_INCREMENT:         increment         = 1;               break;
      case IDX_INCREMENT_MIN:     increment_min      = atoi (optarg);
                                  increment_min_chgd = 1;              break;
      case IDX_INCREMENT_MAX:     increment_max      = atoi (optarg);
                                  increment_max_chgd = 1;              break;
      case IDX_CUSTOM_CHARSET_1:  custom_charset_1  = optarg;          break;
      case IDX_CUSTOM_CHARSET_2:  custom_charset_2  = optarg;          break;
      case IDX_CUSTOM_CHARSET_3:  custom_charset_3  = optarg;          break;
      case IDX_CUSTOM_CHARSET_4:  custom_charset_4  = optarg;          break;

      default:
        log_error ("ERROR: Invalid argument specified");
        return (-1);
    }
  }

  if (optopt != 0)
  {
    log_error ("ERROR: Invalid argument specified");

    return (-1);
  }

  /**
   * Inform user things getting started,
   * - this is giving us a visual header before preparations start, so we do not need to clear them afterwards
   * - we do not need to check algorithm_pos
   */

  if (quiet == 0)
  {
    if (benchmark == 1)
    {
      log_info ("%s v%.2f starting in benchmark-mode...", PROGNAME, (float) VERSION_BIN / 100);

      log_info ("");
    }
    else if (restore == 1)
    {
      log_info ("%s v%.2f starting in restore-mode...", PROGNAME, (float) VERSION_BIN / 100);

      log_info ("");
    }
    else
    {
      log_info ("%s v%.2f starting...", PROGNAME, (float) VERSION_BIN / 100);

      log_info ("");
    }
  }

  /**
   * sanity check
   */

  if (attack_mode > 7)
  {
    log_error ("ERROR: Invalid attack-mode specified");

    return (-1);
  }

  if (runtime_chgd && runtime == 0) // just added to remove compiler warnings for runtime_chgd
  {
    log_error ("ERROR: Invalid runtime specified");

    return (-1);
  }

  if (hash_mode_chgd && hash_mode > 12800) // just added to remove compiler warnings for hash_mode_chgd
  {
    log_error ("ERROR: Invalid hash-type specified");

    return (-1);
  }

  // renamed hash modes

  if (hash_mode_chgd)
  {
    int n = -1;

    switch (hash_mode)
    {
      case 123: n = 124;
                break;
    }

    if (n >= 0)
    {
      log_error ("Old -m specified, use -m %d instead", n);

      return (-1);
    }
  }

  if (username == 1)
  {
    if ((hash_mode == 2500) || (hash_mode == 5200) || ((hash_mode >= 6200) && (hash_mode <= 6299)))
    {
      log_error ("ERROR: Mixing support for user names and hashes of type %s is not supported", strhashtype (hash_mode));

      return (-1);
    }
  }

  if (outfile_format > 16)
  {
    log_error ("ERROR: Invalid outfile-format specified");

    return (-1);
  }

  if (left == 1)
  {
    if (outfile_format_chgd == 1)
    {
      if (outfile_format > 1)
      {
        log_error ("ERROR: Mixing outfile-format > 1 is not allowed together with left parameter");

        return (-1);
      }
    }
    else
    {
      outfile_format = OUTFILE_FMT_HASH;
    }
  }

  if (show == 1)
  {
    if (outfile_format_chgd == 1)
    {
      if ((outfile_format > 7) && (outfile_format < 16))
      {
        log_error ("ERROR: Mixing outfile-format > 7 is not allowed together with show parameter");

        return (-1);
      }
    }
  }

  if (increment_min < INCREMENT_MIN)
  {
    log_error ("ERROR: Invalid increment-min specified");

    return (-1);
  }

  if (increment_max > INCREMENT_MAX)
  {
    log_error ("ERROR: Invalid increment-max specified");

    return (-1);
  }

  if (increment_min > increment_max)
  {
    log_error ("ERROR: Invalid increment-min specified");

    return (-1);
  }

  if ((increment == 1) && (attack_mode == ATTACK_MODE_STRAIGHT))
  {
    log_error ("ERROR: increment is not allowed in attack-mode 0");

    return (-1);
  }

  if ((increment == 0) && (increment_min_chgd == 1))
  {
    log_error ("ERROR: increment-min is only supported together with increment switch");

    return (-1);
  }

  if ((increment == 0) && (increment_max_chgd == 1))
  {
    log_error ("ERROR: increment-max is only supported together with increment switch");

    return (-1);
  }

  if (rp_files_cnt && rp_gen)
  {
    log_error ("ERROR: Use of both rules-file and rules-generate is not supported");

    return (-1);
  }

  if (rp_files_cnt || rp_gen)
  {
    if (attack_mode != ATTACK_MODE_STRAIGHT)
    {
      log_error ("ERROR: Use of rules-file or rules-generate only allowed in attack-mode 0");

      return (-1);
    }
  }

  if (rp_gen_func_min > rp_gen_func_max)
  {
    log_error ("ERROR: Invalid rp-gen-func-min specified");

    return (-1);
  }

  if (gpu_accel_chgd == 1)
  {
    if (workload_profile != WORKLOAD_PROFILE)
    {
      log_error ("ERROR: gpu-accel parameter can only be set when workload-profile %i is used", WORKLOAD_PROFILE);

      return (-1);
    }

    if (gpu_accel < 1)
    {
      log_error ("ERROR: Invalid gpu-accel specified");

      return (-1);
    }

    if (gpu_accel > 800)
    {
      log_error ("ERROR: Invalid gpu-accel specified");

      return (-1);
    }
  }

  if (gpu_loops_chgd == 1)
  {
    if (workload_profile != WORKLOAD_PROFILE)
    {
      log_error ("ERROR: gpu-loops parameter can only be set when workload-profile %i is used", WORKLOAD_PROFILE);

      return (-1);
    }

    if (gpu_loops < 1)
    {
      log_error ("ERROR: Invalid gpu-loops specified");

      return (-1);
    }

    if (gpu_loops > 1024)
    {
      log_error ("ERROR: Invalid gpu-loops specified");

      return (-1);
    }
  }

  if (benchmark == 1)
  {
    if (workload_profile != WORKLOAD_PROFILE)
    {
      log_error ("ERROR: Using the workload-profile in benchmark mode is not allowed");

      return (-1);
    }
  }

  if ((workload_profile < 1) || (workload_profile > 3))
  {
    log_error ("ERROR: workload-profile %i not available", workload_profile);

    return (-1);
  }

  if (show == 1 || left == 1)
  {
    attack_mode = ATTACK_MODE_NONE;

    if (remove == 1)
    {
      log_error ("ERROR: Mixing remove parameter not allowed with show parameter or left parameter");

      return (-1);
    }

    if (potfile_disable == 1)
    {
      log_error ("ERROR: Mixing potfile-disable parameter not allowed with show parameter or left parameter");

      return (-1);
    }
  }

  uint attack_kern = ATTACK_KERN_NONE;

  switch (attack_mode)
  {
    case ATTACK_MODE_STRAIGHT: attack_kern = ATTACK_KERN_STRAIGHT; break;
    case ATTACK_MODE_COMBI:    attack_kern = ATTACK_KERN_COMBI;    break;
    case ATTACK_MODE_BF:       attack_kern = ATTACK_KERN_BF;       break;
    case ATTACK_MODE_HYBRID1:  attack_kern = ATTACK_KERN_COMBI;    break;
    case ATTACK_MODE_HYBRID2:  attack_kern = ATTACK_KERN_COMBI;    break;
  }

  if (benchmark == 0)
  {
    if (keyspace == 1)
    {
      int num_additional_params = 1;

      if (attack_kern == ATTACK_KERN_COMBI)
      {
        num_additional_params = 2;
      }

      int keyspace_wordlist_specified = myargc - optind - num_additional_params;

      if (keyspace_wordlist_specified == 0) optind--;
    }

    if (attack_kern == ATTACK_KERN_NONE)
    {
      if ((optind + 1) != myargc)
      {
        usage_mini_print (myargv[0]);

        return (-1);
      }
    }
    else if (attack_kern == ATTACK_KERN_STRAIGHT)
    {
      if ((optind + 1) > myargc)
      {
        usage_mini_print (myargv[0]);

        return (-1);
      }
    }
    else if (attack_kern == ATTACK_KERN_COMBI)
    {
      if ((optind + 3) != myargc)
      {
        usage_mini_print (myargv[0]);

        return (-1);
      }
    }
    else if (attack_kern == ATTACK_KERN_BF)
    {
      if ((optind + 1) > myargc)
      {
        usage_mini_print (myargv[0]);

        return (-1);
      }
    }
    else
    {
      usage_mini_print (myargv[0]);

      return (-1);
    }
  }
  else
  {
    if (myargv[optind] != 0)
    {
      log_error ("ERROR: Invalid argument for benchmark mode specified");

      return (-1);
    }

    if (attack_mode_chgd == 1)
    {
      if (attack_mode != ATTACK_MODE_BF)
      {
        log_error ("ERROR: Only attack-mode 3 allowed in benchmark mode");

        return (-1);
      }
    }

    if (benchmark_mode == 0)
    {
      // nothing to do
    }
    else if (benchmark_mode == 1)
    {
      if (gpu_accel_chgd == 1 || gpu_loops_chgd == 1)
      {
        log_error ("ERROR: Benchmark-mode 1 does not allow gpu-accel or gpu-loops changed");

        return (-1);
      }
    }
    else
    {
      log_error ("ERROR: Benchmark-mode must be 0 or 1");

      return (-1);
    }
  }

  if (skip != 0 && limit != 0)
  {
    limit += skip;
  }

  if (keyspace == 1)
  {
    if (show == 1)
    {
      log_error ("ERROR: Mixing show parameter not supported with keyspace parameter");

      return (-1);
    }
    else if (left == 1)
    {
      log_error ("ERROR: Mixing left parameter not supported wiht keyspace parameter");

      return (-1);
    }

    potfile_disable = 1;

    restore_disable = 1;

    restore = 0;

    weak_hash_threshold = 0;

    quiet = 1;
  }

  if (remove_timer_chgd == 1)
  {
    if (remove == 0)
    {
      log_error ("ERROR: Parameter remove-timer require parameter remove enabled");

      return (-1);
    }

    if (remove_timer < 1)
    {
      log_error ("ERROR: Parameter remove-timer must have a value greater than or equal to 1");

      return (-1);
    }
  }

  if (loopback == 1)
  {
    if (attack_mode == ATTACK_MODE_BF)
    {
      log_error ("ERROR: Parameter loopback not allowed in attack-mode 3");

      return (-1);
    }
    else if (attack_mode == ATTACK_MODE_STRAIGHT)
    {
      if ((rp_files_cnt == 0) && (rp_gen == 0))
      {
        log_error ("ERROR: Parameter loopback not allowed without rules-file or rules-generate");

        return (-1);
      }
    }
  }

  if (debug_mode > 0)
  {
    if (attack_mode != ATTACK_MODE_STRAIGHT)
    {
      log_error ("ERROR: Parameter debug-mode option is only available with attack-mode 0");

      return (-1);
    }

    if ((rp_files_cnt == 0) && (rp_gen == 0))
    {
      log_error ("ERROR: Parameter debug-mode not allowed without rules-file or rules-generate");

      return (-1);
    }
  }

  if (debug_mode > 4)
  {
    log_error ("ERROR: Invalid debug-mode specified");

    return (-1);
  }

  if (debug_file != NULL)
  {
    if (debug_mode < 1)
    {
      log_error ("ERROR: Parameter debug-file requires parameter debug-mode to be set");

      return (-1);
    }
  }

  if (induction_dir != NULL)
  {
    if (attack_mode == ATTACK_MODE_BF)
    {
      log_error ("ERROR: Parameter induction-dir not allowed with brute-force attacks");

      return (-1);
    }
  }

  /**
   * induction directory
   */

  char *induction_directory = NULL;

  if (attack_mode != ATTACK_MODE_BF)
  {
    if (induction_dir == NULL)
    {
      induction_directory = (char *) mymalloc (session_size);

      snprintf (induction_directory, session_size - 1, "%s.%s", session, INDUCT_DIR);

      // create induction folder if it does not already exist

      if (keyspace == 0)
      {
        if (rmdir (induction_directory) == -1)
        {
          if (errno == ENOENT)
          {
            // good, we can ignore
          }
          else if (errno == ENOTEMPTY)
          {
            char *induction_directory_mv = (char *) mymalloc (session_size);

            snprintf (induction_directory_mv, session_size - 1, "%s.induct.%d", session, (int) proc_start);

            if (rename (induction_directory, induction_directory_mv) != 0)
            {
              log_error ("ERROR: Rename directory %s to %s: %s", induction_directory, induction_directory_mv, strerror (errno));

              return (-1);
            }
          }
          else
          {
            log_error ("ERROR: %s: %s", induction_directory, strerror (errno));

            return (-1);
          }
        }

        #ifdef _WIN
        #define mkdir(name,mode) mkdir (name)
        #endif

        if (mkdir (induction_directory, 0700) == -1)
        {
          log_error ("ERROR: %s: %s", induction_directory, strerror (errno));

          return (-1);
        }
      }
    }
    else
    {
      induction_directory = induction_dir;
    }
  }

  data.induction_directory = induction_directory;

  /**
   * loopback
   */

  size_t loopback_size = session_size + strlen (LOOPBACK_FILE) + 12;

  char *loopback_file = (char *) mymalloc (loopback_size);

  /**
   * outfile-check directory
   */

  char *outfile_check_directory = NULL;

  if (outfile_check_dir == NULL)
  {
    outfile_check_directory = (char *) mymalloc (session_size);

    snprintf (outfile_check_directory, session_size - 1, "%s.%s", session, OUTFILES_DIR);
  }
  else
  {
    outfile_check_directory = outfile_check_dir;
  }

  data.outfile_check_directory = outfile_check_directory;

  if (keyspace == 0)
  {
    struct stat outfile_check_stat;

    if (stat (outfile_check_directory, &outfile_check_stat) == 0)
    {
      uint is_dir = S_ISDIR (outfile_check_stat.st_mode);

      if (is_dir == 0)
      {
        log_error ("ERROR: Directory specified in outfile-check '%s' is not a valid directory", outfile_check_directory);

        return (-1);
      }
    }
    else if (outfile_check_dir == NULL)
    {
      #ifdef _WIN
      #define mkdir(name,mode) mkdir (name)
      #endif

      if (mkdir (outfile_check_directory, 0700) == -1)
      {
        log_error ("ERROR: %s: %s", outfile_check_directory, strerror (errno));

        return (-1);
      }
    }
  }

  /**
   * special other stuff
   */

  if (hash_mode == 9710)
  {
    outfile_format      = 5;
    outfile_format_chgd = 1;
  }

  if (hash_mode == 9810)
  {
    outfile_format      = 5;
    outfile_format_chgd = 1;
  }

  if (hash_mode == 10410)
  {
    outfile_format      = 5;
    outfile_format_chgd = 1;
  }

  /**
   * store stuff
   */

  data.hash_mode         = hash_mode;
  data.restore           = restore;
  data.restore_timer     = restore_timer;
  data.restore_disable   = restore_disable;
  data.status            = status;
  data.status_timer      = status_timer;
  data.status_automat    = status_automat;
  data.loopback          = loopback;
  data.runtime           = runtime;
  data.remove            = remove;
  data.remove_timer      = remove_timer;
  data.debug_mode        = debug_mode;
  data.debug_file        = debug_file;
  data.username          = username;
  data.quiet             = quiet;
  data.outfile           = outfile;
  data.outfile_format    = outfile_format;
  data.outfile_autohex   = outfile_autohex;
  data.hex_charset       = hex_charset;
  data.hex_salt          = hex_salt;
  data.hex_wordlist      = hex_wordlist;
  data.separator         = separator;
  data.rp_files          = rp_files;
  data.rp_files_cnt      = rp_files_cnt;
  data.rp_gen            = rp_gen;
  data.rp_gen_seed       = rp_gen_seed;
  data.force             = force;
  data.benchmark         = benchmark;
  data.skip              = skip;
  data.limit             = limit;
  data.powertune_enable  = powertune_enable;
  data.logfile_disable   = logfile_disable;
  data.truecrypt_keyfiles = truecrypt_keyfiles;
  data.scrypt_tmto       = scrypt_tmto;

  /**
   * install_dir
   */

  char *install_dir = get_install_dir (myargv[0]);

  data.install_dir = install_dir;

  /**
   * cpu affinity
   */

  if (cpu_affinity)
  {
    set_cpu_affinity (cpu_affinity);
  }

  if (rp_gen_seed_chgd == 0)
  {
    srand (proc_start);
  }
  else
  {
    srand (rp_gen_seed);
  }

  /**
   * logfile init
   */

  if (logfile_disable == 0)
  {
    size_t logfile_size = strlen (session) + 32;

    char *logfile = (char *) mymalloc (logfile_size);

    snprintf (logfile, logfile_size - 1, "%s.log", session);

    data.logfile = logfile;

    char *topid = logfile_generate_topid ();

    data.topid = topid;
  }

  // logfile_append() checks for logfile_disable internally to make it easier from here

  #define logfile_top_msg(msg)            logfile_append ("%s\t%s",           data.topid,             (msg));
  #define logfile_sub_msg(msg)            logfile_append ("%s\t%s\t%s",       data.topid, data.subid, (msg));
  #define logfile_top_var_uint64(var,val) logfile_append ("%s\t%s\t%llu",     data.topid,             (var), (val));
  #define logfile_sub_var_uint64(var,val) logfile_append ("%s\t%s\t%s\t%llu", data.topid, data.subid, (var), (val));
  #define logfile_top_var_uint(var,val)   logfile_append ("%s\t%s\t%u",       data.topid,             (var), (val));
  #define logfile_sub_var_uint(var,val)   logfile_append ("%s\t%s\t%s\t%u",   data.topid, data.subid, (var), (val));
  #define logfile_top_var_char(var,val)   logfile_append ("%s\t%s\t%c",       data.topid,             (var), (val));
  #define logfile_sub_var_char(var,val)   logfile_append ("%s\t%s\t%s\t%c",   data.topid, data.subid, (var), (val));
  #define logfile_top_var_string(var,val) if ((val) != NULL) logfile_append ("%s\t%s\t%s",       data.topid,             (var), (val));
  #define logfile_sub_var_string(var,val) if ((val) != NULL) logfile_append ("%s\t%s\t%s\t%s",   data.topid, data.subid, (var), (val));

  #define logfile_top_uint64(var)         logfile_top_var_uint64 (#var, (var));
  #define logfile_sub_uint64(var)         logfile_sub_var_uint64 (#var, (var));
  #define logfile_top_uint(var)           logfile_top_var_uint   (#var, (var));
  #define logfile_sub_uint(var)           logfile_sub_var_uint   (#var, (var));
  #define logfile_top_char(var)           logfile_top_var_char   (#var, (var));
  #define logfile_sub_char(var)           logfile_sub_var_char   (#var, (var));
  #define logfile_top_string(var)         logfile_top_var_string (#var, (var));
  #define logfile_sub_string(var)         logfile_sub_var_string (#var, (var));

  logfile_top_msg ("START");

  logfile_top_uint   (attack_mode);
  logfile_top_uint   (attack_kern);
  logfile_top_uint   (benchmark);
  logfile_top_uint   (benchmark_mode);
  logfile_top_uint   (bitmap_min);
  logfile_top_uint   (bitmap_max);
  logfile_top_uint   (debug_mode);
  logfile_top_uint   (force);
  logfile_top_uint   (gpu_accel);
  logfile_top_uint   (gpu_async);
  logfile_top_uint   (gpu_loops);
  logfile_top_uint   (gpu_temp_abort);
  logfile_top_uint   (gpu_temp_disable);
  logfile_top_uint   (gpu_temp_retain);
  logfile_top_uint   (hash_mode);
  logfile_top_uint   (hex_charset);
  logfile_top_uint   (hex_salt);
  logfile_top_uint   (hex_wordlist);
  logfile_top_uint   (increment);
  logfile_top_uint   (increment_max);
  logfile_top_uint   (increment_min);
  logfile_top_uint   (keyspace);
  logfile_top_uint   (left);
  logfile_top_uint   (logfile_disable);
  logfile_top_uint   (loopback);
  logfile_top_uint   (markov_classic);
  logfile_top_uint   (markov_disable);
  logfile_top_uint   (markov_threshold);
  logfile_top_uint   (outfile_autohex);
  logfile_top_uint   (outfile_check_timer);
  logfile_top_uint   (outfile_format);
  logfile_top_uint   (potfile_disable);
  logfile_top_uint   (powertune_enable);
  logfile_top_uint   (scrypt_tmto);
  logfile_top_uint   (quiet);
  logfile_top_uint   (remove);
  logfile_top_uint   (remove_timer);
  logfile_top_uint   (restore);
  logfile_top_uint   (restore_disable);
  logfile_top_uint   (restore_timer);
  logfile_top_uint   (rp_gen);
  logfile_top_uint   (rp_gen_func_max);
  logfile_top_uint   (rp_gen_func_min);
  logfile_top_uint   (rp_gen_seed);
  logfile_top_uint   (runtime);
  logfile_top_uint   (segment_size);
  logfile_top_uint   (show);
  logfile_top_uint   (status);
  logfile_top_uint   (status_automat);
  logfile_top_uint   (status_timer);
  logfile_top_uint   (usage);
  logfile_top_uint   (username);
  logfile_top_uint   (version);
  logfile_top_uint   (weak_hash_threshold);
  logfile_top_uint   (workload_profile);
  logfile_top_uint64 (limit);
  logfile_top_uint64 (skip);
  logfile_top_char   (separator);
  logfile_top_string (cpu_affinity);
  logfile_top_string (custom_charset_1);
  logfile_top_string (custom_charset_2);
  logfile_top_string (custom_charset_3);
  logfile_top_string (custom_charset_4);
  logfile_top_string (debug_file);
  logfile_top_string (gpu_devices);
  logfile_top_string (induction_dir);
  logfile_top_string (markov_hcstat);
  logfile_top_string (outfile);
  logfile_top_string (outfile_check_dir);
  logfile_top_string (rule_buf_l);
  logfile_top_string (rule_buf_r);
  logfile_top_string (session);
  logfile_top_string (truecrypt_keyfiles);

  /**
   * devices
   */

  uint gpu_devicemask = devices_to_devicemask (gpu_devices);

  /**
   * benchmark
   */

  if (benchmark == 1)
  {
    /**
     * disable useless stuff for benchmark
     */

    restore_timer    = 0;
    status_timer     = 0;
    restore_disable  = 1;
    potfile_disable  = 1;
    weak_hash_threshold = 0;

    data.restore_timer   = restore_timer;
    data.status_timer    = status_timer;
    data.restore_disable = restore_disable;

    if (benchmark_mode == 1)
    {
      markov_disable   = 1;
    }

    /**
     * force attack mode to be bruteforce
     */

    attack_mode = ATTACK_MODE_BF;
    attack_kern = ATTACK_KERN_BF;

    if (runtime_chgd == 0)
    {
      runtime =  4;

      if (benchmark_mode == 1) runtime = 17;

      data.runtime = runtime;
    }
  }

  /**
   * config
   */

  uint hash_type   = 0;
  uint salt_type   = 0;
  uint attack_exec = 0;
  uint opts_type   = 0;
  uint kern_type   = 0;
  uint dgst_size   = 0;
  uint esalt_size  = 0;
  uint opti_type   = 0;
  uint dgst_pos0   = -1;
  uint dgst_pos1   = -1;
  uint dgst_pos2   = -1;
  uint dgst_pos3   = -1;

  int (*parse_func) (char *, uint, hash_t *);
  int (*sort_by_digest) (const void *, const void *);

  uint algorithm_pos = 0;
  uint algorithm_max = 1;

  uint *algorithms = default_benchmark_algorithms;

  if (benchmark == 1 && hash_mode_chgd == 0) algorithm_max = NUM_DEFAULT_BENCHMARK_ALGORITHMS;

  for (algorithm_pos = 0; algorithm_pos < algorithm_max; algorithm_pos++)
  {
    /*
     * We need to reset 'rd' in benchmark mode otherwise when the user hits 'bypass'
     * the following algos are skipped entirely
     */

    if (algorithm_pos > 0)
    {
      local_free (rd);

      rd = init_restore (argc, argv);

      data.rd = rd;
    }

    /**
     * update hash_mode in case of multihash benchmark
     */

    if (benchmark == 1)
    {
      if (hash_mode_chgd == 0)
      {
        hash_mode = algorithms[algorithm_pos];

        data.hash_mode = hash_mode;
      }

      quiet = 1;

      data.quiet = quiet;
    }

    switch (hash_mode)
    {
      case     0:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_MEET_IN_MIDDLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    10:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_PWSLT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5s_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_MEET_IN_MIDDLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    11:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_PWSLT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = joomla_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_MEET_IN_MIDDLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    12:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_PWSLT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = postgresql_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_MEET_IN_MIDDLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    20:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_SLTPW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5s_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    21:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_SLTPW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = osc_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    22:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_SLTPW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = netscreen_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    23:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_SLTPW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = skype_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    30:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_PWUSLT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5s_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_MEET_IN_MIDDLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    40:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_PT_UNICODE;
                   kern_type   = KERN_TYPE_MD5_SLTPWU;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5s_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    50:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS14;
                   kern_type   = KERN_TYPE_HMACMD5_PW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = hmacmd5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case    60:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_HMACMD5_SLT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = hmacmd5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   100:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   101:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1b64_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   110:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1_PWSLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1s_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   111:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1_PWSLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1b64s_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   112:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_SHA1_PWSLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = oracles_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   120:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1_SLTPW;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1s_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   121:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15
                               | OPTS_TYPE_ST_LOWER;
                   kern_type   = KERN_TYPE_SHA1_SLTPW;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = smf_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   122:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_SHA1_SLTPW;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = osx1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   124:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1_SLTPW;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = djangosha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   130:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1_PWUSLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1s_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   131:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_PT_UPPER
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_SHA1_PWUSLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = mssql2000_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   132:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_SHA1_PWUSLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = mssql2005_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   133:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1_PWUSLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = peoplesoft_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   140:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15
                               | OPTS_TYPE_PT_UNICODE;
                   kern_type   = KERN_TYPE_SHA1_SLTPWU;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1s_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   141:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_BASE64;
                   kern_type   = KERN_TYPE_SHA1_SLTPWU;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = episerver_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   150:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_HMACSHA1_PW;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = hmacsha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   160:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_HMACSHA1_SLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = hmacsha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   190:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1_LINKEDIN;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1linkedin_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 4;
                   dgst_pos2   = 3;
                   dgst_pos3   = 2;
                   break;

      case   200:  hash_type   = HASH_TYPE_MYSQL;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = 0;
                   kern_type   = KERN_TYPE_MYSQL;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = mysql323_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_SCALAR_MODE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case   300:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_MYSQL41;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case   400:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_PHPASS;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = phpass_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case   500:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_MD5CRYPT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5crypt_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case   501:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_MD5CRYPT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = juniper_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case   900:  hash_type   = HASH_TYPE_MD4;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD4;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md4_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_MEET_IN_MIDDLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  1000:  hash_type   = HASH_TYPE_MD4;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_PT_UNICODE;
                   kern_type   = KERN_TYPE_MD4_PWU;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md4_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_MEET_IN_MIDDLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  1100:  hash_type   = HASH_TYPE_MD4;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_UNICODE
                               | OPTS_TYPE_ST_LOWER;
                   kern_type   = KERN_TYPE_MD44_PWUSLT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = dcc_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  1400:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA256;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = sha256_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1410:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA256_PWSLT;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = sha256s_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1420:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA256_SLTPW;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = sha256s_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1421:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA256_SLTPW;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = hmailserver_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1430:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA256_PWUSLT;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = sha256s_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1440:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15
                               | OPTS_TYPE_PT_UNICODE;
                   kern_type   = KERN_TYPE_SHA256_SLTPWU;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = sha256s_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1441:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_BASE64;
                   kern_type   = KERN_TYPE_SHA256_SLTPWU;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = episerver4_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1450:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_HMACSHA256_PW;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = hmacsha256_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1460:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_HMACSHA256_SLT;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = hmacsha256_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  1500:  hash_type   = HASH_TYPE_DESCRYPT;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_BITSLICE;
                   kern_type   = KERN_TYPE_DESCRYPT;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = descrypt_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_PRECOMPUTE_PERMUT;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  1600:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_APR1CRYPT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5apr1_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  1700:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA512;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha512_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1710:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA512_PWSLT;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha512s_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1711:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA512_PWSLT;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha512b64s_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1720:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA512_SLTPW;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha512s_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1722:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_SHA512_SLTPW;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = osx512_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1730:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA512_PWSLTU;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha512s_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1731:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_SHA512_PWSLTU;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = mssql2012_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1740:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15
                               | OPTS_TYPE_PT_UNICODE;
                   kern_type   = KERN_TYPE_SHA512_SLTPWU;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha512s_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1750:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_HMACSHA512_PW;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = hmacsha512_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1760:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_HMACSHA512_SLT;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = hmacsha512_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 14;
                   dgst_pos1   = 15;
                   dgst_pos2   = 6;
                   dgst_pos3   = 7;
                   break;

      case  1800:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_SHA512CRYPT;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha512crypt_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  2100:  hash_type   = HASH_TYPE_DCC2;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE  // should be OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_LOWER
                               | OPTS_TYPE_ST_UNICODE;
                   kern_type   = KERN_TYPE_DCC2;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = dcc2_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  2400:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_MD5PIX;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5pix_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  2410:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_MD5ASA;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5asa_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  2500:  hash_type   = HASH_TYPE_WPA;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_WPA;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = wpa_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  2600:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_VIRTUAL;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_MD55_PWSLT1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5md5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  2611:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_MD55_PWSLT1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = vb3_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  2612:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_MD55_PWSLT1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = phps_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  2711:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_MD55_PWSLT2;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = vb30_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  2811:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD55_SLTPW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = ipb2_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  3000:  hash_type   = HASH_TYPE_LM;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_UPPER
                               | OPTS_TYPE_PT_BITSLICE;
                   kern_type   = KERN_TYPE_LM;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = lm_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_PRECOMPUTE_PERMUT;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  3100:  hash_type   = HASH_TYPE_ORACLEH;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_UPPER
                               | OPTS_TYPE_ST_UPPER;
                   kern_type   = KERN_TYPE_ORACLEH;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = oracleh_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_SCALAR_MODE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  3200:  hash_type   = HASH_TYPE_BCRYPT;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_GENERATE_LE;
                   kern_type   = KERN_TYPE_BCRYPT;
                   dgst_size   = DGST_SIZE_4_6;
                   parse_func  = bcrypt_parse_hash;
                   sort_by_digest = sort_by_digest_4_6;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  3710:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_SLT_MD5_PW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5s_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  3711:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_SLT_MD5_PW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = mediawiki_b_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  3800:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_SLT_PW_SLT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5s_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  4300:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_VIRTUAL;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_MD5U5_PWSLT1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5md5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;


      case  4400:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_MD5_SHA1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  4500:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA11;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_SALTED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  4700:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_SHA1_MD5;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  4800:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5_CHAP;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = chap_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_MEET_IN_MIDDLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  4900:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_SHA1_SLT_PW_SLT;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1s_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  5000:  hash_type   = HASH_TYPE_KECCAK;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD01;
                   kern_type   = KERN_TYPE_KECCAK;
                   dgst_size   = DGST_SIZE_8_25;
                   parse_func  = keccak_parse_hash;
                   sort_by_digest = sort_by_digest_8_25;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 2;
                   dgst_pos1   = 3;
                   dgst_pos2   = 4;
                   dgst_pos3   = 5;
                   break;

      case  5100:  hash_type   = HASH_TYPE_MD5H;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14;
                   kern_type   = KERN_TYPE_MD5H;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = md5half_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  5200:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_PSAFE3;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = psafe3_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  5300:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_IKEPSK_MD5;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = ikepsk_md5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  5400:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_IKEPSK_SHA1;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = ikepsk_sha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  5500:  hash_type   = HASH_TYPE_NETNTLM;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_NETNTLMv1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = netntlmv1_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_PRECOMPUTE_PERMUT;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  5600:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS14
                               | OPTS_TYPE_PT_UNICODE;
                   kern_type   = KERN_TYPE_NETNTLMv2;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = netntlmv2_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  5700:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA256;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = cisco4_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  5800:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE  // should be OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_ANDROIDPIN;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = androidpin_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6000:  hash_type   = HASH_TYPE_RIPEMD160;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80;
                   kern_type   = KERN_TYPE_RIPEMD160;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = ripemd160_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6100:  hash_type   = HASH_TYPE_WHIRLPOOL;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80;
                   kern_type   = KERN_TYPE_WHIRLPOOL;
                   dgst_size   = DGST_SIZE_4_16;
                   parse_func  = whirlpool_parse_hash;
                   sort_by_digest = sort_by_digest_4_16;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6211:  hash_type   = HASH_TYPE_RIPEMD160;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCRIPEMD160_XTS512;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = truecrypt_parse_hash_2k;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6212:  hash_type   = HASH_TYPE_RIPEMD160;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCRIPEMD160_XTS1024;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = truecrypt_parse_hash_2k;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6213:  hash_type   = HASH_TYPE_RIPEMD160;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCRIPEMD160_XTS1536;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = truecrypt_parse_hash_2k;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6221:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_TCSHA512_XTS512;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6222:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_TCSHA512_XTS1024;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6223:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_TCSHA512_XTS1536;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6231:  hash_type   = HASH_TYPE_WHIRLPOOL;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCWHIRLPOOL_XTS512;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6232:  hash_type   = HASH_TYPE_WHIRLPOOL;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCWHIRLPOOL_XTS1024;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6233:  hash_type   = HASH_TYPE_WHIRLPOOL;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCWHIRLPOOL_XTS1536;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6241:  hash_type   = HASH_TYPE_RIPEMD160;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCRIPEMD160_XTS512;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6242:  hash_type   = HASH_TYPE_RIPEMD160;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCRIPEMD160_XTS1024;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6243:  hash_type   = HASH_TYPE_RIPEMD160;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_TCRIPEMD160_XTS1536;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = truecrypt_parse_hash_1k;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6300:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_MD5AIX;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = md5aix_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6400:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_SHA256AIX;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = sha256aix_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6500:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_SHA512AIX;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha512aix_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6600:  hash_type   = HASH_TYPE_AES;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_AGILEKEY;
                   dgst_size   = DGST_SIZE_4_5; // because kernel uses _SHA1_
                   parse_func  = agilekey_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6700:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_SHA1AIX;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sha1aix_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6800:  hash_type   = HASH_TYPE_AES;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_LASTPASS;
                   dgst_size   = DGST_SIZE_4_8; // because kernel uses _SHA256_
                   parse_func  = lastpass_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  6900:  hash_type   = HASH_TYPE_GOST;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_GOST;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = gost_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  7100:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_PBKDF2_SHA512;
                   dgst_size   = DGST_SIZE_8_16;
                   parse_func  = sha512osx_parse_hash;
                   sort_by_digest = sort_by_digest_8_16;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  7200:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_PBKDF2_SHA512;
                   dgst_size   = DGST_SIZE_8_16;
                   parse_func  = sha512grub_parse_hash;
                   sort_by_digest = sort_by_digest_8_16;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  7300:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15;
                   kern_type   = KERN_TYPE_RAKP;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = rakp_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  7400:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_SHA256CRYPT;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = sha256crypt_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  7500:  hash_type   = HASH_TYPE_KRB5PA;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_KRB5PA;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = krb5pa_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  7600:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA1_SLT_SHA1_PW;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = redmine_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  7700:  hash_type   = HASH_TYPE_SAPB;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_UPPER
                               | OPTS_TYPE_ST_UPPER;
                   kern_type   = KERN_TYPE_SAPB;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = sapb_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  7800:  hash_type   = HASH_TYPE_SAPG;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_UPPER;
                   kern_type   = KERN_TYPE_SAPG;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = sapg_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  7900:  hash_type   = HASH_TYPE_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_DRUPAL7;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = drupal7_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  8000:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_SYBASEASE;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = sybasease_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case  8100:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE;
                   kern_type   = KERN_TYPE_NETSCALER;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = netscaler_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  8200:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_CLOUDKEY;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = cloudkey_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  8300:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_HEX
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_NSEC3;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = nsec3_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  8400:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_WBB3;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = wbb3_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case  8500:  hash_type   = HASH_TYPE_DESRACF;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_UPPER;
                   kern_type   = KERN_TYPE_RACF;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = racf_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_PRECOMPUTE_PERMUT;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  8600:  hash_type   = HASH_TYPE_LOTUS5;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_LOTUS5;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = lotus5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  8700:  hash_type   = HASH_TYPE_LOTUS6;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_LOTUS6;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = lotus6_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  8800:  hash_type   = HASH_TYPE_ANDROIDFDE;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_ANDROIDFDE;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = androidfde_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  8900:  hash_type   = HASH_TYPE_SCRYPT;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_SCRYPT;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = scrypt_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9000:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_GENERATE_LE;
                   kern_type   = KERN_TYPE_PSAFE2;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = psafe2_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9100:  hash_type   = HASH_TYPE_LOTUS8;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_LOTUS8;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = lotus8_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9200:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_PBKDF2_SHA256;
                   dgst_size   = DGST_SIZE_4_32;
                   parse_func  = cisco8_parse_hash;
                   sort_by_digest = sort_by_digest_4_32;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9300:  hash_type   = HASH_TYPE_SCRYPT;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_SCRYPT;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = cisco9_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9400:  hash_type   = HASH_TYPE_OFFICE2007;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_OFFICE2007;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = office2007_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9500:  hash_type   = HASH_TYPE_OFFICE2010;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_OFFICE2010;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = office2010_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9600:  hash_type   = HASH_TYPE_OFFICE2013;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_OFFICE2013;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = office2013_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9700:  hash_type   = HASH_TYPE_OLDOFFICE01;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_UNICODE;
                   kern_type   = KERN_TYPE_OLDOFFICE01;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = oldoffice01_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9710:  hash_type   = HASH_TYPE_OLDOFFICE01;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80;
                   kern_type   = KERN_TYPE_OLDOFFICE01CM1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = oldoffice01cm1_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9720:  hash_type   = HASH_TYPE_OLDOFFICE01;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_PT_NEVERCRACK;
                   kern_type   = KERN_TYPE_OLDOFFICE01CM2;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = oldoffice01cm2_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9800:  hash_type   = HASH_TYPE_OLDOFFICE34;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_UNICODE;
                   kern_type   = KERN_TYPE_OLDOFFICE34;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = oldoffice34_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9810:  hash_type   = HASH_TYPE_OLDOFFICE34;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_OLDOFFICE34CM1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = oldoffice34cm1_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9820:  hash_type   = HASH_TYPE_OLDOFFICE34;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_UNICODE
                               | OPTS_TYPE_PT_NEVERCRACK;
                   kern_type   = KERN_TYPE_OLDOFFICE34CM2;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = oldoffice34cm2_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case  9900:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_RADMIN2;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = radmin2_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case 10000:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_PBKDF2_SHA256;
                   dgst_size   = DGST_SIZE_4_32;
                   parse_func  = djangopbkdf2_parse_hash;
                   sort_by_digest = sort_by_digest_4_32;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 10100:  hash_type   = HASH_TYPE_SIPHASH;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_SIPHASH;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = siphash_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 10200:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS14;
                   kern_type   = KERN_TYPE_HMACMD5_PW;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = crammd5_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case 10300:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE; // should be OPTS_TYPE_PT_GENERATE_BE
                   kern_type   = KERN_TYPE_SAPH_SHA1;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = saph_sha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 10400:  hash_type   = HASH_TYPE_PDFU16;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_PDF11;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = pdf11_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 10410:  hash_type   = HASH_TYPE_PDFU16;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_PDF11CM1;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = pdf11cm1_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 10420:  hash_type   = HASH_TYPE_PDFU16;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_PDF11CM2;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = pdf11cm2_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 10500:  hash_type   = HASH_TYPE_PDFU16;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_PDF14;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = pdf14_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 10600:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_ST_ADD80
                               | OPTS_TYPE_ST_ADDBITS15
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_SHA256_PWSLT;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = pdf17l3_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_APPENDED_SALT
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case 10700:  hash_type   = HASH_TYPE_PDFU32;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_PDF17L8;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = pdf17l8_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 10800:  hash_type   = HASH_TYPE_SHA384;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_PT_ADDBITS15;
                   kern_type   = KERN_TYPE_SHA384;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = sha384_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_SCALAR_MODE
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_NOT_SALTED
                               | OPTI_TYPE_RAW_HASH;
                   dgst_pos0   = 6;
                   dgst_pos1   = 7;
                   dgst_pos2   = 4;
                   dgst_pos3   = 5;
                   break;

      case 10900:  hash_type   = HASH_TYPE_PBKDF2_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_BASE64
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_PBKDF2_SHA256;
                   dgst_size   = DGST_SIZE_4_32;
                   parse_func  = pbkdf2_sha256_parse_hash;
                   sort_by_digest = sort_by_digest_4_32;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 11000:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80;
                   kern_type   = KERN_TYPE_PRESTASHOP;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = prestashop_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_NOT_ITERATED
                               | OPTI_TYPE_PREPENDED_SALT;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case 11100:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_POSTGRESQL_AUTH;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = postgresql_auth_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_PRECOMPUTE_MERKLE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case 11200:  hash_type   = HASH_TYPE_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_MYSQL_AUTH;
                   dgst_size   = DGST_SIZE_4_5;
                   parse_func  = mysql_auth_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_EARLY_SKIP;
                   dgst_pos0   = 3;
                   dgst_pos1   = 4;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case 11300:  hash_type   = HASH_TYPE_BITCOIN_WALLET;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_HEX
                               | OPTS_TYPE_ST_ADD80;
                   kern_type   = KERN_TYPE_BITCOIN_WALLET;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = bitcoin_wallet_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 11400:  hash_type   = HASH_TYPE_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD80
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_SIP_AUTH;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = sip_auth_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 3;
                   dgst_pos2   = 2;
                   dgst_pos3   = 1;
                   break;

      case 11500:  hash_type   = HASH_TYPE_CRC32;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_GENERATE_LE
                               | OPTS_TYPE_ST_HEX;
                   kern_type   = KERN_TYPE_CRC32;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = crc32_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_SCALAR_MODE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 11600:  hash_type   = HASH_TYPE_AES;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_NEVERCRACK;
                   kern_type   = KERN_TYPE_SEVEN_ZIP;
                   dgst_size   = DGST_SIZE_4_4; // originally DGST_SIZE_4_2
                   parse_func  = seven_zip_parse_hash;
                   sort_by_digest = sort_by_digest_4_4; // originally sort_by_digest_4_2
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 11700:  hash_type   = HASH_TYPE_GOST_2012SBOG_256;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD01;
                   kern_type   = KERN_TYPE_GOST_2012SBOG_256;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = gost2012sbog_256_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 11800:  hash_type   = HASH_TYPE_GOST_2012SBOG_512;
                   salt_type   = SALT_TYPE_NONE;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_PT_ADD01;
                   kern_type   = KERN_TYPE_GOST_2012SBOG_512;
                   dgst_size   = DGST_SIZE_4_16;
                   parse_func  = gost2012sbog_512_parse_hash;
                   sort_by_digest = sort_by_digest_4_16;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 11900:  hash_type   = HASH_TYPE_PBKDF2_MD5;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_BASE64
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_PBKDF2_MD5;
                   dgst_size   = DGST_SIZE_4_32;
                   parse_func  = pbkdf2_md5_parse_hash;
                   sort_by_digest = sort_by_digest_4_32;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 12000:  hash_type   = HASH_TYPE_PBKDF2_SHA1;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_BASE64
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_PBKDF2_SHA1;
                   dgst_size   = DGST_SIZE_4_32;
                   parse_func  = pbkdf2_sha1_parse_hash;
                   sort_by_digest = sort_by_digest_4_32;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 12100:  hash_type   = HASH_TYPE_PBKDF2_SHA512;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_ST_BASE64
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_PBKDF2_SHA512;
                   dgst_size   = DGST_SIZE_8_16;
                   parse_func  = pbkdf2_sha512_parse_hash;
                   sort_by_digest = sort_by_digest_8_16;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 12200:  hash_type   = HASH_TYPE_ECRYPTFS;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_ECRYPTFS;
                   dgst_size   = DGST_SIZE_8_8;
                   parse_func  = ecryptfs_parse_hash;
                   sort_by_digest = sort_by_digest_8_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 12300:  hash_type   = HASH_TYPE_ORACLET;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_ORACLET;
                   dgst_size   = DGST_SIZE_8_16;
                   parse_func  = oraclet_parse_hash;
                   sort_by_digest = sort_by_digest_8_16;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 12400:  hash_type   = HASH_TYPE_BSDICRYPT;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_BSDICRYPT;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = bsdicrypt_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_PERMUT;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 12500:  hash_type   = HASH_TYPE_RAR3HP;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_RAR3;
                   dgst_size   = DGST_SIZE_4_4;
                   parse_func  = rar3hp_parse_hash;
                   sort_by_digest = sort_by_digest_4_4;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 12600:  hash_type   = HASH_TYPE_SHA256;
                   salt_type   = SALT_TYPE_INTERN;
                   attack_exec = ATTACK_EXEC_ON_GPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_BE
                               | OPTS_TYPE_PT_ADD80;
                   kern_type   = KERN_TYPE_CF10;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = cf10_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE
                               | OPTI_TYPE_PRECOMPUTE_INIT
                               | OPTI_TYPE_EARLY_SKIP
                               | OPTI_TYPE_NOT_ITERATED;
                   dgst_pos0   = 3;
                   dgst_pos1   = 7;
                   dgst_pos2   = 2;
                   dgst_pos3   = 6;
                   break;

      case 12700:  hash_type   = HASH_TYPE_AES;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE
                               | OPTS_TYPE_HASH_COPY;
                   kern_type   = KERN_TYPE_MYWALLET;
                   dgst_size   = DGST_SIZE_4_5; // because kernel uses _SHA1_
                   parse_func  = mywallet_parse_hash;
                   sort_by_digest = sort_by_digest_4_5;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      case 12800:  hash_type   = HASH_TYPE_PBKDF2_SHA256;
                   salt_type   = SALT_TYPE_EMBEDDED;
                   attack_exec = ATTACK_EXEC_ON_CPU;
                   opts_type   = OPTS_TYPE_PT_GENERATE_LE;
                   kern_type   = KERN_TYPE_MS_DRSR;
                   dgst_size   = DGST_SIZE_4_8;
                   parse_func  = ms_drsr_parse_hash;
                   sort_by_digest = sort_by_digest_4_8;
                   opti_type   = OPTI_TYPE_ZERO_BYTE;
                   dgst_pos0   = 0;
                   dgst_pos1   = 1;
                   dgst_pos2   = 2;
                   dgst_pos3   = 3;
                   break;

      default:     usage_mini_print (PROGNAME); return (-1);
    }

    /**
     * transpose
     */

    data.parse_func = parse_func;

    /**
     * misc stuff
     */

    if (hex_salt)
    {
      if (salt_type == SALT_TYPE_INTERN)
      {
        opts_type |= OPTS_TYPE_ST_HEX;
      }
      else
      {
        log_error ("ERROR: Parameter hex-salt not valid for hash-type %u", hash_mode);

        return (-1);
      }
    }

    uint isSalted = ((salt_type == SALT_TYPE_INTERN)
                  |  (salt_type == SALT_TYPE_EXTERN)
                  |  (salt_type == SALT_TYPE_EMBEDDED)
                  |  (salt_type == SALT_TYPE_VIRTUAL));

    sort_by_digest = sort_by_digest_p0p1;  // overruled by 64 bit digest

    data.hash_type   = hash_type;
    data.attack_mode = attack_mode;
    data.attack_kern = attack_kern;
    data.attack_exec = attack_exec;
    data.kern_type   = kern_type;
    data.opts_type   = opts_type;
    data.dgst_size   = dgst_size;
    data.salt_type   = salt_type;
    data.isSalted    = isSalted;
    data.sort_by_digest = sort_by_digest;
    data.dgst_pos0   = dgst_pos0;
    data.dgst_pos1   = dgst_pos1;
    data.dgst_pos2   = dgst_pos2;
    data.dgst_pos3   = dgst_pos3;

    esalt_size = 0;

    switch (hash_mode)
    {
      case  2500:  esalt_size = sizeof (wpa_t);           break;
      case  5300:  esalt_size = sizeof (ikepsk_t);        break;
      case  5400:  esalt_size = sizeof (ikepsk_t);        break;
      case  5500:  esalt_size = sizeof (netntlm_t);       break;
      case  5600:  esalt_size = sizeof (netntlm_t);       break;
      case  6211:
      case  6212:
      case  6213:
      case  6221:
      case  6222:
      case  6223:
      case  6231:
      case  6232:
      case  6233:
      case  6241:
      case  6242:
      case  6243:  esalt_size = sizeof (tc_t);            break;
      case  6600:  esalt_size = sizeof (agilekey_t);      break;
      case  7100:  esalt_size = sizeof (pbkdf2_sha512_t); break;
      case  7200:  esalt_size = sizeof (pbkdf2_sha512_t); break;
      case  7300:  esalt_size = sizeof (rakp_t);          break;
      case  7500:  esalt_size = sizeof (krb5pa_t);        break;
      case  8200:  esalt_size = sizeof (cloudkey_t);      break;
      case  8800:  esalt_size = sizeof (androidfde_t);    break;
      case  9200:  esalt_size = sizeof (pbkdf2_sha256_t); break;
      case  9400:  esalt_size = sizeof (office2007_t);    break;
      case  9500:  esalt_size = sizeof (office2010_t);    break;
      case  9600:  esalt_size = sizeof (office2013_t);    break;
      case  9700:  esalt_size = sizeof (oldoffice01_t);   break;
      case  9710:  esalt_size = sizeof (oldoffice01_t);   break;
      case  9720:  esalt_size = sizeof (oldoffice01_t);   break;
      case  9800:  esalt_size = sizeof (oldoffice34_t);   break;
      case  9810:  esalt_size = sizeof (oldoffice34_t);   break;
      case  9820:  esalt_size = sizeof (oldoffice34_t);   break;
      case 10000:  esalt_size = sizeof (pbkdf2_sha256_t); break;
      case 10200:  esalt_size = sizeof (cram_md5_t);      break;
      case 10400:  esalt_size = sizeof (pdf_t);           break;
      case 10410:  esalt_size = sizeof (pdf_t);           break;
      case 10420:  esalt_size = sizeof (pdf_t);           break;
      case 10500:  esalt_size = sizeof (pdf_t);           break;
      case 10600:  esalt_size = sizeof (pdf_t);           break;
      case 10700:  esalt_size = sizeof (pdf_t);           break;
      case 10900:  esalt_size = sizeof (pbkdf2_sha256_t); break;
      case 11300:  esalt_size = sizeof (bitcoin_wallet_t); break;
      case 11400:  esalt_size = sizeof (sip_t);           break;
      case 11600:  esalt_size = sizeof (seven_zip_t);     break;
      case 11900:  esalt_size = sizeof (pbkdf2_md5_t);    break;
      case 12000:  esalt_size = sizeof (pbkdf2_sha1_t);   break;
      case 12100:  esalt_size = sizeof (pbkdf2_sha512_t); break;
    }

    data.esalt_size = esalt_size;

    /**
     * choose dictionary parser
     */

    if (hash_type == HASH_TYPE_LM)
    {
      get_next_word_func = get_next_word_lm;
    }
    else if (opts_type & OPTS_TYPE_PT_UPPER)
    {
      get_next_word_func = get_next_word_uc;
    }
    else
    {
      get_next_word_func = get_next_word_std;
    }

    /**
     * dictstat
     */

    dictstat_t *dictstat_base = (dictstat_t *) mycalloc (MAX_DICTSTAT, sizeof (dictstat_t));

    #ifdef _POSIX
    size_t dictstat_nmemb = 0;
    #endif

    #ifdef _WIN
    uint   dictstat_nmemb = 0;
    #endif

    char dictstat[256];

    FILE *dictstat_fp = NULL;

    if (keyspace == 0)
    {
      memset (dictstat, 0, sizeof (dictstat));

      snprintf (dictstat, sizeof (dictstat) - 1, "%s/%s.dictstat", install_dir, PROGNAME);

      dictstat_fp = fopen (dictstat, "rb");

      if (dictstat_fp)
      {
        #ifdef _POSIX
        struct stat tmpstat;

        fstat (fileno (dictstat_fp), &tmpstat);
        #endif

        #ifdef _WIN
        struct stat64 tmpstat;

        _fstat64 (fileno (dictstat_fp), &tmpstat);
        #endif

        if (tmpstat.st_mtime < COMPTIME)
        {
          /* with v0.15 the format changed so we have to ensure user is using a good version
             since there is no version-header in the dictstat file */

          fclose (dictstat_fp);

          unlink (dictstat);
        }
        else
        {
          while (!feof (dictstat_fp))
          {
            dictstat_t d;

            if (fread (&d, sizeof (dictstat_t), 1, dictstat_fp) == 0) continue;

            lsearch (&d, dictstat_base, &dictstat_nmemb, sizeof (dictstat_t), sort_by_dictstat);

            if (dictstat_nmemb == (MAX_DICTSTAT - 1000))
            {
              log_error ("ERROR: There are too many entries in the %s database. You have to remove/rename it.", dictstat);

              return -1;
            }
          }

          fclose (dictstat_fp);
        }
      }
    }

    /**
     * potfile
     */

    char potfile[256];

    memset (potfile, 0, sizeof (potfile));

    snprintf (potfile, sizeof (potfile) - 1, "%s.pot", session);

    data.pot_fp = NULL;

    FILE *out_fp = NULL;
    FILE *pot_fp = NULL;

    if (show == 1 || left == 1)
    {
      pot_fp = fopen (potfile, "rb");

      if (pot_fp == NULL)
      {
        log_error ("ERROR: %s: %s", potfile, strerror (errno));

        return (-1);
      }

      if (outfile != NULL)
      {
        if ((out_fp = fopen (outfile, "ab")) == NULL)
        {
          log_error ("ERROR: %s: %s", outfile, strerror (errno));

          fclose (pot_fp);

          return (-1);
        }
      }
      else
      {
        out_fp = stdout;
      }
    }
    else
    {
      if (potfile_disable == 0)
      {
        pot_fp = fopen (potfile, "ab");

        if (pot_fp == NULL)
        {
          log_error ("ERROR: %s: %s", potfile, strerror (errno));

          return (-1);
        }

        data.pot_fp = pot_fp;
      }
    }

    pot_t *pot = NULL;

    uint pot_cnt   = 0;
    uint pot_avail = 0;

    if (show == 1 || left == 1)
    {
      SUPPRESS_OUTPUT = 1;

      pot_avail = count_lines (pot_fp);

      rewind (pot_fp);

      pot = (pot_t *) mycalloc (pot_avail, sizeof (pot_t));

      uint pot_hashes_avail = 0;

      uint line_num = 0;

      while (!feof (pot_fp))
      {
        line_num++;

        char line_buf[BUFSIZ];

        int line_len = fgetl (pot_fp, line_buf);

        if (line_len == 0) continue;

        char *plain_buf = line_buf + line_len;

        pot_t *pot_ptr = &pot[pot_cnt];

        hash_t *hashes_buf = &pot_ptr->hash;

        // we do not initialize all hashes_buf->digest etc at the beginning, since many lines may not be
        // valid lines of this specific hash type (otherwise it would be more waste of memory than gain)

        if (pot_cnt == pot_hashes_avail)
        {
          uint pos = 0;

          for (pos = 0; pos < INCR_POT; pos++)
          {
            if ((pot_cnt + pos) >= pot_avail) break;

            pot_t *tmp_pot = &pot[pot_cnt + pos];

            hash_t *tmp_hash = &tmp_pot->hash;

            tmp_hash->digest = mymalloc (dgst_size);

            if (isSalted)
            {
              tmp_hash->salt = (salt_t *) mymalloc (sizeof (salt_t));
            }

            if (esalt_size)
            {
              tmp_hash->esalt = mymalloc (esalt_size);
            }

            pot_hashes_avail++;
          }
        }

        int plain_len = 0;

        int parser_status;

        int iter = MAX_CUT_TRIES;

        do
        {
          for (int i = line_len - 1; i; i--, plain_len++, plain_buf--, line_len--)
          {
            if (line_buf[i] == ':')
            {
              line_len--;

              break;
            }
          }

          if (data.hash_mode != 2500)
          {
            parser_status = parse_func (line_buf, line_len, hashes_buf);
          }
          else
          {
            int max_salt_size = sizeof (hashes_buf->salt->salt_buf);

            if (line_len > max_salt_size)
            {
              parser_status = PARSER_GLOBAL_LENGTH;
            }
            else
            {
              memset (&hashes_buf->salt->salt_buf, 0, max_salt_size);

              memcpy (&hashes_buf->salt->salt_buf, line_buf, line_len);

              hashes_buf->salt->salt_len = line_len;

              parser_status = PARSER_OK;
            }
          }

          // if NOT parsed without error, we add the ":" to the plain

          if (parser_status == PARSER_GLOBAL_LENGTH || parser_status == PARSER_HASH_LENGTH || parser_status == PARSER_SALT_LENGTH)
          {
            plain_len++;
            plain_buf--;
          }

        } while ((parser_status == PARSER_GLOBAL_LENGTH || parser_status == PARSER_HASH_LENGTH || parser_status == PARSER_SALT_LENGTH) && --iter);

        if (parser_status < PARSER_GLOBAL_ZERO)
        {
          // log_info ("WARNING: Potfile '%s' in line %u (%s): %s", potfile, line_num, line_buf, strparser (parser_status));

          continue;
        }

        memcpy (pot_ptr->plain_buf, plain_buf, plain_len);

        pot_ptr->plain_len = plain_len;

        pot_cnt++;
      }

      fclose (pot_fp);

      SUPPRESS_OUTPUT = 0;

      qsort (pot, pot_cnt, sizeof (pot_t), sort_by_pot);
    }

    /**
     * gpu accel and loops auto adjustment
     */

    if (gpu_accel_chgd == 0) gpu_accel = set_gpu_accel (hash_mode);
    if (gpu_loops_chgd == 0) gpu_loops = set_gpu_loops (hash_mode);

    if (workload_profile == 1)
    {
      gpu_loops /= 8;
      gpu_accel /= 4;

      if (gpu_loops == 0) gpu_loops = 8;
      if (gpu_accel == 0) gpu_accel = 2;
    }
    else if (workload_profile == 3)
    {
      gpu_loops *= 8;
      gpu_accel *= 4;

      if (gpu_loops > 1024) gpu_loops = 1024;
      if (gpu_accel >  256) gpu_accel =  256; // causes memory problems otherwise
    }

    // those hashes *must* run at a specific gpu_loops count because of some optimization inside the kernel

    if ((opts_type & OPTS_TYPE_PT_BITSLICE) && (attack_mode == ATTACK_MODE_BF))
    {
      gpu_loops = 1024;
    }

    if (hash_mode == 12500)
    {
      gpu_loops = ROUNDS_RAR3 / 16;
    }

    data.gpu_accel = gpu_accel;
    data.gpu_loops = gpu_loops;

    /**
     * word len
     */

    uint pw_min = PW_MIN;
    uint pw_max = PW_MAX;

    switch (hash_mode)
    {
      case   400: if (pw_max > 40) pw_max = 40;
                  break;
      case   500: if (pw_max > 16) pw_max = 16;
                  break;
      case  1500: if (pw_max >  8) pw_max =  8;
                  break;
      case  1600: if (pw_max > 16) pw_max = 16;
                  break;
      case  1800: if (pw_max > 16) pw_max = 16;
                  break;
      case  2100: if (pw_max > 16) pw_max = 16;
                  break;
      case  2500: if (pw_min <  8) pw_min =  8;
                  break;
      case  3000: if (pw_max >  7) pw_max =  7;
                  break;
      case  5200: if (pw_max > 24) pw_max = 24;
                  break;
      case  5800: if (pw_max > 16) pw_max = 16;
                  break;
      case  6300: if (pw_max > 16) pw_max = 16;
                  break;
      case  7400: if (pw_max > 16) pw_max = 16;
                  break;
      case  7900: if (pw_max > 48) pw_max = 48;
                  break;
      case  8500: if (pw_max >  8) pw_max =  8;
                  break;
      case  8600: if (pw_max > 16) pw_max = 16;
                  break;
      case  9710: pw_min = 5;
                  pw_max = 5;
                  break;
      case  9810: pw_min = 5;
                  pw_max = 5;
                  break;
      case 10410: pw_min = 5;
                  pw_max = 5;
                  break;
      case 10300: if (pw_max <  3) pw_min =  3;
                  if (pw_max > 40) pw_max = 40;
                  break;
      case 10500: if (pw_max <  3) pw_min =  3;
                  if (pw_max > 40) pw_max = 40;
                  break;
      case 10700: if (pw_max > 16) pw_max = 16;
                  break;
      case 11300: if (pw_max > 40) pw_max = 40;
                  break;
      case 12500: if (pw_max > 20) pw_max = 20;
                  break;
      case 12800: if (pw_max > 24) pw_max = 24;
                  break;
    }

    if (attack_exec == ATTACK_EXEC_ON_GPU)
    {
      switch (attack_kern)
      {
        case ATTACK_KERN_STRAIGHT:  if (pw_max > PW_DICTMAX) pw_max = PW_DICTMAX1;
                                    break;
        case ATTACK_KERN_COMBI:     if (pw_max > PW_DICTMAX) pw_max = PW_DICTMAX1;
                                    break;
      }
    }

    /**
     * charsets : keep them together for more easy maintainnce
     */

    cs_t mp_sys[6];
    cs_t mp_usr[4];

    memset (mp_sys, 0, sizeof (mp_sys));
    memset (mp_usr, 0, sizeof (mp_usr));

    mp_setup_sys (mp_sys);

    if (custom_charset_1) mp_setup_usr (mp_sys, mp_usr, custom_charset_1, 0);
    if (custom_charset_2) mp_setup_usr (mp_sys, mp_usr, custom_charset_2, 1);
    if (custom_charset_3) mp_setup_usr (mp_sys, mp_usr, custom_charset_3, 2);
    if (custom_charset_4) mp_setup_usr (mp_sys, mp_usr, custom_charset_4, 3);

    /**
     * load hashes, part I: find input mode, count hashes
     */

    uint hashlist_mode   = 0;
    uint hashlist_format = HLFMT_HASHCAT;

    uint hashes_avail = 0;

    if (benchmark == 0)
    {
      struct stat f;

      hashlist_mode = (stat (myargv[optind], &f) == 0) ? HL_MODE_FILE : HL_MODE_ARG;

      if ((hash_mode == 2500) ||
          (hash_mode == 5200) ||
          ((hash_mode >= 6200) && (hash_mode <= 6299)) ||
          (hash_mode == 9000))
      {
        hashlist_mode = HL_MODE_ARG;

        char *hashfile = myargv[optind];

        data.hashfile = hashfile;

        logfile_top_var_string ("target", hashfile);
      }

      if (hashlist_mode == HL_MODE_ARG)
      {
        if (hash_mode == 2500)
        {
          struct stat st;

          if (stat (data.hashfile, &st) == -1)
          {
            log_error ("ERROR: %s: %s", data.hashfile, strerror (errno));

            return (-1);
          }

          hashes_avail = st.st_size / sizeof (hccap_t);
        }
        else
        {
          hashes_avail = 1;
        }
      }
      else if (hashlist_mode == HL_MODE_FILE)
      {
        char *hashfile = myargv[optind];

        data.hashfile = hashfile;

        logfile_top_var_string ("target", hashfile);

        FILE *fp = NULL;

        if ((fp = fopen (hashfile, "rb")) == NULL)
        {
          log_error ("ERROR: %s: %s", hashfile, strerror (errno));

          return (-1);
        }

        if (data.quiet == 0) log_info_nn ("Counting lines in %s", hashfile);

        hashes_avail = count_lines (fp);

        rewind (fp);

        if (hashes_avail == 0)
        {
          log_error ("ERROR: hashfile is empty or corrupt");

          fclose (fp);

          return (-1);
        }

        hashlist_format = hlfmt_detect (fp, 100); // 100 = max numbers to "scan". could be hashes_avail, too

        if ((remove == 1) && (hashlist_format != HLFMT_HASHCAT))
        {
          log_error ("ERROR: remove not supported in native hashfile-format mode");

          fclose (fp);

          return (-1);
        }

        fclose (fp);
      }
    }
    else
    {
      hashlist_mode = HL_MODE_ARG;

      hashes_avail = 1;
    }

    if (hash_mode == 3000) hashes_avail *= 2;

    data.hashlist_mode   = hashlist_mode;
    data.hashlist_format = hashlist_format;

    logfile_top_uint (hashlist_mode);
    logfile_top_uint (hashlist_format);

    /**
     * load hashes, part II: allocate required memory, set pointers
     */

    hash_t *hashes_buf  = NULL;
    void   *digests_buf = NULL;
    salt_t *salts_buf   = NULL;
    void   *esalts_buf  = NULL;

    hashes_buf = (hash_t *) mycalloc (hashes_avail, sizeof (hash_t));

    digests_buf = (void *) mycalloc (hashes_avail, dgst_size);

    if ((username && (remove || show)) || (opts_type & OPTS_TYPE_HASH_COPY))
    {
      uint32_t hash_pos;

      for (hash_pos = 0; hash_pos < hashes_avail; hash_pos++)
      {
        hashinfo_t *hash_info = (hashinfo_t *) mymalloc (sizeof (hashinfo_t));

        hashes_buf[hash_pos].hash_info = hash_info;

        if (username && (remove || show || left))
        {
          hash_info->user = (user_t*) mymalloc (sizeof (user_t));
        }

        if (benchmark)
        {
          hash_info->orighash = (char *) mymalloc (256);
        }
      }
    }

    if (isSalted)
    {
      salts_buf = (salt_t *) mycalloc (hashes_avail, sizeof (salt_t));

      if (esalt_size)
      {
        esalts_buf = (void *) mycalloc (hashes_avail, esalt_size);
      }
    }
    else
    {
      salts_buf = (salt_t *) mycalloc (1, sizeof (salt_t));
    }

    for (uint hash_pos = 0; hash_pos < hashes_avail; hash_pos++)
    {
      hashes_buf[hash_pos].digest = ((char *) digests_buf) + (hash_pos * dgst_size);

      if (isSalted)
      {
        hashes_buf[hash_pos].salt = &salts_buf[hash_pos];

        if (esalt_size)
        {
          hashes_buf[hash_pos].esalt = ((char *) esalts_buf) + (hash_pos * esalt_size);
        }
      }
      else
      {
        hashes_buf[hash_pos].salt = &salts_buf[0];
      }
    }

    /**
     * load hashes, part III: parse hashes or generate them if benchmark
     */

    uint hashes_cnt = 0;

    if (benchmark == 0)
    {
      if (keyspace == 1)
      {
        // useless to read hash file for keyspace, cheat a little bit w/ optind
      }
      else if (hashes_avail == 0)
      {
      }
      else if (hashlist_mode == HL_MODE_ARG)
      {
        char *input_buf = myargv[optind];

        uint input_len = strlen (input_buf);

        logfile_top_var_string ("target", input_buf);

        char *hash_buf = NULL;
        int   hash_len = 0;

        hlfmt_hash (hashlist_format, input_buf, input_len, &hash_buf, &hash_len);

        if (hash_len)
        {
          if (opts_type & OPTS_TYPE_HASH_COPY)
          {
            hashinfo_t *hash_info_tmp = hashes_buf[hashes_cnt].hash_info;

            hash_info_tmp->orighash = mystrdup (hash_buf);
          }

          if (isSalted)
          {
            memset (hashes_buf[0].salt, 0, sizeof (salt_t));
          }

          int parser_status = PARSER_OK;

          if (hash_mode == 2500)
          {
            if (hash_len == 0)
            {
              log_error ("ERROR: hccap file not specified");

              return (-1);
            }

            hashlist_mode = HL_MODE_FILE;

            data.hashlist_mode = hashlist_mode;

            FILE *fp = fopen (hash_buf, "rb");

            if (fp == NULL)
            {
              log_error ("ERROR: %s: %s", hash_buf, strerror (errno));

              return (-1);
            }

            if (hashes_avail < 1)
            {
              log_error ("ERROR: hccap file is empty or corrupt");

              fclose (fp);

              return (-1);
            }

            uint hccap_size = sizeof (hccap_t);

            char in[hccap_size];

            while (!feof (fp))
            {
              int n = fread (&in, hccap_size, 1, fp);

              if (n != 1)
              {
                if (hashes_cnt < 1) parser_status = PARSER_HCCAP_FILE_SIZE;

                break;
              }

              parser_status = parse_func (in, hccap_size, &hashes_buf[hashes_cnt]);

              if (parser_status != PARSER_OK)
              {
                log_info ("WARNING: Hash '%s': %s", hash_buf, strparser (parser_status));

                continue;
              }

              // hack: append MAC1 and MAC2 s.t. in --show and --left the line matches with the .pot file format (i.e. ESSID:MAC1:MAC2)

              if ((show == 1) || (left == 1))
              {
                salt_t *tmp_salt = hashes_buf[hashes_cnt].salt;

                char *salt_ptr = (char *) tmp_salt->salt_buf;

                int cur_pos = tmp_salt->salt_len;
                int rem_len = sizeof (hashes_buf[hashes_cnt].salt->salt_buf) - cur_pos;

                wpa_t *wpa = (wpa_t *) hashes_buf[hashes_cnt].esalt;

                unsigned char *pke_ptr = (unsigned char *) wpa->pke;

                // do the appending task

                snprintf (salt_ptr + cur_pos,
                          rem_len,
                          ":%02x%02x%02x%02x%02x%02x:%02x%02x%02x%02x%02x%02x",
                          pke_ptr[20], pke_ptr[27], pke_ptr[26], pke_ptr[25], pke_ptr[24], pke_ptr[31],  // MAC1
                          pke_ptr[30], pke_ptr[29], pke_ptr[28], pke_ptr[35], pke_ptr[34], pke_ptr[33]); // MAC2


                // memset () the remaining part of the salt

                cur_pos = tmp_salt->salt_len + 1 + 12 + 1 + 12;
                rem_len = sizeof (hashes_buf[hashes_cnt].salt->salt_buf) - cur_pos;

                if (rem_len > 0) memset (salt_ptr + cur_pos, 0, rem_len);

                tmp_salt->salt_len += 1 + 12 + 1 + 12;
              }

              if (show == 1) handle_show_request (pot, pot_cnt, (char *) hashes_buf[hashes_cnt].salt->salt_buf, hashes_buf[hashes_cnt].salt->salt_len, &hashes_buf[hashes_cnt], sort_by_salt_buf, out_fp);
              if (left == 1) handle_left_request (pot, pot_cnt, (char *) hashes_buf[hashes_cnt].salt->salt_buf, hashes_buf[hashes_cnt].salt->salt_len, &hashes_buf[hashes_cnt], sort_by_salt_buf, out_fp);

              hashes_cnt++;
            }

            fclose (fp);
          }
          else if (hash_mode == 3000)
          {
            if (hash_len == 32)
            {
              parser_status = parse_func (hash_buf, 16, &hashes_buf[hashes_cnt]);

              hash_t *lm_hash_left = NULL;

              if (parser_status == PARSER_OK)
              {
                lm_hash_left = &hashes_buf[hashes_cnt];

                hashes_cnt++;
              }
              else
              {
                log_info ("WARNING: Hash '%s': %s", input_buf, strparser (parser_status));
              }


              parser_status = parse_func (hash_buf + 16, 16, &hashes_buf[hashes_cnt]);

              hash_t *lm_hash_right = NULL;

              if (parser_status == PARSER_OK)
              {
                lm_hash_right = &hashes_buf[hashes_cnt];

                hashes_cnt++;
              }
              else
              {
                log_info ("WARNING: Hash '%s': %s", input_buf, strparser (parser_status));
              }

              // show / left

              if ((lm_hash_left != NULL) && (lm_hash_right != NULL))
              {
                if (show == 1) handle_show_request_lm (pot, pot_cnt, input_buf, input_len, lm_hash_left, lm_hash_right, sort_by_pot, out_fp);
                if (left == 1) handle_left_request_lm (pot, pot_cnt, input_buf, input_len, lm_hash_left, lm_hash_right, sort_by_pot, out_fp);
              }
            }
            else
            {
              parser_status = parse_func (hash_buf, hash_len, &hashes_buf[hashes_cnt]);

              if (parser_status == PARSER_OK)
              {
                if (show == 1) handle_show_request (pot, pot_cnt, input_buf, input_len, &hashes_buf[hashes_cnt], sort_by_pot, out_fp);
                if (left == 1) handle_left_request (pot, pot_cnt, input_buf, input_len, &hashes_buf[hashes_cnt], sort_by_pot, out_fp);
              }

              if (parser_status == PARSER_OK)
              {
                hashes_cnt++;
              }
              else
              {
                log_info ("WARNING: Hash '%s': %s", input_buf, strparser (parser_status));
              }
            }
          }
          else
          {
            parser_status = parse_func (hash_buf, hash_len, &hashes_buf[hashes_cnt]);

            if (parser_status == PARSER_OK)
            {
              if (show == 1) handle_show_request (pot, pot_cnt, input_buf, input_len, &hashes_buf[hashes_cnt], sort_by_pot, out_fp);
              if (left == 1) handle_left_request (pot, pot_cnt, input_buf, input_len, &hashes_buf[hashes_cnt], sort_by_pot, out_fp);
            }

            if (parser_status == PARSER_OK)
            {
              hashes_cnt++;
            }
            else
            {
              log_info ("WARNING: Hash '%s': %s", input_buf, strparser (parser_status));
            }
          }
        }
      }
      else if (hashlist_mode == HL_MODE_FILE)
      {
        char *hashfile = data.hashfile;

        FILE *fp;

        if ((fp = fopen (hashfile, "rb")) == NULL)
        {
          log_error ("ERROR: %s: %s", hashfile, strerror (errno));

          return (-1);
        }

        uint line_num = 0;

        while (!feof (fp))
        {
          line_num++;

          char line_buf[BUFSIZ];

          int line_len = fgetl (fp, line_buf);

          if (line_len == 0) continue;

          char *hash_buf = NULL;
          int   hash_len = 0;

          hlfmt_hash (hashlist_format, line_buf, line_len, &hash_buf, &hash_len);

          if (username)
          {
            char *user_buf = NULL;
            int   user_len = 0;

            hlfmt_user (hashlist_format, line_buf, line_len, &user_buf, &user_len);

            if (remove || show)
            {
              user_t **user = &hashes_buf[hashes_cnt].hash_info->user;

              *user = (user_t *) malloc (sizeof (user_t));

              user_t *user_ptr = *user;

              if (user_buf != NULL)
              {
                user_ptr->user_name = mystrdup (user_buf);
              }
              else
              {
                user_ptr->user_name = mystrdup ("");
              }

              user_ptr->user_len = user_len;
            }
          }

          if (opts_type & OPTS_TYPE_HASH_COPY)
          {
            hashinfo_t *hash_info_tmp = hashes_buf[hashes_cnt].hash_info;

            hash_info_tmp->orighash = mystrdup (hash_buf);
          }

          if (isSalted)
          {
            memset (hashes_buf[hashes_cnt].salt, 0, sizeof (salt_t));
          }

          if (hash_mode == 3000)
          {
            if (hash_len == 32)
            {
              int parser_status = parse_func (hash_buf, 16, &hashes_buf[hashes_cnt]);

              if (parser_status < PARSER_GLOBAL_ZERO)
              {
                log_info ("WARNING: Hashfile '%s' in line %u (%s): %s", data.hashfile, line_num, line_buf, strparser (parser_status));

                continue;
              }

              hash_t *lm_hash_left = &hashes_buf[hashes_cnt];

              hashes_cnt++;

              parser_status = parse_func (hash_buf + 16, 16, &hashes_buf[hashes_cnt]);

              if (parser_status < PARSER_GLOBAL_ZERO)
              {
                log_info ("WARNING: Hashfile '%s' in line %u (%s): %s", data.hashfile, line_num, line_buf, strparser (parser_status));

                continue;
              }

              hash_t *lm_hash_right = &hashes_buf[hashes_cnt];

              if (data.quiet == 0) if ((hashes_cnt % 0x20000) == 0) log_info_nn ("Parsed Hashes: %u/%u (%0.2f%%)", hashes_cnt, hashes_avail, ((float) hashes_cnt / hashes_avail) * 100);

              hashes_cnt++;

              // show / left

              if (show == 1) handle_show_request_lm (pot, pot_cnt, line_buf, line_len, lm_hash_left, lm_hash_right, sort_by_pot, out_fp);
              if (left == 1) handle_left_request_lm (pot, pot_cnt, line_buf, line_len, lm_hash_left, lm_hash_right, sort_by_pot, out_fp);
            }
            else
            {
              int parser_status = parse_func (hash_buf, hash_len, &hashes_buf[hashes_cnt]);

              if (parser_status < PARSER_GLOBAL_ZERO)
              {
                log_info ("WARNING: Hashfile '%s' in line %u (%s): %s", data.hashfile, line_num, line_buf, strparser (parser_status));

                continue;
              }

              if (data.quiet == 0) if ((hashes_cnt % 0x20000) == 0) log_info_nn ("Parsed Hashes: %u/%u (%0.2f%%)", hashes_cnt, hashes_avail, ((float) hashes_cnt / hashes_avail) * 100);

              if (show == 1) handle_show_request (pot, pot_cnt, line_buf, line_len, &hashes_buf[hashes_cnt], sort_by_pot, out_fp);
              if (left == 1) handle_left_request (pot, pot_cnt, line_buf, line_len, &hashes_buf[hashes_cnt], sort_by_pot, out_fp);

              hashes_cnt++;
            }
          }
          else
          {
            int parser_status = parse_func (hash_buf, hash_len, &hashes_buf[hashes_cnt]);

            if (parser_status < PARSER_GLOBAL_ZERO)
            {
              log_info ("WARNING: Hashfile '%s' in line %u (%s): %s", data.hashfile, line_num, line_buf, strparser (parser_status));

              continue;
            }

            if (data.quiet == 0) if ((hashes_cnt % 0x20000) == 0) log_info_nn ("Parsed Hashes: %u/%u (%0.2f%%)", hashes_cnt, hashes_avail, ((float) hashes_cnt / hashes_avail) * 100);

            if (show == 1) handle_show_request (pot, pot_cnt, line_buf, line_len, &hashes_buf[hashes_cnt], sort_by_pot, out_fp);
            if (left == 1) handle_left_request (pot, pot_cnt, line_buf, line_len, &hashes_buf[hashes_cnt], sort_by_pot, out_fp);

            hashes_cnt++;
          }
        }

        fclose (fp);

        if (data.quiet == 0) log_info_nn ("Parsed Hashes: %u/%u (%0.2f%%)", hashes_avail, hashes_avail, 100.00);

        if ((out_fp != NULL) && (out_fp != stdout)) fclose (out_fp);
      }
    }
    else
    {
      if (isSalted)
      {
        hashes_buf[0].salt->salt_len = 8;

        // special salt handling

        switch (hash_mode)
        {
          case  1500: hashes_buf[0].salt->salt_len = 2;
                      break;
          case  1731: hashes_buf[0].salt->salt_len = 4;
                      break;
          case  2410: hashes_buf[0].salt->salt_len = 4;
                      break;
          case  2500: memcpy (hashes_buf[0].salt->salt_buf, "hashcat.net", 11);
                      break;
          case  3100: hashes_buf[0].salt->salt_len = 1;
                      break;
          case  5000: hashes_buf[0].salt->keccak_mdlen = 32;
                      break;
          case  5800: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  6800: hashes_buf[0].salt->salt_len = 32;
                      break;
          case  8400: hashes_buf[0].salt->salt_len = 40;
                      break;
          case  8800: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  8900: hashes_buf[0].salt->salt_len = 16;
                      hashes_buf[0].salt->scrypt_N = 1024;
                      hashes_buf[0].salt->scrypt_r = 1;
                      hashes_buf[0].salt->scrypt_p = 1;
                      break;
          case  9100: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9300: hashes_buf[0].salt->salt_len = 14;
                      hashes_buf[0].salt->scrypt_N = 16384;
                      hashes_buf[0].salt->scrypt_r = 1;
                      hashes_buf[0].salt->scrypt_p = 1;
                      break;
          case  9400: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9500: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9600: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9700: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9710: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9720: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9800: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9810: hashes_buf[0].salt->salt_len = 16;
                      break;
          case  9820: hashes_buf[0].salt->salt_len = 16;
                      break;
          case 10300: hashes_buf[0].salt->salt_len = 12;
                      break;
          case 11500: hashes_buf[0].salt->salt_len = 4;
                      break;
          case 11600: hashes_buf[0].salt->salt_len = 4;
                      break;
          case 12400: hashes_buf[0].salt->salt_len = 4;
                      break;
          case 12500: hashes_buf[0].salt->salt_len = 8;
                      break;
          case 12600: hashes_buf[0].salt->salt_len = 64;
                      break;
        }

        // special esalt handling

        switch (hash_mode)
        {
          case  2500: ((wpa_t *)     hashes_buf[0].esalt)->eapol_size   = 128;
                      break;
          case  5300: ((ikepsk_t *)  hashes_buf[0].esalt)->nr_len       = 1;
                      ((ikepsk_t *)  hashes_buf[0].esalt)->msg_len      = 1;
                      break;
          case  5400: ((ikepsk_t *)  hashes_buf[0].esalt)->nr_len       = 1;
                      ((ikepsk_t *)  hashes_buf[0].esalt)->msg_len      = 1;
                      break;
          case  5500: ((netntlm_t *) hashes_buf[0].esalt)->user_len     = 1;
                      ((netntlm_t *) hashes_buf[0].esalt)->domain_len   = 1;
                      ((netntlm_t *) hashes_buf[0].esalt)->srvchall_len = 1;
                      ((netntlm_t *) hashes_buf[0].esalt)->clichall_len = 1;
                      break;
          case  5600: ((netntlm_t *) hashes_buf[0].esalt)->user_len     = 1;
                      ((netntlm_t *) hashes_buf[0].esalt)->domain_len   = 1;
                      ((netntlm_t *) hashes_buf[0].esalt)->srvchall_len = 1;
                      ((netntlm_t *) hashes_buf[0].esalt)->clichall_len = 1;
                      break;
          case  7300: ((rakp_t *)    hashes_buf[0].esalt)->salt_len     = 32;
                      break;
          case 10400: ((pdf_t *)     hashes_buf[0].esalt)->id_len       = 16;
                      ((pdf_t *)     hashes_buf[0].esalt)->o_len        = 32;
                      ((pdf_t *)     hashes_buf[0].esalt)->u_len        = 32;
                      break;
          case 10410: ((pdf_t *)     hashes_buf[0].esalt)->id_len       = 16;
                      ((pdf_t *)     hashes_buf[0].esalt)->o_len        = 32;
                      ((pdf_t *)     hashes_buf[0].esalt)->u_len        = 32;
                      break;
          case 10420: ((pdf_t *)     hashes_buf[0].esalt)->id_len       = 16;
                      ((pdf_t *)     hashes_buf[0].esalt)->o_len        = 32;
                      ((pdf_t *)     hashes_buf[0].esalt)->u_len        = 32;
                      break;
          case 10500: ((pdf_t *)     hashes_buf[0].esalt)->id_len       = 16;
                      ((pdf_t *)     hashes_buf[0].esalt)->o_len        = 32;
                      ((pdf_t *)     hashes_buf[0].esalt)->u_len        = 32;
                      break;
          case 10600: ((pdf_t *)     hashes_buf[0].esalt)->id_len       = 16;
                      ((pdf_t *)     hashes_buf[0].esalt)->o_len        = 127;
                      ((pdf_t *)     hashes_buf[0].esalt)->u_len        = 127;
                      break;
          case 10700: ((pdf_t *)     hashes_buf[0].esalt)->id_len       = 16;
                      ((pdf_t *)     hashes_buf[0].esalt)->o_len        = 127;
                      ((pdf_t *)     hashes_buf[0].esalt)->u_len        = 127;
                      break;
          case 11600: ((seven_zip_t *) hashes_buf[0].esalt)->iv_len      = 16;
                      ((seven_zip_t *) hashes_buf[0].esalt)->data_len    = 112;
                      ((seven_zip_t *) hashes_buf[0].esalt)->unpack_size = 112;
                      break;
        }
      }

      // set hashfile

      switch (hash_mode)
      {
        case 5200:  data.hashfile = mystrdup ("hashcat.psafe3");
                    break;
        case 5300:  data.hashfile = mystrdup ("hashcat.ikemd5");
                    break;
        case 5400:  data.hashfile = mystrdup ("hashcat.ikesha1");
                    break;
        case 6211:
        case 6212:
        case 6213:
        case 6221:
        case 6222:
        case 6223:
        case 6231:
        case 6232:
        case 6233:
        case 6241:
        case 6242:
        case 6243:  data.hashfile = mystrdup ("hashcat.tc");
                    break;
        case 6600:  data.hashfile = mystrdup ("hashcat.agilekey");
                    break;
        case 8200:  data.hashfile = mystrdup ("hashcat.cloudkey");
                    break;
        case 9000:  data.hashfile = mystrdup ("hashcat.psafe2");
                    break;
      }

      // set default iterations

      switch (hash_mode)
      {
        case   400:  hashes_buf[0].salt->salt_iter = ROUNDS_PHPASS;
                     break;
        case   500:  hashes_buf[0].salt->salt_iter = ROUNDS_MD5CRYPT;
                     break;
        case   501:  hashes_buf[0].salt->salt_iter = ROUNDS_MD5CRYPT;
                     break;
        case  1600:  hashes_buf[0].salt->salt_iter = ROUNDS_MD5CRYPT;
                     break;
        case  1800:  hashes_buf[0].salt->salt_iter = ROUNDS_SHA512CRYPT;
                     break;
        case  2100:  hashes_buf[0].salt->salt_iter = ROUNDS_DCC2;
                     break;
        case  2500:  hashes_buf[0].salt->salt_iter = ROUNDS_WPA2;
                     break;
        case  3200:  hashes_buf[0].salt->salt_iter = ROUNDS_BCRYPT;
                     break;
        case  5200:  hashes_buf[0].salt->salt_iter = ROUNDS_PSAFE3;
                     break;
        case  5800:  hashes_buf[0].salt->salt_iter = ROUNDS_ANDROIDPIN - 1;
                     break;
        case  6211:
        case  6212:
        case  6213:  hashes_buf[0].salt->salt_iter = ROUNDS_TRUECRYPT_2K;
                     break;
        case  6221:
        case  6222:
        case  6223:  hashes_buf[0].salt->salt_iter = ROUNDS_TRUECRYPT_1K;
                     break;
        case  6231:
        case  6232:
        case  6233:  hashes_buf[0].salt->salt_iter = ROUNDS_TRUECRYPT_1K;
                     break;
        case  6241:
        case  6242:
        case  6243:  hashes_buf[0].salt->salt_iter = ROUNDS_TRUECRYPT_1K;
                     break;
        case  6300:  hashes_buf[0].salt->salt_iter = ROUNDS_MD5CRYPT;
                     break;
        case  6400:  hashes_buf[0].salt->salt_iter = ROUNDS_SHA256AIX;
                     break;
        case  6500:  hashes_buf[0].salt->salt_iter = ROUNDS_SHA512AIX;
                     break;
        case  6700:  hashes_buf[0].salt->salt_iter = ROUNDS_SHA1AIX;
                     break;
        case  6600:  hashes_buf[0].salt->salt_iter = ROUNDS_AGILEKEY;
                     break;
        case  6800:  hashes_buf[0].salt->salt_iter = ROUNDS_LASTPASS;
                     break;
        case  7100:  hashes_buf[0].salt->salt_iter = ROUNDS_SHA512OSX;
                     break;
        case  7200:  hashes_buf[0].salt->salt_iter = ROUNDS_GRUB;
                     break;
        case  7400:  hashes_buf[0].salt->salt_iter = ROUNDS_SHA256CRYPT;
                     break;
        case  7900:  hashes_buf[0].salt->salt_iter = ROUNDS_DRUPAL7;
                     break;
        case  8200:  hashes_buf[0].salt->salt_iter = ROUNDS_CLOUDKEY;
                     break;
        case  8300:  hashes_buf[0].salt->salt_iter = ROUNDS_NSEC3;
                     break;
        case  8800:  hashes_buf[0].salt->salt_iter = ROUNDS_ANDROIDFDE;
                     break;
        case  8900:  hashes_buf[0].salt->salt_iter = 1;
                     break;
        case  9000:  hashes_buf[0].salt->salt_iter = ROUNDS_PSAFE2;
                     break;
        case  9100:  hashes_buf[0].salt->salt_iter = ROUNDS_LOTUS8;
                     break;
        case  9200:  hashes_buf[0].salt->salt_iter = ROUNDS_CISCO8;
                     break;
        case  9300:  hashes_buf[0].salt->salt_iter = 1;
                     break;
        case  9400:  hashes_buf[0].salt->salt_iter = ROUNDS_OFFICE2007;
                     break;
        case  9500:  hashes_buf[0].salt->salt_iter = ROUNDS_OFFICE2010;
                     break;
        case  9600:  hashes_buf[0].salt->salt_iter = ROUNDS_OFFICE2013;
                     break;
        case 10000:  hashes_buf[0].salt->salt_iter = ROUNDS_DJANGOPBKDF2;
                     break;
        case 10300:  hashes_buf[0].salt->salt_iter = ROUNDS_SAPH_SHA1 - 1;
                     break;
        case 10500:  hashes_buf[0].salt->salt_iter = ROUNDS_PDF14;
                     break;
        case 10700:  hashes_buf[0].salt->salt_iter = ROUNDS_PDF17L8;
                     break;
        case 10900:  hashes_buf[0].salt->salt_iter = ROUNDS_PBKDF2_SHA256 - 1;
                     break;
        case 11300:  hashes_buf[0].salt->salt_iter = ROUNDS_BITCOIN_WALLET - 1;
                     break;
        case 11600:  hashes_buf[0].salt->salt_iter = ROUNDS_SEVEN_ZIP;
                     break;
        case 11900:  hashes_buf[0].salt->salt_iter = ROUNDS_PBKDF2_MD5 - 1;
                     break;
        case 12000:  hashes_buf[0].salt->salt_iter = ROUNDS_PBKDF2_SHA1 - 1;
                     break;
        case 12100:  hashes_buf[0].salt->salt_iter = ROUNDS_PBKDF2_SHA512 - 1;
                     break;
        case 12200:  hashes_buf[0].salt->salt_iter = ROUNDS_ECRYPTFS - 1;
                     break;
        case 12300:  hashes_buf[0].salt->salt_iter = ROUNDS_ORACLET - 1;
                     break;
        case 12400:  hashes_buf[0].salt->salt_iter = ROUNDS_BSDICRYPT - 1;
                     break;
        case 12500:  hashes_buf[0].salt->salt_iter = ROUNDS_RAR3;
                     break;
        case 12700:  hashes_buf[0].salt->salt_iter = ROUNDS_MYWALLET;
                     break;
        case 12800:  hashes_buf[0].salt->salt_iter = ROUNDS_MS_DRSR - 1;
                     break;
      }

      // set special tuning for benchmark-mode 1

      if (benchmark_mode == 1)
      {
        gpu_loops *= 8;
        gpu_accel *= 4;

        switch (hash_mode)
        {
          case   400:  gpu_loops = ROUNDS_PHPASS;
                       gpu_accel = 32;
                       break;
          case   500:  gpu_loops = ROUNDS_MD5CRYPT;
                       gpu_accel = 32;
                       break;
          case   501:  gpu_loops = ROUNDS_MD5CRYPT;
                       gpu_accel = 32;
                       break;
          case  1600:  gpu_loops = ROUNDS_MD5CRYPT;
                       gpu_accel = 32;
                       break;
          case  1800:  gpu_loops = ROUNDS_SHA512CRYPT;
                       gpu_accel = 8;
                       break;
          case  2100:  gpu_loops = ROUNDS_DCC2;
                       gpu_accel = 16;
                       break;
          case  2500:  gpu_loops = ROUNDS_WPA2;
                       gpu_accel = 32;
                       break;
          case  3200:  gpu_loops = ROUNDS_BCRYPT;
                       gpu_accel = 2;
                       break;
          case  5200:  gpu_loops = ROUNDS_PSAFE3;
                       gpu_accel = 16;
                       break;
          case  5800:  gpu_loops = ROUNDS_ANDROIDPIN;
                       gpu_accel = 16;
                       break;
          case  6211:  gpu_loops = ROUNDS_TRUECRYPT_2K;
                       gpu_accel = 64;
                       break;
          case  6212:  gpu_loops = ROUNDS_TRUECRYPT_2K;
                       gpu_accel = 32;
                       break;
          case  6213:  gpu_loops = ROUNDS_TRUECRYPT_2K;
                       gpu_accel = 32;
                       break;
          case  6221:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 8;
                       break;
          case  6222:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 8;
                       break;
          case  6223:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 8;
                       break;
          case  6231:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 8;
                       break;
          case  6232:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 8;
                       break;
          case  6233:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 8;
                       break;
          case  6241:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 128;
                       break;
          case  6242:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 64;
                       break;
          case  6243:  gpu_loops = ROUNDS_TRUECRYPT_1K;
                       gpu_accel = 64;
                       break;
          case  6300:  gpu_loops = ROUNDS_MD5CRYPT;
                       gpu_accel = 32;
                       break;
          case  6700:  gpu_loops = ROUNDS_SHA1AIX;
                       gpu_accel = 128;
                       break;
          case  6400:  gpu_loops = ROUNDS_SHA256AIX;
                       gpu_accel = 128;
                       break;
          case  6500:  gpu_loops = ROUNDS_SHA512AIX;
                       gpu_accel = 32;
                       break;
          case  6600:  gpu_loops = ROUNDS_AGILEKEY;
                       gpu_accel = 64;
                       break;
          case  6800:  gpu_loops = ROUNDS_LASTPASS;
                       gpu_accel = 64;
                       break;
          case  7100:  gpu_loops = ROUNDS_SHA512OSX;
                       gpu_accel = 2;
                       break;
          case  7200:  gpu_loops = ROUNDS_GRUB;
                       gpu_accel = 2;
                       break;
          case  7400:  gpu_loops = ROUNDS_SHA256CRYPT;
                       gpu_accel = 4;
                       break;
          case  7900:  gpu_loops = ROUNDS_DRUPAL7;
                       gpu_accel = 8;
                       break;
          case  8200:  gpu_loops = ROUNDS_CLOUDKEY;
                       gpu_accel = 2;
                       break;
          case  8800:  gpu_loops = ROUNDS_ANDROIDFDE;
                       gpu_accel = 32;
                       break;
          case  8900:  gpu_loops = 1;
                       gpu_accel = 64;
                       break;
          case  9000:  gpu_loops = ROUNDS_PSAFE2;
                       gpu_accel = 16;
                       break;
          case  9100:  gpu_loops = ROUNDS_LOTUS8;
                       gpu_accel = 64;
                       break;
          case  9200:  gpu_loops = ROUNDS_CISCO8;
                       gpu_accel = 8;
                       break;
          case  9300:  gpu_loops = 1;
                       gpu_accel = 4;
                       break;
          case  9400:  gpu_loops = ROUNDS_OFFICE2007;
                       gpu_accel = 32;
                       break;
          case  9500:  gpu_loops = ROUNDS_OFFICE2010;
                       gpu_accel = 32;
                       break;
          case  9600:  gpu_loops = ROUNDS_OFFICE2013;
                       gpu_accel = 4;
                       break;
          case 10000:  gpu_loops = ROUNDS_DJANGOPBKDF2;
                       gpu_accel = 8;
                       break;
          case 10300:  gpu_loops = ROUNDS_SAPH_SHA1;
                       gpu_accel = 16;
                       break;
          case 10500:  gpu_loops = ROUNDS_PDF14;
                       gpu_accel = 256;
                       break;
          case 10700:  gpu_loops = ROUNDS_PDF17L8;
                       gpu_accel = 8;
                       break;
          case 10900:  gpu_loops = ROUNDS_PBKDF2_SHA256;
                       gpu_accel = 8;
                       break;
          case 11300:  gpu_loops = ROUNDS_BITCOIN_WALLET;
                       gpu_accel = 2;
                       break;
          case 11600:  gpu_loops = ROUNDS_SEVEN_ZIP;
                       gpu_accel = 4;
                       break;
          case 11900:  gpu_loops = ROUNDS_PBKDF2_MD5;
                       gpu_accel = 8;
                       break;
          case 12000:  gpu_loops = ROUNDS_PBKDF2_SHA1;
                       gpu_accel = 8;
                       break;
          case 12100:  gpu_loops = ROUNDS_PBKDF2_SHA512;
                       gpu_accel = 8;
                       break;
          case 12200:  gpu_loops = ROUNDS_ECRYPTFS;
                       gpu_accel = 8;
                       break;
          case 12300:  gpu_loops = ROUNDS_ORACLET;
                       gpu_accel = 8;
                       break;
          case 12500:  gpu_loops = ROUNDS_RAR3;
                       gpu_accel = 32;
                       break;
          case 12700:  gpu_loops = ROUNDS_MYWALLET;
                       gpu_accel = 512;
                       break;
          case 12800:  gpu_loops = ROUNDS_MS_DRSR;
                       gpu_accel = 512;
                       break;
        }

        // some algorithm collide too fast, make that impossible

        switch (hash_mode)
        {
          case 11500:  ((uint *) digests_buf)[1] = 1;
                       break;
        }

        if (gpu_loops > 1024) gpu_loops = 1024;
        if (gpu_accel >  256) gpu_accel =  256; // causes memory problems otherwise
      }

      if ((opts_type & OPTS_TYPE_PT_BITSLICE) && (attack_mode == ATTACK_MODE_BF))
      {
        gpu_loops = 1024;
      }

      if (hash_mode == 12500)
      {
        gpu_loops = ROUNDS_RAR3 / 16;
      }

      data.gpu_accel = gpu_accel;
      data.gpu_loops = gpu_loops;

      hashes_cnt = 1;
    }

    if (show == 1 || left == 1)
    {
      for (uint i = 0; i < pot_cnt; i++)
      {
        pot_t *pot_ptr = &pot[i];

        hash_t *hashes_buf = &pot_ptr->hash;

        local_free (hashes_buf->digest);

        if (isSalted)
        {
          local_free (hashes_buf->salt);
        }
      }

      local_free (pot);

      if (data.quiet == 0) log_info_nn ("");

      return (0);
    }

    if (keyspace == 0)
    {
      if (hashes_cnt == 0)
      {
        log_error ("ERROR: No hashes loaded");

        return (-1);
      }
    }

    /**
     * Sanity check for hashfile vs outfile (should not point to the same physical file)
     */

    if (data.outfile != NULL)
    {
      if (data.hashfile != NULL)
      {
        #ifdef _POSIX
        struct stat tmpstat_outfile;
        struct stat tmpstat_hashfile;
        #endif

        #ifdef _WIN
        struct stat64 tmpstat_outfile;
        struct stat64 tmpstat_hashfile;
        #endif

        FILE *tmp_outfile_fp = fopen (data.outfile, "r");

        if (tmp_outfile_fp)
        {
          #ifdef _POSIX
          fstat (fileno (tmp_outfile_fp), &tmpstat_outfile);
          #endif

          #ifdef _WIN
          _fstat64 (fileno (tmp_outfile_fp), &tmpstat_outfile);
          #endif

          fclose (tmp_outfile_fp);
        }

        FILE *tmp_hashfile_fp = fopen (data.hashfile, "r");

        if (tmp_hashfile_fp)
        {
          #ifdef _POSIX
          fstat (fileno (tmp_hashfile_fp), &tmpstat_hashfile);
          #endif

          #ifdef _WIN
          _fstat64 (fileno (tmp_hashfile_fp), &tmpstat_hashfile);
          #endif

          fclose (tmp_hashfile_fp);
        }

        if (tmp_outfile_fp && tmp_outfile_fp)
        {
          tmpstat_outfile.st_mode     = 0;
          tmpstat_outfile.st_nlink    = 0;
          tmpstat_outfile.st_uid      = 0;
          tmpstat_outfile.st_gid      = 0;
          tmpstat_outfile.st_rdev     = 0;
          tmpstat_outfile.st_atime    = 0;

          tmpstat_hashfile.st_mode    = 0;
          tmpstat_hashfile.st_nlink   = 0;
          tmpstat_hashfile.st_uid     = 0;
          tmpstat_hashfile.st_gid     = 0;
          tmpstat_hashfile.st_rdev    = 0;
          tmpstat_hashfile.st_atime   = 0;

          #ifdef _POSIX
          tmpstat_outfile.st_blksize  = 0;
          tmpstat_outfile.st_blocks   = 0;

          tmpstat_hashfile.st_blksize = 0;
          tmpstat_hashfile.st_blocks  = 0;
          #endif

          #ifdef _POSIX
          if (memcmp (&tmpstat_outfile, &tmpstat_hashfile, sizeof (struct stat)) == 0)
          {
            log_error ("ERROR: Hashfile and Outfile are not allowed to point to the same file");

            return (-1);
          }
          #endif

          #ifdef _WIN
          if (memcmp (&tmpstat_outfile, &tmpstat_hashfile, sizeof (struct stat64)) == 0)
          {
            log_error ("ERROR: Hashfile and Outfile are not allowed to point to the same file");

            return (-1);
          }
          #endif
        }
      }
    }

    /**
     * Remove duplicates
     */

    if (data.quiet == 0) log_info_nn ("Removing duplicate hashes...");

    if (isSalted)
    {
      qsort (hashes_buf, hashes_cnt, sizeof (hash_t), sort_by_hash);
    }
    else
    {
      qsort (hashes_buf, hashes_cnt, sizeof (hash_t), sort_by_hash_no_salt);
    }

    uint hashes_cnt_orig = hashes_cnt;

    hashes_cnt = 1;

    for (uint hashes_pos = 1; hashes_pos < hashes_cnt_orig; hashes_pos++)
    {
      if (isSalted)
      {
        if (sort_by_salt (hashes_buf[hashes_pos].salt, hashes_buf[hashes_pos - 1].salt) == 0)
        {
          if (sort_by_digest (hashes_buf[hashes_pos].digest, hashes_buf[hashes_pos - 1].digest) == 0) continue;
        }
      }
      else
      {
        if (sort_by_digest (hashes_buf[hashes_pos].digest, hashes_buf[hashes_pos - 1].digest) == 0) continue;
      }

      if (hashes_pos > hashes_cnt)
      {
        memcpy (&hashes_buf[hashes_cnt], &hashes_buf[hashes_pos], sizeof (hash_t));
      }

      hashes_cnt++;
    }

    /**
     * Potfile removes
     */

    uint potfile_remove_cracks = 0;

    if (potfile_disable == 0)
    {
      hash_t hash_buf;

      hash_buf.digest    = mymalloc (dgst_size);
      hash_buf.salt      = NULL;
      hash_buf.esalt     = NULL;
      hash_buf.hash_info = NULL;
      hash_buf.cracked   = 0;

      if (isSalted)
      {
        hash_buf.salt = (salt_t *) mymalloc (sizeof (salt_t));
      }

      if (esalt_size)
      {
        hash_buf.esalt = mymalloc (esalt_size);
      }

      if (quiet == 0) log_info_nn ("Comparing hashes with potfile entries...");

      // no solution for these special hash types (for instane because they use hashfile in output etc)
      if ((hash_mode != 5200) &&
          !((hash_mode >= 6200) && (hash_mode <= 6299)) &&
          (hash_mode != 9000))
      {
        FILE *fp = fopen (potfile, "rb");

        if (fp != NULL)
        {
          while (!feof (fp))
          {
            char line_buf[BUFSIZ];

            memset (line_buf, 0, BUFSIZ);

            char *ptr = fgets (line_buf, BUFSIZ - 1, fp);

            if (ptr == NULL) break;

            int line_len = strlen (line_buf);

            if (line_len == 0) continue;

            int iter = MAX_CUT_TRIES;

            for (int i = line_len - 1; i && iter; i--, line_len--)
            {
              if (line_buf[i] != ':') continue;

              if (isSalted)
              {
                memset (hash_buf.salt, 0, sizeof (salt_t));
              }

              hash_t *found = NULL;

              if (hash_mode == 6800)
              {
                if (i < 48) // 48 = 12 * uint in salt_buf[]
                {
                  // manipulate salt_buf
                  memcpy (hash_buf.salt->salt_buf, line_buf, i);

                  hash_buf.salt->salt_len = i;

                  found = (hash_t *) bsearch (&hash_buf, hashes_buf, hashes_cnt, sizeof (hash_t), sort_by_hash_t_salt);
                }
              }
              else if (hash_mode == 2500)
              {
                if (i < 48) // 48 = 12 * uint in salt_buf[]
                {
                  // here we have in line_buf: ESSID:MAC1:MAC2   (without the plain)
                  // manipulate salt_buf

                  // to be safe work with a copy (because of line_len loop, i etc)

                  char line_buf_cpy[BUFSIZ];
                  memset (line_buf_cpy, 0, BUFSIZ);

                  memset (line_buf_cpy, 0, sizeof (line_buf_cpy));

                  memcpy (line_buf_cpy, line_buf, i);

                  char *mac2_pos = strrchr (line_buf_cpy, ':');

                  if (mac2_pos == NULL) continue;

                  mac2_pos[0] = 0;
                  mac2_pos++;

                  if (strlen (mac2_pos) != 12) continue;

                  char *mac1_pos = strrchr (line_buf_cpy, ':');

                  if (mac1_pos == NULL) continue;

                  mac1_pos[0] = 0;
                  mac1_pos++;

                  if (strlen (mac1_pos) != 12) continue;

                  uint essid_length = mac1_pos - line_buf_cpy - 1;

                  // here we need the ESSID
                  memcpy (hash_buf.salt->salt_buf, line_buf_cpy, essid_length);

                  hash_buf.salt->salt_len = essid_length;

                  found = (hash_t *) bsearch (&hash_buf, hashes_buf, hashes_cnt, sizeof (hash_t), sort_by_hash_t_salt_hccap);

                  if (found)
                  {
                    wpa_t *wpa = (wpa_t *) found->esalt;

                    uint pke[25];

                    char *pke_ptr = (char *) pke;

                    for (uint i = 0; i < 25; i++)
                    {
                      pke[i] = byte_swap_32 (wpa->pke[i]);
                    }

                    unsigned char mac1[6];
                    unsigned char mac2[6];

                    memcpy (mac1, pke_ptr + 23, 6);
                    memcpy (mac2, pke_ptr + 29, 6);

                    // compare hex string(s) vs binary MAC address(es)

                    for (uint i = 0, j = 0; i < 6; i++, j += 2)
                    {
                      if (mac1[i] != (unsigned char) hex_to_char (&mac1_pos[j]))
                      {
                        found = NULL;
                        break;
                      }
                    }

                    // early skip ;)
                    if (!found) continue;

                    for (uint i = 0, j = 0; i < 6; i++, j += 2)
                    {
                      if (mac2[i] != (unsigned char) hex_to_char (&mac2_pos[j]))
                      {
                        found = NULL;
                        break;
                      }
                    }
                  }
                }
              }
              else
              {
                int parser_status = parse_func (line_buf, line_len - 1, &hash_buf);

                if (parser_status == PARSER_OK)
                {
                  if (isSalted)
                  {
                    found = (hash_t *) bsearch (&hash_buf, hashes_buf, hashes_cnt, sizeof (hash_t), sort_by_hash);
                  }
                  else
                  {
                    found = (hash_t *) bsearch (&hash_buf, hashes_buf, hashes_cnt, sizeof (hash_t), sort_by_hash_no_salt);
                  }
                }
              }

              if (found == NULL) continue;

              if (!found->cracked) potfile_remove_cracks++;

              found->cracked = 1;

              if (found) break;

              iter--;
            }
          }

          fclose (fp);
        }
      }

      if (esalt_size)
      {
        local_free (hash_buf.esalt);
      }

      if (isSalted)
      {
        local_free (hash_buf.salt);
      }

      local_free (hash_buf.digest);
    }

    /**
     * Now generate all the buffers required for later
     */

    void   *digests_buf_new = (void *) mycalloc (hashes_avail, dgst_size);

    salt_t *salts_buf_new  = NULL;
    void   *esalts_buf_new = NULL;

    if (isSalted)
    {
      salts_buf_new = (salt_t *) mycalloc (hashes_avail, sizeof (salt_t));

      if (esalt_size)
      {
        esalts_buf_new = (void *) mycalloc (hashes_avail, esalt_size);
      }
    }
    else
    {
      salts_buf_new = (salt_t *) mycalloc (1, sizeof (salt_t));
    }

    if (data.quiet == 0) log_info_nn ("Structuring salts for cracking task...");

    uint digests_cnt  = hashes_cnt;
    uint digests_done = 0;

    uint size_digests = digests_cnt * dgst_size;
    uint size_shown   = digests_cnt * sizeof (uint);

    uint *digests_shown     = (uint *) mymalloc (size_shown);
    uint *digests_shown_tmp = (uint *) mymalloc (size_shown);

    uint salts_cnt   = 0;
    uint salts_done  = 0;

    hashinfo_t **hash_info = NULL;

    if ((username && (remove || show)) || (opts_type & OPTS_TYPE_HASH_COPY))
    {
      hash_info = (hashinfo_t**) mymalloc (hashes_cnt * sizeof (hashinfo_t *));

      if (username && (remove || show))
      {
        uint user_pos;

        for (user_pos = 0; user_pos < hashes_cnt; user_pos++)
        {
          hash_info[user_pos] = (hashinfo_t*) mycalloc (hashes_cnt, sizeof (hashinfo_t));

          hash_info[user_pos]->user = (user_t*) mymalloc (sizeof (user_t));
        }
      }
    }

    uint *salts_shown = (uint *) mymalloc (size_shown);

    salt_t *salt_buf;

    {
      // copied from inner loop

      salt_buf = &salts_buf_new[salts_cnt];

      memcpy (salt_buf, hashes_buf[0].salt, sizeof (salt_t));

      if (esalt_size)
      {
        memcpy (((char *) esalts_buf_new) + (salts_cnt * esalt_size), hashes_buf[0].esalt, esalt_size);
      }

      salt_buf->digests_cnt    = 0;
      salt_buf->digests_done   = 0;
      salt_buf->digests_offset = 0;

      salts_cnt++;
    }

    if (hashes_buf[0].cracked == 1)
    {
      digests_shown[0] = 1;

      digests_done++;

      salt_buf->digests_done++;
    }

    salt_buf->digests_cnt++;

    memcpy (((char *) digests_buf_new) + (0 * dgst_size), hashes_buf[0].digest, dgst_size);

    if ((username && (remove || show)) || (opts_type & OPTS_TYPE_HASH_COPY))
    {
      hash_info[0] = hashes_buf[0].hash_info;
    }

    // copy from inner loop

    for (uint hashes_pos = 1; hashes_pos < hashes_cnt; hashes_pos++)
    {
      if (isSalted)
      {
        if (sort_by_salt (hashes_buf[hashes_pos].salt, hashes_buf[hashes_pos - 1].salt) != 0)
        {
          salt_buf = &salts_buf_new[salts_cnt];

          memcpy (salt_buf, hashes_buf[hashes_pos].salt, sizeof (salt_t));

          if (esalt_size)
          {
            memcpy (((char *) esalts_buf_new) + (salts_cnt * esalt_size), hashes_buf[hashes_pos].esalt, esalt_size);
          }

          salt_buf->digests_cnt    = 0;
          salt_buf->digests_done   = 0;
          salt_buf->digests_offset = hashes_pos;

          salts_cnt++;
        }
      }

      if (hashes_buf[hashes_pos].cracked == 1)
      {
        digests_shown[hashes_pos] = 1;

        digests_done++;

        salt_buf->digests_done++;
      }

      salt_buf->digests_cnt++;

      memcpy (((char *) digests_buf_new) + (hashes_pos * dgst_size), hashes_buf[hashes_pos].digest, dgst_size);

      if ((username && (remove || show)) || (opts_type & OPTS_TYPE_HASH_COPY))
      {
        hash_info[hashes_pos] = hashes_buf[hashes_pos].hash_info;
      }
    }

    for (uint salt_pos = 0; salt_pos < salts_cnt; salt_pos++)
    {
      salt_t *salt_buf = &salts_buf_new[salt_pos];

      if (salt_buf->digests_done == salt_buf->digests_cnt)
      {
        salts_shown[salt_pos] = 1;

        salts_done++;
      }

      if (salts_done == salts_cnt) data.devices_status = STATUS_CRACKED;
    }

    local_free (digests_buf);
    local_free (salts_buf);
    local_free (esalts_buf);

    digests_buf = digests_buf_new;
    salts_buf   = salts_buf_new;
    esalts_buf  = esalts_buf_new;

    local_free (hashes_buf);

    /**
     * special modification not set from parser
     */

    switch (hash_mode)
    {
      case  6211: salts_buf->truecrypt_mdlen = 1 * 512; break;
      case  6212: salts_buf->truecrypt_mdlen = 2 * 512; break;
      case  6213: salts_buf->truecrypt_mdlen = 3 * 512; break;
      case  6221: salts_buf->truecrypt_mdlen = 1 * 512; break;
      case  6222: salts_buf->truecrypt_mdlen = 2 * 512; break;
      case  6223: salts_buf->truecrypt_mdlen = 3 * 512; break;
      case  6231: salts_buf->truecrypt_mdlen = 1 * 512; break;
      case  6232: salts_buf->truecrypt_mdlen = 2 * 512; break;
      case  6233: salts_buf->truecrypt_mdlen = 3 * 512; break;
      case  6241: salts_buf->truecrypt_mdlen = 1 * 512; break;
      case  6242: salts_buf->truecrypt_mdlen = 2 * 512; break;
      case  6243: salts_buf->truecrypt_mdlen = 3 * 512; break;
    }

    if (truecrypt_keyfiles)
    {
      uint *keyfile_buf = ((tc_t *) esalts_buf)->keyfile_buf;

      char *keyfiles = strdup (truecrypt_keyfiles);

      char *keyfile = strtok (keyfiles, ",");

      do
      {
        truecrypt_crc32 (keyfile, (unsigned char *) keyfile_buf);

      } while ((keyfile = strtok (NULL, ",")) != NULL);

      free (keyfiles);
    }

    data.digests_cnt        = digests_cnt;
    data.digests_done       = digests_done;
    data.digests_buf        = digests_buf;
    data.digests_shown      = digests_shown;
    data.digests_shown_tmp  = digests_shown_tmp;

    data.salts_cnt          = salts_cnt;
    data.salts_done         = salts_done;
    data.salts_buf          = salts_buf;
    data.salts_shown        = salts_shown;

    data.esalts_buf         = esalts_buf;
    data.hash_info          = hash_info;

    /**
     * Automatic Optimizers
     */

    if (salts_cnt == 1)
      opti_type |= OPTI_TYPE_SINGLE_SALT;

    if (digests_cnt == 1)
      opti_type |= OPTI_TYPE_SINGLE_HASH;

    if (attack_exec == ATTACK_EXEC_ON_GPU)
      opti_type |= OPTI_TYPE_NOT_ITERATED;

    if (attack_mode == ATTACK_MODE_BF)
      opti_type |= OPTI_TYPE_BRUTE_FORCE;

    data.opti_type = opti_type;

    if (opti_type & OPTI_TYPE_BRUTE_FORCE)
    {
      if (opti_type & OPTI_TYPE_SINGLE_HASH)
      {
        if (opti_type & OPTI_TYPE_APPENDED_SALT)
        {
          if (opts_type & OPTS_TYPE_ST_ADD80)
          {
            opts_type &= ~OPTS_TYPE_ST_ADD80;
            opts_type |=  OPTS_TYPE_PT_ADD80;
          }

          if (opts_type & OPTS_TYPE_ST_ADDBITS14)
          {
            opts_type &= ~OPTS_TYPE_ST_ADDBITS14;
            opts_type |=  OPTS_TYPE_PT_ADDBITS14;
          }

          if (opts_type & OPTS_TYPE_ST_ADDBITS15)
          {
            opts_type &= ~OPTS_TYPE_ST_ADDBITS15;
            opts_type |=  OPTS_TYPE_PT_ADDBITS15;
          }
        }
      }
    }

    /**
     * generate bitmap tables
     */

    const uint bitmap_shift1 = 5;
    const uint bitmap_shift2 = 13;

    if (bitmap_max < bitmap_min) bitmap_max = bitmap_min;

    uint *bitmap_s1_a = (uint *) mymalloc ((1 << bitmap_max) * sizeof (uint));
    uint *bitmap_s1_b = (uint *) mymalloc ((1 << bitmap_max) * sizeof (uint));
    uint *bitmap_s1_c = (uint *) mymalloc ((1 << bitmap_max) * sizeof (uint));
    uint *bitmap_s1_d = (uint *) mymalloc ((1 << bitmap_max) * sizeof (uint));
    uint *bitmap_s2_a = (uint *) mymalloc ((1 << bitmap_max) * sizeof (uint));
    uint *bitmap_s2_b = (uint *) mymalloc ((1 << bitmap_max) * sizeof (uint));
    uint *bitmap_s2_c = (uint *) mymalloc ((1 << bitmap_max) * sizeof (uint));
    uint *bitmap_s2_d = (uint *) mymalloc ((1 << bitmap_max) * sizeof (uint));

    uint bitmap_bits;
    uint bitmap_nums;
    uint bitmap_mask;
    uint bitmap_size;

    for (bitmap_bits = bitmap_min; bitmap_bits < bitmap_max; bitmap_bits++)
    {
      if (data.quiet == 0) log_info_nn ("Generating bitmap tables with %u bits...", bitmap_bits);

      bitmap_nums = 1 << bitmap_bits;

      bitmap_mask = bitmap_nums - 1;

      bitmap_size = bitmap_nums * sizeof (uint);

      if ((hashes_cnt & bitmap_mask) == hashes_cnt) break;

      if (generate_bitmaps (digests_cnt, dgst_size, bitmap_shift1, (char *) data.digests_buf, bitmap_mask, bitmap_size, bitmap_s1_a, bitmap_s1_b, bitmap_s1_c, bitmap_s1_d, digests_cnt / 2) == 0x7fffffff) continue;
      if (generate_bitmaps (digests_cnt, dgst_size, bitmap_shift2, (char *) data.digests_buf, bitmap_mask, bitmap_size, bitmap_s1_a, bitmap_s1_b, bitmap_s1_c, bitmap_s1_d, digests_cnt / 2) == 0x7fffffff) continue;

      break;
    }

    bitmap_nums = 1 << bitmap_bits;

    bitmap_mask = bitmap_nums - 1;

    bitmap_size = bitmap_nums * sizeof (uint);

    generate_bitmaps (digests_cnt, dgst_size, bitmap_shift1, (char *) data.digests_buf, bitmap_mask, bitmap_size, bitmap_s1_a, bitmap_s1_b, bitmap_s1_c, bitmap_s1_d, -1);
    generate_bitmaps (digests_cnt, dgst_size, bitmap_shift2, (char *) data.digests_buf, bitmap_mask, bitmap_size, bitmap_s2_a, bitmap_s2_b, bitmap_s2_c, bitmap_s2_d, -1);

    /**
     * prepare quick rule
     */

    data.rule_buf_l = rule_buf_l;
    data.rule_buf_r = rule_buf_r;

    int rule_len_l = (int) strlen (rule_buf_l);
    int rule_len_r = (int) strlen (rule_buf_r);

    data.rule_len_l = rule_len_l;
    data.rule_len_r = rule_len_r;

    /**
     * load rules
     */

    uint *all_gpu_rules_cnt = NULL;

    gpu_rule_t **all_gpu_rules_buf = NULL;

    if (rp_files_cnt)
    {
      all_gpu_rules_cnt = (uint *) mycalloc (rp_files_cnt, sizeof (uint));

      all_gpu_rules_buf = (gpu_rule_t **) mycalloc (rp_files_cnt, sizeof (gpu_rule_t *));
    }

    char rule_buf[BUFSIZ];

    int rule_len = 0;

    for (uint i = 0; i < rp_files_cnt; i++)
    {
      uint gpu_rules_avail = 0;

      uint gpu_rules_cnt = 0;

      gpu_rule_t *gpu_rules_buf = NULL;

      char *rp_file = rp_files[i];

      char in[BLOCK_SIZE];
      char out[BLOCK_SIZE];

      FILE *fp = NULL;

      uint rule_line = 0;

      if ((fp = fopen (rp_file, "rb")) == NULL)
      {
        log_error ("ERROR: %s: %s", rp_file, strerror (errno));

        return (-1);
      }

      while (!feof (fp))
      {
        memset (rule_buf, 0, BUFSIZ);

        rule_len = fgetl (fp, rule_buf);

        rule_line++;

        if (rule_len == 0) continue;

        if (rule_buf[0] == '#') continue;

        if (gpu_rules_avail == gpu_rules_cnt)
        {
          gpu_rules_buf = (gpu_rule_t *) myrealloc (gpu_rules_buf, gpu_rules_avail * sizeof (gpu_rule_t), INCR_RULES * sizeof (gpu_rule_t));

          gpu_rules_avail += INCR_RULES;
        }

        memset (in,  0, BLOCK_SIZE);
        memset (out, 0, BLOCK_SIZE);

        int result = _old_apply_rule (rule_buf, rule_len, in, 1, out);

        if (result == -1)
        {
          log_info ("WARNING: Skipping invalid or unsupported rule in file %s in line %u: %s", rp_file, rule_line, rule_buf);

          continue;
        }

        if (cpu_rule_to_gpu_rule (rule_buf, rule_len, &gpu_rules_buf[gpu_rules_cnt]) == -1)
        {
          log_info ("WARNING: Cannot convert rule for use on GPU in file %s in line %u: %s", rp_file, rule_line, rule_buf);

          memset (&gpu_rules_buf[gpu_rules_cnt], 0, sizeof (gpu_rule_t)); // needs to be cleared otherwise we could have some remaining data

          continue;
        }

        /* its so slow
        if (rulefind (&gpu_rules_buf[gpu_rules_cnt], gpu_rules_buf, gpu_rules_cnt, sizeof (gpu_rule_t), sort_by_gpu_rule))
        {
          log_info ("Duplicate rule for use on GPU in file %s in line %u: %s", rp_file, rule_line, rule_buf);

          continue;
        }
        */

        gpu_rules_cnt++;
      }

      fclose (fp);

      all_gpu_rules_cnt[i] = gpu_rules_cnt;

      all_gpu_rules_buf[i] = gpu_rules_buf;
    }

    /**
     * merge rules or automatic rule generator
     */

    uint gpu_rules_cnt = 0;

    gpu_rule_t *gpu_rules_buf = NULL;

    if (attack_mode == ATTACK_MODE_STRAIGHT)
    {
      if (rp_files_cnt)
      {
        gpu_rules_cnt = 1;

        uint *repeats = (uint *) mycalloc (rp_files_cnt + 1, sizeof (uint));

        repeats[0] = gpu_rules_cnt;

        for (uint i = 0; i < rp_files_cnt; i++)
        {
          gpu_rules_cnt *= all_gpu_rules_cnt[i];

          repeats[i + 1] = gpu_rules_cnt;
        }

        gpu_rules_buf = (gpu_rule_t *) mycalloc (gpu_rules_cnt, sizeof (gpu_rule_t));

        memset (gpu_rules_buf, 0, gpu_rules_cnt * sizeof (gpu_rule_t));

        for (uint i = 0; i < gpu_rules_cnt; i++)
        {
          uint out_pos = 0;

          gpu_rule_t *out = &gpu_rules_buf[i];

          for (uint j = 0; j < rp_files_cnt; j++)
          {
            uint in_off = (i / repeats[j]) % all_gpu_rules_cnt[j];
            uint in_pos;

            gpu_rule_t *in = &all_gpu_rules_buf[j][in_off];

            for (in_pos = 0; in->cmds[in_pos]; in_pos++, out_pos++)
            {
              if (out_pos == RULES_MAX - 1)
              {
                // log_info ("WARNING: Truncating chaining of rule %d and rule %d as maximum number of function calls per rule exceeded", i, in_off);

                break;
              }

              out->cmds[out_pos] = in->cmds[in_pos];
            }
          }
        }

        local_free (repeats);
      }
      else if (rp_gen)
      {
        uint gpu_rules_avail = 0;

        while (gpu_rules_cnt < rp_gen)
        {
          if (gpu_rules_avail == gpu_rules_cnt)
          {
            gpu_rules_buf = (gpu_rule_t *) myrealloc (gpu_rules_buf, gpu_rules_avail * sizeof (gpu_rule_t), INCR_RULES * sizeof (gpu_rule_t));

            gpu_rules_avail += INCR_RULES;
          }

          memset (rule_buf, 0, BLOCK_SIZE);

          rule_len = (int) generate_random_rule (rule_buf, rp_gen_func_min, rp_gen_func_max);

          if (cpu_rule_to_gpu_rule (rule_buf, rule_len, &gpu_rules_buf[gpu_rules_cnt]) == -1) continue;

          gpu_rules_cnt++;
        }
      }
    }

    /**
     * generate NOP rules
     */

    if (gpu_rules_cnt == 0)
    {
      gpu_rules_buf = (gpu_rule_t *) mymalloc (sizeof (gpu_rule_t));

      gpu_rules_buf[gpu_rules_cnt].cmds[0] = RULE_OP_MANGLE_NOOP;

      gpu_rules_cnt++;
    }

    data.gpu_rules_cnt = gpu_rules_cnt;
    data.gpu_rules_buf = gpu_rules_buf;

    /**
     * platform
     */

    #ifdef _CUDA
    if (cuInit (0) != CUDA_SUCCESS)
    {
      log_error ("ERROR: No NVidia compatible platform found");

      return (-1);
    }
    #endif

    /**
     * devices get
     */

    uint devices_all_cnt = 0;

    #ifdef _CUDA
    CUdevice devices_all[DEVICES_MAX];
    CUdevice devices[DEVICES_MAX];

    hc_cuDeviceGetCount ((int *) &devices_all_cnt);

    for (uint i = 0; i < devices_all_cnt; i++)
    {
      hc_cuDeviceGet (&devices_all[i], i);
    }

    #elif _OCL
    cl_platform_id CL_platform = NULL;

    cl_platform_id CL_platforms[CL_PLATFORMS_MAX];

    uint CL_platforms_cnt = 0;

    hc_clGetPlatformIDs (CL_PLATFORMS_MAX, CL_platforms, &CL_platforms_cnt);

    for (uint i = 0; i < CL_platforms_cnt; i++)
    {
      char CL_platform_vendor[INFOSZ];

      memset (CL_platform_vendor, 0, sizeof (CL_platform_vendor));

      hc_clGetPlatformInfo (CL_platforms[i], CL_PLATFORM_VENDOR, sizeof (CL_platform_vendor), CL_platform_vendor, NULL);

      if ((strcmp (CL_platform_vendor, CL_VENDOR_AMD)   != 0)
       && (strcmp (CL_platform_vendor, CL_VENDOR_SDS)   != 0)
       && (strcmp (CL_platform_vendor, CL_VENDOR_APPLE) != 0)) continue;

      if (strcmp (CL_platform_vendor, CL_VENDOR_SDS) == 0) gpu_temp_disable = 1;

      CL_platform = CL_platforms[i];
    }

    if (CL_platform == NULL)
    {
      log_error ("ERROR: No AMD/SDS compatible platform found");

      return (-1);
    }

    cl_device_id devices_all[DEVICES_MAX];
    cl_device_id devices[DEVICES_MAX];

    hc_clGetDeviceIDs (CL_platform, CL_DEVICE_TYPE_GPU, DEVICES_MAX, devices_all, (uint *) &devices_all_cnt);

    #endif

    int hm_adapters_all = devices_all_cnt;

    hm_attrs_t hm_adapter_all[DEVICES_MAX];

    memset (hm_adapter_all, 0, sizeof (hm_adapter_all));

    if (gpu_temp_disable == 0)
    {
      #ifdef _CUDA
      #ifdef LINUX
      if (hc_NVML_nvmlInit () == NVML_SUCCESS)
      {
        HM_ADAPTER nvGPUHandle[DEVICES_MAX];

        int tmp_in = hm_get_adapter_index (nvGPUHandle);

        int tmp_out = 0;

        for (int i = 0; i < tmp_in; i++)
        {
          hm_adapter_all[tmp_out++].adapter_index = nvGPUHandle[i];
        }

        hm_adapters_all = tmp_out;

        for (int i = 0; i < tmp_out; i++)
        {
          unsigned int speed;

          if (nvmlDeviceGetFanSpeed (hm_adapter_all[i].adapter_index, &speed) != NVML_ERROR_NOT_SUPPORTED) hm_adapter_all[i].fan_supported = 1;
        }
      }
      #endif

      #ifdef WIN
      if (NvAPI_Initialize () == NVAPI_OK)
      {
        HM_ADAPTER nvGPUHandle[DEVICES_MAX];

        int tmp_in = hm_get_adapter_index (nvGPUHandle);

        int tmp_out = 0;

        for (int i = 0; i < tmp_in; i++)
        {
          hm_adapter_all[tmp_out++].adapter_index = nvGPUHandle[i];
        }

        hm_adapters_all = tmp_out;

        for (int i = 0; i < tmp_out; i++)
        {
          NvU32 speed;

          if (NvAPI_GPU_GetTachReading (hm_adapter_all[i].adapter_index, &speed) != NVAPI_NOT_SUPPORTED) hm_adapter_all[i].fan_supported = 1;
        }
      }
      #endif
      #endif

      #ifdef _OCL
      #ifndef OSX
      HM_LIB hm_dll = hm_init ();

      data.hm_dll = hm_dll;

      if (hc_ADL_Main_Control_Create (hm_dll, ADL_Main_Memory_Alloc, 0) == ADL_OK)
      {
        // total number of adapters

        int hm_adapters_num;

        if (get_adapters_num (hm_dll, &hm_adapters_num) != 0) return (-1);

        // adapter info

        LPAdapterInfo lpAdapterInfo = hm_get_adapter_info (hm_dll, hm_adapters_num);

        if (lpAdapterInfo == NULL) return (-1);

        // get a list (of ids of) valid/usable adapters

        int num_adl_adapters = 0;

        uint32_t *valid_adl_device_list = hm_get_list_valid_adl_adapters (hm_adapters_num, &num_adl_adapters, lpAdapterInfo);

        if (num_adl_adapters > 0)
        {
          hc_thread_mutex_lock (mux_adl);

          // hm_get_opencl_busid_devid (hm_adapter_all, devices_all_cnt, devices_all);

          hm_get_adapter_index (hm_adapter_all, valid_adl_device_list, num_adl_adapters, lpAdapterInfo);

          hm_get_overdrive_version  (hm_dll, hm_adapter_all, valid_adl_device_list, num_adl_adapters, lpAdapterInfo);
          hm_check_fanspeed_control (hm_dll, hm_adapter_all, valid_adl_device_list, num_adl_adapters, lpAdapterInfo);

          hc_thread_mutex_unlock (mux_adl);
        }

        hm_adapters_all = num_adl_adapters;

        myfree (valid_adl_device_list);
        myfree (lpAdapterInfo);
      }
      #endif
      #endif
    }

    if (hm_adapters_all == 0)
    {
      gpu_temp_disable = 1;
    }

    if (gpu_temp_disable == 1)
    {
      gpu_temp_abort  = 0;
      gpu_temp_retain = 0;
    }

    /**
     * enable custom signal handler(s)
     */

    if (benchmark == 0)
    {
      hc_signal (sigHandler_default);
    }
    else
    {
      hc_signal (sigHandler_benchmark);
    }

    /**
     * devices mask and properties
     */

    uint devices_cnt = 0;

    for (uint device_all_id = 0; device_all_id < devices_all_cnt; device_all_id++)
    {
      if (gpu_devicemask)
      {
        uint device_all_id_mask = 1 << device_all_id;

        if ((device_all_id_mask & gpu_devicemask) != device_all_id_mask)
        {
          if (quiet == 0 && algorithm_pos == 0) log_info ("Device #%d: skipped by user", device_all_id_mask + 1);

          continue;
        }
      }

      const uint device_id = devices_cnt;

      devices[device_id] = devices_all[device_all_id];

      memcpy (&data.hm_device[device_id], &hm_adapter_all[device_all_id], sizeof (hm_attrs_t));

      char device_name[INFOSZ];

      memset (device_name, 0, sizeof (device_name));

      #ifdef _CUDA
      size_t    global_mem_size;
      int       max_clock_frequency;
      int       max_compute_units;
      int       kernel_exec_timeout;

      hc_cuDeviceGetName (device_name, sizeof (device_name), devices[device_id]);
      hc_cuDeviceTotalMem (&global_mem_size, devices[device_id]);
      hc_cuDeviceGetAttribute (&max_clock_frequency, CU_DEVICE_ATTRIBUTE_CLOCK_RATE, devices[device_id]); max_clock_frequency /= 1000;
      hc_cuDeviceGetAttribute (&max_compute_units, CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT, devices[device_id]);
      hc_cuDeviceGetAttribute (&kernel_exec_timeout, CU_DEVICE_ATTRIBUTE_KERNEL_EXEC_TIMEOUT, devices[device_id]);

      #elif _OCL
      cl_ulong  global_mem_size;
      cl_uint   max_clock_frequency;
      cl_uint   max_compute_units;

      hc_clGetDeviceInfo (devices[device_id], CL_DEVICE_NAME,                sizeof (device_name),         &device_name,         NULL);
      hc_clGetDeviceInfo (devices[device_id], CL_DEVICE_GLOBAL_MEM_SIZE,     sizeof (global_mem_size),     &global_mem_size,     NULL);
      hc_clGetDeviceInfo (devices[device_id], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof (max_clock_frequency), &max_clock_frequency, NULL);
      hc_clGetDeviceInfo (devices[device_id], CL_DEVICE_MAX_COMPUTE_UNITS,   sizeof (max_compute_units),   &max_compute_units,   NULL);

      #endif

      if ((benchmark == 1 || quiet == 0) && (algorithm_pos == 0))
      {
        log_info ("Device #%u: %s, %luMB, %dMhz, %uMCU",
                  device_all_id + 1,
                  device_name,
                  (unsigned int) (global_mem_size / 1024 / 1024),
                  (unsigned int) (max_clock_frequency),
                  (unsigned int) max_compute_units);
      }

      #ifdef _CUDA
      if (quiet == 0 && kernel_exec_timeout != 0 && algorithm_pos == 0)
      {
        log_info ("Device #%u: WARNING! Kernel exec timeout is not disabled, it might cause you errors of code 702", device_id + 1);
        #if _WIN
        log_info ("           You can disable it with a regpatch, see here: http://hashcat.net/wiki/doku.php?id=timeout_patch");
        #endif
      }
      #endif

      devices_cnt++;
    }

    if (devices_cnt == 0)
    {
      log_error ("ERROR: No devices left that matches your specification.");

      return (-1);
    }

    if ((benchmark == 1 || quiet == 0) && (algorithm_pos == 0))
    {
      log_info ("");
    }

    /**
     * inform the user
     */

    // gpu temp sanity check

    if ((gpu_temp_abort != 0) && (gpu_temp_retain != 0))
    {
      if (gpu_temp_abort < gpu_temp_retain)
      {
        log_error ("ERROR: invalid values for gpu-temp-abort. Parameter gpu-temp-abort is less than gpu-temp-retain.");

        return (-1);
      }
    }

    if (data.quiet == 0)
    {
      log_info ("Hashes: %u hashes; %u unique digests, %u unique salts", hashes_cnt_orig, digests_cnt, salts_cnt);

      log_info ("Bitmaps: %u bits, %u entries, 0x%08x mask, %u bytes, %u/%u rotates", bitmap_bits, bitmap_nums, bitmap_mask, bitmap_size, bitmap_shift1, bitmap_shift2);

      if (attack_mode == ATTACK_MODE_STRAIGHT)
      {
        log_info ("Rules: %u", gpu_rules_cnt);
      }

      if (opti_type)
      {
        log_info ("Applicable Optimizers:");

        for (uint i = 0; i < 32; i++)
        {
          const uint opti_bit = 1 << i;

          if (opti_type & opti_bit) log_info ("* %s", stroptitype (opti_bit));
        }
      }

      /**
       * Watchdog and Temperature balance
       */

      if (gpu_temp_abort == 0)
      {
        log_info ("Watchdog: Temperature abort trigger disabled");
      }
      else
      {
        log_info ("Watchdog: Temperature abort trigger set to %uc", gpu_temp_abort);
      }

      if (gpu_temp_retain == 0)
      {
        log_info ("Watchdog: Temperature retain trigger disabled");
      }
      else
      {
        log_info ("Watchdog: Temperature retain trigger set to %uc", gpu_temp_retain);
      }
    }

    /**
     * store all the preparation, not hash_mode dependant
     */

    data.gpu_temp_disable = gpu_temp_disable;
    data.gpu_temp_abort   = gpu_temp_abort;
    data.gpu_temp_retain  = gpu_temp_retain;

    data.devices_cnt = devices_cnt;

    #ifdef _OCL
    /**
     * catalyst driver check
     */

    int catalyst_check = (force == 1) ? 0 : 1;

    int catalyst_warn = 0;

    int catalyst_broken = 0;
    #endif

    /**
     * devices init
     */

    #ifdef _OCL
    #ifndef OSX
    int gpu_temp_retain_set = 0;

    int *temp_retain_fanspeed_value = (int *) mycalloc (devices_cnt, sizeof (int));

    ADLOD6MemClockState *od_clock_mem_status = (ADLOD6MemClockState *) mycalloc (devices_cnt, sizeof (ADLOD6MemClockState));

    int *od_power_control_status = (int *) mycalloc (devices_cnt, sizeof (int));
    #endif
    #endif

    hc_device_param_t *devices_param = (hc_device_param_t *) mycalloc (devices_cnt, sizeof (hc_device_param_t));

    data.devices_param = devices_param;

    for (uint device_id = 0; device_id < devices_cnt; device_id++)
    {
      hc_device_param_t *device_param = &data.devices_param[device_id];

      #ifdef _CUDA
      CUdevice device = devices[device_id];

      device_param->device = device;

      size_t bytes;

      hc_cuDeviceTotalMem (&bytes, device);

      device_param->gpu_maxmem_alloc = bytes;

      int sm_major = 0;
      int sm_minor = 0;
      int max_compute_units = 0;

      hc_cuDeviceComputeCapability (&sm_major, &sm_minor, device);

      if (sm_major == 1)
      {
        log_error ("ERROR: Shader Model 1.0 - 1.3 based GPU detected. Support for CUDA was dropped by NVidia.");
        log_error ("       Remove it from your system or use -d and select only supported cards.");

        return (-1);
      }

      device_param->sm_major = sm_major;
      device_param->sm_minor = sm_minor;

      hc_cuDeviceGetAttribute (&max_compute_units, CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT, device);

      if (sm_major >= 5)
      {
        // those maxwell and newer are so good compared to older chipsets we need to equalize
        // their power to older chipsets, otherwise workload distribution which is based on the compute_units
        // gets out of control

        max_compute_units *= 3;
      }

      device_param->gpu_processors = max_compute_units;

      /**
       * calculate vector size
       */

      uint vliw = get_vliw_by_compute_capability (sm_major, sm_minor);

      device_param->gpu_vector_width = get_gpu_vector_width (hash_mode, attack_kern, attack_exec, opti_type, vliw);

      #elif _OCL
      cl_device_id device = devices[device_id];

      device_param->device = device;

      cl_uint max_compute_units = 0;

      hc_clGetDeviceInfo (device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof (max_compute_units), &max_compute_units, NULL);

      device_param->gpu_processors = max_compute_units;

      cl_ulong max_mem_alloc_size = 0;

      hc_clGetDeviceInfo (device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof (max_mem_alloc_size), &max_mem_alloc_size, NULL);

      device_param->gpu_maxmem_alloc = max_mem_alloc_size;

      char tmp[INFOSZ], t1[64];

      memset (tmp, 0, sizeof (tmp));

      hc_clGetDeviceInfo (device, CL_DEVICE_NAME, sizeof (tmp), &tmp, NULL);

      device_param->device_name = mystrdup (tmp);

      memset (tmp, 0, sizeof (tmp));

      hc_clGetDeviceInfo (device, CL_DEVICE_VERSION, sizeof (tmp), &tmp, NULL);

      memset (t1, 0, sizeof (t1));

      sscanf (tmp, "%*16s %*16s %*16s (%[^)]16s)", t1);

      device_param->device_version = mystrdup (t1);

      memset (tmp, 0, sizeof (tmp));

      hc_clGetDeviceInfo (device, CL_DRIVER_VERSION, sizeof (tmp), &tmp, NULL);

      device_param->driver_version = mystrdup (tmp);

      /**
       * calculate vector size
       */

      uint vliw = get_vliw_by_device_name (device_param->device_name);

      device_param->gpu_vector_width = get_gpu_vector_width (hash_mode, attack_kern, attack_exec, opti_type, vliw);

      /**
       * catalyst driver check
       */

      if (catalyst_check == 1)
      {
        catalyst_warn = 1;

        // v14.9 and higher
        if ((atoi (device_param->device_version) >= 1573)
         && (atoi (device_param->driver_version) >= 1573))
        {
          catalyst_warn = 0;
        }

        /*
        // v14.9
        if ((strstr (device_param->device_version, "1573.") != NULL)
         && (strstr (device_param->driver_version, "1573.") != NULL))
        {
          catalyst_warn = 0;
        }

        // v14.12 -- version overlaps with v15.4 beta
        if ((strstr (device_param->device_version, "1642.") != NULL)
         && (strstr (device_param->driver_version, "1642.") != NULL))
        {
          catalyst_broken = 1;
        }

        // v15.4 (Beta, Windows only release)
        if ((strstr (device_param->device_version, "1642.") != NULL)
         && (strstr (device_param->driver_version, "1642.") != NULL))
        {
          catalyst_warn = 0;
        }

        // v15.5 (Release, Linux)
        if ((strstr (device_param->device_version, "1702.") != NULL)
         && (strstr (device_param->driver_version, "1702.") != NULL))
        {
          catalyst_warn = 0;
        }

        // v15.3 (Beta, Ubuntu repository release)
        if ((strstr (device_param->device_version, "1729.") != NULL)
         && (strstr (device_param->driver_version, "1729.") != NULL))
        {
          catalyst_warn = 0;
        }
        */

        catalyst_check = 0;
      }
      #endif
    }

    #ifdef _OCL
    if (catalyst_broken == 1)
    {
      log_error ("");
      log_error ("ATTENTION! The installed GPU driver in your system is known to be broken!");
      log_error ("It will pass over cracked hashes and does not report them as cracked");
      log_error ("You are STRONGLY encouraged not to use it");
      log_error ("You can use --force to override this but do not post error reports if you do so");

      return (-1);
    }

    if (catalyst_warn == 1)
    {
      log_error ("");
      log_error ("ATTENTION! Unsupported or incorrect installed GPU driver detected!");
      log_error ("You are STRONGLY encouraged to use the official supported GPU driver for good reasons");
      log_error ("See oclHashcat's homepage for official supported GPU drivers");
      #ifdef _WIN
      log_error ("Also see: http://hashcat.net/wiki/doku.php?id=upgrading_amd_drivers_how_to");
      #endif
      log_error ("You can use --force to override this but do not post error reports if you do so");

      return (-1);
    }
    #endif

    uint gpu_blocks_all = 0;

    #ifdef _CUDA
    for (uint device_id = 0; device_id < devices_cnt; device_id++)
    {
      /**
       * host buffer
       */

      hc_device_param_t *device_param = &data.devices_param[device_id];

      /**
       * device properties
       */

      int  sm_minor         = device_param->sm_minor;
      int  sm_major         = device_param->sm_major;

      uint gpu_processors   = device_param->gpu_processors;
      uint gpu_vector_width = device_param->gpu_vector_width;

      /**
       * create context for each device
       */

      uint flags = 0;

      if (gpu_async == 0) flags |= CU_CTX_SCHED_BLOCKING_SYNC;
      else                flags |= CU_CTX_SCHED_SPIN;

      hc_cuCtxCreate (&device_param->context, flags, device_param->device);

      // does bad things hc_cuCtxSetCacheConfig (CU_FUNC_CACHE_PREFER_L1);

      /**
       * create input buffers on device
       */

      uint gpu_threads = GPU_THREADS_NV;

      if (hash_mode == 1500)  gpu_threads = 64;
      if (hash_mode == 3000)  gpu_threads = 64;
      if (hash_mode == 3200)  gpu_threads = 8;
      if (hash_mode == 7500)  gpu_threads = 64;
      if (hash_mode == 8900)  gpu_threads = 64;
      if (hash_mode == 9000)  gpu_threads = 8;
      if (hash_mode == 9300)  gpu_threads = 64;
      if (hash_mode == 9700)  gpu_threads = 64;
      if (hash_mode == 9710)  gpu_threads = 64;
      if (hash_mode == 9800)  gpu_threads = 64;
      if (hash_mode == 9810)  gpu_threads = 64;
      if (hash_mode == 10400) gpu_threads = 64;
      if (hash_mode == 10410) gpu_threads = 64;
      if (hash_mode == 10500) gpu_threads = 64;

      uint gpu_power  = gpu_processors * gpu_threads * gpu_accel;
      uint gpu_blocks = gpu_power * gpu_vector_width;

      device_param->gpu_threads      = gpu_threads;
      device_param->gpu_power_user   = gpu_power;
      device_param->gpu_blocks_user  = gpu_blocks;

      gpu_blocks_all += gpu_blocks;

      uint size_pws = gpu_power * sizeof (pw_t);

      uint size_tmps = 4;

      switch (hash_mode)
      {
        case   400: size_tmps = gpu_blocks * sizeof (phpass_tmp_t);        break;
        case   500: size_tmps = gpu_blocks * sizeof (md5crypt_tmp_t);      break;
        case   501: size_tmps = gpu_blocks * sizeof (md5crypt_tmp_t);      break;
        case  1600: size_tmps = gpu_blocks * sizeof (md5crypt_tmp_t);      break;
        case  1800: size_tmps = gpu_blocks * sizeof (sha512crypt_tmp_t);   break;
        case  2100: size_tmps = gpu_blocks * sizeof (dcc2_tmp_t);          break;
        case  2500: size_tmps = gpu_blocks * sizeof (wpa_tmp_t);           break;
        case  3200: size_tmps = gpu_blocks * sizeof (bcrypt_tmp_t);        break;
        case  5200: size_tmps = gpu_blocks * sizeof (pwsafe3_tmp_t);       break;
        case  5800: size_tmps = gpu_blocks * sizeof (androidpin_tmp_t);    break;
        case  6211:
        case  6212:
        case  6213: size_tmps = gpu_blocks * sizeof (tc_tmp_t);            break;
        case  6221:
        case  6222:
        case  6223: size_tmps = gpu_blocks * sizeof (tc64_tmp_t);          break;
        case  6231:
        case  6232:
        case  6233: size_tmps = gpu_blocks * sizeof (tc_tmp_t);            break;
        case  6241:
        case  6242:
        case  6243: size_tmps = gpu_blocks * sizeof (tc_tmp_t);            break;
        case  6300: size_tmps = gpu_blocks * sizeof (md5crypt_tmp_t);      break;
        case  6400: size_tmps = gpu_blocks * sizeof (sha256aix_tmp_t);     break;
        case  6500: size_tmps = gpu_blocks * sizeof (sha512aix_tmp_t);     break;
        case  6600: size_tmps = gpu_blocks * sizeof (agilekey_tmp_t);      break;
        case  6700: size_tmps = gpu_blocks * sizeof (sha1aix_tmp_t);       break;
        case  6800: size_tmps = gpu_blocks * sizeof (lastpass_tmp_t);      break;
        case  7100: size_tmps = gpu_blocks * sizeof (pbkdf2_sha512_tmp_t); break;
        case  7200: size_tmps = gpu_blocks * sizeof (pbkdf2_sha512_tmp_t); break;
        case  7400: size_tmps = gpu_blocks * sizeof (sha256crypt_tmp_t);   break;
        case  7900: size_tmps = gpu_blocks * sizeof (drupal7_tmp_t);       break;
        case  8200: size_tmps = gpu_blocks * sizeof (pbkdf2_sha512_tmp_t); break;
        case  8800: size_tmps = gpu_blocks * sizeof (androidfde_tmp_t);    break;
        case  8900: size_tmps = gpu_blocks * sizeof (scrypt_tmp_t);        break;
        case  9000: size_tmps = gpu_blocks * sizeof (pwsafe2_tmp_t);       break;
        case  9100: size_tmps = gpu_blocks * sizeof (lotus8_tmp_t);        break;
        case  9200: size_tmps = gpu_blocks * sizeof (pbkdf2_sha256_tmp_t); break;
        case  9300: size_tmps = gpu_blocks * sizeof (scrypt_tmp_t);        break;
        case  9400: size_tmps = gpu_blocks * sizeof (office2007_tmp_t);    break;
        case  9500: size_tmps = gpu_blocks * sizeof (office2010_tmp_t);    break;
        case  9600: size_tmps = gpu_blocks * sizeof (office2013_tmp_t);    break;
        case 10000: size_tmps = gpu_blocks * sizeof (pbkdf2_sha256_tmp_t); break;
        case 10200: size_tmps = gpu_blocks * sizeof (cram_md5_t);          break;
        case 10300: size_tmps = gpu_blocks * sizeof (saph_sha1_tmp_t);     break;
        case 10500: size_tmps = gpu_blocks * sizeof (pdf14_tmp_t);         break;
        case 10700: size_tmps = gpu_blocks * sizeof (pdf17l8_tmp_t);       break;
        case 10900: size_tmps = gpu_blocks * sizeof (pbkdf2_sha256_tmp_t); break;
        case 11300: size_tmps = gpu_blocks * sizeof (bitcoin_wallet_tmp_t); break;
        case 11600: size_tmps = gpu_blocks * sizeof (seven_zip_tmp_t);     break;
        case 11900: size_tmps = gpu_blocks * sizeof (pbkdf2_md5_tmp_t);    break;
        case 12000: size_tmps = gpu_blocks * sizeof (pbkdf2_sha1_tmp_t);   break;
        case 12100: size_tmps = gpu_blocks * sizeof (pbkdf2_sha512_tmp_t); break;
        case 12200: size_tmps = gpu_blocks * sizeof (ecryptfs_tmp_t);      break;
        case 12300: size_tmps = gpu_blocks * sizeof (oraclet_tmp_t);       break;
        case 12400: size_tmps = gpu_blocks * sizeof (bsdicrypt_tmp_t);     break;
        case 12500: size_tmps = gpu_blocks * sizeof (rar3_tmp_t);          break;
        case 12700: size_tmps = gpu_blocks * sizeof (mywallet_tmp_t);      break;
        case 12800: size_tmps = gpu_blocks * sizeof (pbkdf2_sha256_tmp_t); break;
      };

      uint size_hooks = 4;

      if ((opts_type & OPTS_TYPE_HOOK12) || (opts_type & OPTS_TYPE_HOOK23))
      {
        // fill size_hook with correct size
      }

      // we can optimize some stuff here...

      device_param->size_pws   = size_pws;
      device_param->size_tmps  = size_tmps;
      device_param->size_hooks = size_hooks;

      uint size_root_css   = SP_PW_MAX *           sizeof (cs_t);
      uint size_markov_css = SP_PW_MAX * CHARSIZ * sizeof (cs_t);

      device_param->size_root_css   = size_root_css;
      device_param->size_markov_css = size_markov_css;

      uint size_results = GPU_THREADS_NV * sizeof (uint);

      device_param->size_results = size_results;

      uint size_rules  = gpu_rules_cnt * sizeof (gpu_rule_t);
      uint size_plains = digests_cnt * sizeof (plain_t);
      uint size_salts  = salts_cnt * sizeof (salt_t);
      uint size_esalts = salts_cnt * esalt_size;

      device_param->size_plains   = size_plains;
      device_param->size_digests  = size_digests;
      device_param->size_shown    = size_shown;
      device_param->size_salts    = size_salts;

      uint size_combs = GPU_COMBS * sizeof (comb_t);
      uint size_bfs   = GPU_BFS   * sizeof (bf_t);
      uint size_tm    = 32        * sizeof (bs_word_t);

      uint64_t size_scryptV = 1;

      if ((hash_mode == 8900) || (hash_mode == 9300))
      {
        #define SHADER_PER_MP 32
        #define WARPS         32

        uint tmto_start = 2;
        uint tmto_stop  = 1024;

        if (scrypt_tmto)
        {
          tmto_start = 1 << scrypt_tmto;
          tmto_stop  = tmto_start + 1;
        }

        for (uint tmto = tmto_start; tmto < tmto_stop; tmto <<= 1)
        {
          // todo -- make sure all salts get the new tmto value

          size_scryptV = (128 * data.salts_buf[0].scrypt_r) * data.salts_buf[0].scrypt_N;

          size_scryptV /= tmto;

          size_scryptV *= gpu_processors * WARPS * SHADER_PER_MP;

          if (size_scryptV > (device_param->gpu_maxmem_alloc / 2)) continue;

          for (uint salts_pos = 0; salts_pos < data.salts_cnt; salts_pos++)
          {
            data.salts_buf[salts_pos].scrypt_tmto = tmto;
            data.salts_buf[salts_pos].scrypt_phy  = gpu_processors * WARPS * SHADER_PER_MP;
          }

          break;
        }

        if (data.salts_buf[0].scrypt_tmto == 0)
        {
          log_error ("ERROR: can't allocate enough GPU memory");

          return -1;
        }

        if (quiet == 0) log_info ("");
        if (quiet == 0) log_info ("SCRYPT tmto optimizer value set to: %u\n", data.salts_buf[0].scrypt_tmto);
      }

      /**
       * stream
       */

      hc_cuStreamCreate (&device_param->stream, 0);

      /**
       * module find
       */

      char module_file[256];

      memset (module_file, 0, sizeof (module_file));

      #ifdef __x86_64__
      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        if (attack_kern == ATTACK_KERN_STRAIGHT)
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d_a0.sm_%d%d.64.cubin", install_dir, (int) kern_type, sm_major, sm_minor);
        else if (attack_kern == ATTACK_KERN_COMBI)
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d_a1.sm_%d%d.64.cubin", install_dir, (int) kern_type, sm_major, sm_minor);
        else if (attack_kern == ATTACK_KERN_BF)
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d_a3.sm_%d%d.64.cubin", install_dir, (int) kern_type, sm_major, sm_minor);
      }
      else
      {
        snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d.sm_%d%d.64.cubin", install_dir, (int) kern_type, sm_major, sm_minor);

        if ((hash_mode == 8900) || (hash_mode == 9300))
        {
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d_%d_%d_%d_%d.sm_%d%d.64.cubin", install_dir, (int) kern_type, data.salts_buf[0].scrypt_N, data.salts_buf[0].scrypt_r, data.salts_buf[0].scrypt_p, data.salts_buf[0].scrypt_tmto, sm_major, sm_minor);
        }
      }

      #else
      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        if (attack_kern == ATTACK_KERN_STRAIGHT)
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d_a0.sm_%d%d.32.cubin", install_dir, (int) kern_type, sm_major, sm_minor);
        else if (attack_kern == ATTACK_KERN_COMBI)
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d_a1.sm_%d%d.32.cubin", install_dir, (int) kern_type, sm_major, sm_minor);
        else if (attack_kern == ATTACK_KERN_BF)
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d_a3.sm_%d%d.32.cubin", install_dir, (int) kern_type, sm_major, sm_minor);
      }
      else
      {
        snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d.sm_%d%d.32.cubin", install_dir, (int) kern_type, sm_major, sm_minor);

        if ((hash_mode == 8900) || (hash_mode == 9300))
        {
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4318/m%05d_%d_%d_%d_%d.sm_%d%d.32.cubin", install_dir, (int) kern_type, data.salts_buf[0].scrypt_N, data.salts_buf[0].scrypt_r, data.salts_buf[0].scrypt_p, data.salts_buf[0].scrypt_tmto, sm_major, sm_minor);
        }
      }

      #endif

      char module_mp_file[256];

      memset (module_mp_file, 0, sizeof (module_mp_file));

      if ((opti_type & OPTI_TYPE_BRUTE_FORCE) && (opts_type & OPTS_TYPE_PT_GENERATE_BE))
      {
        #ifdef __x86_64__
        snprintf (module_mp_file, sizeof (module_mp_file) - 1, "%s/kernels/4318/markov_be_v%d.sm_%d%d.64.cubin", install_dir, gpu_vector_width, sm_major, sm_minor);
        #else
        snprintf (module_mp_file, sizeof (module_mp_file) - 1, "%s/kernels/4318/markov_be_v%d.sm_%d%d.32.cubin", install_dir, gpu_vector_width, sm_major, sm_minor);
        #endif
      }
      else
      {
        #ifdef __x86_64__
        snprintf (module_mp_file, sizeof (module_mp_file) - 1, "%s/kernels/4318/markov_le_v%d.sm_%d%d.64.cubin", install_dir, gpu_vector_width, sm_major, sm_minor);
        #else
        snprintf (module_mp_file, sizeof (module_mp_file) - 1, "%s/kernels/4318/markov_le_v%d.sm_%d%d.32.cubin", install_dir, gpu_vector_width, sm_major, sm_minor);
        #endif
      }

      char module_amp_file[256];

      memset (module_amp_file, 0, sizeof (module_amp_file));

      #ifdef __x86_64__
      snprintf (module_amp_file, sizeof (module_amp_file) - 1, "%s/kernels/4318/amp_a%d_v%d.sm_%d%d.64.cubin", install_dir, attack_kern, gpu_vector_width, sm_major, sm_minor);
      #else
      snprintf (module_amp_file, sizeof (module_amp_file) - 1, "%s/kernels/4318/amp_a%d_v%d.sm_%d%d.32.cubin", install_dir, attack_kern, gpu_vector_width, sm_major, sm_minor);
      #endif

      /**
       * module load
       */

      hc_cuModuleLoad (&device_param->module, module_file);

      if (quiet == 0) log_info ("Device #%u: Kernel %s", device_id + 1, module_file);

      if (attack_mode != ATTACK_MODE_STRAIGHT)
      {
        hc_cuModuleLoad (&device_param->module_mp, module_mp_file);

        if (quiet == 0) log_info ("Device #%u: Kernel %s", device_id + 1, module_mp_file);
      }

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        // nothing to do
      }
      else
      {
        hc_cuModuleLoad (&device_param->module_amp, module_amp_file);

        if (quiet == 0) log_info ("Device #%u: Kernel %s", device_id + 1, module_amp_file);
      }

      /**
       * module functions
       */

      char module_name[64];

      memset (module_name, 0, sizeof (module_name));

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        if (opti_type & OPTI_TYPE_SINGLE_HASH)
        {
          snprintf (module_name, sizeof (module_name) - 1, "m%05d_s%02d", kern_type, 4);

          hc_cuModuleGetFunction (&device_param->function1, device_param->module, module_name);

          snprintf (module_name, sizeof (module_name) - 1, "m%05d_s%02d", kern_type, 8);

          hc_cuModuleGetFunction (&device_param->function2, device_param->module, module_name);

          snprintf (module_name, sizeof (module_name) - 1, "m%05d_s%02d", kern_type, 16);

          hc_cuModuleGetFunction (&device_param->function3, device_param->module, module_name);
        }
        else
        {
          snprintf (module_name, sizeof (module_name) - 1, "m%05d_m%02d",  kern_type, 4);

          hc_cuModuleGetFunction (&device_param->function1, device_param->module, module_name);

          snprintf (module_name, sizeof (module_name) - 1, "m%05d_m%02d",  kern_type, 8);

          hc_cuModuleGetFunction (&device_param->function2, device_param->module, module_name);

          snprintf (module_name, sizeof (module_name) - 1, "m%05d_m%02d",  kern_type, 16);

          hc_cuModuleGetFunction (&device_param->function3, device_param->module, module_name);
        }

        if (attack_mode == ATTACK_MODE_BF)
        {
          if (opts_type & OPTS_TYPE_PT_BITSLICE)
          {
            snprintf (module_name, sizeof (module_name) - 1, "m%05d_tb", kern_type);

            hc_cuModuleGetFunction (&device_param->function_tb, device_param->module, module_name);

            snprintf (module_name, sizeof (module_name) - 1, "m%05d_tm", kern_type);

            hc_cuModuleGetFunction (&device_param->function_tm, device_param->module, module_name);
          }
        }
      }
      else
      {
        snprintf (module_name, sizeof (module_name) - 1, "m%05d_init", kern_type);

        hc_cuModuleGetFunction (&device_param->function1, device_param->module, module_name);

        snprintf (module_name, sizeof (module_name) - 1, "m%05d_loop", kern_type);

        hc_cuModuleGetFunction (&device_param->function2, device_param->module, module_name);

        snprintf (module_name, sizeof (module_name) - 1, "m%05d_comp", kern_type);

        hc_cuModuleGetFunction (&device_param->function3, device_param->module, module_name);

        if (opts_type & OPTS_TYPE_HOOK12)
        {
          snprintf (module_name, sizeof (module_name) - 1, "m%05d_hook12", kern_type);

          hc_cuModuleGetFunction (&device_param->function12, device_param->module, module_name);
        }

        if (opts_type & OPTS_TYPE_HOOK23)
        {
          snprintf (module_name, sizeof (module_name) - 1, "m%05d_hook23", kern_type);

          hc_cuModuleGetFunction (&device_param->function23, device_param->module, module_name);
        }
      }

      if (attack_mode == ATTACK_MODE_BF)
      {
        hc_cuModuleGetFunction (&device_param->function_mp_l, device_param->module_mp, "l_markov");
        hc_cuModuleGetFunction (&device_param->function_mp_r, device_param->module_mp, "r_markov");
      }
      else if (attack_mode == ATTACK_MODE_HYBRID1)
      {
        hc_cuModuleGetFunction (&device_param->function_mp, device_param->module_mp, "C_markov");
      }
      else if (attack_mode == ATTACK_MODE_HYBRID2)
      {
        hc_cuModuleGetFunction (&device_param->function_mp, device_param->module_mp, "C_markov");
      }

      /**
       * amplifiers are not independant
       */

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        // nothing to do
      }
      else
      {
        hc_cuModuleGetFunction (&device_param->function_amp, device_param->module_amp, "amp");
      }

      /**
       * global buffers
       */

      hc_cuMemAlloc (&device_param->d_pws_buf,          size_pws);
      hc_cuMemAlloc (&device_param->d_pws_amp_buf,      size_pws);
      hc_cuMemAlloc (&device_param->d_tmps,             size_tmps);
      hc_cuMemAlloc (&device_param->d_hooks,            size_hooks);
      hc_cuMemAlloc (&device_param->d_bitmap_s1_a,      bitmap_size);
      hc_cuMemAlloc (&device_param->d_bitmap_s1_b,      bitmap_size);
      hc_cuMemAlloc (&device_param->d_bitmap_s1_c,      bitmap_size);
      hc_cuMemAlloc (&device_param->d_bitmap_s1_d,      bitmap_size);
      hc_cuMemAlloc (&device_param->d_bitmap_s2_a,      bitmap_size);
      hc_cuMemAlloc (&device_param->d_bitmap_s2_b,      bitmap_size);
      hc_cuMemAlloc (&device_param->d_bitmap_s2_c,      bitmap_size);
      hc_cuMemAlloc (&device_param->d_bitmap_s2_d,      bitmap_size);
      hc_cuMemAlloc (&device_param->d_plain_bufs,       size_plains);
      hc_cuMemAlloc (&device_param->d_digests_buf,      size_digests);
      hc_cuMemAlloc (&device_param->d_digests_shown,    size_shown);
      hc_cuMemAlloc (&device_param->d_salt_bufs,        size_salts);
      hc_cuMemAlloc (&device_param->d_result,           size_results);
      hc_cuMemAlloc (&device_param->d_scryptV_buf,      size_scryptV);

      hc_cuMemcpyHtoD (device_param->d_bitmap_s1_a,     bitmap_s1_a,        bitmap_size);
      hc_cuMemcpyHtoD (device_param->d_bitmap_s1_b,     bitmap_s1_b,        bitmap_size);
      hc_cuMemcpyHtoD (device_param->d_bitmap_s1_c,     bitmap_s1_c,        bitmap_size);
      hc_cuMemcpyHtoD (device_param->d_bitmap_s1_d,     bitmap_s1_d,        bitmap_size);
      hc_cuMemcpyHtoD (device_param->d_bitmap_s2_a,     bitmap_s2_a,        bitmap_size);
      hc_cuMemcpyHtoD (device_param->d_bitmap_s2_b,     bitmap_s2_b,        bitmap_size);
      hc_cuMemcpyHtoD (device_param->d_bitmap_s2_c,     bitmap_s2_c,        bitmap_size);
      hc_cuMemcpyHtoD (device_param->d_bitmap_s2_d,     bitmap_s2_d,        bitmap_size);
      hc_cuMemcpyHtoD (device_param->d_digests_buf,     data.digests_buf,   size_digests);
      hc_cuMemcpyHtoD (device_param->d_digests_shown,   data.digests_shown, size_shown);
      hc_cuMemcpyHtoD (device_param->d_salt_bufs,       data.salts_buf,     size_salts);

      run_kernel_bzero (device_param, device_param->d_pws_buf,        size_pws);
      run_kernel_bzero (device_param, device_param->d_pws_amp_buf,    size_pws);
      run_kernel_bzero (device_param, device_param->d_tmps,           size_tmps);
      run_kernel_bzero (device_param, device_param->d_hooks,          size_hooks);
      run_kernel_bzero (device_param, device_param->d_plain_bufs,     size_plains);
      run_kernel_bzero (device_param, device_param->d_result,         size_results);

      /**
       * special buffers
       */

      if (attack_kern == ATTACK_KERN_STRAIGHT)
      {
        hc_cuMemAlloc  (&device_param->d_rules, size_rules);

        hc_cuMemcpyHtoD (device_param->d_rules, gpu_rules_buf, size_rules);
      }
      else if (attack_kern == ATTACK_KERN_COMBI)
      {
        hc_cuMemAlloc (&device_param->d_combs,          size_combs);
        hc_cuMemAlloc (&device_param->d_root_css_buf,   size_root_css);
        hc_cuMemAlloc (&device_param->d_markov_css_buf, size_markov_css);

        run_kernel_bzero (device_param, device_param->d_combs,          size_combs);
        run_kernel_bzero (device_param, device_param->d_root_css_buf,   size_root_css);
        run_kernel_bzero (device_param, device_param->d_markov_css_buf, size_markov_css);
      }
      else if (attack_kern == ATTACK_KERN_BF)
      {
        hc_cuMemAlloc (&device_param->d_bfs,            size_bfs);
        hc_cuMemAlloc (&device_param->d_tm,             size_tm);
        hc_cuMemAlloc (&device_param->d_root_css_buf,   size_root_css);
        hc_cuMemAlloc (&device_param->d_markov_css_buf, size_markov_css);

        run_kernel_bzero (device_param, device_param->d_bfs,            size_bfs);
        run_kernel_bzero (device_param, device_param->d_tm,             size_tm);
        run_kernel_bzero (device_param, device_param->d_root_css_buf,   size_root_css);
        run_kernel_bzero (device_param, device_param->d_markov_css_buf, size_markov_css);
      }

      if (size_esalts)
      {
        hc_cuMemAlloc (&device_param->d_esalt_bufs, size_esalts);

        hc_cuMemcpyHtoD (device_param->d_esalt_bufs, data.esalts_buf, size_esalts);
      }

      /**
       * main host data
       */

      uint *result = (uint *) mymalloc (size_results);

      memset (result, 0, size_results);

      device_param->result = result;

      pw_t *pws_buf = (pw_t *) mymalloc (size_pws);

      memset (pws_buf, 0, size_pws);

      device_param->pws_buf = pws_buf;

      pw_cache_t *pw_caches = (pw_cache_t *) mycalloc (64, sizeof (pw_cache_t));

      for (int i = 0; i < 64; i++)
      {
        pw_caches[i].pw_buf.pw_len = i;
        pw_caches[i].cnt = 0;
      }

      device_param->pw_caches = pw_caches;

      comb_t *combs_buf = (comb_t *) mycalloc (GPU_COMBS, sizeof (comb_t));

      device_param->combs_buf = combs_buf;

      void *hooks_buf = mymalloc (size_hooks);

      device_param->hooks_buf = hooks_buf;

      switch (device_param->gpu_vector_width)
      {
        case 1: device_param->pw_transpose  = pw_transpose_to_hi1;
                device_param->pw_add        = pw_add_to_hc1;
                break;
        case 2: device_param->pw_transpose  = pw_transpose_to_hi2;
                device_param->pw_add        = pw_add_to_hc2;
                break;
        case 4: device_param->pw_transpose  = pw_transpose_to_hi4;
                device_param->pw_add        = pw_add_to_hc4;
                break;
      }

      /**
       * module args
       */

      device_param->kernel_params_buf32[21] = bitmap_mask;
      device_param->kernel_params_buf32[22] = bitmap_shift1;
      device_param->kernel_params_buf32[23] = bitmap_shift2;
      device_param->kernel_params_buf32[24] = 0; // salt_pos
      device_param->kernel_params_buf32[25] = 0; // loop_pos
      device_param->kernel_params_buf32[26] = 0; // loop_cnt
      device_param->kernel_params_buf32[27] = 0; // gpu_rules_cnt
      device_param->kernel_params_buf32[28] = 0; // digests_cnt
      device_param->kernel_params_buf32[29] = 0; // digests_offset
      device_param->kernel_params_buf32[30] = 0; // combs_mode
      device_param->kernel_params_buf32[31] = 0; // gid_max

      device_param->kernel_params[ 0] = (attack_exec == ATTACK_EXEC_ON_GPU)
                                      ? &device_param->d_pws_buf
                                      : &device_param->d_pws_amp_buf;
      device_param->kernel_params[ 1] = &device_param->d_rules;
      device_param->kernel_params[ 2] = &device_param->d_combs;
      device_param->kernel_params[ 3] = &device_param->d_bfs;
      device_param->kernel_params[ 4] = &device_param->d_tmps;
      device_param->kernel_params[ 5] = &device_param->d_hooks;
      device_param->kernel_params[ 6] = &device_param->d_bitmap_s1_a;
      device_param->kernel_params[ 7] = &device_param->d_bitmap_s1_b;
      device_param->kernel_params[ 8] = &device_param->d_bitmap_s1_c;
      device_param->kernel_params[ 9] = &device_param->d_bitmap_s1_d;
      device_param->kernel_params[10] = &device_param->d_bitmap_s2_a;
      device_param->kernel_params[11] = &device_param->d_bitmap_s2_b;
      device_param->kernel_params[12] = &device_param->d_bitmap_s2_c;
      device_param->kernel_params[13] = &device_param->d_bitmap_s2_d;
      device_param->kernel_params[14] = &device_param->d_plain_bufs;
      device_param->kernel_params[15] = &device_param->d_digests_buf;
      device_param->kernel_params[16] = &device_param->d_digests_shown;
      device_param->kernel_params[17] = &device_param->d_salt_bufs;
      device_param->kernel_params[18] = &device_param->d_esalt_bufs;
      device_param->kernel_params[19] = &device_param->d_result;
      device_param->kernel_params[20] = &device_param->d_scryptV_buf;
      device_param->kernel_params[21] = &device_param->kernel_params_buf32[21];
      device_param->kernel_params[22] = &device_param->kernel_params_buf32[22];
      device_param->kernel_params[23] = &device_param->kernel_params_buf32[23];
      device_param->kernel_params[24] = &device_param->kernel_params_buf32[24];
      device_param->kernel_params[25] = &device_param->kernel_params_buf32[25];
      device_param->kernel_params[26] = &device_param->kernel_params_buf32[26];
      device_param->kernel_params[27] = &device_param->kernel_params_buf32[27];
      device_param->kernel_params[28] = &device_param->kernel_params_buf32[28];
      device_param->kernel_params[29] = &device_param->kernel_params_buf32[29];
      device_param->kernel_params[30] = &device_param->kernel_params_buf32[30];
      device_param->kernel_params[31] = &device_param->kernel_params_buf32[31];

      device_param->kernel_params_mp_buf64[3] = 0;
      device_param->kernel_params_mp_buf32[4] = 0;
      device_param->kernel_params_mp_buf32[5] = 0;
      device_param->kernel_params_mp_buf32[6] = 0;
      device_param->kernel_params_mp_buf32[7] = 0;
      device_param->kernel_params_mp_buf32[8] = 0;

      device_param->kernel_params_mp[0] = NULL;
      device_param->kernel_params_mp[1] = NULL;
      device_param->kernel_params_mp[2] = NULL;
      device_param->kernel_params_mp[3] = &device_param->kernel_params_mp_buf64[3];
      device_param->kernel_params_mp[4] = &device_param->kernel_params_mp_buf32[4];
      device_param->kernel_params_mp[5] = &device_param->kernel_params_mp_buf32[5];
      device_param->kernel_params_mp[6] = &device_param->kernel_params_mp_buf32[6];
      device_param->kernel_params_mp[7] = &device_param->kernel_params_mp_buf32[7];
      device_param->kernel_params_mp[8] = &device_param->kernel_params_mp_buf32[8];

      device_param->kernel_params_mp_l_buf64[3] = 0;
      device_param->kernel_params_mp_l_buf32[4] = 0;
      device_param->kernel_params_mp_l_buf32[5] = 0;
      device_param->kernel_params_mp_l_buf32[6] = 0;
      device_param->kernel_params_mp_l_buf32[7] = 0;
      device_param->kernel_params_mp_l_buf32[8] = 0;
      device_param->kernel_params_mp_l_buf32[9] = 0;

      device_param->kernel_params_mp_l[0] = NULL;
      device_param->kernel_params_mp_l[1] = NULL;
      device_param->kernel_params_mp_l[2] = NULL;
      device_param->kernel_params_mp_l[3] = &device_param->kernel_params_mp_l_buf64[3];
      device_param->kernel_params_mp_l[4] = &device_param->kernel_params_mp_l_buf32[4];
      device_param->kernel_params_mp_l[5] = &device_param->kernel_params_mp_l_buf32[5];
      device_param->kernel_params_mp_l[6] = &device_param->kernel_params_mp_l_buf32[6];
      device_param->kernel_params_mp_l[7] = &device_param->kernel_params_mp_l_buf32[7];
      device_param->kernel_params_mp_l[8] = &device_param->kernel_params_mp_l_buf32[8];
      device_param->kernel_params_mp_l[9] = &device_param->kernel_params_mp_l_buf32[9];

      device_param->kernel_params_mp_r_buf64[3] = 0;
      device_param->kernel_params_mp_r_buf32[4] = 0;
      device_param->kernel_params_mp_r_buf32[5] = 0;
      device_param->kernel_params_mp_r_buf32[6] = 0;
      device_param->kernel_params_mp_r_buf32[7] = 0;
      device_param->kernel_params_mp_r_buf32[8] = 0;

      device_param->kernel_params_mp_r[0] = NULL;
      device_param->kernel_params_mp_r[1] = NULL;
      device_param->kernel_params_mp_r[2] = NULL;
      device_param->kernel_params_mp_r[3] = &device_param->kernel_params_mp_r_buf64[3];
      device_param->kernel_params_mp_r[4] = &device_param->kernel_params_mp_r_buf32[4];
      device_param->kernel_params_mp_r[5] = &device_param->kernel_params_mp_r_buf32[5];
      device_param->kernel_params_mp_r[6] = &device_param->kernel_params_mp_r_buf32[6];
      device_param->kernel_params_mp_r[7] = &device_param->kernel_params_mp_r_buf32[7];
      device_param->kernel_params_mp_r[8] = &device_param->kernel_params_mp_r_buf32[8];

      device_param->kernel_params_amp_buf32[5] = 0; // combs_mode
      device_param->kernel_params_amp_buf32[6] = 0; // gid_max

      device_param->kernel_params_amp[0] = &device_param->d_pws_buf;
      device_param->kernel_params_amp[1] = &device_param->d_pws_amp_buf;
      device_param->kernel_params_amp[2] = &device_param->d_rules;
      device_param->kernel_params_amp[3] = &device_param->d_combs;
      device_param->kernel_params_amp[4] = &device_param->d_bfs;
      device_param->kernel_params_amp[5] = &device_param->kernel_params_amp_buf32[5];
      device_param->kernel_params_amp[6] = &device_param->kernel_params_amp_buf32[6];

      device_param->kernel_params_tb[0] = &device_param->d_pws_buf;

      device_param->kernel_params_tm[0] = &device_param->d_bfs;
      device_param->kernel_params_tm[1] = &device_param->d_tm;

      /* constant memory init */

      CUmodule c_module;

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        c_module = device_param->module;
      }
      else
      {
        c_module = device_param->module_amp;
      }

      size_t c_bytes;

      if (attack_kern == ATTACK_KERN_STRAIGHT)
      {
        CUdeviceptr c_rules;

        hc_cuModuleGetGlobal (&c_rules, &c_bytes, c_module, "c_rules");

        device_param->c_rules = c_rules;
        device_param->c_bytes = c_bytes;

        hc_cuMemsetD8 (c_rules, 0, c_bytes);
      }
      else if (attack_kern == ATTACK_KERN_COMBI)
      {
        CUdeviceptr c_combs;

        hc_cuModuleGetGlobal (&c_combs, &c_bytes, c_module, "c_combs");

        device_param->c_combs = c_combs;
        device_param->c_bytes = c_bytes;

        hc_cuMemsetD8 (c_combs, 0, c_bytes);
      }
      else if (attack_kern == ATTACK_KERN_BF)
      {
        CUdeviceptr c_bfs;

        hc_cuModuleGetGlobal (&c_bfs, &c_bytes, c_module, "c_bfs");

        device_param->c_bfs   = c_bfs;
        device_param->c_bytes = c_bytes;

        hc_cuMemsetD8 (c_bfs, 0, c_bytes);

        if (data.opts_type & OPTS_TYPE_PT_BITSLICE)
        {
          size_t bytes;

          CUdeviceptr c_tm;

          hc_cuModuleGetGlobal (&c_tm, &bytes, c_module, "c_tm");

          device_param->c_tm = c_tm;

          hc_cuMemsetD8 (c_tm, 0, bytes);
        }
      }

      hc_cuCtxPopCurrent (NULL);
    }

    #elif _OCL
    for (uint device_id = 0; device_id < devices_cnt; device_id++)
    {
      /**
       * host buffer
       */

      hc_device_param_t *device_param = &data.devices_param[device_id];

      /**
       * device properties
       */

      char *device_name    = device_param->device_name;
      char *device_version = device_param->device_version;
      char *driver_version = device_param->driver_version;

      uint gpu_processors   = device_param->gpu_processors;
      uint gpu_vector_width = device_param->gpu_vector_width;

      /**
       * create context for each device
       */

      device_param->context = hc_clCreateContext (NULL, 1, &device_param->device, NULL, NULL);

      /**
       * create command-queue
       */

      device_param->command_queue = hc_clCreateCommandQueueWithProperties (device_param->context, device_param->device, NULL);

      /**
       * create input buffers on device
       */

      uint gpu_threads = GPU_THREADS_AMD;

      if (hash_mode == 3200) gpu_threads = 8;
      if (hash_mode == 9000) gpu_threads = 8;

      uint gpu_power  = gpu_processors * gpu_threads * gpu_accel;

      uint gpu_blocks = gpu_power * gpu_vector_width;

      device_param->gpu_threads      = gpu_threads;
      device_param->gpu_power_user   = gpu_power;
      device_param->gpu_blocks_user  = gpu_blocks;

      gpu_blocks_all += gpu_blocks;

      uint size_pws = gpu_power * sizeof (pw_t);

      uint size_tmps = 4;

      switch (hash_mode)
      {
        case   400: size_tmps = gpu_blocks * sizeof (phpass_tmp_t);        break;
        case   500: size_tmps = gpu_blocks * sizeof (md5crypt_tmp_t);      break;
        case   501: size_tmps = gpu_blocks * sizeof (md5crypt_tmp_t);      break;
        case  1600: size_tmps = gpu_blocks * sizeof (md5crypt_tmp_t);      break;
        case  1800: size_tmps = gpu_blocks * sizeof (sha512crypt_tmp_t);   break;
        case  2100: size_tmps = gpu_blocks * sizeof (dcc2_tmp_t);          break;
        case  2500: size_tmps = gpu_blocks * sizeof (wpa_tmp_t);           break;
        case  3200: size_tmps = gpu_blocks * sizeof (bcrypt_tmp_t);        break;
        case  5200: size_tmps = gpu_blocks * sizeof (pwsafe3_tmp_t);       break;
        case  5800: size_tmps = gpu_blocks * sizeof (androidpin_tmp_t);    break;
        case  6211:
        case  6212:
        case  6213: size_tmps = gpu_blocks * sizeof (tc_tmp_t);            break;
        case  6221:
        case  6222:
        case  6223: size_tmps = gpu_blocks * sizeof (tc64_tmp_t);          break;
        case  6231:
        case  6232:
        case  6233: size_tmps = gpu_blocks * sizeof (tc_tmp_t);            break;
        case  6241:
        case  6242:
        case  6243: size_tmps = gpu_blocks * sizeof (tc_tmp_t);            break;
        case  6300: size_tmps = gpu_blocks * sizeof (md5crypt_tmp_t);      break;
        case  6400: size_tmps = gpu_blocks * sizeof (sha256aix_tmp_t);     break;
        case  6500: size_tmps = gpu_blocks * sizeof (sha512aix_tmp_t);     break;
        case  6600: size_tmps = gpu_blocks * sizeof (agilekey_tmp_t);      break;
        case  6700: size_tmps = gpu_blocks * sizeof (sha1aix_tmp_t);       break;
        case  6800: size_tmps = gpu_blocks * sizeof (lastpass_tmp_t);      break;
        case  7100: size_tmps = gpu_blocks * sizeof (pbkdf2_sha512_tmp_t); break;
        case  7200: size_tmps = gpu_blocks * sizeof (pbkdf2_sha512_tmp_t); break;
        case  7400: size_tmps = gpu_blocks * sizeof (sha256crypt_tmp_t);   break;
        case  7900: size_tmps = gpu_blocks * sizeof (drupal7_tmp_t);       break;
        case  8200: size_tmps = gpu_blocks * sizeof (pbkdf2_sha512_tmp_t); break;
        case  8800: size_tmps = gpu_blocks * sizeof (androidfde_tmp_t);    break;
        case  8900: size_tmps = gpu_blocks * sizeof (scrypt_tmp_t);        break;
        case  9000: size_tmps = gpu_blocks * sizeof (pwsafe2_tmp_t);       break;
        case  9100: size_tmps = gpu_blocks * sizeof (lotus8_tmp_t);        break;
        case  9200: size_tmps = gpu_blocks * sizeof (pbkdf2_sha256_tmp_t); break;
        case  9300: size_tmps = gpu_blocks * sizeof (scrypt_tmp_t);        break;
        case  9400: size_tmps = gpu_blocks * sizeof (office2007_tmp_t);    break;
        case  9500: size_tmps = gpu_blocks * sizeof (office2010_tmp_t);    break;
        case  9600: size_tmps = gpu_blocks * sizeof (office2013_tmp_t);    break;
        case 10000: size_tmps = gpu_blocks * sizeof (pbkdf2_sha256_tmp_t); break;
        case 10200: size_tmps = gpu_blocks * sizeof (cram_md5_t);          break;
        case 10300: size_tmps = gpu_blocks * sizeof (saph_sha1_tmp_t);     break;
        case 10500: size_tmps = gpu_blocks * sizeof (pdf14_tmp_t);         break;
        case 10700: size_tmps = gpu_blocks * sizeof (pdf17l8_tmp_t);       break;
        case 10900: size_tmps = gpu_blocks * sizeof (pbkdf2_sha256_tmp_t); break;
        case 11300: size_tmps = gpu_blocks * sizeof (bitcoin_wallet_tmp_t); break;
        case 11600: size_tmps = gpu_blocks * sizeof (seven_zip_tmp_t);     break;
        case 11900: size_tmps = gpu_blocks * sizeof (pbkdf2_md5_tmp_t);    break;
        case 12000: size_tmps = gpu_blocks * sizeof (pbkdf2_sha1_tmp_t);   break;
        case 12100: size_tmps = gpu_blocks * sizeof (pbkdf2_sha512_tmp_t); break;
        case 12200: size_tmps = gpu_blocks * sizeof (ecryptfs_tmp_t);      break;
        case 12300: size_tmps = gpu_blocks * sizeof (oraclet_tmp_t);       break;
        case 12400: size_tmps = gpu_blocks * sizeof (bsdicrypt_tmp_t);     break;
        case 12500: size_tmps = gpu_blocks * sizeof (rar3_tmp_t);          break;
        case 12700: size_tmps = gpu_blocks * sizeof (mywallet_tmp_t);      break;
        case 12800: size_tmps = gpu_blocks * sizeof (pbkdf2_sha256_tmp_t); break;
      };

      uint size_hooks = 4;

      if ((opts_type & OPTS_TYPE_HOOK12) || (opts_type & OPTS_TYPE_HOOK23))
      {
        // insert correct hook size
      }

      // we can optimize some stuff here...

      device_param->size_pws   = size_pws;
      device_param->size_tmps  = size_tmps;
      device_param->size_hooks = size_hooks;

      uint size_root_css   = SP_PW_MAX *           sizeof (cs_t);
      uint size_markov_css = SP_PW_MAX * CHARSIZ * sizeof (cs_t);

      device_param->size_root_css   = size_root_css;
      device_param->size_markov_css = size_markov_css;

      uint size_results = GPU_THREADS_AMD * sizeof (uint);

      device_param->size_results  = size_results;

      uint size_rules   = gpu_rules_cnt * sizeof (gpu_rule_t);
      uint size_rules_c = GPU_RULES * sizeof (gpu_rule_t);
      uint size_plains  = digests_cnt * sizeof (plain_t);
      uint size_salts   = salts_cnt * sizeof (salt_t);
      uint size_esalts  = salts_cnt * esalt_size;

      device_param->size_plains   = size_plains;
      device_param->size_digests  = size_digests;
      device_param->size_shown    = size_shown;
      device_param->size_salts    = size_salts;

      uint size_combs = GPU_COMBS * sizeof (comb_t);
      uint size_bfs   = GPU_BFS   * sizeof (bf_t);
      uint size_tm    = 32        * sizeof (bs_word_t);

      uint64_t size_scryptV = 1;

      if ((hash_mode == 8900) || (hash_mode == 9300))
      {
        #define SHADER_PER_MP 8
        #define WAVEFRONTS    64

        uint tmto_start = 2;
        uint tmto_stop  = 1024;

        if (scrypt_tmto)
        {
          tmto_start = 1 << scrypt_tmto;
          tmto_stop  = tmto_start + 1;
        }

        for (uint tmto = tmto_start; tmto < tmto_stop; tmto <<= 1)
        {
          // todo -- make sure all salts get the new tmto value

          size_scryptV = (128 * data.salts_buf[0].scrypt_r) * data.salts_buf[0].scrypt_N;

          size_scryptV /= tmto;

          size_scryptV *= gpu_processors * WAVEFRONTS * SHADER_PER_MP;

          if (size_scryptV > (device_param->gpu_maxmem_alloc / 2)) continue;

          for (uint salts_pos = 0; salts_pos < data.salts_cnt; salts_pos++)
          {
            data.salts_buf[salts_pos].scrypt_tmto = tmto;
            data.salts_buf[salts_pos].scrypt_phy  = gpu_processors * WAVEFRONTS * SHADER_PER_MP;
          }

          break;
        }

        if (data.salts_buf[0].scrypt_tmto == 0)
        {
          log_error ("ERROR: can't allocate enough GPU memory");

          return -1;
        }

        if (quiet == 0) log_info ("");
        if (quiet == 0) log_info ("SCRYPT tmto optimizer value set to: %u\n", data.salts_buf[0].scrypt_tmto);
      }

      /**
       * kernel find
       */

      uint vliw = get_vliw_by_device_name (device_name);

      struct stat st;

      char kernel_file[256];

      memset (kernel_file, 0, sizeof (kernel_file));

      size_t *kernel_lengths = (size_t *) mymalloc (sizeof (size_t));

      const unsigned char **kernel_sources = (const unsigned char **) mymalloc (sizeof (unsigned char *));

      #ifdef BINARY_KERNEL
      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        if (attack_kern == ATTACK_KERN_STRAIGHT)
          snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/kernels/4098/m%05d_a0.%s_%s_%s_%d.kernel", install_dir, (int) kern_type, device_name, device_version, driver_version, COMPTIME);
        else if (attack_kern == ATTACK_KERN_COMBI)
          snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/kernels/4098/m%05d_a1.%s_%s_%s_%d.kernel", install_dir, (int) kern_type, device_name, device_version, driver_version, COMPTIME);
        else if (attack_kern == ATTACK_KERN_BF)
          snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/kernels/4098/m%05d_a3.%s_%s_%s_%d.kernel", install_dir, (int) kern_type, device_name, device_version, driver_version, COMPTIME);
      }
      else
      {
        snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/kernels/4098/m%05d.%s_%s_%s_%d.kernel", install_dir, (int) kern_type, device_name, device_version, driver_version, COMPTIME);

        if ((hash_mode == 8900) || (hash_mode == 9300))
        {
          snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/kernels/4098/m%05d_%d_%d_%d_%d.%s_%s_%s_%d.kernel", install_dir, (int) kern_type, data.salts_buf[0].scrypt_N, data.salts_buf[0].scrypt_r, data.salts_buf[0].scrypt_p, data.salts_buf[0].scrypt_tmto, device_name, device_version, driver_version, COMPTIME);
        }
      }

      if (stat (kernel_file, &st) == -1)
      {
        if (quiet == 0) log_info ("Device #%u: Kernel %s not found in cache! Building may take a while...", device_id + 1, kernel_file);

        char module_file[256];

        memset (module_file, 0, sizeof (module_file));

        if (attack_exec == ATTACK_EXEC_ON_GPU)
        {
          if (attack_kern == ATTACK_KERN_STRAIGHT)
            snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4098/m%05d_a0.VLIW%d.llvmir", install_dir, (int) kern_type, vliw);
          else if (attack_kern == ATTACK_KERN_COMBI)
            snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4098/m%05d_a1.VLIW%d.llvmir", install_dir, (int) kern_type, vliw);
          else if (attack_kern == ATTACK_KERN_BF)
            snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4098/m%05d_a3.VLIW%d.llvmir", install_dir, (int) kern_type, vliw);
        }
        else
        {
          snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4098/m%05d.VLIW%d.llvmir", install_dir, (int) kern_type, vliw);

          if ((hash_mode == 8900) || (hash_mode == 9300))
          {
            snprintf (module_file, sizeof (module_file) - 1, "%s/kernels/4098/m%05d_%d_%d_%d_%d.VLIW%d.llvmir", install_dir, (int) kern_type, data.salts_buf[0].scrypt_N, data.salts_buf[0].scrypt_r, data.salts_buf[0].scrypt_p, data.salts_buf[0].scrypt_tmto, vliw);
          }
        }

        load_kernel (module_file, 1, kernel_lengths, kernel_sources);

        cl_program program = hc_clCreateProgramWithBinary (device_param->context, 1, &device_param->device, kernel_lengths, (const unsigned char **) kernel_sources, NULL);

        local_free (kernel_sources[0]);

        hc_clBuildProgram (program, 1, &device_param->device, "-cl-std=CL1.2", NULL, NULL);

        size_t binary_size;

        clGetProgramInfo (program, CL_PROGRAM_BINARY_SIZES, sizeof (size_t), &binary_size, NULL);

        unsigned char *binary = (unsigned char *) mymalloc (binary_size);

        clGetProgramInfo (program, CL_PROGRAM_BINARIES, sizeof (binary), &binary, NULL);

        writeProgramBin (kernel_file, binary, binary_size);

        local_free (binary);

        stat (kernel_file, &st); // to reload filesize
      }

      #else
      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        if (attack_kern == ATTACK_KERN_STRAIGHT)
          snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/amd/m%05d_a0.cl", install_dir, (int) kern_type);
        else if (attack_kern == ATTACK_KERN_COMBI)
          snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/amd/m%05d_a1.cl", install_dir, (int) kern_type);
        else if (attack_kern == ATTACK_KERN_BF)
          snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/amd/m%05d_a3.cl", install_dir, (int) kern_type);
      }
      else
        snprintf (kernel_file, sizeof (kernel_file) - 1, "%s/amd/m%05d.cl", install_dir, (int) kern_type);

      if (stat (kernel_file, &st) == -1)
      {
        log_error ("ERROR: %s: %s", kernel_file, strerror (errno));

        return -1;
      }

      #endif

      load_kernel (kernel_file, 1, kernel_lengths, kernel_sources);

      if (quiet == 0) log_info ("Device #%u: Kernel %s (%ld bytes)", device_id + 1, kernel_file, st.st_size);

      #ifdef BINARY_KERNEL
      device_param->program = hc_clCreateProgramWithBinary (device_param->context, 1, &device_param->device, kernel_lengths, (const unsigned char **) kernel_sources, NULL);
      #else
      device_param->program = hc_clCreateProgramWithSource (device_param->context, 1, (const char **) kernel_sources, NULL);
      #endif

      local_free (kernel_lengths);

      local_free (kernel_sources[0]);

      local_free (kernel_sources)

      /**
       * kernel mp find
       */

      if (attack_mode != ATTACK_MODE_STRAIGHT)
      {
        char kernel_mp_file[256];

        memset (kernel_mp_file, 0, sizeof (kernel_mp_file));

        size_t *kernel_mp_lengths = (size_t *) mymalloc (sizeof (size_t));

        const unsigned char **kernel_mp_sources = (const unsigned char **) mymalloc (sizeof (unsigned char *));

        #ifdef BINARY_KERNEL
        if ((opti_type & OPTI_TYPE_BRUTE_FORCE) && (opts_type & OPTS_TYPE_PT_GENERATE_BE))
        {
          snprintf (kernel_mp_file, sizeof (kernel_mp_file) - 1, "%s/kernels/4098/markov_be_v%d.%s_%s_%s_%d.kernel", install_dir, gpu_vector_width, device_name, device_version, driver_version, COMPTIME);
        }
        else
        {
          snprintf (kernel_mp_file, sizeof (kernel_mp_file) - 1, "%s/kernels/4098/markov_le_v%d.%s_%s_%s_%d.kernel", install_dir, gpu_vector_width, device_name, device_version, driver_version, COMPTIME);
        }

        if (stat (kernel_mp_file, &st) == -1)
        {
          if (quiet == 0) log_info ("Device #%u: Kernel %s not found in cache! Building may take a while...", device_id + 1, kernel_mp_file);

          char module_mp_file[256];

          memset (module_mp_file, 0, sizeof (module_mp_file));

          if ((opti_type & OPTI_TYPE_BRUTE_FORCE) && (opts_type & OPTS_TYPE_PT_GENERATE_BE))
          {
            snprintf (module_mp_file, sizeof (module_mp_file) - 1, "%s/kernels/4098/markov_be_v%d.llvmir", install_dir, gpu_vector_width);
          }
          else
          {
            snprintf (module_mp_file, sizeof (module_mp_file) - 1, "%s/kernels/4098/markov_le_v%d.llvmir", install_dir, gpu_vector_width);
          }

          load_kernel (module_mp_file, 1, kernel_mp_lengths, kernel_mp_sources);

          cl_program program_mp = hc_clCreateProgramWithBinary (device_param->context, 1, &device_param->device, kernel_mp_lengths, (const unsigned char **) kernel_mp_sources, NULL);

          local_free (kernel_mp_sources[0]);

          hc_clBuildProgram (program_mp, 1, &device_param->device, "-cl-std=CL1.2", NULL, NULL);

          size_t binary_mp_size;

          clGetProgramInfo (program_mp, CL_PROGRAM_BINARY_SIZES, sizeof (size_t), &binary_mp_size, NULL);

          unsigned char *binary_mp = (unsigned char *) mymalloc (binary_mp_size);

          clGetProgramInfo (program_mp, CL_PROGRAM_BINARIES, sizeof (binary_mp), &binary_mp, NULL);

          writeProgramBin (kernel_mp_file, binary_mp, binary_mp_size);

          local_free (binary_mp);

          stat (kernel_mp_file, &st); // to reload filesize
        }

        #else
        if ((opti_type & OPTI_TYPE_BRUTE_FORCE) && (opts_type & OPTS_TYPE_PT_GENERATE_BE))
        {
          snprintf (kernel_mp_file, sizeof (kernel_mp_file) - 1, "%s/amd/markov_be_v%d.cl", install_dir, gpu_vector_width);
        }
        else
        {
          snprintf (kernel_mp_file, sizeof (kernel_mp_file) - 1, "%s/amd/markov_le_v%d.cl", install_dir, gpu_vector_width);
        }

        if (stat (kernel_mp_file, &st) == -1)
        {
          log_error ("ERROR: %s: %s", kernel_mp_file, strerror (errno));

          return -1;
        }

        #endif

        load_kernel (kernel_mp_file, 1, kernel_mp_lengths, kernel_mp_sources);

        if (quiet == 0) log_info ("Device #%u: Kernel %s (%ld bytes)", device_id + 1, kernel_mp_file, st.st_size);

        #ifdef BINARY_KERNEL
        device_param->program_mp = hc_clCreateProgramWithBinary (device_param->context, 1, &device_param->device, kernel_mp_lengths, (const unsigned char **) kernel_mp_sources, NULL);
        #else
        device_param->program_mp = hc_clCreateProgramWithSource (device_param->context, 1, (const char **) kernel_mp_sources, NULL);
        #endif

        local_free (kernel_mp_lengths);

        local_free (kernel_mp_sources[0]);

        local_free (kernel_mp_sources);
      }

      /**
       * kernel amp find
       */

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        // nothing to do
      }
      else
      {
        char kernel_amp_file[256];

        memset (kernel_amp_file, 0, sizeof (kernel_amp_file));

        size_t *kernel_amp_lengths = (size_t *) mymalloc (sizeof (size_t));

        const unsigned char **kernel_amp_sources = (const unsigned char **) mymalloc (sizeof (unsigned char *));

        #ifdef BINARY_KERNEL
        snprintf (kernel_amp_file, sizeof (kernel_amp_file) - 1, "%s/kernels/4098/amp_a%d_v%d.%s_%s_%s_%d.kernel", install_dir, attack_kern, gpu_vector_width, device_name, device_version, driver_version, COMPTIME);

        if (stat (kernel_amp_file, &st) == -1)
        {
          if (quiet == 0) log_info ("Device #%u: Kernel %s not found in cache! Building may take a while...", device_id + 1, kernel_amp_file);

          char module_amp_file[256];

          memset (module_amp_file, 0, sizeof (module_amp_file));

          snprintf (module_amp_file, sizeof (module_amp_file) - 1, "%s/kernels/4098/amp_a%d_v%d.llvmir", install_dir, attack_kern, gpu_vector_width);

          load_kernel (module_amp_file, 1, kernel_amp_lengths, kernel_amp_sources);

          cl_program program_amp = hc_clCreateProgramWithBinary (device_param->context, 1, &device_param->device, kernel_amp_lengths, (const unsigned char **) kernel_amp_sources, NULL);

          local_free (kernel_amp_sources[0]);

          hc_clBuildProgram (program_amp, 1, &device_param->device, "-cl-std=CL1.2", NULL, NULL);

          size_t binary_amp_size;

          clGetProgramInfo (program_amp, CL_PROGRAM_BINARY_SIZES, sizeof (size_t), &binary_amp_size, NULL);

          unsigned char *binary_amp = (unsigned char *) mymalloc (binary_amp_size);

          clGetProgramInfo (program_amp, CL_PROGRAM_BINARIES, sizeof (binary_amp), &binary_amp, NULL);

          writeProgramBin (kernel_amp_file, binary_amp, binary_amp_size);

          local_free (binary_amp);

          stat (kernel_amp_file, &st); // to reload filesize
        }
        #else
        snprintf (kernel_amp_file, sizeof (kernel_amp_file) - 1, "%s/amd/amp_a%d_v%d.cl", install_dir, attack_kern, gpu_vector_width);

        if (stat (kernel_amp_file, &st) == -1)
        {
          log_error ("ERROR: %s: %s", kernel_amp_file, strerror (errno));

          return -1;
        }
        #endif

        load_kernel (kernel_amp_file, 1, kernel_amp_lengths, kernel_amp_sources);

        if (quiet == 0) log_info ("Device #%u: Kernel %s (%ld bytes)", device_id + 1, kernel_amp_file, st.st_size);

        #ifdef BINARY_KERNEL
        device_param->program_amp = hc_clCreateProgramWithBinary (device_param->context, 1, &device_param->device, kernel_amp_lengths, (const unsigned char **) kernel_amp_sources, NULL);
        #else
        device_param->program_amp = hc_clCreateProgramWithSource (device_param->context, 1, (const char **) kernel_amp_sources, NULL);
        #endif

        local_free (kernel_amp_lengths);

        local_free (kernel_amp_sources[0]);

        local_free (kernel_amp_sources);
      }

      /**
       * kernel compile
       */

      char *build_opts = NULL;

      #ifndef BINARY_KERNEL
      build_opts = (char *) mymalloc (256);

      sprintf (build_opts, "-I . -I amd/ -D VLIW%d -x clc++ -cl-std=CL1.2", vliw);

      if ((hash_mode == 8900) || (hash_mode == 9300))
      {
        sprintf (build_opts, "-I . -I amd/ -D VLIW%d -x clc++ -cl-std=CL1.2 -DSCRYPT_N=%d -DSCRYPT_R=%d -DSCRYPT_P=%d -DSCRYPT_TMTO=%d", vliw, data.salts_buf[0].scrypt_N, data.salts_buf[0].scrypt_r, data.salts_buf[0].scrypt_p, data.salts_buf[0].scrypt_tmto);
      }
      #endif

      clBuildProgram (device_param->program, 1, &device_param->device, build_opts, NULL, NULL);

      size_t ret_val_size = 0;

      clGetProgramBuildInfo (device_param->program, device_param->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);

      if (ret_val_size > 1)
      {
        char *build_log = (char *) malloc (ret_val_size + 1);

        memset (build_log, 0, ret_val_size + 1);

        clGetProgramBuildInfo (device_param->program, device_param->device, CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);

        puts (build_log);

        free (build_log);
      }

      if (attack_mode != ATTACK_MODE_STRAIGHT)
      {
        hc_clBuildProgram (device_param->program_mp, 1, &device_param->device, build_opts, NULL, NULL);
      }

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        // nothing to do
      }
      else
      {
        hc_clBuildProgram (device_param->program_amp, 1, &device_param->device, build_opts, NULL, NULL);
      }

      /**
       * amp is not independent
       */

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        // nothing to do
      }
      else
      {
        device_param->kernel_amp = hc_clCreateKernel (device_param->program_amp, "amp");
      }

      /**
       * global buffers
       */

      device_param->d_pws_buf       = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   size_pws,     NULL);
      device_param->d_pws_amp_buf   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   size_pws,     NULL);
      device_param->d_tmps          = hc_clCreateBuffer (device_param->context, CL_MEM_READ_WRITE,  size_tmps,    NULL);
      device_param->d_hooks         = hc_clCreateBuffer (device_param->context, CL_MEM_READ_WRITE,  size_hooks,   NULL);
      device_param->d_bitmap_s1_a   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   bitmap_size,  NULL);
      device_param->d_bitmap_s1_b   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   bitmap_size,  NULL);
      device_param->d_bitmap_s1_c   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   bitmap_size,  NULL);
      device_param->d_bitmap_s1_d   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   bitmap_size,  NULL);
      device_param->d_bitmap_s2_a   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   bitmap_size,  NULL);
      device_param->d_bitmap_s2_b   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   bitmap_size,  NULL);
      device_param->d_bitmap_s2_c   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   bitmap_size,  NULL);
      device_param->d_bitmap_s2_d   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   bitmap_size,  NULL);
      device_param->d_plain_bufs    = hc_clCreateBuffer (device_param->context, CL_MEM_READ_WRITE,  size_plains,  NULL);
      device_param->d_digests_buf   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   size_digests, NULL);
      device_param->d_digests_shown = hc_clCreateBuffer (device_param->context, CL_MEM_READ_WRITE,  size_shown,   NULL);
      device_param->d_salt_bufs     = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY,   size_salts,   NULL);
      device_param->d_result        = hc_clCreateBuffer (device_param->context, CL_MEM_READ_WRITE,  size_results, NULL);
      device_param->d_scryptV_buf   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_WRITE,  size_scryptV, NULL);

      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_bitmap_s1_a,    CL_TRUE, 0, bitmap_size,  bitmap_s1_a,        0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_bitmap_s1_b,    CL_TRUE, 0, bitmap_size,  bitmap_s1_b,        0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_bitmap_s1_c,    CL_TRUE, 0, bitmap_size,  bitmap_s1_c,        0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_bitmap_s1_d,    CL_TRUE, 0, bitmap_size,  bitmap_s1_d,        0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_bitmap_s2_a,    CL_TRUE, 0, bitmap_size,  bitmap_s2_a,        0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_bitmap_s2_b,    CL_TRUE, 0, bitmap_size,  bitmap_s2_b,        0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_bitmap_s2_c,    CL_TRUE, 0, bitmap_size,  bitmap_s2_c,        0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_bitmap_s2_d,    CL_TRUE, 0, bitmap_size,  bitmap_s2_d,        0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_digests_buf,    CL_TRUE, 0, size_digests, data.digests_buf,   0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_digests_shown,  CL_TRUE, 0, size_shown,   data.digests_shown, 0, NULL, NULL);
      hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_salt_bufs,      CL_TRUE, 0, size_salts,   data.salts_buf,     0, NULL, NULL);

      run_kernel_bzero (device_param, device_param->d_pws_buf,        size_pws);
      run_kernel_bzero (device_param, device_param->d_pws_amp_buf,    size_pws);
      run_kernel_bzero (device_param, device_param->d_tmps,           size_tmps);
      run_kernel_bzero (device_param, device_param->d_hooks,          size_hooks);
      run_kernel_bzero (device_param, device_param->d_plain_bufs,     size_plains);
      run_kernel_bzero (device_param, device_param->d_result,         size_results);

      /**
       * special buffers
       */

      if (attack_kern == ATTACK_KERN_STRAIGHT)
      {
        device_param->d_rules   = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_rules,   NULL);
        device_param->d_rules_c = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_rules_c, NULL);

        hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_rules, CL_TRUE, 0, size_rules, gpu_rules_buf, 0, NULL, NULL);

        run_kernel_bzero (device_param, device_param->d_rules_c,     size_rules_c);
      }
      else if (attack_kern == ATTACK_KERN_COMBI)
      {
        device_param->d_combs           = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_combs,      NULL);
        device_param->d_combs_c         = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_combs,      NULL);
        device_param->d_root_css_buf    = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_root_css,   NULL);
        device_param->d_markov_css_buf  = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_markov_css, NULL);

        run_kernel_bzero (device_param, device_param->d_combs,          size_combs);
        run_kernel_bzero (device_param, device_param->d_combs_c,        size_combs);
        run_kernel_bzero (device_param, device_param->d_root_css_buf,   size_root_css);
        run_kernel_bzero (device_param, device_param->d_markov_css_buf, size_markov_css);
      }
      else if (attack_kern == ATTACK_KERN_BF)
      {
        device_param->d_bfs             = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_bfs,        NULL);
        device_param->d_bfs_c           = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_bfs,        NULL);
        device_param->d_tm_c            = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_tm,         NULL);
        device_param->d_root_css_buf    = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_root_css,   NULL);
        device_param->d_markov_css_buf  = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_markov_css, NULL);

        run_kernel_bzero (device_param, device_param->d_bfs,            size_bfs);
        run_kernel_bzero (device_param, device_param->d_bfs_c,          size_bfs);
        run_kernel_bzero (device_param, device_param->d_tm_c,           size_tm);
        run_kernel_bzero (device_param, device_param->d_root_css_buf,   size_root_css);
        run_kernel_bzero (device_param, device_param->d_markov_css_buf, size_markov_css);
      }

      if (size_esalts)
      {
        device_param->d_esalt_bufs = hc_clCreateBuffer (device_param->context, CL_MEM_READ_ONLY, size_esalts, NULL);

        hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_esalt_bufs, CL_TRUE, 0, size_esalts, data.esalts_buf, 0, NULL, NULL);
      }

      /**
       * main host data
       */

      uint *result = (uint *) mymalloc (size_results);

      memset (result, 0, size_results);

      device_param->result = result;

      pw_t *pws_buf = (pw_t *) mymalloc (size_pws);

      memset (pws_buf, 0, size_pws);

      device_param->pws_buf = pws_buf;

      pw_cache_t *pw_caches = (pw_cache_t *) mycalloc (64, sizeof (pw_cache_t));

      for (int i = 0; i < 64; i++)
      {
        pw_caches[i].pw_buf.pw_len = i;
        pw_caches[i].cnt = 0;
      }

      device_param->pw_caches = pw_caches;

      comb_t *combs_buf = (comb_t *) mycalloc (GPU_COMBS, sizeof (comb_t));

      device_param->combs_buf = combs_buf;

      void *hooks_buf = mymalloc (size_hooks);

      device_param->hooks_buf = hooks_buf;

      switch (device_param->gpu_vector_width)
      {
        case 1: device_param->pw_transpose  = pw_transpose_to_hi1;
                device_param->pw_add        = pw_add_to_hc1;
                break;
        case 2: device_param->pw_transpose  = pw_transpose_to_hi2;
                device_param->pw_add        = pw_add_to_hc2;
                break;
        case 4: device_param->pw_transpose  = pw_transpose_to_hi4;
                device_param->pw_add        = pw_add_to_hc4;
                break;
      }

      /**
       * kernel args
       */

      device_param->kernel_params_buf32[21] = bitmap_mask;
      device_param->kernel_params_buf32[22] = bitmap_shift1;
      device_param->kernel_params_buf32[23] = bitmap_shift2;
      device_param->kernel_params_buf32[24] = 0; // salt_pos
      device_param->kernel_params_buf32[25] = 0; // loop_pos
      device_param->kernel_params_buf32[26] = 0; // loop_cnt
      device_param->kernel_params_buf32[27] = 0; // gpu_rules_cnt
      device_param->kernel_params_buf32[28] = 0; // digests_cnt
      device_param->kernel_params_buf32[29] = 0; // digests_offset
      device_param->kernel_params_buf32[30] = 0; // combs_mode
      device_param->kernel_params_buf32[31] = 0; // gid_max

      device_param->kernel_params[ 0] = (attack_exec == ATTACK_EXEC_ON_GPU)
                                      ? &device_param->d_pws_buf
                                      : &device_param->d_pws_amp_buf;
      device_param->kernel_params[ 1] = &device_param->d_rules_c;
      device_param->kernel_params[ 2] = &device_param->d_combs_c;
      device_param->kernel_params[ 3] = &device_param->d_bfs_c;
      device_param->kernel_params[ 4] = &device_param->d_tmps;
      device_param->kernel_params[ 5] = &device_param->d_hooks;
      device_param->kernel_params[ 6] = &device_param->d_bitmap_s1_a;
      device_param->kernel_params[ 7] = &device_param->d_bitmap_s1_b;
      device_param->kernel_params[ 8] = &device_param->d_bitmap_s1_c;
      device_param->kernel_params[ 9] = &device_param->d_bitmap_s1_d;
      device_param->kernel_params[10] = &device_param->d_bitmap_s2_a;
      device_param->kernel_params[11] = &device_param->d_bitmap_s2_b;
      device_param->kernel_params[12] = &device_param->d_bitmap_s2_c;
      device_param->kernel_params[13] = &device_param->d_bitmap_s2_d;
      device_param->kernel_params[14] = &device_param->d_plain_bufs;
      device_param->kernel_params[15] = &device_param->d_digests_buf;
      device_param->kernel_params[16] = &device_param->d_digests_shown;
      device_param->kernel_params[17] = &device_param->d_salt_bufs;
      device_param->kernel_params[18] = &device_param->d_esalt_bufs;
      device_param->kernel_params[19] = &device_param->d_result;
      device_param->kernel_params[20] = &device_param->d_scryptV_buf;
      device_param->kernel_params[21] = &device_param->kernel_params_buf32[21];
      device_param->kernel_params[22] = &device_param->kernel_params_buf32[22];
      device_param->kernel_params[23] = &device_param->kernel_params_buf32[23];
      device_param->kernel_params[24] = &device_param->kernel_params_buf32[24];
      device_param->kernel_params[25] = &device_param->kernel_params_buf32[25];
      device_param->kernel_params[26] = &device_param->kernel_params_buf32[26];
      device_param->kernel_params[27] = &device_param->kernel_params_buf32[27];
      device_param->kernel_params[28] = &device_param->kernel_params_buf32[28];
      device_param->kernel_params[29] = &device_param->kernel_params_buf32[29];
      device_param->kernel_params[30] = &device_param->kernel_params_buf32[30];
      device_param->kernel_params[31] = &device_param->kernel_params_buf32[31];

      device_param->kernel_params_mp_buf64[3] = 0;
      device_param->kernel_params_mp_buf32[4] = 0;
      device_param->kernel_params_mp_buf32[5] = 0;
      device_param->kernel_params_mp_buf32[6] = 0;
      device_param->kernel_params_mp_buf32[7] = 0;
      device_param->kernel_params_mp_buf32[8] = 0;

      device_param->kernel_params_mp[0] = NULL;
      device_param->kernel_params_mp[1] = NULL;
      device_param->kernel_params_mp[2] = NULL;
      device_param->kernel_params_mp[3] = &device_param->kernel_params_mp_buf64[3];
      device_param->kernel_params_mp[4] = &device_param->kernel_params_mp_buf32[4];
      device_param->kernel_params_mp[5] = &device_param->kernel_params_mp_buf32[5];
      device_param->kernel_params_mp[6] = &device_param->kernel_params_mp_buf32[6];
      device_param->kernel_params_mp[7] = &device_param->kernel_params_mp_buf32[7];
      device_param->kernel_params_mp[8] = &device_param->kernel_params_mp_buf32[8];

      device_param->kernel_params_mp_l_buf64[3] = 0;
      device_param->kernel_params_mp_l_buf32[4] = 0;
      device_param->kernel_params_mp_l_buf32[5] = 0;
      device_param->kernel_params_mp_l_buf32[6] = 0;
      device_param->kernel_params_mp_l_buf32[7] = 0;
      device_param->kernel_params_mp_l_buf32[8] = 0;
      device_param->kernel_params_mp_l_buf32[9] = 0;

      device_param->kernel_params_mp_l[0] = NULL;
      device_param->kernel_params_mp_l[1] = NULL;
      device_param->kernel_params_mp_l[2] = NULL;
      device_param->kernel_params_mp_l[3] = &device_param->kernel_params_mp_l_buf64[3];
      device_param->kernel_params_mp_l[4] = &device_param->kernel_params_mp_l_buf32[4];
      device_param->kernel_params_mp_l[5] = &device_param->kernel_params_mp_l_buf32[5];
      device_param->kernel_params_mp_l[6] = &device_param->kernel_params_mp_l_buf32[6];
      device_param->kernel_params_mp_l[7] = &device_param->kernel_params_mp_l_buf32[7];
      device_param->kernel_params_mp_l[8] = &device_param->kernel_params_mp_l_buf32[8];
      device_param->kernel_params_mp_l[9] = &device_param->kernel_params_mp_l_buf32[9];

      device_param->kernel_params_mp_r_buf64[3] = 0;
      device_param->kernel_params_mp_r_buf32[4] = 0;
      device_param->kernel_params_mp_r_buf32[5] = 0;
      device_param->kernel_params_mp_r_buf32[6] = 0;
      device_param->kernel_params_mp_r_buf32[7] = 0;
      device_param->kernel_params_mp_r_buf32[8] = 0;

      device_param->kernel_params_mp_r[0] = NULL;
      device_param->kernel_params_mp_r[1] = NULL;
      device_param->kernel_params_mp_r[2] = NULL;
      device_param->kernel_params_mp_r[3] = &device_param->kernel_params_mp_r_buf64[3];
      device_param->kernel_params_mp_r[4] = &device_param->kernel_params_mp_r_buf32[4];
      device_param->kernel_params_mp_r[5] = &device_param->kernel_params_mp_r_buf32[5];
      device_param->kernel_params_mp_r[6] = &device_param->kernel_params_mp_r_buf32[6];
      device_param->kernel_params_mp_r[7] = &device_param->kernel_params_mp_r_buf32[7];
      device_param->kernel_params_mp_r[8] = &device_param->kernel_params_mp_r_buf32[8];

      device_param->kernel_params_amp_buf32[5] = 0; // combs_mode
      device_param->kernel_params_amp_buf32[6] = 0; // gid_max

      device_param->kernel_params_amp[0] = &device_param->d_pws_buf;
      device_param->kernel_params_amp[1] = &device_param->d_pws_amp_buf;
      device_param->kernel_params_amp[2] = &device_param->d_rules_c;
      device_param->kernel_params_amp[3] = &device_param->d_combs_c;
      device_param->kernel_params_amp[4] = &device_param->d_bfs_c;
      device_param->kernel_params_amp[5] = &device_param->kernel_params_amp_buf32[5];
      device_param->kernel_params_amp[6] = &device_param->kernel_params_amp_buf32[6];

      device_param->kernel_params_tb[0] = &device_param->d_pws_buf;

      device_param->kernel_params_tm[0] = &device_param->d_bfs_c;
      device_param->kernel_params_tm[1] = &device_param->d_tm_c;

      /**
       * kernel name
       */

      char kernel_name[64];

      memset (kernel_name, 0, sizeof (kernel_name));

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        if (opti_type & OPTI_TYPE_SINGLE_HASH)
        {
          snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_s%02d", kern_type, 4);

          device_param->kernel1 = hc_clCreateKernel (device_param->program, kernel_name);

          snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_s%02d", kern_type, 8);

          device_param->kernel2 = hc_clCreateKernel (device_param->program, kernel_name);

          snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_s%02d", kern_type, 16);

          device_param->kernel3 = hc_clCreateKernel (device_param->program, kernel_name);
        }
        else
        {
          snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_m%02d",  kern_type, 4);

          device_param->kernel1 = hc_clCreateKernel (device_param->program, kernel_name);

          snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_m%02d",  kern_type, 8);

          device_param->kernel2 = hc_clCreateKernel (device_param->program, kernel_name);

          snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_m%02d",  kern_type, 16);

          device_param->kernel3 = hc_clCreateKernel (device_param->program, kernel_name);
        }

        if (data.attack_mode == ATTACK_MODE_BF)
        {
          if (opts_type & OPTS_TYPE_PT_BITSLICE)
          {
            snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_tb", kern_type);

            device_param->kernel_tb = hc_clCreateKernel (device_param->program, kernel_name);

            snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_tm", kern_type);

            device_param->kernel_tm = hc_clCreateKernel (device_param->program, kernel_name);
          }
        }
      }
      else
      {
        snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_init", kern_type);

        device_param->kernel1 = hc_clCreateKernel (device_param->program, kernel_name);

        snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_loop", kern_type);

        device_param->kernel2 = hc_clCreateKernel (device_param->program, kernel_name);

        snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_comp", kern_type);

        device_param->kernel3 = hc_clCreateKernel (device_param->program, kernel_name);

        if (opts_type & OPTS_TYPE_HOOK12)
        {
          snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_hook12", kern_type);

          device_param->kernel12 = hc_clCreateKernel (device_param->program, kernel_name);
        }

        if (opts_type & OPTS_TYPE_HOOK23)
        {
          snprintf (kernel_name, sizeof (kernel_name) - 1, "m%05d_hook23", kern_type);

          device_param->kernel23 = hc_clCreateKernel (device_param->program, kernel_name);
        }
      }

      for (uint i = 0; i <= 20; i++)
      {
        hc_clSetKernelArg (device_param->kernel1, i, sizeof (cl_mem), device_param->kernel_params[i]);
        hc_clSetKernelArg (device_param->kernel2, i, sizeof (cl_mem), device_param->kernel_params[i]);
        hc_clSetKernelArg (device_param->kernel3, i, sizeof (cl_mem), device_param->kernel_params[i]);

        if (opts_type & OPTS_TYPE_HOOK12) hc_clSetKernelArg (device_param->kernel12, i, sizeof (cl_mem), device_param->kernel_params[i]);
        if (opts_type & OPTS_TYPE_HOOK23) hc_clSetKernelArg (device_param->kernel23, i, sizeof (cl_mem), device_param->kernel_params[i]);
      }

      for (uint i = 21; i <= 31; i++)
      {
        hc_clSetKernelArg (device_param->kernel1, i, sizeof (cl_uint), device_param->kernel_params[i]);
        hc_clSetKernelArg (device_param->kernel2, i, sizeof (cl_uint), device_param->kernel_params[i]);
        hc_clSetKernelArg (device_param->kernel3, i, sizeof (cl_uint), device_param->kernel_params[i]);

        if (opts_type & OPTS_TYPE_HOOK12) hc_clSetKernelArg (device_param->kernel12, i, sizeof (cl_uint), device_param->kernel_params[i]);
        if (opts_type & OPTS_TYPE_HOOK23) hc_clSetKernelArg (device_param->kernel23, i, sizeof (cl_uint), device_param->kernel_params[i]);
      }

      if (attack_mode == ATTACK_MODE_BF)
      {
        device_param->kernel_mp_l = hc_clCreateKernel (device_param->program_mp, "l_markov");
        device_param->kernel_mp_r = hc_clCreateKernel (device_param->program_mp, "r_markov");

        if (opts_type & OPTS_TYPE_PT_BITSLICE)
        {
          hc_clSetKernelArg (device_param->kernel_tb, 0, sizeof (cl_mem), device_param->kernel_params_tb[0]);

          hc_clSetKernelArg (device_param->kernel_tm, 0, sizeof (cl_mem), device_param->kernel_params_tm[0]);
          hc_clSetKernelArg (device_param->kernel_tm, 1, sizeof (cl_mem), device_param->kernel_params_tm[1]);
        }
      }
      else if (attack_mode == ATTACK_MODE_HYBRID1)
      {
        device_param->kernel_mp = hc_clCreateKernel (device_param->program_mp, "C_markov");
      }
      else if (attack_mode == ATTACK_MODE_HYBRID2)
      {
        device_param->kernel_mp = hc_clCreateKernel (device_param->program_mp, "C_markov");
      }

      if (attack_exec == ATTACK_EXEC_ON_GPU)
      {
        // nothing to do
      }
      else
      {
        for (uint i = 0; i < 5; i++)
        {
          hc_clSetKernelArg (device_param->kernel_amp, i, sizeof (cl_mem), device_param->kernel_params_amp[i]);
        }

        for (uint i = 5; i < 7; i++)
        {
          hc_clSetKernelArg (device_param->kernel_amp, i, sizeof (cl_uint), device_param->kernel_params_amp[i]);
        }
      }

      /**
       * Store initial fanspeed if gpu_temp_retain is enabled
       */

      if (gpu_temp_disable == 0)
      {
        if (gpu_temp_retain != 0)
        {
          hc_thread_mutex_lock (mux_adl);

          if (data.hm_device[device_id].fan_supported == 1)
          {
            if (gpu_temp_retain_chgd == 0)
            {
              uint cur_temp = 0;
              uint default_temp = 0;

              int ADL_rc = hc_ADL_Overdrive6_TargetTemperatureData_Get (data.hm_dll, data.hm_device[device_id].adapter_index, (int *) &cur_temp, (int *) &default_temp);

              if (ADL_rc == ADL_OK)
              {
                #define GPU_TEMP_RETAIN_ABORT_DIFF 15

                const uint gpu_temp_retain_target = default_temp - GPU_TEMP_RETAIN_ABORT_DIFF;

                // special case with multi gpu setups: always use minimum retain

                if (gpu_temp_retain_set == 0)
                {
                  gpu_temp_retain = gpu_temp_retain_target;
                  gpu_temp_retain_set = 1;
                }
                else
                {
                  gpu_temp_retain = MIN (gpu_temp_retain, gpu_temp_retain_target);
                }

                if (gpu_temp_abort_chgd == 0) gpu_temp_abort = gpu_temp_retain + GPU_TEMP_RETAIN_ABORT_DIFF;
              }
            }

            const int fan_speed = hm_get_fanspeed_with_device_id (device_id);

            temp_retain_fanspeed_value[device_id] = fan_speed;

            if (fan_speed == -1)
            {
              log_info ("WARNING: Failed to get current fan speed settings for gpu number: %i:", device_id + 1);

              temp_retain_fanspeed_value[device_id] = 0;
            }
          }

          hc_thread_mutex_unlock (mux_adl);
        }
      }

      /**
       * Store original powercontrol/clocks settings, set overdrive 6 performance tuning settings
       */

      if (powertune_enable == 1)
      {
        hc_thread_mutex_lock (mux_adl);

        if (data.hm_device[device_id].od_version == 6)
        {
          int ADL_rc;

          // check powertune capabilities first, if not available then skip device

          int powertune_supported = 0;

          if ((ADL_rc = hc_ADL_Overdrive6_PowerControl_Caps (data.hm_dll, data.hm_device[device_id].adapter_index, &powertune_supported)) != ADL_OK)
          {
            log_error ("ERROR: Failed to get ADL PowerControl Capabilities");

            return (-1);
          }

          if (powertune_supported != 0)
          {
            // powercontrol settings

            ADLOD6PowerControlInfo powertune = {0, 0, 0, 0, 0};

            if ((ADL_rc = hc_ADL_Overdrive_PowerControlInfo_Get (data.hm_dll, data.hm_device[device_id].adapter_index, &powertune)) == ADL_OK)
            {
              ADL_rc = hc_ADL_Overdrive_PowerControl_Get (data.hm_dll, data.hm_device[device_id].adapter_index, &od_power_control_status[device_id]);
            }

            if (ADL_rc != ADL_OK)
            {
              log_error ("ERROR: Failed to get current ADL PowerControl settings");

              return (-1);
            }

            if ((ADL_rc = hc_ADL_Overdrive_PowerControl_Set (data.hm_dll, data.hm_device[device_id].adapter_index, powertune.iMaxValue)) != ADL_OK)
            {
              log_error ("ERROR: Failed to set new ADL PowerControl values");

              return (-1);
            }

            // clocks

            memset (&od_clock_mem_status[device_id], 0, sizeof (ADLOD6MemClockState));

            od_clock_mem_status[device_id].state.iNumberOfPerformanceLevels = 2;

            if ((ADL_rc = hc_ADL_Overdrive_StateInfo_Get (data.hm_dll, data.hm_device[device_id].adapter_index, ADL_OD6_GETSTATEINFO_CUSTOM_PERFORMANCE, &od_clock_mem_status[device_id])) != ADL_OK)
            {
              log_error ("ERROR: Failed to get ADL memory and engine clock frequency");

              return (-1);
            }

            // Query capabilities only to see if profiles were not "damaged", if so output a warning but do accept the users profile settings

            ADLOD6Capabilities caps = {0, 0, 0, {0, 0, 0}, {0, 0, 0}, 0, 0};

            if ((ADL_rc = hc_ADL_Overdrive_Capabilities_Get (data.hm_dll, data.hm_device[device_id].adapter_index, &caps)) != ADL_OK)
            {
              log_error ("ERROR: Failed to get ADL device capabilities");

              return (-1);
            }

            int engine_clock_max = caps.sEngineClockRange.iMax * 0.6666;
            int memory_clock_max = caps.sMemoryClockRange.iMax * 0.6250;

            int warning_trigger_engine = (int) (0.25 * (float) engine_clock_max);
            int warning_trigger_memory = (int) (0.25 * (float) memory_clock_max);

            int engine_clock_profile_max = od_clock_mem_status[device_id].state.aLevels[1].iEngineClock;
            int memory_clock_profile_max = od_clock_mem_status[device_id].state.aLevels[1].iMemoryClock;

            // warning if profile has to low max values

            if ((engine_clock_max - engine_clock_profile_max) > warning_trigger_engine)
            {
              log_info ("WARN: the custom profile seems to have too low maximum engine clock values. You therefore may not reach full performance");
            }

            if ((memory_clock_max - memory_clock_profile_max) > warning_trigger_memory)
            {
              log_info ("WARN: the custom profile seems to have too low maximum memory clock values. You therefore may not reach full performance");
            }

            ADLOD6StateInfo *performance_state = (ADLOD6StateInfo*) mycalloc (1, sizeof (ADLOD6StateInfo) + sizeof (ADLOD6PerformanceLevel));

            performance_state->iNumberOfPerformanceLevels = 2;

            performance_state->aLevels[0].iEngineClock = engine_clock_profile_max;
            performance_state->aLevels[1].iEngineClock = engine_clock_profile_max;
            performance_state->aLevels[0].iMemoryClock = memory_clock_profile_max;
            performance_state->aLevels[1].iMemoryClock = memory_clock_profile_max;

            if ((ADL_rc = hc_ADL_Overdrive_State_Set (data.hm_dll, data.hm_device[device_id].adapter_index, ADL_OD6_SETSTATE_PERFORMANCE, performance_state)) != ADL_OK)
            {
              log_info ("ERROR: Failed to set ADL performance state");

              return (-1);
            }

            local_free (performance_state);
          }
        }

        hc_thread_mutex_unlock (mux_adl);
      }
    }

    /* Temporary fix:
     * with AMD r9 295x cards it seems that we need to set the powertune value just AFTER the ocl init stuff
     * otherwise after hc_clCreateContext () etc, powertune value was set back to "normal" and cards unfortunately
     * were not working @ full speed (setting hc_ADL_Overdrive_PowerControl_Set () here seems to fix the problem)
     * Driver / ADL bug?
     */

    if (powertune_enable == 1)
    {
      hc_thread_mutex_lock (mux_adl);

      for (uint i = 0; i < devices_cnt; i++)
      {
        if (data.hm_device[i].od_version == 6)
        {
          // set powertune value only

          int powertune_supported = 0;

          int ADL_rc = 0;

          if ((ADL_rc = hc_ADL_Overdrive6_PowerControl_Caps (data.hm_dll, data.hm_device[i].adapter_index, &powertune_supported)) != ADL_OK)
          {
            log_error ("ERROR: Failed to get ADL PowerControl Capabilities");

            return (-1);
          }

          if (powertune_supported != 0)
          {
            // powertune set
            ADLOD6PowerControlInfo powertune = {0, 0, 0, 0, 0};

            if ((ADL_rc = hc_ADL_Overdrive_PowerControlInfo_Get (data.hm_dll, data.hm_device[i].adapter_index, &powertune)) != ADL_OK)
            {
              log_error ("ERROR: Failed to get current ADL PowerControl settings");

              return (-1);
            }

            if ((ADL_rc = hc_ADL_Overdrive_PowerControl_Set (data.hm_dll, data.hm_device[i].adapter_index, powertune.iMaxValue)) != ADL_OK)
            {
              log_error ("ERROR: Failed to set new ADL PowerControl values");

              return (-1);
            }
          }
        }
      }

      hc_thread_mutex_unlock (mux_adl);
    }
    #endif

    data.gpu_blocks_all = gpu_blocks_all;

    #ifdef _OCL
    if (gpu_async == 0) gpu_async = 1; // get rid of the warning
    #endif

    if (data.quiet == 0) log_info ("");

    /**
     * Inform user which algorithm is checked and at which workload setting
     */

    if (benchmark == 1)
    {
      quiet = 0;

      data.quiet = quiet;

      char *hash_type = strhashtype (data.hash_mode); // not a bug

      log_info ("Hashtype: %s", hash_type);
      log_info ("Workload: %u loops, %u accel", gpu_loops, gpu_accel);
      log_info ("");
    }

    /**
     * keep track of the progress
     */

    data.words_progress_done     = (uint64_t *) mycalloc (data.salts_cnt, sizeof (uint64_t));
    data.words_progress_rejected = (uint64_t *) mycalloc (data.salts_cnt, sizeof (uint64_t));
    data.words_progress_restored = (uint64_t *) mycalloc (data.salts_cnt, sizeof (uint64_t));

    /**
     * open filehandles
     */

    #if _WIN
    if (_setmode (_fileno (stdin), _O_BINARY) == -1)
    {
      log_error ("ERROR: %s: %s", "stdin", strerror (errno));

      return (-1);
    }

    if (_setmode (_fileno (stdout), _O_BINARY) == -1)
    {
      log_error ("ERROR: %s: %s", "stdout", strerror (errno));

      return (-1);
    }

    if (_setmode (_fileno (stderr), _O_BINARY) == -1)
    {
      log_error ("ERROR: %s: %s", "stderr", strerror (errno));

      return (-1);
    }
    #endif

    /**
     * dictionary pad
     */

    segment_size *= (1024 * 1024);

    data.segment_size = segment_size;

    wl_data_t *wl_data = (wl_data_t *) mymalloc (sizeof (wl_data_t));

    wl_data->buf   = (char *) mymalloc (segment_size);
    wl_data->avail = segment_size;
    wl_data->incr  = segment_size;
    wl_data->cnt   = 0;
    wl_data->pos   = 0;

    uint wordlist_mode = ((optind + 1) < myargc) ? WL_MODE_FILE : WL_MODE_STDIN;

    data.wordlist_mode = wordlist_mode;

    cs_t  *css_buf   = NULL;
    uint   css_cnt   = 0;
    uint   dictcnt   = 0;
    uint   maskcnt   = 1;
    char **masks     = NULL;
    char **dictfiles = NULL;

    uint   mask_from_file = 0;

    if (attack_mode == ATTACK_MODE_STRAIGHT)
    {
      if (wordlist_mode == WL_MODE_FILE)
      {
        int wls_left = myargc - (optind + 1);

        for (int i = 0; i < wls_left; i++)
        {
          char *l0_filename = myargv[optind + 1 + i];

          struct stat l0_stat;

          if (stat (l0_filename, &l0_stat) == -1)
          {
            log_error ("ERROR: %s: %s", l0_filename, strerror (errno));

            return (-1);
          }

          uint is_dir = S_ISDIR (l0_stat.st_mode);

          if (is_dir == 0)
          {
            dictfiles = (char **) myrealloc (dictfiles, dictcnt * sizeof (char *), sizeof (char *));

            dictcnt++;

            dictfiles[dictcnt - 1] = l0_filename;
          }
          else
          {
            // do not allow --keyspace w/ a directory

            if (keyspace == 1)
            {
              log_error ("ERROR: keyspace parameter is not allowed together with a directory");

              return (-1);
            }

            char **dictionary_files = NULL;

            dictionary_files = scan_directory (l0_filename);

            if (dictionary_files != NULL)
            {
              qsort (dictionary_files, count_dictionaries (dictionary_files), sizeof (char *), sort_by_stringptr);

              for (int d = 0; dictionary_files[d] != NULL; d++)
              {
                char *l1_filename = dictionary_files[d];

                struct stat l1_stat;

                if (stat (l1_filename, &l1_stat) == -1)
                {
                  log_error ("ERROR: %s: %s", l1_filename, strerror (errno));

                  return (-1);
                }

                if (S_ISREG (l1_stat.st_mode))
                {
                  dictfiles = (char **) myrealloc (dictfiles, dictcnt * sizeof (char *), sizeof (char *));

                  dictcnt++;

                  dictfiles[dictcnt - 1] = strdup (l1_filename);
                }
              }
            }

            local_free (dictionary_files);
          }
        }

        if (dictcnt < 1)
        {
          log_error ("ERROR: No usable dictionary file found.");

          return (-1);
        }
      }
      else if (wordlist_mode == WL_MODE_STDIN)
      {
        dictcnt = 1;
      }
    }
    else if (attack_mode == ATTACK_MODE_COMBI)
    {
      // display

      char *dictfile1 = myargv[optind + 1 + 0];
      char *dictfile2 = myargv[optind + 1 + 1];

      // find the bigger dictionary and use as base

      FILE *fp1;
      FILE *fp2;

      struct stat tmp_stat;

      if ((fp1 = fopen (dictfile1, "rb")) == NULL)
      {
        log_error ("ERROR: %s: %s", dictfile1, strerror (errno));

        return (-1);
      }

      if (stat (dictfile1, &tmp_stat) == -1)
      {
        log_error ("ERROR: %s: %s", dictfile1, strerror (errno));

        fclose (fp1);

        return (-1);
      }

      if (S_ISDIR (tmp_stat.st_mode))
      {
        log_error ("ERROR: %s must be a regular file", dictfile1, strerror (errno));

        fclose (fp1);

        return (-1);
      }

      if ((fp2 = fopen (dictfile2, "rb")) == NULL)
      {
        log_error ("ERROR: %s: %s", dictfile2, strerror (errno));

        fclose (fp1);

        return (-1);
      }

      if (stat (dictfile2, &tmp_stat) == -1)
      {
        log_error ("ERROR: %s: %s", dictfile2, strerror (errno));

        fclose (fp1);
        fclose (fp2);

        return (-1);
      }

      if (S_ISDIR (tmp_stat.st_mode))
      {
        log_error ("ERROR: %s must be a regular file", dictfile2, strerror (errno));

        fclose (fp1);
        fclose (fp2);

        return (-1);
      }

      data.combs_cnt = 1;

      data.quiet = 1;

      const uint64_t words1_cnt = count_words (wl_data, fp1, dictfile1, dictstat_base, &dictstat_nmemb);

      data.quiet = quiet;

      if (words1_cnt == 0)
      {
        log_error ("ERROR: %s: empty file", dictfile1);

        fclose (fp1);
        fclose (fp2);

        return (-1);
      }

      data.combs_cnt = 1;

      data.quiet = 1;

      const uint64_t words2_cnt = count_words (wl_data, fp2, dictfile2, dictstat_base, &dictstat_nmemb);

      data.quiet = quiet;

      if (words2_cnt == 0)
      {
        log_error ("ERROR: %s: empty file", dictfile2);

        fclose (fp1);
        fclose (fp2);

        return (-1);
      }

      fclose (fp1);
      fclose (fp2);

      data.dictfile  = dictfile1;
      data.dictfile2 = dictfile2;

      if (words1_cnt >= words2_cnt)
      {
        data.combs_cnt  = words2_cnt;
        data.combs_mode = COMBINATOR_MODE_BASE_LEFT;

        dictfiles = &data.dictfile;

        dictcnt = 1;
      }
      else
      {
        data.combs_cnt  = words1_cnt;
        data.combs_mode = COMBINATOR_MODE_BASE_RIGHT;

        dictfiles = &data.dictfile2;

        dictcnt = 1;

        // we also have to switch wordlist related rules!

        char *tmpc = data.rule_buf_l;

        data.rule_buf_l = data.rule_buf_r;
        data.rule_buf_r = tmpc;

        int   tmpi = data.rule_len_l;

        data.rule_len_l = data.rule_len_r;
        data.rule_len_r = tmpi;
      }
    }
    else if (attack_mode == ATTACK_MODE_BF)
    {
      char *mask = NULL;

      maskcnt = 0;

      if (benchmark == 0)
      {
        mask = myargv[optind + 1];

        masks = (char **) mymalloc (INCR_MASKS * sizeof (char *));

        if ((optind + 2) <= myargc)
        {
          struct stat file_stat;

          if (stat (mask, &file_stat) == -1)
          {
            maskcnt = 1;

            masks[maskcnt - 1] = mystrdup (mask);
          }
          else
          {
            int wls_left = myargc - (optind + 1);

            uint masks_avail = INCR_MASKS;

            for (int i = 0; i < wls_left; i++)
            {
              if (i != 0)
              {
                mask = myargv[optind + 1 + i];

                if (stat (mask, &file_stat) == -1)
                {
                  log_error ("ERROR: %s: %s", mask, strerror (errno));

                  return (-1);
                }
              }

              uint is_file = S_ISREG (file_stat.st_mode);

              if (is_file == 1)
              {
                FILE *mask_fp;

                if ((mask_fp = fopen (mask, "r")) == NULL)
                {
                  log_error ("ERROR: %s: %s", mask, strerror (errno));

                  return (-1);
                }

                char line_buf[BUFSIZ];

                while (!feof (mask_fp))
                {
                  memset (line_buf, 0, BUFSIZ);

                  int line_len = fgetl (mask_fp, line_buf);

                  if (line_len == 0) continue;

                  if (line_buf[0] == '#') continue;

                  if (masks_avail == maskcnt)
                  {
                    masks = (char **) myrealloc (masks, masks_avail * sizeof (char *), INCR_MASKS * sizeof (char *));

                    masks_avail += INCR_MASKS;
                  }

                  masks[maskcnt] = mystrdup (line_buf);

                  maskcnt++;
                }

                fclose (mask_fp);
              }
              else
              {
                log_error ("ERROR: %s: unsupported file-type", mask);

                return (-1);
              }
            }

            mask_from_file = 1;
          }
        }
        else
        {
          custom_charset_1 = (char *) "?l?d?u";
          custom_charset_2 = (char *) "?l?d";
          custom_charset_3 = (char *) "?l?d*!$@_";

          mp_setup_usr (mp_sys, mp_usr, custom_charset_1, 0);
          mp_setup_usr (mp_sys, mp_usr, custom_charset_2, 1);
          mp_setup_usr (mp_sys, mp_usr, custom_charset_3, 2);

          masks[maskcnt] = mystrdup ("?1?2?2?2?2?2?2?3?3?3?3?d?d?d?d");

          wordlist_mode = WL_MODE_MASK;

          data.wordlist_mode = wordlist_mode;

          increment = 1;

          maskcnt   = 1;
        }
      }
      else
      {
        /**
         * generate full masks and charsets
         */

        masks = (char **) mymalloc (sizeof (char *));

        switch (hash_mode)
        {
          case  1731: pw_min = 5;
                      pw_max = 5;
                      mask = mystrdup ("?b?b?b?b?b");
                      break;
          case 12500: pw_min = 5;
                      pw_max = 5;
                      mask = mystrdup ("?b?b?b?b?b");
                      break;
          default:    pw_min = 7;
                      pw_max = 7;
                      mask = mystrdup ("?b?b?b?b?b?b?b");
                      break;
        }

        maskcnt = 1;

        masks[maskcnt - 1] = mystrdup (mask);

        wordlist_mode = WL_MODE_MASK;

        data.wordlist_mode = wordlist_mode;

        increment = 1;
      }

      dictfiles = (char **) mycalloc (pw_max, sizeof (char *));

      if (increment)
      {
        if (increment_min > pw_min) pw_min = increment_min;

        if (increment_max < pw_max) pw_max = increment_max;
      }
    }
    else if (attack_mode == ATTACK_MODE_HYBRID1)
    {
      data.combs_mode = COMBINATOR_MODE_BASE_LEFT;

      // display

      char *mask = myargv[myargc - 1];

      maskcnt = 0;

      masks = (char **) mymalloc (1 * sizeof (char *));

      // mod

      struct stat file_stat;

      if (stat (mask, &file_stat) == -1)
      {
        maskcnt = 1;

        masks[maskcnt - 1] = mystrdup (mask);
      }
      else
      {
        uint is_file = S_ISREG (file_stat.st_mode);

        if (is_file == 1)
        {
          FILE *mask_fp;

          if ((mask_fp = fopen (mask, "r")) == NULL)
          {
            log_error ("ERROR: %s: %s", mask, strerror (errno));

            return (-1);
          }

          char line_buf[BUFSIZ];

          uint masks_avail = 1;

          while (!feof (mask_fp))
          {
            memset (line_buf, 0, BUFSIZ);

            int line_len = fgetl (mask_fp, line_buf);

            if (line_len == 0) continue;

            if (line_buf[0] == '#') continue;

            if (masks_avail == maskcnt)
            {
              masks = (char **) myrealloc (masks, masks_avail * sizeof (char *), INCR_MASKS * sizeof (char *));

              masks_avail += INCR_MASKS;
            }

            masks[maskcnt] = mystrdup (line_buf);

            maskcnt++;
          }

          fclose (mask_fp);

          mask_from_file = 1;
        }
        else
        {
          maskcnt = 1;

          masks[maskcnt - 1] = mystrdup (mask);
        }
      }

      // base

      int wls_left = myargc - (optind + 2);

      for (int i = 0; i < wls_left; i++)
      {
        char *filename = myargv[optind + 1 + i];

        struct stat file_stat;

        if (stat (filename, &file_stat) == -1)
        {
          log_error ("ERROR: %s: %s", filename, strerror (errno));

          return (-1);
        }

        uint is_dir = S_ISDIR (file_stat.st_mode);

        if (is_dir == 0)
        {
          dictfiles = (char **) myrealloc (dictfiles, dictcnt * sizeof (char *), sizeof (char *));

          dictcnt++;

          dictfiles[dictcnt - 1] = filename;
        }
        else
        {
          // do not allow --keyspace w/ a directory

          if (keyspace == 1)
          {
            log_error ("ERROR: keyspace parameter is not allowed together with a directory");

            return (-1);
          }

          char **dictionary_files = NULL;

          dictionary_files = scan_directory (filename);

          if (dictionary_files != NULL)
          {
            qsort (dictionary_files, count_dictionaries (dictionary_files), sizeof (char *), sort_by_stringptr);

            for (int d = 0; dictionary_files[d] != NULL; d++)
            {
              char *l1_filename = dictionary_files[d];

              struct stat l1_stat;

              if (stat (l1_filename, &l1_stat) == -1)
              {
                log_error ("ERROR: %s: %s", l1_filename, strerror (errno));

                return (-1);
              }

              if (S_ISREG (l1_stat.st_mode))
              {
                dictfiles = (char **) myrealloc (dictfiles, dictcnt * sizeof (char *), sizeof (char *));

                dictcnt++;

                dictfiles[dictcnt - 1] = strdup (l1_filename);
              }
            }
          }

          local_free (dictionary_files);
        }
      }

      if (dictcnt < 1)
      {
        log_error ("ERROR: No usable dictionary file found.");

        return (-1);
      }

      if (increment)
      {
        maskcnt = 0;

        uint mask_min = increment_min; // we can't reject smaller masks here
        uint mask_max = (increment_max < pw_max) ? increment_max : pw_max;

        for (uint mask_cur = mask_min; mask_cur <= mask_max; mask_cur++)
        {
          char *cur_mask = mp_get_truncated_mask (mask, strlen (mask), mask_cur);

          if (cur_mask == NULL) break;

          masks[maskcnt] = cur_mask;

          maskcnt++;

          masks = (char **) myrealloc (masks, maskcnt * sizeof (char *), sizeof (char *));
        }
      }
    }
    else if (attack_mode == ATTACK_MODE_HYBRID2)
    {
      data.combs_mode = COMBINATOR_MODE_BASE_RIGHT;

      // display

      char *mask = myargv[optind + 1 + 0];

      maskcnt = 0;

      masks = (char **) mymalloc (1 * sizeof (char *));

      // mod

      struct stat file_stat;

      if (stat (mask, &file_stat) == -1)
      {
        maskcnt = 1;

        masks[maskcnt - 1] = mystrdup (mask);
      }
      else
      {
        uint is_file = S_ISREG (file_stat.st_mode);

        if (is_file == 1)
        {
          FILE *mask_fp;

          if ((mask_fp = fopen (mask, "r")) == NULL)
          {
            log_error ("ERROR: %s: %s", mask, strerror (errno));

            return (-1);
          }

          char line_buf[BUFSIZ];

          uint masks_avail = 1;

          while (!feof (mask_fp))
          {
            memset (line_buf, 0, BUFSIZ);

            int line_len = fgetl (mask_fp, line_buf);

            if (line_len == 0) continue;

            if (line_buf[0] == '#') continue;

            if (masks_avail == maskcnt)
            {
              masks = (char **) myrealloc (masks, masks_avail * sizeof (char *), INCR_MASKS * sizeof (char *));

              masks_avail += INCR_MASKS;
            }

            masks[maskcnt] = mystrdup (line_buf);

            maskcnt++;
          }

          fclose (mask_fp);

          mask_from_file = 1;
        }
        else
        {
          maskcnt = 1;

          masks[maskcnt - 1] = mystrdup (mask);
        }
      }

      // base

      int wls_left = myargc - (optind + 2);

      for (int i = 0; i < wls_left; i++)
      {
        char *filename = myargv[optind + 2 + i];

        struct stat file_stat;

        if (stat (filename, &file_stat) == -1)
        {
          log_error ("ERROR: %s: %s", filename, strerror (errno));

          return (-1);
        }

        uint is_dir = S_ISDIR (file_stat.st_mode);

        if (is_dir == 0)
        {
          dictfiles = (char **) myrealloc (dictfiles, dictcnt * sizeof (char *), sizeof (char *));

          dictcnt++;

          dictfiles[dictcnt - 1] = filename;
        }
        else
        {
          // do not allow --keyspace w/ a directory

          if (keyspace == 1)
          {
            log_error ("ERROR: keyspace parameter is not allowed together with a directory");

            return (-1);
          }

          char **dictionary_files = NULL;

          dictionary_files = scan_directory (filename);

          if (dictionary_files != NULL)
          {
            qsort (dictionary_files, count_dictionaries (dictionary_files), sizeof (char *), sort_by_stringptr);

            for (int d = 0; dictionary_files[d] != NULL; d++)
            {
              char *l1_filename = dictionary_files[d];

              struct stat l1_stat;

              if (stat (l1_filename, &l1_stat) == -1)
              {
                log_error ("ERROR: %s: %s", l1_filename, strerror (errno));

                return (-1);
              }

              if (S_ISREG (l1_stat.st_mode))
              {
                dictfiles = (char **) myrealloc (dictfiles, dictcnt * sizeof (char *), sizeof (char *));

                dictcnt++;

                dictfiles[dictcnt - 1] = strdup (l1_filename);
              }
            }
          }

          local_free (dictionary_files);
        }
      }

      if (dictcnt < 1)
      {
        log_error ("ERROR: No usable dictionary file found.");

        return (-1);
      }

      if (increment)
      {
        maskcnt = 0;

        uint mask_min = increment_min; // we can't reject smaller masks here
        uint mask_max = (increment_max < pw_max) ? increment_max : pw_max;

        for (uint mask_cur = mask_min; mask_cur <= mask_max; mask_cur++)
        {
          char *cur_mask = mp_get_truncated_mask (mask, strlen (mask), mask_cur);

          if (cur_mask == NULL) break;

          masks[maskcnt] = cur_mask;

          maskcnt++;

          masks = (char **) myrealloc (masks, maskcnt * sizeof (char *), sizeof (char *));
        }
      }
    }

    data.pw_min = pw_min;
    data.pw_max = pw_max;

    /**
     * weak hash check
     */

    if (weak_hash_threshold >= salts_cnt)
    {
      if (data.quiet == 0) log_info_nn ("Checking for weak hashes...");

      for (uint salt_pos = 0; salt_pos < salts_cnt; salt_pos++)
      {
        weak_hash_check (&data.devices_param[0], salt_pos, gpu_loops);
      }
    }

    // Display hack, guarantee that there is at least one \r before real start

    if (data.quiet == 0) log_info_nn ("");

    /**
     * status and monitor threads
     */

    if (data.devices_status != STATUS_CRACKED) data.devices_status = STATUS_STARTING;

    hc_thread_t i_thread = 0;

    if ((data.wordlist_mode == WL_MODE_FILE) || (data.wordlist_mode == WL_MODE_MASK))
    {
      hc_thread_create (i_thread, thread_keypress, &benchmark);
    }

    if (wordlist_mode == WL_MODE_STDIN) data.status = 1;

    uint ni_threads_cnt = 0;

    hc_thread_t *ni_threads = (hc_thread_t *) mycalloc (10, sizeof (hc_thread_t));

    hc_thread_create (ni_threads[ni_threads_cnt], thread_monitor, NULL);

    ni_threads_cnt++;

    /**
      * Outfile remove
      */

    if (keyspace == 0)
    {
      if (outfile_check_timer != 0)
      {
        if (data.outfile_check_directory != NULL)
        {
          if ((hash_mode != 5200) &&
              !((hash_mode >= 6200) && (hash_mode <= 6299)) &&
              (hash_mode != 9000))
          {
            hc_thread_create (ni_threads[ni_threads_cnt], thread_outfile_remove, NULL);

            ni_threads_cnt++;
          }
          else
          {
            outfile_check_timer = 0;
          }
        }
        else
        {
          outfile_check_timer = 0;
        }
      }
    }

    /**
     * Inform the user if we got some hashes remove because of the pot file remove feature
     */

    if (data.quiet == 0)
    {
      if (potfile_remove_cracks > 0)
      {
        if (potfile_remove_cracks == 1) log_info ("INFO: removed 1 hash found in pot file\n");
        else                            log_info ("INFO: removed %u hashes found in pot file\n", potfile_remove_cracks);
      }
    }

    data.outfile_check_timer = outfile_check_timer;

    /**
     * main loop
     */

    char **induction_dictionaries = NULL;

    int induction_dictionaries_cnt = 0;

    hcstat_table_t *root_table_buf   = NULL;
    hcstat_table_t *markov_table_buf = NULL;

    uint initial_restore_done = 0;

    data.maskcnt = maskcnt;

    for (uint maskpos = rd->maskpos; maskpos < maskcnt; maskpos++)
    {
      if (data.devices_status == STATUS_CRACKED) break;

      data.devices_status = STATUS_INIT;

      if (maskpos > rd->maskpos)
      {
        rd->dictpos = 0;
      }

      rd->maskpos  = maskpos;
      data.maskpos = maskpos;

      if (attack_mode == ATTACK_MODE_HYBRID1 || attack_mode == ATTACK_MODE_HYBRID2 || attack_mode == ATTACK_MODE_BF)
      {
        char *mask = masks[maskpos];

        if (mask_from_file == 1)
        {
          if (mask[0] == '\\' && mask[1] == '#') mask++; // escaped comment sign (sharp) "\#"

          char *str_ptr;
          uint  str_pos;

          uint mask_offset = 0;

          uint separator_cnt;

          for (separator_cnt = 0; separator_cnt < 4; separator_cnt++)
          {
            str_ptr = strstr (mask + mask_offset, ",");

            if (str_ptr == NULL) break;

            str_pos = str_ptr - mask;

            // escaped separator, i.e. "\,"

            if (str_pos > 0)
            {
              if (mask[str_pos - 1] == '\\')
              {
                separator_cnt --;

                mask_offset = str_pos + 1;

                continue;
              }
            }

            // reset the offset

            mask_offset = 0;

            mask[str_pos] = '\0';

            switch (separator_cnt)
            {
              case 0:
                mp_reset_usr (mp_usr, 0);

                custom_charset_1 = mask;
                mp_setup_usr (mp_sys, mp_usr, custom_charset_1, 0);
                break;

              case 1:
                mp_reset_usr (mp_usr, 1);

                custom_charset_2 = mask;
                mp_setup_usr (mp_sys, mp_usr, custom_charset_2, 1);
                break;

              case 2:
                mp_reset_usr (mp_usr, 2);

                custom_charset_3 = mask;
                mp_setup_usr (mp_sys, mp_usr, custom_charset_3, 2);
                break;

              case 3:
                mp_reset_usr (mp_usr, 3);

                custom_charset_4 = mask;
                mp_setup_usr (mp_sys, mp_usr, custom_charset_4, 3);
                break;
            }

            mask = mask + str_pos + 1;
          }
        }

        if ((attack_mode == ATTACK_MODE_HYBRID1) || (attack_mode == ATTACK_MODE_HYBRID2))
        {
          if (maskpos > 0)
          {
            local_free (css_buf);
            local_free (data.root_css_buf);
            local_free (data.markov_css_buf);

            local_free (masks[maskpos - 1]);
          }

          css_buf = mp_gen_css (mask, strlen (mask), mp_sys, mp_usr, &css_cnt);

          data.mask = mask;
          data.css_cnt = css_cnt;
          data.css_buf = css_buf;

          uint uniq_tbls[SP_PW_MAX][CHARSIZ];

          memset (uniq_tbls, 0, sizeof (uniq_tbls));

          mp_css_to_uniq_tbl (css_cnt, css_buf, uniq_tbls);

          if (root_table_buf   == NULL) root_table_buf   = (hcstat_table_t *) mycalloc (SP_ROOT_CNT,   sizeof (hcstat_table_t));
          if (markov_table_buf == NULL) markov_table_buf = (hcstat_table_t *) mycalloc (SP_MARKOV_CNT, sizeof (hcstat_table_t));

          sp_setup_tbl (install_dir, markov_hcstat, markov_disable, markov_classic, root_table_buf, markov_table_buf);

          markov_threshold = (markov_threshold != 0) ? markov_threshold : CHARSIZ;

          cs_t *root_css_buf   = (cs_t *) mycalloc (SP_PW_MAX,           sizeof (cs_t));
          cs_t *markov_css_buf = (cs_t *) mycalloc (SP_PW_MAX * CHARSIZ, sizeof (cs_t));

          data.root_css_buf   = root_css_buf;
          data.markov_css_buf = markov_css_buf;

          sp_tbl_to_css (root_table_buf, markov_table_buf, root_css_buf, markov_css_buf, markov_threshold, uniq_tbls);

          data.combs_cnt = sp_get_sum (0, css_cnt, root_css_buf);

          local_free (root_table_buf);
          local_free (markov_table_buf);

          // args

          for (uint device_id = 0; device_id < devices_cnt; device_id++)
          {
            hc_device_param_t *device_param = &data.devices_param[device_id];

            device_param->kernel_params_mp[0] = &device_param->d_combs;
            device_param->kernel_params_mp[1] = &device_param->d_root_css_buf;
            device_param->kernel_params_mp[2] = &device_param->d_markov_css_buf;

            device_param->kernel_params_mp_buf64[3] = 0;
            device_param->kernel_params_mp_buf32[4] = css_cnt;
            device_param->kernel_params_mp_buf32[5] = 0;
            device_param->kernel_params_mp_buf32[6] = 0;
            device_param->kernel_params_mp_buf32[7] = 0;

            if (attack_mode == ATTACK_MODE_HYBRID1)
            {
              if (opts_type & OPTS_TYPE_PT_ADD01)     device_param->kernel_params_mp_buf32[5] = full01;
              if (opts_type & OPTS_TYPE_PT_ADD80)     device_param->kernel_params_mp_buf32[5] = full80;
              if (opts_type & OPTS_TYPE_PT_ADDBITS14) device_param->kernel_params_mp_buf32[6] = 1;
              if (opts_type & OPTS_TYPE_PT_ADDBITS15) device_param->kernel_params_mp_buf32[7] = 1;
            }
            else if (attack_mode == ATTACK_MODE_HYBRID2)
            {
              device_param->kernel_params_mp_buf32[5] = 0;
              device_param->kernel_params_mp_buf32[6] = 0;
              device_param->kernel_params_mp_buf32[7] = 0;
            }

            #ifdef _CUDA
            hc_cuCtxPushCurrent (device_param->context);

            hc_cuMemcpyHtoD (device_param->d_root_css_buf,   root_css_buf,   device_param->size_root_css);
            hc_cuMemcpyHtoD (device_param->d_markov_css_buf, markov_css_buf, device_param->size_markov_css);

            hc_cuCtxPopCurrent (&device_param->context);
            #elif _OCL
            for (uint i = 0; i < 3; i++) hc_clSetKernelArg (device_param->kernel_mp, i, sizeof (cl_mem),   (void *) device_param->kernel_params_mp[i]);
            for (uint i = 3; i < 4; i++) hc_clSetKernelArg (device_param->kernel_mp, i, sizeof (cl_ulong), (void *) device_param->kernel_params_mp[i]);
            for (uint i = 4; i < 8; i++) hc_clSetKernelArg (device_param->kernel_mp, i, sizeof (cl_uint),  (void *) device_param->kernel_params_mp[i]);

            hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_root_css_buf,   CL_TRUE, 0, device_param->size_root_css,   root_css_buf,   0, NULL, NULL);
            hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_markov_css_buf, CL_TRUE, 0, device_param->size_markov_css, markov_css_buf, 0, NULL, NULL);
            #endif
          }
        }
        else if (attack_mode == ATTACK_MODE_BF)
        {
          dictcnt = 0;  // number of "sub-masks", i.e. when using incremental mode

          if (increment)
          {
            for (uint i = 0; i < dictcnt; i++)
            {
              local_free (dictfiles[i]);
            }

            for (uint pw_len = MAX (1, pw_min); pw_len <= pw_max; pw_len++)
            {
              char *l1_filename = mp_get_truncated_mask (mask, strlen (mask), pw_len);

              if (l1_filename == NULL) break;

              dictcnt++;

              dictfiles[dictcnt - 1] = l1_filename;
            }
          }
          else
          {
            dictcnt++;

            dictfiles[dictcnt - 1] = mask;
          }

          if (dictcnt == 0)
          {
            log_error ("ERROR: Mask is too small");

            return (-1);
          }
        }
      }

      free (induction_dictionaries);

      // induction_dictionaries_cnt = 0; // implied

      if (attack_mode != ATTACK_MODE_BF)
      {
        if (keyspace == 0)
        {
          induction_dictionaries = scan_directory (induction_directory);

          induction_dictionaries_cnt = count_dictionaries (induction_dictionaries);
        }
      }

      if (induction_dictionaries_cnt)
      {
        qsort (induction_dictionaries, induction_dictionaries_cnt, sizeof (char *), sort_by_mtime);
      }

      /**
       * prevent the user from using --keyspace together w/ maskfile and or dictfile
       */
      if (keyspace == 1)
      {
        if ((maskcnt > 1) || (dictcnt > 1))
        {
          log_error ("ERROR: --keyspace is not supported with --increment or mask files");

          return (-1);
        }
      }

      for (uint dictpos = rd->dictpos; dictpos < dictcnt; )
      {
        char *subid = logfile_generate_subid ();

        data.subid = subid;

        logfile_sub_msg ("START");

        data.devices_status = STATUS_INIT;

        memset (data.words_progress_done,     0, data.salts_cnt * sizeof (uint64_t));
        memset (data.words_progress_rejected, 0, data.salts_cnt * sizeof (uint64_t));
        memset (data.words_progress_restored, 0, data.salts_cnt * sizeof (uint64_t));

        memset (data.cpt_buf, 0, CPT_BUF * sizeof (cpt_t));

        data.cpt_pos = 0;

        data.cpt_start = time (NULL);

        data.cpt_total = 0;

        if (data.restore == 0)
        {
          rd->words_cur = skip;

          skip = 0;

          data.skip = 0;
        }

        data.ms_paused = 0;

        data.words_cur = rd->words_cur;

        for (uint device_id = 0; device_id < data.devices_cnt; device_id++)
        {
          hc_device_param_t *device_param = &data.devices_param[device_id];

          device_param->speed_pos = 0;

          memset (device_param->speed_cnt, 0, SPEED_CACHE * sizeof (uint64_t));
          memset (device_param->speed_ms,  0, SPEED_CACHE * sizeof (float));
          memset (device_param->speed_rec, 0, SPEED_CACHE * sizeof (hc_timer_t));

          device_param->gpu_power  = device_param->gpu_power_user;
          device_param->gpu_blocks = device_param->gpu_blocks_user;

          device_param->outerloop_pos  = 0;
          device_param->outerloop_left = 0;
          device_param->innerloop_pos  = 0;
          device_param->innerloop_left = 0;

          // some more resets:

          memset (device_param->pw_caches, 0, 64 * sizeof (pw_cache_t));

          memset (device_param->pws_buf, 0, device_param->size_pws);

          device_param->pw_cnt  = 0;
          device_param->pws_cnt = 0;

          device_param->words_off  = 0;
          device_param->words_done = 0;
        }

        data.gpu_blocks_div = 0;

        // figure out some workload

        if (attack_mode == ATTACK_MODE_STRAIGHT)
        {
          if (data.wordlist_mode == WL_MODE_FILE)
          {
            char *dictfile = NULL;

            if (induction_dictionaries_cnt)
            {
              dictfile = induction_dictionaries[0];
            }
            else
            {
              dictfile = dictfiles[dictpos];
            }

            data.dictfile = dictfile;

            logfile_sub_string (dictfile);

            for (uint i = 0; i < rp_files_cnt; i++)
            {
              logfile_sub_var_string ("rulefile", rp_files[i]);
            }

            FILE *fd2 = fopen (dictfile, "rb");

            if (fd2 == NULL)
            {
              log_error ("ERROR: %s: %s", dictfile, strerror (errno));

              return (-1);
            }

            data.words_cnt = count_words (wl_data, fd2, dictfile, dictstat_base, &dictstat_nmemb);

            fclose (fd2);

            if (data.words_cnt == 0)
            {
              if (data.devices_status == STATUS_CRACKED) break;
              if (data.devices_status == STATUS_ABORTED) break;

              dictpos++;

              continue;
            }
          }
        }
        else if (attack_mode == ATTACK_MODE_COMBI)
        {
          char *dictfile  = data.dictfile;
          char *dictfile2 = data.dictfile2;

          logfile_sub_string (dictfile);
          logfile_sub_string (dictfile2);

          if (data.combs_mode == COMBINATOR_MODE_BASE_LEFT)
          {
            FILE *fd2 = fopen (dictfile, "rb");

            if (fd2 == NULL)
            {
              log_error ("ERROR: %s: %s", dictfile, strerror (errno));

              return (-1);
            }

            data.words_cnt = count_words (wl_data, fd2, dictfile, dictstat_base, &dictstat_nmemb);

            fclose (fd2);
          }
          else if (data.combs_mode == COMBINATOR_MODE_BASE_RIGHT)
          {
            FILE *fd2 = fopen (dictfile2, "rb");

            if (fd2 == NULL)
            {
              log_error ("ERROR: %s: %s", dictfile2, strerror (errno));

              return (-1);
            }

            data.words_cnt = count_words (wl_data, fd2, dictfile2, dictstat_base, &dictstat_nmemb);

            fclose (fd2);
          }

          if (data.words_cnt == 0)
          {
            if (data.devices_status == STATUS_CRACKED) break;
            if (data.devices_status == STATUS_ABORTED) break;

            dictpos++;

            continue;
          }
        }
        else if ((attack_mode == ATTACK_MODE_HYBRID1) || (attack_mode == ATTACK_MODE_HYBRID2))
        {
          char *dictfile = NULL;

          if (induction_dictionaries_cnt)
          {
            dictfile = induction_dictionaries[0];
          }
          else
          {
            dictfile = dictfiles[dictpos];
          }

          data.dictfile = dictfile;

          char *mask = data.mask;

          logfile_sub_string (dictfile);
          logfile_sub_string (mask);

          FILE *fd2 = fopen (dictfile, "rb");

          if (fd2 == NULL)
          {
            log_error ("ERROR: %s: %s", dictfile, strerror (errno));

            return (-1);
          }

          data.words_cnt = count_words (wl_data, fd2, dictfile, dictstat_base, &dictstat_nmemb);

          fclose (fd2);

          if (data.words_cnt == 0)
          {
            if (data.devices_status == STATUS_CRACKED) break;
            if (data.devices_status == STATUS_ABORTED) break;

            dictpos++;

            continue;
          }
        }
        else if (attack_mode == ATTACK_MODE_BF)
        {
          local_free (css_buf);
          local_free (data.root_css_buf);
          local_free (data.markov_css_buf);

          char *mask = dictfiles[dictpos];

          logfile_sub_string (mask);

          // base

          css_buf = mp_gen_css (mask, strlen (mask), mp_sys, mp_usr, &css_cnt);

          if (opts_type & OPTS_TYPE_PT_UNICODE)
          {
            uint css_cnt_unicode = css_cnt * 2;

            cs_t *css_buf_unicode = (cs_t *) mycalloc (css_cnt_unicode, sizeof (cs_t));

            for (uint i = 0, j = 0; i < css_cnt; i += 1, j += 2)
            {
              memcpy (&css_buf_unicode[j + 0], &css_buf[i], sizeof (cs_t));

              css_buf_unicode[j + 1].cs_buf[0] = 0;
              css_buf_unicode[j + 1].cs_len    = 1;
            }

            free (css_buf);

            css_buf = css_buf_unicode;
            css_cnt = css_cnt_unicode;
          }

          // check if mask is not too large or too small for pw_min/pw_max  (*2 if unicode)

          uint mask_min = pw_min;
          uint mask_max = pw_max;

          if (opts_type & OPTS_TYPE_PT_UNICODE)
          {
            mask_min *= 2;
            mask_max *= 2;
          }

          if ((css_cnt < mask_min) || (css_cnt > mask_max))
          {
            if (css_cnt < mask_min)
            {
              log_info ("WARNING: skipping mask '%s' because it is smaller than the minimum password length", mask);
            }

            if (css_cnt > mask_max)
            {
              log_info ("WARNING: skipping mask '%s' because it is larger than the maximum password length", mask);
            }

            // skip to next mask

            dictpos++;

            rd->dictpos = dictpos;

            logfile_sub_msg ("STOP");

            continue;
          }

          uint save_css_cnt = css_cnt;

          if (opti_type & OPTI_TYPE_SINGLE_HASH)
          {
            if (opti_type & OPTI_TYPE_APPENDED_SALT)
            {
              uint  salt_len = (uint)   data.salts_buf[0].salt_len;
              char *salt_buf = (char *) data.salts_buf[0].salt_buf;

              uint css_cnt_salt = css_cnt + salt_len;

              cs_t *css_buf_salt = (cs_t *) mycalloc (css_cnt_salt, sizeof (cs_t));

              memcpy (css_buf_salt, css_buf, css_cnt * sizeof (cs_t));

              for (uint i = 0, j = css_cnt; i < salt_len; i++, j++)
              {
                css_buf_salt[j].cs_buf[0] = salt_buf[i];
                css_buf_salt[j].cs_len    = 1;
              }

              free (css_buf);

              css_buf = css_buf_salt;
              css_cnt = css_cnt_salt;
            }
          }

          data.mask = mask;
          data.css_cnt = css_cnt;
          data.css_buf = css_buf;

          if (maskpos > 0 && dictpos == 0) free (masks[maskpos - 1]);

          uint uniq_tbls[SP_PW_MAX][CHARSIZ];

          memset (uniq_tbls, 0, sizeof (uniq_tbls));

          mp_css_to_uniq_tbl (css_cnt, css_buf, uniq_tbls);

          if (root_table_buf   == NULL) root_table_buf   = (hcstat_table_t *) mycalloc (SP_ROOT_CNT,   sizeof (hcstat_table_t));
          if (markov_table_buf == NULL) markov_table_buf = (hcstat_table_t *) mycalloc (SP_MARKOV_CNT, sizeof (hcstat_table_t));

          sp_setup_tbl (install_dir, markov_hcstat, markov_disable, markov_classic, root_table_buf, markov_table_buf);

          markov_threshold = (markov_threshold != 0) ? markov_threshold : CHARSIZ;

          cs_t *root_css_buf   = (cs_t *) mycalloc (SP_PW_MAX,           sizeof (cs_t));
          cs_t *markov_css_buf = (cs_t *) mycalloc (SP_PW_MAX * CHARSIZ, sizeof (cs_t));

          data.root_css_buf   = root_css_buf;
          data.markov_css_buf = markov_css_buf;

          sp_tbl_to_css (root_table_buf, markov_table_buf, root_css_buf, markov_css_buf, markov_threshold, uniq_tbls);

          data.words_cnt = sp_get_sum (0, css_cnt, root_css_buf);

          local_free (root_table_buf);
          local_free (markov_table_buf);

          // copy + args

          uint css_cnt_l = css_cnt;
          uint css_cnt_r;

          if (attack_exec == ATTACK_EXEC_ON_GPU)
          {
            if (save_css_cnt < 6)
            {
              css_cnt_r = 1;
            }
            else if (save_css_cnt == 6)
            {
              css_cnt_r = 2;
            }
            else
            {
              if (opts_type & OPTS_TYPE_PT_UNICODE)
              {
                if (save_css_cnt == 8 || save_css_cnt == 10)
                {
                  css_cnt_r = 2;
                }
                else
                {
                  css_cnt_r = 4;
                }
              }
              else
              {
                if ((css_buf[0].cs_len * css_buf[1].cs_len * css_buf[2].cs_len) > 256)
                {
                  css_cnt_r = 3;
                }
                else
                {
                  css_cnt_r = 4;
                }
              }
            }
          }
          else
          {
            css_cnt_r = 1;

            /* unfinished code?
            int sum = css_buf[css_cnt_r - 1].cs_len;

            for (uint i = 1; i < 4 && i < css_cnt; i++)
            {
              if (sum > 1) break; // we really don't need alot of amplifier them for slow hashes

              css_cnt_r++;

              sum *= css_buf[css_cnt_r - 1].cs_len;
            }
            */
          }

          css_cnt_l -= css_cnt_r;

          data.bfs_cnt = sp_get_sum (0, css_cnt_r, root_css_buf);

          for (uint device_id = 0; device_id < devices_cnt; device_id++)
          {
            hc_device_param_t *device_param = &data.devices_param[device_id];

            device_param->kernel_params_mp_l[0] = &device_param->d_pws_buf;
            device_param->kernel_params_mp_l[1] = &device_param->d_root_css_buf;
            device_param->kernel_params_mp_l[2] = &device_param->d_markov_css_buf;

            device_param->kernel_params_mp_l_buf64[3] = 0;
            device_param->kernel_params_mp_l_buf32[4] = css_cnt_l;
            device_param->kernel_params_mp_l_buf32[5] = css_cnt_r;
            device_param->kernel_params_mp_l_buf32[6] = 0;
            device_param->kernel_params_mp_l_buf32[7] = 0;
            device_param->kernel_params_mp_l_buf32[8] = 0;

            if (opts_type & OPTS_TYPE_PT_ADD01)     device_param->kernel_params_mp_l_buf32[6] = full01;
            if (opts_type & OPTS_TYPE_PT_ADD80)     device_param->kernel_params_mp_l_buf32[6] = full80;
            if (opts_type & OPTS_TYPE_PT_ADDBITS14) device_param->kernel_params_mp_l_buf32[7] = 1;
            if (opts_type & OPTS_TYPE_PT_ADDBITS15) device_param->kernel_params_mp_l_buf32[8] = 1;

            device_param->kernel_params_mp_r[0] = &device_param->d_bfs;
            device_param->kernel_params_mp_r[1] = &device_param->d_root_css_buf;
            device_param->kernel_params_mp_r[2] = &device_param->d_markov_css_buf;

            device_param->kernel_params_mp_r_buf64[3] = 0;
            device_param->kernel_params_mp_r_buf32[4] = css_cnt_r;
            device_param->kernel_params_mp_r_buf32[5] = 0;
            device_param->kernel_params_mp_r_buf32[6] = 0;
            device_param->kernel_params_mp_r_buf32[7] = 0;

            #ifdef _CUDA
            hc_cuCtxPushCurrent (device_param->context);

            hc_cuMemcpyHtoD (device_param->d_root_css_buf,   root_css_buf,   device_param->size_root_css);
            hc_cuMemcpyHtoD (device_param->d_markov_css_buf, markov_css_buf, device_param->size_markov_css);

            hc_cuCtxPopCurrent (&device_param->context);
            #elif _OCL
            for (uint i = 0; i < 3; i++) hc_clSetKernelArg (device_param->kernel_mp_l, i, sizeof (cl_mem),   (void *) device_param->kernel_params_mp_l[i]);
            for (uint i = 3; i < 4; i++) hc_clSetKernelArg (device_param->kernel_mp_l, i, sizeof (cl_ulong), (void *) device_param->kernel_params_mp_l[i]);
            for (uint i = 4; i < 9; i++) hc_clSetKernelArg (device_param->kernel_mp_l, i, sizeof (cl_uint),  (void *) device_param->kernel_params_mp_l[i]);

            for (uint i = 0; i < 3; i++) hc_clSetKernelArg (device_param->kernel_mp_r, i, sizeof (cl_mem),   (void *) device_param->kernel_params_mp_r[i]);
            for (uint i = 3; i < 4; i++) hc_clSetKernelArg (device_param->kernel_mp_r, i, sizeof (cl_ulong), (void *) device_param->kernel_params_mp_r[i]);
            for (uint i = 4; i < 8; i++) hc_clSetKernelArg (device_param->kernel_mp_r, i, sizeof (cl_uint),  (void *) device_param->kernel_params_mp_r[i]);

            hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_root_css_buf,   CL_TRUE, 0, device_param->size_root_css,   root_css_buf,   0, NULL, NULL);
            hc_clEnqueueWriteBuffer (device_param->command_queue, device_param->d_markov_css_buf, CL_TRUE, 0, device_param->size_markov_css, markov_css_buf, 0, NULL, NULL);
            #endif
          }
        }

        uint64_t words_base = data.words_cnt;

        if (data.attack_kern == ATTACK_KERN_STRAIGHT)
        {
          if (data.gpu_rules_cnt)
          {
            words_base /= data.gpu_rules_cnt;
          }
        }
        else if (data.attack_kern == ATTACK_KERN_COMBI)
        {
          if (data.combs_cnt)
          {
            words_base /= data.combs_cnt;
          }
        }
        else if (data.attack_kern == ATTACK_KERN_BF)
        {
          if (data.bfs_cnt)
          {
            words_base /= data.bfs_cnt;
          }
        }

        data.words_base = words_base;

        if (keyspace == 1)
        {
          log_info ("%llu", (unsigned long long int) words_base);

          return (0);
        }

        if (data.words_cur > data.words_base)
        {
          log_error ("ERROR: restore value greater keyspace");

          return (-1);
        }

        if (data.words_cur)
        {
          if (data.attack_kern == ATTACK_KERN_STRAIGHT)
          {
            for (uint i = 0; i < data.salts_cnt; i++)
            {
              data.words_progress_restored[i] = data.words_cur * data.gpu_rules_cnt;
            }
          }
          else if (data.attack_kern == ATTACK_KERN_COMBI)
          {
            for (uint i = 0; i < data.salts_cnt; i++)
            {
              data.words_progress_restored[i] = data.words_cur * data.combs_cnt;
            }
          }
          else if (data.attack_kern == ATTACK_KERN_BF)
          {
            for (uint i = 0; i < data.salts_cnt; i++)
            {
              data.words_progress_restored[i] = data.words_cur * data.bfs_cnt;
            }
          }
        }

        /*
         * Inform user about possible slow speeds
         */

        if ((wordlist_mode == WL_MODE_FILE) || (wordlist_mode == WL_MODE_MASK))
        {
          if (data.words_base < gpu_blocks_all)
          {
            if (quiet == 0)
            {
              log_info ("");
              log_info ("ATTENTION!");
              log_info ("  The wordlist or mask you are using is too small.");
              log_info ("  Therefore, oclHashcat is unable to utilize the full parallelization power of your GPU(s).");
              log_info ("  The cracking speed will drop.");
              log_info ("  Workaround: https://hashcat.net/wiki/doku.php?id=frequently_asked_questions#how_to_create_more_work_for_full_speed");
              log_info ("");
            }
          }
        }

        /*
         * Update loopback file
         */

        if (loopback == 1)
        {
          time_t now;

          time (&now);

          uint random_num = get_random_num (0, 9999);

          snprintf (loopback_file, loopback_size - 1, "%s/%s.%d_%i", induction_directory, LOOPBACK_FILE, (int) now, random_num);

          data.loopback_file = loopback_file;
        }

        /*
         * Update dictionary statistic
         */

        if (keyspace == 0)
        {
          dictstat_fp = fopen (dictstat, "wb");

          if (dictstat_fp)
          {
            fwrite (dictstat_base, sizeof (dictstat_t), dictstat_nmemb, dictstat_fp);

            fclose (dictstat_fp);
          }
        }

        data.devices_status = STATUS_RUNNING;

        if (initial_restore_done == 0)
        {
          if (data.restore_disable == 0) cycle_restore ();

          initial_restore_done = 1;
        }

        hc_timer_set (&data.timer_running);

        if ((wordlist_mode == WL_MODE_FILE) || (wordlist_mode == WL_MODE_MASK))
        {
          if ((quiet == 0) && (status == 0) && (benchmark == 0))
          {
            if (quiet == 0) fprintf (stdout, "%s", PROMPT);
            if (quiet == 0) fflush (stdout);
          }
        }
        else if (wordlist_mode == WL_MODE_STDIN)
        {
          if (data.quiet == 0) log_info ("Starting attack in stdin mode...");
          if (data.quiet == 0) log_info ("");
        }

        time_t runtime_start;

        time (&runtime_start);

        data.runtime_start = runtime_start;

        /**
         * create cracker threads
         */

        hc_thread_t *c_threads = (hc_thread_t *) mycalloc (devices_cnt, sizeof (hc_thread_t));

        for (uint device_id = 0; device_id < devices_cnt; device_id++)
        {
          hc_device_param_t *device_param = &devices_param[device_id];

          device_param->device_id = device_id;

          if (wordlist_mode == WL_MODE_STDIN)
          {
            hc_thread_create (c_threads[device_id], thread_calc_stdin, device_param);
          }
          else
          {
            hc_thread_create (c_threads[device_id], thread_calc, device_param);
          }
        }

        // wait for crack threads to exit

        hc_thread_wait (devices_cnt, c_threads);

        local_free (c_threads);

        data.restore = 0;

        // finalize task

        logfile_sub_var_uint ("status-after-work", data.devices_status);

        if (data.devices_status == STATUS_CRACKED) break;
        if (data.devices_status == STATUS_ABORTED) break;

        if (data.devices_status == STATUS_BYPASS)
        {
          data.devices_status = STATUS_RUNNING;
        }

        if (induction_dictionaries_cnt)
        {
          unlink (induction_dictionaries[0]);
        }

        free (induction_dictionaries);

        if (attack_mode != ATTACK_MODE_BF)
        {
          induction_dictionaries = scan_directory (induction_directory);

          induction_dictionaries_cnt = count_dictionaries (induction_dictionaries);
        }

        if (benchmark == 0)
        {
          if (((dictpos + 1) < dictcnt) || ((maskpos + 1) < maskcnt) || induction_dictionaries_cnt)
          {
            if (quiet == 0) clear_prompt ();

            if (quiet == 0) log_info ("");

            if (status == 1)
            {
              status_display ();
            }
            else
            {
              if (quiet == 0) status_display ();
            }

            if (quiet == 0) log_info ("");
          }
        }

        if (attack_mode == ATTACK_MODE_BF)
        {
          dictpos++;

          rd->dictpos = dictpos;
        }
        else
        {
          if (induction_dictionaries_cnt)
          {
            qsort (induction_dictionaries, induction_dictionaries_cnt, sizeof (char *), sort_by_mtime);
          }
          else
          {
            dictpos++;

            rd->dictpos = dictpos;
          }
        }

        time_t runtime_stop;

        time (&runtime_stop);

        data.runtime_stop = runtime_stop;

        logfile_sub_uint (runtime_start);
        logfile_sub_uint (runtime_stop);

        logfile_sub_msg ("STOP");

        global_free (subid);
      }

      if (data.devices_status == STATUS_CRACKED) break;
      if (data.devices_status == STATUS_ABORTED) break;
      if (data.devices_status == STATUS_QUIT)    break;

      if (data.devices_status == STATUS_BYPASS)
      {
        data.devices_status = STATUS_RUNNING;
      }
    }

    // problems could occur if already at startup everything was cracked (because of .pot file reading etc), we must set some variables here to avoid NULL pointers

    if (attack_mode == ATTACK_MODE_STRAIGHT)
    {
      if (data.wordlist_mode == WL_MODE_FILE)
      {
        if (data.dictfile == NULL)
        {
          if (dictfiles != NULL)
          {
            data.dictfile = dictfiles[0];

            hc_timer_set (&data.timer_running);
          }
        }
      }
    }
    // NOTE: combi is okay because it is already set beforehand
    else if (attack_mode == ATTACK_MODE_HYBRID1 || attack_mode == ATTACK_MODE_HYBRID2)
    {
      if (data.dictfile == NULL)
      {
        if (dictfiles != NULL)
        {
          hc_timer_set (&data.timer_running);

          data.dictfile = dictfiles[0];
        }
      }
    }
    else if (attack_mode == ATTACK_MODE_BF)
    {
      if (data.mask == NULL)
      {
        hc_timer_set (&data.timer_running);

        data.mask = masks[0];
      }
    }

    if ((data.devices_status != STATUS_CRACKED) && (data.devices_status != STATUS_ABORTED) && (data.devices_status != STATUS_QUIT))
    {
      data.devices_status = STATUS_EXHAUSTED;
    }

    // if cracked / aborted remove last induction dictionary

    for (int file_pos = 0; file_pos < induction_dictionaries_cnt; file_pos++)
    {
      struct stat induct_stat;

      if (stat (induction_dictionaries[file_pos], &induct_stat) == 0)
      {
        unlink (induction_dictionaries[file_pos]);
      }
    }

    // wait for non-interactive threads

    for (uint thread_idx = 0; thread_idx < ni_threads_cnt; thread_idx++)
    {
      hc_thread_wait (1, &ni_threads[thread_idx]);
    }

    local_free (ni_threads);

    // wait for interactive threads

    if ((data.wordlist_mode == WL_MODE_FILE) || (data.wordlist_mode == WL_MODE_MASK))
    {
      hc_thread_wait (1, &i_thread);
    }

    // we dont need restore file anymore
    if (data.restore_disable == 0)
    {
      if ((data.devices_status == STATUS_EXHAUSTED) || (data.devices_status == STATUS_CRACKED))
      {
        unlink (eff_restore_file);
        unlink (new_restore_file);
      }
      else
      {
        cycle_restore ();
      }
    }

    // finally save left hashes

    if ((hashlist_mode == HL_MODE_FILE) && (remove == 1) && (data.digests_saved != data.digests_done))
    {
      save_hash ();
    }

    /**
     * Clean up
     */

    if (benchmark == 1)
    {
      status_benchmark ();

      log_info ("");
    }
    else
    {
      if (quiet == 0) clear_prompt ();

      if (quiet == 0) log_info ("");

      if (status == 1)
      {
        status_display ();
      }
      else
      {
        if (quiet == 0) status_display ();
      }

      if (quiet == 0) log_info ("");
    }

    for (uint device_id = 0; device_id < devices_cnt; device_id++)
    {
      hc_device_param_t *device_param = &data.devices_param[device_id];

      local_free (device_param->result);

      local_free (device_param->pw_caches);

      local_free (device_param->combs_buf);

      local_free (device_param->hooks_buf);

      #ifdef _CUDA
      hc_cuCtxPushCurrent (device_param->context);

      if (device_param->pws_buf)            myfree                    (device_param->pws_buf);
      if (device_param->d_pws_buf)          hc_cuMemFree              (device_param->d_pws_buf);
      if (device_param->d_pws_amp_buf)      hc_cuMemFree              (device_param->d_pws_amp_buf);
      if (device_param->d_rules)            hc_cuMemFree              (device_param->d_rules);
      if (device_param->d_combs)            hc_cuMemFree              (device_param->d_combs);
      if (device_param->d_bfs)              hc_cuMemFree              (device_param->d_bfs);
      if (device_param->d_bitmap_s1_a)      hc_cuMemFree              (device_param->d_bitmap_s1_a);
      if (device_param->d_bitmap_s1_b)      hc_cuMemFree              (device_param->d_bitmap_s1_b);
      if (device_param->d_bitmap_s1_c)      hc_cuMemFree              (device_param->d_bitmap_s1_c);
      if (device_param->d_bitmap_s1_d)      hc_cuMemFree              (device_param->d_bitmap_s1_d);
      if (device_param->d_bitmap_s2_a)      hc_cuMemFree              (device_param->d_bitmap_s2_a);
      if (device_param->d_bitmap_s2_b)      hc_cuMemFree              (device_param->d_bitmap_s2_b);
      if (device_param->d_bitmap_s2_c)      hc_cuMemFree              (device_param->d_bitmap_s2_c);
      if (device_param->d_bitmap_s2_d)      hc_cuMemFree              (device_param->d_bitmap_s2_d);
      if (device_param->d_plain_bufs)       hc_cuMemFree              (device_param->d_plain_bufs);
      if (device_param->d_digests_buf)      hc_cuMemFree              (device_param->d_digests_buf);
      if (device_param->d_digests_shown)    hc_cuMemFree              (device_param->d_digests_shown);
      if (device_param->d_salt_bufs)        hc_cuMemFree              (device_param->d_salt_bufs);
      if (device_param->d_esalt_bufs)       hc_cuMemFree              (device_param->d_esalt_bufs);
      if (device_param->d_tmps)             hc_cuMemFree              (device_param->d_tmps);
      if (device_param->d_hooks)            hc_cuMemFree              (device_param->d_hooks);
      if (device_param->d_result)           hc_cuMemFree              (device_param->d_result);
      if (device_param->d_scryptV_buf)      hc_cuMemFree              (device_param->d_scryptV_buf);
      if (device_param->d_root_css_buf)     hc_cuMemFree              (device_param->d_root_css_buf);
      if (device_param->d_markov_css_buf)   hc_cuMemFree              (device_param->d_markov_css_buf);

      if (device_param->stream)             hc_cuStreamDestroy        (device_param->stream);
      if (device_param->module)             hc_cuModuleUnload         (device_param->module);

      hc_cuCtxPopCurrent (&device_param->context);

      if (device_param->context)            hc_cuCtxDestroy           (device_param->context);

      #elif _OCL
      local_free (device_param->device_name);

      local_free (device_param->device_version);

      local_free (device_param->driver_version);

      if (device_param->pws_buf)            myfree                    (device_param->pws_buf);
      if (device_param->d_pws_buf)          hc_clReleaseMemObject     (device_param->d_pws_buf);
      if (device_param->d_pws_amp_buf)      hc_clReleaseMemObject     (device_param->d_pws_amp_buf);
      if (device_param->d_rules)            hc_clReleaseMemObject     (device_param->d_rules);
      if (device_param->d_rules_c)          hc_clReleaseMemObject     (device_param->d_rules_c);
      if (device_param->d_combs)            hc_clReleaseMemObject     (device_param->d_combs);
      if (device_param->d_combs_c)          hc_clReleaseMemObject     (device_param->d_combs_c);
      if (device_param->d_bfs)              hc_clReleaseMemObject     (device_param->d_bfs);
      if (device_param->d_bfs_c)            hc_clReleaseMemObject     (device_param->d_bfs_c);
      if (device_param->d_bitmap_s1_a)      hc_clReleaseMemObject     (device_param->d_bitmap_s1_a);
      if (device_param->d_bitmap_s1_b)      hc_clReleaseMemObject     (device_param->d_bitmap_s1_b);
      if (device_param->d_bitmap_s1_c)      hc_clReleaseMemObject     (device_param->d_bitmap_s1_c);
      if (device_param->d_bitmap_s1_d)      hc_clReleaseMemObject     (device_param->d_bitmap_s1_d);
      if (device_param->d_bitmap_s2_a)      hc_clReleaseMemObject     (device_param->d_bitmap_s2_a);
      if (device_param->d_bitmap_s2_b)      hc_clReleaseMemObject     (device_param->d_bitmap_s2_b);
      if (device_param->d_bitmap_s2_c)      hc_clReleaseMemObject     (device_param->d_bitmap_s2_c);
      if (device_param->d_bitmap_s2_d)      hc_clReleaseMemObject     (device_param->d_bitmap_s2_d);
      if (device_param->d_plain_bufs)       hc_clReleaseMemObject     (device_param->d_plain_bufs);
      if (device_param->d_digests_buf)      hc_clReleaseMemObject     (device_param->d_digests_buf);
      if (device_param->d_digests_shown)    hc_clReleaseMemObject     (device_param->d_digests_shown);
      if (device_param->d_salt_bufs)        hc_clReleaseMemObject     (device_param->d_salt_bufs);
      if (device_param->d_esalt_bufs)       hc_clReleaseMemObject     (device_param->d_esalt_bufs);
      if (device_param->d_tmps)             hc_clReleaseMemObject     (device_param->d_tmps);
      if (device_param->d_hooks)            hc_clReleaseMemObject     (device_param->d_hooks);
      if (device_param->d_result)           hc_clReleaseMemObject     (device_param->d_result);
      if (device_param->d_scryptV_buf)      hc_clReleaseMemObject     (device_param->d_scryptV_buf);
      if (device_param->d_root_css_buf)     hc_clReleaseMemObject     (device_param->d_root_css_buf);
      if (device_param->d_markov_css_buf)   hc_clReleaseMemObject     (device_param->d_markov_css_buf);

      if (device_param->kernel1)            hc_clReleaseKernel        (device_param->kernel1);
      if (device_param->kernel12)           hc_clReleaseKernel        (device_param->kernel12);
      if (device_param->kernel2)            hc_clReleaseKernel        (device_param->kernel2);
      if (device_param->kernel23)           hc_clReleaseKernel        (device_param->kernel23);
      if (device_param->kernel3)            hc_clReleaseKernel        (device_param->kernel3);
      if (device_param->kernel_mp)          hc_clReleaseKernel        (device_param->kernel_mp);
      if (device_param->kernel_mp_l)        hc_clReleaseKernel        (device_param->kernel_mp_l);
      if (device_param->kernel_mp_r)        hc_clReleaseKernel        (device_param->kernel_mp_r);

      if (device_param->program)            hc_clReleaseProgram       (device_param->program);
      if (device_param->program_mp)         hc_clReleaseProgram       (device_param->program_mp);
      if (device_param->command_queue)      hc_clReleaseCommandQueue  (device_param->command_queue);
      if (device_param->context)            hc_clReleaseContext       (device_param->context);
      #endif
    }

    #ifdef _OCL
    #ifndef OSX

    // reset default fan speed

    if (gpu_temp_disable == 0)
    {
      if (gpu_temp_retain != 0)
      {
        hc_thread_mutex_lock (mux_adl);

        for (uint i = 0; i < data.devices_cnt; i++)
        {
          if (data.hm_device[i].fan_supported == 1)
          {
            int fanspeed = temp_retain_fanspeed_value[i];

            if (fanspeed == -1) continue;

            int rc = hm_set_fanspeed_with_device_id (i, fanspeed);

            if (rc == -1) log_info ("WARNING: Failed to restore default fan speed for gpu number: %i:", i);
          }
        }

        hc_thread_mutex_unlock (mux_adl);
      }
    }

    // reset power tuning

    if (powertune_enable == 1)
    {
      hc_thread_mutex_lock (mux_adl);

      for (uint i = 0; i < data.devices_cnt; i++)
      {
        if (data.hm_device[i].od_version == 6)
        {
          // check powertune capabilities first, if not available then skip device

          int powertune_supported = 0;

          if ((hc_ADL_Overdrive6_PowerControl_Caps (data.hm_dll, data.hm_device[i].adapter_index, &powertune_supported)) != ADL_OK)
          {
            log_error ("ERROR: Failed to get ADL PowerControl Capabilities");

            return (-1);
          }

          if (powertune_supported != 0)
          {
            // powercontrol settings

            if ((hc_ADL_Overdrive_PowerControl_Set (data.hm_dll, data.hm_device[i].adapter_index, od_power_control_status[i])) != ADL_OK)
            {
              log_info ("ERROR: Failed to restore the ADL PowerControl values");

              return (-1);
            }

            // clocks

            ADLOD6StateInfo *performance_state = (ADLOD6StateInfo*) mycalloc (1, sizeof (ADLOD6StateInfo) + sizeof (ADLOD6PerformanceLevel));

            performance_state->iNumberOfPerformanceLevels = 2;

            performance_state->aLevels[0].iEngineClock = od_clock_mem_status[i].state.aLevels[0].iEngineClock;
            performance_state->aLevels[1].iEngineClock = od_clock_mem_status[i].state.aLevels[1].iEngineClock;
            performance_state->aLevels[0].iMemoryClock = od_clock_mem_status[i].state.aLevels[0].iMemoryClock;
            performance_state->aLevels[1].iMemoryClock = od_clock_mem_status[i].state.aLevels[1].iMemoryClock;

            if ((hc_ADL_Overdrive_State_Set (data.hm_dll, data.hm_device[i].adapter_index, ADL_OD6_SETSTATE_PERFORMANCE, performance_state)) != ADL_OK)
            {
              log_info ("ERROR: Failed to restore ADL performance state");

              return (-1);
            }

            local_free (performance_state);
          }
        }
      }

      hc_thread_mutex_unlock (mux_adl);
    }

    #endif
    #endif

    if (gpu_temp_disable == 0)
    {
      #ifdef _CUDA
      #ifdef LINUX
      hc_NVML_nvmlShutdown ();
      #endif

      #ifdef WIN
      NvAPI_Unload ();
      #endif
      #endif

      #ifdef _OCL
      #ifndef OSX
      hc_ADL_Main_Control_Destroy (data.hm_dll);

      hm_close (data.hm_dll);
      #endif
      #endif
    }

    // free memory

    local_free (masks);

    local_free (dictstat_base);

    for (uint pot_pos = 0; pot_pos < pot_cnt; pot_pos++)
    {
      pot_t *pot_ptr = &pot[pot_pos];

      hash_t *hash = &pot_ptr->hash;

      local_free (hash->digest);

      if (isSalted)
      {
        local_free (hash->salt);
      }
    }

    local_free (pot);

    local_free (all_gpu_rules_cnt);
    local_free (all_gpu_rules_buf);

    local_free (wl_data->buf);
    local_free (wl_data);

    local_free (bitmap_s1_a);
    local_free (bitmap_s1_b);
    local_free (bitmap_s1_c);
    local_free (bitmap_s1_d);
    local_free (bitmap_s2_a);
    local_free (bitmap_s2_b);
    local_free (bitmap_s2_c);
    local_free (bitmap_s2_d);

    #ifdef _OCL
    #ifndef OSX
    local_free (temp_retain_fanspeed_value);
    local_free (od_clock_mem_status);
    local_free (od_power_control_status);
    #endif
    #endif

    global_free (devices_param);

    global_free (gpu_rules_buf);

    global_free (root_css_buf);
    global_free (markov_css_buf);

    global_free (digests_buf);
    global_free (digests_shown);
    global_free (digests_shown_tmp);

    global_free (salts_buf);
    global_free (salts_shown);

    global_free (esalts_buf);

    global_free (words_progress_done);
    global_free (words_progress_rejected);
    global_free (words_progress_restored);

    if (pot_fp) fclose (pot_fp);

    if (data.devices_status == STATUS_QUIT) break;
  }

  // destroy others mutex

  hc_thread_mutex_delete (mux_dispatcher);
  hc_thread_mutex_delete (mux_counter);
  hc_thread_mutex_delete (mux_display);
  hc_thread_mutex_delete (mux_adl);

  // free memory

  local_free (eff_restore_file);
  local_free (new_restore_file);

  local_free (rd);

  // loopback

  local_free (loopback_file);

  if (loopback == 1) unlink (loopback_file);

  // induction directory

  if (induction_dir == NULL)
  {
    if (attack_mode != ATTACK_MODE_BF)
    {
      if (rmdir (induction_directory) == -1)
      {
        if (errno == ENOENT)
        {
          // good, we can ignore
        }
        else if (errno == ENOTEMPTY)
        {
          // good, we can ignore
        }
        else
        {
          log_error ("ERROR: %s: %s", induction_directory, strerror (errno));

          return (-1);
        }
      }

      local_free (induction_directory);
    }
  }

  // outfile-check directory

  if (outfile_check_dir == NULL)
  {
    if (rmdir (outfile_check_directory) == -1)
    {
      if (errno == ENOENT)
      {
        // good, we can ignore
      }
      else if (errno == ENOTEMPTY)
      {
        // good, we can ignore
      }
      else
      {
        log_error ("ERROR: %s: %s", outfile_check_directory, strerror (errno));

        return (-1);
      }
    }

    local_free (outfile_check_directory);
  }

  time_t proc_stop;

  time (&proc_stop);

  logfile_top_uint (proc_start);
  logfile_top_uint (proc_stop);

  logfile_top_msg ("STOP");

  if (quiet == 0) log_info_nn ("Started: %s", ctime (&proc_start));
  if (quiet == 0) log_info_nn ("Stopped: %s", ctime (&proc_stop));

  if (data.devices_status == STATUS_ABORTED)   return 2;
  if (data.devices_status == STATUS_QUIT)      return 2;
  if (data.devices_status == STATUS_EXHAUSTED) return 1;
  if (data.devices_status == STATUS_CRACKED)   return 0;

  return -1;
}
