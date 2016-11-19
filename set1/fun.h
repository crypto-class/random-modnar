#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <sstream>
#include <vector>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/des.h"
#include "cryptopp/base64.h"
#include "cryptopp/md5.h"
#include "cryptopp/hex.h"
#include "cryptopp/files.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/osrng.h"
#include <cryptopp/dh2.h>
#include <cryptopp/dh.h>
#include <cryptopp\rsa.h>
using namespace CryptoPP;

using namespace std;

#pragma warning (disable:4996)

#define pb push_back
#define mp make_pair
#define LL long long
#define BYTELEN 8
const int INF = 0x3f3f3f3f;
const double SIG_STD_FRE_SUM = 0.065;
const double alphabetFre[30] = { 0.08167,0.01492,0.02782,0.04253,0.12705,0.02228,0.02015,0.06094,0.06996,0.00153,0.00772,0.04025,0.02406,0.06749,0.07507,0.01929,0.0009,0.05987,0.06327,0.09056,0.02758,0.00978,0.02360,0.0015,0.01974,0.00074 };



string char2Base(char cc, int base, int n);
string hex2str(string s);
string str2hex(string s);
string AXORB(string a, string b);
string AXORWord(string a, string word);
void kmp_pre(string x, int m, int nxt[]);
int kmp_count(string pa, int m, string s, int n);
int CountBitOne(char a);
int HammingDist(string a, string b);
double CountFrequencySTD(string a);
string Base64Enc(string strSrc);
string Base64Dec(string strSrc);
string ECB_AES_DEC(string cipher, string key);
string ECB_AES_ENC(string plain, string key);
string PKSC_7(string s, int blocksize);
string CBC_AES_DEC(string cipher, string key, byte* iv);
string CBC_AES_ENC(string plain, string key, byte* iv);
string CBC_AES_DEC(string cipher, byte aes_key[], byte* iv);
string CBC_AES_ENC(string plain, byte aes_key[], byte* iv);
vector<string> chunk(string s, int blocksize);
Integer powMod(Integer a, Integer n, Integer mod);
Integer Byte2Inter(byte b[], int n);
void Inter2Byte(Integer a, byte b[], int n);
string Inter2Str(Integer a);
Integer Str2Inter(string a, int base);
double Inte2Doub(Integer a);

