#include <string>
#include "fun.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

using namespace CryptoPP;
using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)

//FILE *fi = freopen("data.in", "r", stdin);
//FILE *fi = freopen("test.in", "r", stdin);
//FILE *f2 = freopen("data.out", "w", stdout);


Integer p, g, KA, KB;

struct User
{
	Integer k;
	string plain, cipher, mac;
	byte keys[AES::DEFAULT_KEYLENGTH];
	Integer SynthesisKey;
}A, B;
struct MUser
{
	Integer k1, k2, K1, K2;
	string plain, cipherA, cipherB, mac;
	byte keyA[AES::DEFAULT_KEYLENGTH];
	byte keyB[AES::DEFAULT_KEYLENGTH];
	Integer SynthesisKeyA, SynthesisKeyB;
}Eve;
AutoSeededRandomPool rnd;

/*
生成各自的私钥
*/
Integer DHGenPriKey(User &U)
{

	SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());
	cout << "produce key = " << key << endl;
	int len = AES::DEFAULT_KEYLENGTH;

	U.k = Byte2Inter(key, len);
	Integer PK = powMod(g, U.k, p);
	return PK;
}

void DHMiddleAttackKey(MUser &U)
{
	int len = AES::DEFAULT_KEYLENGTH;

	SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());
	cout << "produce key1 = " << key << endl;
	U.k1 = Byte2Inter(key, len);
	U.K1 = powMod(g, U.k1, p);

	rnd.GenerateBlock(key, key.size());
	cout << "produce key2 = " << key << endl;
	U.k2 = Byte2Inter(key, len);
	U.K2 = powMod(g, U.k2, p);
}
/*
协商 p,g
*/
void DHGenPandG(Integer &p, Integer &g, int len)
{
	DH dh;
	dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, len);
	p = dh.GetGroupParameters().GetModulus();
	g = dh.GetGroupParameters().GetGenerator();
	cout << "p = " << p << endl;
	cout << "g = " << g << endl;
}
/*
协商出来的共同密钥g^(a*b)
*/
void DHCalSynthesisKey(User &U, Integer KnowKey)
{
	U.SynthesisKey = powMod(KnowKey, U.k, p);
	cout << "synthesisKey = " << U.SynthesisKey << endl;
	Inter2Byte(U.SynthesisKey, U.keys, AES::DEFAULT_KEYLENGTH);
	cout << "AES_KEY = " << U.keys << endl;
}
/*
中间人也模拟上述协商过程
*/
void DHCalSynthesisKeyWithEve(User &U, Integer KnowKeyA, MUser &E, Integer KnowKeyE, Integer PriKey, Integer &SKEY, byte byteKey[])
{
	U.SynthesisKey = powMod(KnowKeyE, U.k, p);
	cout << "User synthesisKey = " << U.SynthesisKey << endl;
	Inter2Byte(U.SynthesisKey, U.keys, AES::DEFAULT_KEYLENGTH);
	cout << "User AES_KEY = " << U.keys << endl;

	SKEY = powMod(KnowKeyA, PriKey, p);
	cout << "Mid synthesisKey = " << SKEY << endl;
	Inter2Byte(SKEY, byteKey, AES::DEFAULT_KEYLENGTH);
	cout << "Mid AES_KEY = " << byteKey << endl;
}


