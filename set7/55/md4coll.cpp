#include <string>
#include <cstdlib>
#include <cassert>
#include "myMD4.h"
#include "Pool.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

using namespace CryptoPP;
using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)

typedef uint32_t _u32;

#define _C32(x)				((_u32)(x))
//取最后32位 mod32
#define _T32(x)				((x)&_C32(0xffffffff))
#define _ROTL32(x,n)		_T32(( (x) << (n) | ( (x) >> (32-n) )))
#define _ROTR32(x,n)		_ROTL32(x,(32-(n)))
//左移右移
#define LROT				_ROTL32
#define RROT				_ROTR32

/*
代码根据王小云Cryptanalysis of the Hash Functions MD4 and RIPEMD的paper
每一步都有具体的paper内的解释

代码架构git from https://github.com/ctz
*/

//取目标位
unsigned attachBit(_u32 a, unsigned n)
{
	return (a >> n) & 1;
}

void random_fill(uint8_t *out, size_t len)
{
	for (size_t i = 0; i < len; i++)
		out[i] = rand() & 0xff;
}

void random_init(void)
{
	srand((int)time(NULL));
}

struct DIY_MD4
{
	/*
	MD4初始化
	*/
	void init(_u32 state[4])
	{
		state[0] = 0x67452301u;
		state[1] = 0xEFCDAB89u;
		state[2] = 0x98BADCFEu;
		state[3] = 0x10325476u;
	}
	void random_block(uint32_t block[16])
	{
		random_fill((uint8_t*)block, 16 * 4);
	}
	void FirstRound(_u32 st[4], _u32 m[16])
	{
		_u32 A0 = st[0];
		_u32 B0 = st[1];
		_u32 C0 = st[2];
		_u32 D0 = st[3];
		/*
		This further means that if M satis?es all the conditions in Table 6, M and M consists of a collision. 
		*/
		_u32 ST;//其实就是当前状态，一个tmp值
		/*
		Single-Step Modi?cation. 
		It is easy to modify M such that the conditions in round 1 hold. 
		For example, m1 can be modi?ed as :
		d1 ← d1 ⊕(d1,7 ? 6)⊕((d1,8 ⊕a1,8) ? 7)⊕((d1,11 ⊕a1,11) ? 10)
		After simple-message modi?cation, 
		(M,M) is a collision with probability 2?25 by Table 6. 
		*/
#define EQL(X,n)			ST^=(attachBit(ST,n-1)^attachBit(X,n-1))<<(n-1)
#define INV(X,n)			ST^=(attachBit(ST,n-1)^attachBit(X,n-1)^1)<<(n-1)
#define SET(n)				ST|=(1<<(n-1))							//令第n位为1
#define CLR(n)				ST&=~(1<<(n-1))							//令第n位为0						
		/*
		接下来按照王小云的

		Table 5. Di?erential Characteristics in the Collision Di?erential for MD4
		Table 6. A SETONE of Su?cient Conditions for Collisions of MD4
		进行如下操作
		先定义

		a = φj(a,b,c,d,wj,4i,sj,4i)
		d = φj(d,a,b,c,wj,4i+1,sj,4i+1)
		c = φj(c,d,a,b,wj,4i+2,sj,4i+2)
		b = φj(b,c,d,a,wj,4i+3,sj,4i+3)

		*/
		
		/*
		?0(a,b,c,d,mk,s) = (( a + F(b,c,d)+mk) mod 232) ? s
		a1 shirt 3
		a1:a(1,7) = b(0,7)
		*/
		/* a1: a1,7 = b0,7 */
		uint32_t A1 = ST = LROT(_T32(A0 + F(B0, C0, D0) + m[0]), 3);
		EQL(B0, 7);
		A1 = ST;
		m[0] = _T32(RROT(ST, 3) - F(B0, C0, D0) - A0);

		/* d1: d1,7 = 0
		*     d1,8 = a1,8
		*     d1,11 = a1,11
		*/
		uint32_t D1 = ST = LROT(_T32(D0 + F(A1, B0, C0) + m[1]), 7);
		CLR(7);
		EQL(A1, 8);
		EQL(A1, 11);
		D1 = ST;
		m[1] = _T32(RROT(ST, 7) - F(A1, B0, C0) - D0);

		/* c1: c1,7 = 1
		*     c1,8 = 1
		*     c1,11 = 0
		*     c1,26 = d1,26
		*/
		uint32_t C1 = ST = LROT(_T32(C0 + F(D1, A1, B0) + m[2]), 11);
		SET(7);
		SET(8);
		CLR(11);
		EQL(D1, 26);
		C1 = ST;
		m[2] = _T32(RROT(ST, 11) - F(D1, A1, B0) - C0);

		/* b1: b1,7 = 1
		*     b1,8 = 0
		*     b1,11 = 0
		*     b1,26 = 0
		*/
		uint32_t B1 = ST = LROT(_T32(B0 + F(C1, D1, A1) + m[3]), 19);
		SET(7);
		CLR(8);
		CLR(11);
		CLR(26);
		B1 = ST;
		m[3] = _T32(RROT(ST, 19) - F(C1, D1, A1) - B0);

		/* a2 a2,8 = 1, a2,11 = 1, a2,26 = 0, a2,14 = b1,14 */
		uint32_t A2 = ST = LROT(_T32(A1 + F(B1, C1, D1) + m[4]), 3);
		SET(8);
		SET(11);
		CLR(26);
		EQL(B1, 14);
		A2 = ST;
		m[4] = _T32(RROT(ST, 3) - F(B1, C1, D1) - A1);

		/* d2 d2,14 = 0, d2,19 = a2,19, d2,20 = a2,20, d2,21 = a2,21, d2,22 = a2,22, d2,26 = 1 */
		uint32_t D2 = ST = LROT(_T32(D1 + F(A2, B1, C1) + m[5]), 7);
		CLR(14);
		EQL(A2, 19);
		EQL(A2, 20);
		EQL(A2, 21);
		EQL(A2, 22);
		SET(26);
		D2 = ST;
		m[5] = _T32(RROT(ST, 7) - F(A2, B1, C1) - D1);

		/* c2 c2,13 = d2,13, c2,14 = 0, c2,15 = d2,15, c2,19 = 0, c2,20 = 0, c2,21 = 1, c2,22 = 0 */
		uint32_t C2 = ST = LROT(_T32(C1 + F(D2, A2, B1) + m[6]), 11);
		EQL(D2, 13);
		CLR(14);
		EQL(D2, 15);
		CLR(19);
		CLR(20);
		SET(21);
		CLR(22);
		C2 = ST;
		m[6] = _T32(RROT(ST, 11) - F(D2, A2, B1) - C1);

		/* b2 b2,13 = 1, b2,14 = 1, b2,15 = 0, b2,17 = c2,17, b2,19 = 0, b2,20 = 0, b2,21 = 0
		b2,22 = 0 */
		uint32_t B2 = ST = LROT(_T32(B1 + F(C2, D2, A2) + m[7]), 19);
		SET(13);
		SET(14);
		CLR(15);
		EQL(C2, 17);
		CLR(19);
		CLR(20);
		CLR(21);
		B2 = ST;
		m[7] = _T32(RROT(ST, 19) - F(C2, D2, A2) - B1);

		/* a3 a3,13 = 1, a3,14 = 1, a3,15 = 1, a3,17 = 0, a3,19 = 0, a3,20 = 0, a3,21 = 0,
		a3,23 = b2,23 a3,22 = 1, a3,26 = b2,26 */
		uint32_t A3 = ST = LROT(_T32(A2 + F(B2, C2, D2) + m[8]), 3);
		SET(13);
		SET(14);
		SET(15);
		CLR(17);
		CLR(19);
		CLR(20);
		CLR(21);
		EQL(B2, 23);
		SET(22);
		EQL(B2, 26);
		A3 = ST;
		m[8] = _T32(RROT(ST, 3) - F(B2, C2, D2) - A2);

		/* d3 d3,13 = 1, d3,14 = 1, d3,15 = 1, d3,17 = 0, d3,20 = 0, d3,21 = 1, d3,22 = 1, d3,23 = 0,
		d3,26 = 1, d3,30 = a3,30 */
		uint32_t D3 = ST = LROT(_T32(D2 + F(A3, B2, C2) + m[9]), 7);
		SET(13);
		SET(14);
		SET(15);
		CLR(17);
		CLR(20);
		SET(21);
		SET(22);
		CLR(23);
		SET(26);
		EQL(A3, 30);
		D3 = ST;
		m[9] = _T32(RROT(ST, 7) - F(A3, B2, C2) - D2);

		/* c3 c3,17 = 1, c3,20 = 0, c3,21 = 0, c3,22 = 0, c3,23 = 0, c3,26 = 0, c3,30 = 1, c3,32 = d3,32 */
		uint32_t C3 = ST = LROT(_T32(C2 + F(D3, A3, B2) + m[10]), 11);
		SET(17);
		CLR(20);
		CLR(21);
		CLR(22);
		CLR(23);
		CLR(26);
		SET(30);
		EQL(D3, 32);
		C3 = ST;
		m[10] = _T32(RROT(ST, 11) - F(D3, A3, B2) - C2);

		/* b3 b3,20 = 0, b3,21 = 1, b3,22 = 1, b3,23 = c3,23, b3,26 = 1, b3,30 = 0, b3,32 = 0 */
		uint32_t B3 = ST = LROT(_T32(B2 + F(C3, D3, A3) + m[11]), 19);
		CLR(20);
		SET(21);
		SET(22);
		EQL(C3, 23);
		SET(26);
		CLR(30);
		CLR(32);
		B3 = ST;
		m[11] = _T32(RROT(ST, 19) - F(C3, D3, A3) - B2);

		/* a4 a4,23 = 0, a4,26 = 0, a4,27 = b3,27, a4,29 = b3,29, a4,30 = 1, a4,32 = 0 */
		uint32_t A4 = ST = LROT(_T32(A3 + F(B3, C3, D3) + m[12]), 3);
		CLR(23);
		CLR(26);
		EQL(B3, 27);
		EQL(B3, 29);
		SET(30);
		CLR(32);
		A4 = ST;
		m[12] = _T32(RROT(ST, 3) - F(B3, C3, D3) - A3);

		/* d4 d4,23 = 0, d4,26 = 0, d4,27 = 1, d4,29 = 1, d4,30 = 0, d4,32 = 1 */
		uint32_t D4 = ST = LROT(_T32(D3 + F(A4, B3, C3) + m[13]), 7);
		CLR(23);
		CLR(26);
		SET(27);
		SET(29);
		CLR(30);
		SET(32);
		D4 = ST;
		m[13] = _T32(RROT(ST, 7) - F(A4, B3, C3) - D3);

		/* c4 c4,19 = d4,19, c4,23 = 1, c4,26 = 1, c4,27 = 0, c4,29 = 0, c4,30 = 0 */
		uint32_t C4 = ST = LROT(_T32(C3 + F(D4, A4, B3) + m[14]), 11);
		EQL(D4, 19);
		SET(23);
		SET(26);
		CLR(27);
		CLR(29);
		CLR(30);
		C4 = ST;
		m[14] = _T32(RROT(ST, 11) - F(D4, A4, B3) - C3);

		/* b4 b4,19 = 0, b4,26 = c4,26 = 1, b4,27 = 1, b4,29 = 1, b4,30 = 0 */
		uint32_t B4 = ST = LROT(_T32(B3 + F(C4, D4, A4) + m[15]), 19);
		CLR(19);
		EQL(C4, 26);
		SET(27);
		SET(29);
		CLR(30);
		B4 = ST;
		m[15] = _T32(RROT(ST, 19) - F(C4, D4, A4) - B3);

		/* a5 a5,19 = c4,19, a5,26 = 1, a5,27 = 0, a5,29 = 1, a5,32 = 1 */
		uint32_t A5 = ST = LROT(_T32(A4 + G(B4, C4, D4) + m[0] + _C32(0x5A827999)), 3);
		EQL(C4, 19);
		SET(26);
		CLR(27);
		SET(29);
		SET(32);
		A5 = ST;
		m[0] = _T32(RROT(ST, 3) - G(B4, C4, D4) - A4 - _C32(0x5A827999));

		/* d5 d5,19 = a5,19, d5,26 = b4,26, d5,27 = b4,27, d5,29 = b4,29, d5,32 = b4,32 */
		uint32_t D5 = ST = LROT(_T32(D4 + G(A5, B4, C4) + m[4] + _C32(0x5A827999)), 5);
		EQL(A5, 19);
		EQL(B4, 26);
		EQL(B4, 27);
		EQL(B4, 29);
		EQL(B4, 32);
		D5 = ST;
		m[4] = _T32(RROT(ST, 5) - G(A5, B4, C4) - D4 - _C32(0x5A827999));

		/* c5 c5,26 = d5,26, c5,27 = d5,27, c5,29 = d5,29, c5,30 = d5,30, c5,32 = d5,32 */
		uint32_t C5 = ST = LROT(_T32(C4 + G(D5, A5, B4) + m[8] + _C32(0x5A827999)), 9);
		EQL(D5, 26);
		EQL(D5, 27);
		EQL(D5, 29);
		EQL(D5, 30);
		EQL(D5, 32);
		C5 = ST;
		m[8] = _T32(RROT(ST, 9) - G(D5, A5, B4) - C4 - _C32(0x5A827999));

		/* b5 b5,29 = c5,29, b5,30 = 1, b5,32 = 0 */
		uint32_t B5 = ST = LROT(_T32(B4 + G(C5, D5, A5) + m[12] + _C32(0x5A827999)), 13);
		EQL(C5, 29);
		SET(30);
		CLR(32);
		B5 = ST;
		m[12] = _T32(RROT(ST, 13) - G(C5, D5, A5) - B4 - _C32(0x5A827999));

		/* a6 a6,29 = 1, a6,32 = 1 */
		uint32_t A6 = ST = LROT(_T32(A5 + G(B5, C5, D5) + m[1] + _C32(0x5A827999)), 3);
		SET(29);
		SET(32);
		A6 = ST;
		m[1] = _T32(RROT(ST, 3) - G(B5, C5, D5) - A5 - _C32(0x5A827999));

		/* d6 d6,29 = b5,29 */
		uint32_t D6 = ST = LROT(_T32(D5 + G(A6, B5, C5) + m[5] + _C32(0x5A827999)), 5);
		EQL(B5, 29);
		D6 = ST;
		m[5] = _T32(RROT(ST, 5) - G(A6, B5, C5) - D5 - _C32(0x5A827999));

		/* c6 c6,29 = d6,29, c6,30 = d6,30 + 1, c6,32 = d6,32 + 1 */
		uint32_t C6 = ST = LROT(_T32(C5 + G(D6, A6, B5) + m[9] + _C32(0x5A827999)), 9);
		EQL(D6, 29);
		INV(D6, 30);
		INV(D6, 32);
		C6 = ST;
		m[9] = _T32(RROT(ST, 9) - G(D6, A6, B5) - C5 - _C32(0x5A827999));

		/* b9 b9,32 = 1 */

		/* a10 a10,32 = 1 */
	}
	/*
	Table 3. Two collisions for MD4. H is the hash value with little-endian and no message padding,
	and H? is the hash value with big-endian and message padding
	M1 4d7a9c83 56cb927a b9d5a578 57a7a5ee de748a3c dcc366b3 b683a020 3b2a5d9f c69d71b3 f9e99198 d79f805e a63bb2e8 45dd8e31 97e31fe5 2794bf08 b9e8c3e9
	M 1 4d7a9c83 d6cb927a 29d5a578 57a7a5ee de748a3c dcc366b3 b683a020 3b2a5d9f c69d71b3 f9e99198 d79f805e a63bb2e8 45dc8e31 97e31fe5 2794bf08 b9e8c3e9

	*/
	void MD4CollsionsTableM1(_u32 block[16])
	{
		block[0] = 0x4d7a9c83u;
		block[1] = 0x56cb927au;
		block[2] = 0xb9d5a578u;
		block[3] = 0x57a7a5eeu;
		block[4] = 0xde748a3cu;
		block[5] = 0xdcc366b3u;
		block[6] = 0xb683a020u;
		block[7] = 0x3b2a5d9fu;
		block[8] = 0xc69d71b3u;
		block[9] = 0xf9e99198u;
		block[10] = 0xd79f805eu;
		block[11] = 0xa63bb2e8u;
		block[12] = 0x45dd8e31u;
		block[13] = 0x97e31fe5u;
		block[14] = 0x2794bf08u;
		block[15] = 0xb9e8c3e9u;
	}
	void MD4CollsionsTableMp(_u32 block[16])
	{
		block[0] = 0x4d7a9c83u;
		block[1] = 0xd6cb927au;
		block[2] = 0x29d5a578u;
		block[3] = 0x57a7a5eeu;
		block[4] = 0xde748a3cu;
		block[5] = 0xdcc366b3u;
		block[6] = 0xb683a020u;
		block[7] = 0x3b2a5d9fu;
		block[8] = 0xc69d71b3u;
		block[9] = 0xf9e99198u;
		block[10] = 0xd79f805eu;
		block[11] = 0xa63bb2e8u;
		block[12] = 0x45dc8e31u;
		block[13] = 0x97e31fe5u;
		block[14] = 0x2794bf08u;
		block[15] = 0xb9e8c3e9u;
	}

