#include <stddef.h>
#include <string.h>

#include "myMD4.h"


static const _u32 IV[4] = {
	_C32(0x67452301), _C32(0xEFCDAB89),
	_C32(0x98BADCFE), _C32(0x10325476)
};

#define MD4_ROUND_BODY(in, r)   do { \
		_u32 A, B, C, D; \
 \
		A = (r)[0]; \
		B = (r)[1]; \
		C = (r)[2]; \
		D = (r)[3]; \
 \
  A = LROT(_T32(A + F(B, C, D) + in( 0)), 3); \
  D = LROT(_T32(D + F(A, B, C) + in( 1)), 7); \
  C = LROT(_T32(C + F(D, A, B) + in( 2)), 11); \
  B = LROT(_T32(B + F(C, D, A) + in( 3)), 19); \
  A = LROT(_T32(A + F(B, C, D) + in( 4)), 3); \
  D = LROT(_T32(D + F(A, B, C) + in( 5)), 7); \
  C = LROT(_T32(C + F(D, A, B) + in( 6)), 11); \
  B = LROT(_T32(B + F(C, D, A) + in( 7)), 19); \
  A = LROT(_T32(A + F(B, C, D) + in( 8)), 3); \
  D = LROT(_T32(D + F(A, B, C) + in( 9)), 7); \
  C = LROT(_T32(C + F(D, A, B) + in(10)), 11); \
  B = LROT(_T32(B + F(C, D, A) + in(11)), 19); \
  A = LROT(_T32(A + F(B, C, D) + in(12)), 3); \
  D = LROT(_T32(D + F(A, B, C) + in(13)), 7); \
  C = LROT(_T32(C + F(D, A, B) + in(14)), 11); \
  B = LROT(_T32(B + F(C, D, A) + in(15)), 19); \
 \
  A = LROT(_T32(A + G(B, C, D) + in( 0) + _C32(0x5A827999)), 3); \
  D = LROT(_T32(D + G(A, B, C) + in( 4) + _C32(0x5A827999)), 5); \
  C = LROT(_T32(C + G(D, A, B) + in( 8) + _C32(0x5A827999)), 9); \
  B = LROT(_T32(B + G(C, D, A) + in(12) + _C32(0x5A827999)), 13); \
  A = LROT(_T32(A + G(B, C, D) + in( 1) + _C32(0x5A827999)), 3); \
  D = LROT(_T32(D + G(A, B, C) + in( 5) + _C32(0x5A827999)), 5); \
  C = LROT(_T32(C + G(D, A, B) + in( 9) + _C32(0x5A827999)), 9); \
  B = LROT(_T32(B + G(C, D, A) + in(13) + _C32(0x5A827999)), 13); \
  A = LROT(_T32(A + G(B, C, D) + in( 2) + _C32(0x5A827999)), 3); \
  D = LROT(_T32(D + G(A, B, C) + in( 6) + _C32(0x5A827999)), 5); \
  C = LROT(_T32(C + G(D, A, B) + in(10) + _C32(0x5A827999)), 9); \
  B = LROT(_T32(B + G(C, D, A) + in(14) + _C32(0x5A827999)), 13); \
  A = LROT(_T32(A + G(B, C, D) + in( 3) + _C32(0x5A827999)), 3); \
  D = LROT(_T32(D + G(A, B, C) + in( 7) + _C32(0x5A827999)), 5); \
  C = LROT(_T32(C + G(D, A, B) + in(11) + _C32(0x5A827999)), 9); \
  B = LROT(_T32(B + G(C, D, A) + in(15) + _C32(0x5A827999)), 13); \
 \
  A = LROT(_T32(A + H(B, C, D) + in( 0) + _C32(0x6ED9EBA1)), 3); \
  D = LROT(_T32(D + H(A, B, C) + in( 8) + _C32(0x6ED9EBA1)), 9); \
  C = LROT(_T32(C + H(D, A, B) + in( 4) + _C32(0x6ED9EBA1)), 11); \
  B = LROT(_T32(B + H(C, D, A) + in(12) + _C32(0x6ED9EBA1)), 15); \
  A = LROT(_T32(A + H(B, C, D) + in( 2) + _C32(0x6ED9EBA1)), 3); \
  D = LROT(_T32(D + H(A, B, C) + in(10) + _C32(0x6ED9EBA1)), 9); \
  C = LROT(_T32(C + H(D, A, B) + in( 6) + _C32(0x6ED9EBA1)), 11); \
  B = LROT(_T32(B + H(C, D, A) + in(14) + _C32(0x6ED9EBA1)), 15); \
  A = LROT(_T32(A + H(B, C, D) + in( 1) + _C32(0x6ED9EBA1)), 3); \
  D = LROT(_T32(D + H(A, B, C) + in( 9) + _C32(0x6ED9EBA1)), 9); \
  C = LROT(_T32(C + H(D, A, B) + in( 5) + _C32(0x6ED9EBA1)), 11); \
  B = LROT(_T32(B + H(C, D, A) + in(13) + _C32(0x6ED9EBA1)), 15); \
  A = LROT(_T32(A + H(B, C, D) + in( 3) + _C32(0x6ED9EBA1)), 3); \
  D = LROT(_T32(D + H(A, B, C) + in(11) + _C32(0x6ED9EBA1)), 9); \
  C = LROT(_T32(C + H(D, A, B) + in( 7) + _C32(0x6ED9EBA1)), 11); \
  B = LROT(_T32(B + H(C, D, A) + in(15) + _C32(0x6ED9EBA1)), 15); \
 \
		(r)[0] = _T32(r[0] + A); \
		(r)[1] = _T32(r[1] + B); \
		(r)[2] = _T32(r[2] + C); \
		(r)[3] = _T32(r[3] + D); \
	} while (0)

/*
* One round of MD4. The data must be aligned for 32-bit access.
*/
static void
md4_round(const unsigned char *data, _u32 r[4])
{
	/*
	* On machines with native little-endian representation, we do
	* not use local variables for the message blocks: we simply
	* reread from the input buffer. Speedup is about 3% on Athlon XP.
	*/
#if SPH_LITTLE_FAST
#define X(idx)    sph_dec32le_aligned(data + 4 * (idx))
#else
	_u32 X_var[16];
	int i;

	for (i = 0; i < 16; i++)
		X_var[i] = sph_dec32le_aligned(data + 4 * i);
#define X(idx)    X_var[idx]
#endif

	MD4_ROUND_BODY(X, r);

#undef X
}

/* see sph_md4.h */
void
sph_md4_init(sph_md4_context *cc)
{
	sph_md4_context *sc;

	sc = cc;
	memcpy(sc->val, IV, sizeof(IV));
#if SPH_64
	sc->count = 0;
#else
	sc->count_high = sc->count_low = 0;
#endif
}

#define RFUN   md4_round
#define HASH   md4
#define LE32   1
/* see sph_md4.h */
//void
//sph_md4_close(void *cc, void *dst)
//{
//	md4_close(cc, dst, 4);
//	sph_md4_init(cc);
//}

/* see sph_md4.h */
void
sph_md4_comp(const _u32 msg[16], _u32 val[4])
{
#define X(i)   msg[i]
	MD4_ROUND_BODY(X, val);
#undef X
}