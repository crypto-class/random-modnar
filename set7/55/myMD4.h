#ifndef SPH_MD4_H__
#define SPH_MD4_H__

#include <stddef.h>
#include <cstdlib>
#include <cassert>
#include "fun.h"

typedef uint32_t _u32;

#define _C32(x)				((_u32)(x))
//取最后32位 mod32
#define _T32(x)				((x)&_C32(0xffffffff))
#define _ROTL32(x,n)		_T32(( (x) << (n) | ( (x) >> (32-n) )))
#define _ROTR32(x,n)		_ROTL32(x,(32-(n)))
//左移右移
#define LROT				_ROTL32
#define RROT				_ROTR32


static inline _u32
sph_dec32le_aligned(const void *src)
{
#if SPH_LITTLE_ENDIAN
	return *(const _u32 *)src;
#elif SPH_BIG_ENDIAN
#if SPH_SPARCV9_GCC && !SPH_NO_ASM
	_u32 tmp;

	__asm__ __volatile__("lda [%1]0x88,%0" : "=r" (tmp) : "r" (src));
	return tmp;
	/*
	* Not worth it generally.
	*
	#elif (SPH_PPC32_GCC || SPH_PPC64_GCC) && !SPH_NO_ASM
	_u32 tmp;
	__asm__ __volatile__ ("lwbrx %0,0,%1" : "=r" (tmp) : "r" (src));
	return tmp;
	*/
#else
	return sph_bswap32(*(const _u32 *)src);
#endif
#else
	return (_u32)(((const unsigned char *)src)[0])
		| ((_u32)(((const unsigned char *)src)[1]) << 8)
		| ((_u32)(((const unsigned char *)src)[2]) << 16)
		| ((_u32)(((const unsigned char *)src)[3]) << 24);
#endif
}

#define F(B, C, D)     ((((C) ^ (D)) & (B)) ^ (D))
#define G(B, C, D)     (((D) & (C)) | (((D) | (C)) & (B)))
#define H(B, C, D)     ((B) ^ (C) ^ (D))

/**
* Output size (in bits) for MD4.
*/
#define SPH_SIZE_md4   128







/**
* This structure is a context for MD4 computations: it contains the
* intermediate values and some data from the last entered block. Once
* a MD4 computation has been performed, the context can be reused for
* another computation.
*
* The contents of this structure are private. A running MD4 computation
* can be cloned by copying the context (e.g. with a simple
* <code>memcpy()</code>).
*/
typedef struct {
#ifndef DOXYGEN_IGNORE
	unsigned char buf[64];    /* first field, for alignment */
	_u32 val[4];
#if SPH_64
	sph_u64 count;
#else
	_u32 count_high, count_low;
#endif
#endif
} sph_md4_context;

/**
* Initialize a MD4 context. This process performs no memory allocation.
*
* @param cc   the MD4 context (pointer to a <code>sph_md4_context</code>)
*/
void sph_md4_init(void *cc);

/**
* Process some data bytes. It is acceptable that <code>len</code> is zero
* (in which case this function does nothing).
*
* @param cc     the MD4 context
* @param data   the input data
* @param len    the input data length (in bytes)
*/
void sph_md4(void *cc, const void *data, size_t len);

/**
* Terminate the current MD4 computation and output the result into the
* provided buffer. The destination buffer must be wide enough to
* accomodate the result (16 bytes). The context is automatically
* reinitialized.
*
* @param cc    the MD4 context
* @param dst   the destination buffer
*/
void sph_md4_close(void *cc, void *dst);

/**
* Apply the MD4 compression function on the provided data. The
* <code>msg</code> parameter contains the 16 32-bit input blocks,
* as numerical values (hence after the little-endian decoding). The
* <code>val</code> parameter contains the 4 32-bit input blocks for
* the compression function; the output is written in place in this
* array.
*
* @param msg   the message block (16 values)
* @param val   the function 128-bit input and output
*/
void sph_md4_comp(const _u32 msg[16], _u32 val[4]);

#endif