	/*
	检查一下冲突表是否构造正确
	*/
	void checkCollosion()
	{
		_u32 state[4] = { 0 };
		_u32 msg[16] = { 0 };
		_u32 msg2[16] = { 0 };

		MD4CollsionsTableM1(msg);

		init(state);
		MD4CollsionsTableM1(msg2);
		FirstRound(state, msg2);
		assert(0 == memcmp(msg, msg2, sizeof(msg)));

		init(state);
		sph_md4_comp(msg, state);
		assert(state[0] == 0x5f5c1a0du && state[1] == 0x71b36046u && state[2] == 0x1b5435dau && state[3] == 0x9b0d807au);

		init(state);
		MD4CollsionsTableMp(msg);
		sph_md4_comp(msg, state);
		assert(state[0] == 0x5f5c1a0du && state[1] == 0x71b36046u && state[2] == 0x1b5435dau && state[3] == 0x9b0d807au);
	}
	/*
	找到冲突的hash
	*/
	void print_pair(uint32_t msg[16], uint32_t st[4])
	{
		printf("message: ");
		for (int i = 0; i < 16; i++)
			printf("%08x ", msg[i]);
		printf("\n");
		printf("= ");
		printf("\n");
		for (int i = 0; i < 16; i++)
			printf("%08x ", st[i]);
		printf("\n");
	}