int main()
{
	srand(time(0));


	DHGenPandG(p, g, BYTELEN * AES::DEFAULT_KEYLENGTH);

	KA = DHGenPriKey(A);
	cout << "KA = " << KA << endl;

	KB = DHGenPriKey(B);
	cout << "KB = " << KB << endl;

	DHCalSynthesisKey(A, KB);
	DHCalSynthesisKey(B, KA);


	cout << sizeof(A.keys) << endl;
	cout << AES::DEFAULT_KEYLENGTH << endl;

	byte iv[AES::BLOCKSIZE];
	rnd.GenerateBlock(iv, AES::BLOCKSIZE);

	A.plain = "I'am y0uqh, from WZ,ZJ";
	cout << "A original plain = " << A.plain << endl;
	A.cipher = CBC_AES_ENC(A.plain, A.keys, iv);
	cout << "A.cipher  = " << A.cipher << endl;
	B.cipher = A.cipher;
	cout << "B.cipher = " << B.cipher << endl;
	B.plain = CBC_AES_DEC(B.cipher, B.keys, iv);
	cout << "B get plain = " << B.plain << endl;

	cout << "-------------now let's attach message by middle attack------------" << endl;
	DHMiddleAttackKey(Eve);
	cout << "now Eve has K1, K2" << endl;
	cout << "K1 = " << Eve.K1 << endl;
	cout << "K2 = " << Eve.K2 << endl;
	cout << "Eve connect whth Alice" << endl;
	DHCalSynthesisKeyWithEve(A, KA, Eve, Eve.K1, Eve.k1, Eve.SynthesisKeyA, Eve.keyA);
	DHCalSynthesisKeyWithEve(B, KB, Eve, Eve.K2, Eve.k2, Eve.SynthesisKeyB, Eve.keyB);

	rnd.GenerateBlock(iv, AES::BLOCKSIZE);
	cout << "A original plain = " << A.plain << endl;
	A.cipher = CBC_AES_ENC(A.plain, A.keys, iv);
	cout << "A.cipher  = " << A.cipher << endl;


	Eve.cipherA = A.cipher;
	cout << "Eve.cipherA = " << Eve.cipherA << endl;
	Eve.plain = CBC_AES_DEC(Eve.cipherA, Eve.keyA, iv);
	cout << "Eve get plain = " << Eve.plain << endl;

	Eve.cipherB = CBC_AES_ENC(Eve.plain, Eve.keyB, iv);
	cout << "Eve.cipherB  = " << A.cipher << endl;

	B.cipher = Eve.cipherB;
	cout << "B.cipher = " << B.cipher << endl;
	B.plain = CBC_AES_DEC(B.cipher, B.keys, iv);
	cout << "B get plain = " << B.plain << endl;

	system("pause");
	return 0;
}

/* the result

p = 212986734512876367363365081865034151483.
g = 3.
produce key = 00A92280
KA = 9339139321790148559065221625811218229.
produce key = 00A91F80
KB = 39328949045090513051630331856230159514.
synthesisKey = 152818627198466759139170623508605725575.
AES_KEY = 噆r??漞傯a    眺r摹n
synthesisKey = 152818627198466759139170623508605725575.
AES_KEY = 噆r??漞傯a    眺r摹n
16
16
A original plain = I'am y0uqh, from WZ,ZJ
A.cipher  = ??爈掁e╒  6=H?頨萖<LFe
B.cipher = ??爈掁e╒   6=H?頨萖<LFe
B get plain = I'am y0uqh, from WZ,ZJ
-------------now let's attach message by middle attack------------
produce key1 = 00A92400
produce key2 = 00A92400
now Eve has K1, K2
K1 = 50044978829830442588092179547425311046.
K2 = 102033191516477144024396794221639868859.
Eve connect whth Alice
User synthesisKey = 130687723681435666417880509380640700684.
User AES_KEY = 鶢']>丱娖@奞b摹n
Mid synthesisKey = 130687723681435666417880509380640700684.
Mid AES_KEY = 鶢']>丱娖@奞b
User synthesisKey = 129688713268634061399723596983557823763.
User AES_KEY = ~8g.跑笆?#慳摹n
Mid synthesisKey = 129688713268634061399723596983557823763.
Mid AES_KEY = ~8g.跑笆?#慳摹n
A original plain = I'am y0uqh, from WZ,ZJ
A.cipher  = 祪(AB鹀笋赁:iV??梀垼馄p35贝赬
Eve.cipherA = 祪(AB鹀笋赁:iV??梀垼馄p35贝赬
Eve get plain = I'am y0uqh, from WZ,ZJ
Eve.cipherB  = 祪(AB鹀笋赁:iV??梀垼馄p35贝赬
B.cipher = L'r滽咆灓w?EG

*/