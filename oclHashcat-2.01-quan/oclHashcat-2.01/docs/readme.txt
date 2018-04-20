oclHashcat v2.00
================

NV users require ForceWare 346.59 or later
AMD users require Catalyst 14.9 or later

##
## Features
##

- World's fastest password cracker
- World's first and only GPGPU-based rule engine
- Free
- Open-Source
- Multi-GPU (up to 128 GPUs)
- Multi-Hash (up to 100 million hashes at once)
- Multi-OS (Linux & Windows native binaries)
- Multi-Platform (OpenCL & CUDA support)
- Multi-Algorithm (see below)
- Low resource utilization; you can still watch movies or play games while cracking
- Focuses on highly iterated modern hashes
- Focuses on dictionary-based attacks
- Supports distributed cracking
- Supports pause / resume while cracking
- Supports sessions
- Supports restore
- Supports reading words from file and stdin
- Supports hex-salt and hex-charset
- Built-in benchmarking system
- Integrated thermal watchdog
- 150+ Hash-types implemented with performance in mind
- ... and much more

##
## Attack-Modes
##

- Straight *
- Combination
- Brute-force
- Hybrid dict + mask
- Hybrid mask + dict

* = Supports rules

##
## Hash-Types
##

- MD4
- MD5
- Half MD5 (left, mid, right)
- SHA1
- SHA-256
- SHA-384
- SHA-512
- SHA-3 (Keccak)
- SipHash
- RipeMD160
- Whirlpool
- GOST R 34.11-94
- GOST R 34.11-2012 (Streebog) 256-bit
- GOST R 34.11-2012 (Streebog) 512-bit
- Double MD5
- Double SHA1
- md5($pass.$salt)
- md5($salt.$pass)
- md5(unicode($pass).$salt)
- md5($salt.unicode($pass))
- md5(sha1($pass))
- md5($salt.md5($pass))
- md5($salt.$pass.$salt)
- md5(strtoupper(md5($pass)))
- sha1($pass.$salt)
- sha1($salt.$pass)
- sha1(unicode($pass).$salt)
- sha1($salt.unicode($pass))
- sha1(md5($pass))
- sha1($salt.$pass.$salt)
- sha256($pass.$salt)
- sha256($salt.$pass)
- sha256(unicode($pass).$salt)
- sha256($salt.unicode($pass))
- sha512($pass.$salt)
- sha512($salt.$pass)
- sha512(unicode($pass).$salt)
- sha512($salt.unicode($pass))
- HMAC-MD5 (key = $pass)
- HMAC-MD5 (key = $salt)
- HMAC-SHA1 (key = $pass)
- HMAC-SHA1 (key = $salt)
- HMAC-SHA256 (key = $pass)
- HMAC-SHA256 (key = $salt)
- HMAC-SHA512 (key = $pass)
- HMAC-SHA512 (key = $salt)
- PBKDF2-HMAC-MD5
- PBKDF2-HMAC-SHA1
- PBKDF2-HMAC-SHA256
- PBKDF2-HMAC-SHA512
- MyBB
- phpBB3
- SMF
- vBulletin
- IPB
- Woltlab Burning Board
- osCommerce
- xt:Commerce
- PrestaShop
- Mediawiki B type
- Wordpress
- Drupal
- Joomla
- PHPS
- Django (SHA-1)
- Django (PBKDF2-SHA256)
- EPiServer
- ColdFusion 10+
- Apache MD5-APR
- MySQL
- PostgreSQL
- MSSQL
- Oracle H: Type (Oracle 7+)
- Oracle S: Type (Oracle 11+)
- Oracle T: Type (Oracle 12+)
- Sybase
- hMailServer
- DNSSEC (NSEC3)
- IKE-PSK
- IPMI2 RAKP
- iSCSI CHAP
- Cram MD5
- MySQL Challenge-Response Authentication (SHA1)
- PostgreSQL Challenge-Response Authentication (MD5)
- SIP Digest Authentication (MD5)
- WPA
- WPA2
- NetNTLMv1
- NetNTLMv1 + ESS
- NetNTLMv2
- Kerberos 5 AS-REQ Pre-Auth etype 23
- Netscape LDAP SHA/SSHA
- LM
- NTLM
- Domain Cached Credentials (DCC), MS Cache
- Domain Cached Credentials 2 (DCC2), MS Cache 2
- MS-AzureSync PBKDF2-HMAC-SHA256
- descrypt
- bsdicrypt
- md5crypt
- sha256crypt
- sha512crypt
- bcrypt
- scrypt
- OSX v10.4
- OSX v10.5
- OSX v10.6
- OSX v10.7
- OSX v10.8
- OSX v10.9
- OSX v10.10
- AIX {smd5}
- AIX {ssha1}
- AIX {ssha256}
- AIX {ssha512}
- Cisco-ASA
- Cisco-PIX
- Cisco-IOS
- Cisco $8$
- Cisco $9$
- Juniper IVE
- Juniper Netscreen/SSG (ScreenOS)
- Android PIN
- GRUB 2
- CRC32
- RACF
- Radmin2
- Redmine
- Citrix Netscaler
- SAP CODVN B (BCODE)
- SAP CODVN F/G (PASSCODE)
- SAP CODVN H (PWDSALTEDHASH) iSSHA-1
- PeopleSoft
- Skype
- 7-Zip
- RAR3-hp
- PDF 1.1 - 1.3 (Acrobat 2 - 4)
- PDF 1.4 - 1.6 (Acrobat 5 - 8)
- PDF 1.7 Level 3 (Acrobat 9)
- PDF 1.7 Level 8 (Acrobat 10 - 11)
- MS Office <= 2003 MD5
- MS Office <= 2003 SHA1
- MS Office 2007
- MS Office 2010
- MS Office 2013
- Lotus Notes/Domino 5
- Lotus Notes/Domino 6
- Lotus Notes/Domino 8
- Bitcoin/Litecoin wallet.dat
- Blockchain, My Wallet
- 1Password, agilekeychain
- 1Password, cloudkeychain
- Lastpass
- Password Safe v2
- Password Safe v3
- eCryptfs
- Android FDE <= 4.3
- TrueCrypt 5.0+

##
## Tested OS's
##

- All Windows and Linux versions should work on both 32 and 64 bit

##
## Tested GPU's
##

- All NVidia CUDA enabled cards >= sm_20
- All AMD OpenCL enabled cards >= HD 5000

To get started, run the example scripts or check out docs/user_manuals.txt