	/*
	开始寻找碰撞
	*/
	void find_collision(void)
	{
		uint32_t message1[16] = { 0 };
		uint32_t hash1[4] = { 0 };
		uint32_t message2[16] = { 0 };
		uint32_t hash2[4] = { 0 };
		uint32_t state[4] = { 0 };

		while (1)
		{
			/* 先生成一个消息串 */
			random_block(message1);

			/* do the appropriate twiddles */
			init(state);
			FirstRound(state, message1);

			init(hash1);
			sph_md4_comp(message1, hash1);

			memcpy(message2, message1, sizeof message2);

			/*
			论文第七页
			检查生成的两个M的delta
			1. Find a collision di?erential in which M and M produces a collision.
			*/
			message2[1] ^= 0x80000000;
			message2[2] ^= 0x90000000;
			message2[12] ^= 0x00010000;

			init(hash2);
			sph_md4_comp(message2, hash2);

			if (memcmp(hash1, hash2, sizeof hash1) == 0 && memcmp(message1, message2, sizeof message1) != 0)
			{
				printf("collision found\n");
				print_pair(message1, hash1);
				print_pair(message2, hash2);

				return;
			}
		}
	}
};


int main(int argc, char **argv)
{
	DIY_MD4 myHash;
	pool p[1] = { pool_create() };
	random_init();

	
	/* first check the given collisions in the paper */
	myHash.checkCollosion();
	cout << "Table5,6 are ok ok" << endl;
	myHash.find_collision();

	p->finish(p);
	return 0;
}

