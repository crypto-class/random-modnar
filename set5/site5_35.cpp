#include <string>
#include "fun.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

using namespace CryptoPP;
using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)


/*
协商过程同上一节
攻击过程如下

when g = 1
KA = 1^a mod p = 1
KB = 1^b mod p = 1
Key = 1^a^b mod p = 1
You can use key = 1 to attack

when g = p
KA = p^a mod p = 0
KB = p^b mod p = 0
Key = p^a^b mod p = 0
You can use key = 0 to attack

when g = p - 1
KA = (p-1)^a mod p = 1 || -1
KB = (p-1)^b mod p = 1 || -1
Key = (p-1)^a^b mod p  = 1 || -1
KA and KB maybe different
You can use key = 1 || p -1  to attack

*/

struct User
{
	Integer k;
	string plain, cipher, mac;
	byte keys[AES::DEFAULT_KEYLENGTH];
	Integer SynthesisKey;
};
struct MUser
{
	Integer k1, k2, K1, K2;
	string plain, cipherA, cipherB, mac;
	byte keyA[AES::DEFAULT_KEYLENGTH];
	byte keyB[AES::DEFAULT_KEYLENGTH];
	Integer SynthesisKeyA, SynthesisKeyB;
};
AutoSeededRandomPool rnd;

Integer p, g, KA, KB;
User A, B;
MUser Eve;



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

void DHGenPandG(Integer &p, Integer &g, int len)
{
	DH dh;
	dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, len);
	p = dh.GetGroupParameters().GetModulus();
	g = dh.GetGroupParameters().GetGenerator();
	cout << "p = " << p << endl;
	//cout << "g = " << g << endl;
}

void DHCalSynthesisKey(User &U, Integer KnowKey)
{
	U.SynthesisKey = powMod(KnowKey, U.k, p);
	cout << "synthesisKey = " << U.SynthesisKey << endl;
	Inter2Byte(U.SynthesisKey, U.keys, AES::DEFAULT_KEYLENGTH);
	cout << "AES_KEY = " << U.keys << endl;
}

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
	cout << "---------condition: g = p - 1--------------" << endl;
	g = p - 1;//////////////////////////////////////////////////////p = 1的情况

	KA = DHGenPriKey(A);
	cout << "KA = " << KA << endl;

	KB = DHGenPriKey(B);
	cout << "KB = " << KB << endl;

	DHCalSynthesisKey(A, KB);
	DHCalSynthesisKey(B, KA);


	//cout << sizeof(A.keys) << endl;
	cout << AES::DEFAULT_KEYLENGTH << endl;

	byte iv[AES::BLOCKSIZE];
	rnd.GenerateBlock(iv, AES::BLOCKSIZE);

	A.plain = "I'am y0uqh, from WZ, ZJ";


	cout << "A original plain = " << A.plain << endl;
	A.cipher = CBC_AES_ENC(A.plain, A.keys, iv);
	cout << "A.cipher  = " << A.cipher << endl;
	B.cipher = A.cipher;
	cout << "B.cipher = " << B.cipher << endl;
	B.plain = CBC_AES_DEC(B.cipher, B.keys, iv);
	cout << "B get plain = " << B.plain << endl;

	byte attackSecret[AES::DEFAULT_KEYLENGTH];


	Integer attackNum_1 = p - 1, attackNum_2 = 1;//攻击尝试的密钥
	try																	
	{
		Inter2Byte(attackNum_1, attackSecret, AES::DEFAULT_KEYLENGTH);
		cout << "attack Secret = " << attackSecret << endl;
		string attachplain = CBC_AES_DEC(A.cipher, attackSecret, iv);
		cout << "set key = p - 1, attack plain = " << attachplain << endl;
	}
	catch (const std::exception&)
	{
		Inter2Byte(attackNum_2, attackSecret, AES::DEFAULT_KEYLENGTH);
		cout << "attack Secret = " << attackSecret << endl;
		string attachplain = CBC_AES_DEC(A.cipher, attackSecret, iv);
		cout << "set key = 1, attack plain = " << attachplain << endl;
	}

	system("pause");
	return 0;
}

/*

p = 312023063253593443142084669790931525439.
---------condition: g = 1--------------
produce key = 00E91F40
KA = 1.
produce key = 00E920C0
KB = 1.
synthesisKey = 1.
AES_KEY = 
synthesisKey = 1.
AES_KEY = 
16
A original plain = I'am y0uqh, from WZ, ZJ
A.cipher  = {x鞀?R鹻Tq釄$慸P#悊膡6寺
B.cipher = {x鞀?R鹻Tq釄$慸P#悊膡6寺
B get plain = I'am y0uqh, from WZ, ZJ
attack Secret = 
set key = 1, attack plain = I'am y0uqh, from WZ, ZJ

----------------------------------------------------------
p = 221850647374049393276527872580879080083.
---------condition: g = p--------------
produce key = 010C2040
KA = 0.
produce key = 010C1F40
KB = 0.
synthesisKey = 0.
AES_KEY =
synthesisKey = 0.
AES_KEY =
16
A original plain = I'am y0uqh, from WZ, ZJ
A.cipher  = !烠隻E?猈誱錅^gGb遖?V?7|
B.cipher = !烠隻E?猈誱錅^gGb遖?V?7|
B get plain = I'am y0uqh, from WZ, ZJ
attack Secret =
set key = 0, attack plain = I'am y0uqh, from WZ, ZJ

-------------------------------------------
p = 317344614065121564244652757381460991963.
---------condition: g = p - 1--------------
produce key = 01214730
KA = 317344614065121564244652757381460991962.
produce key = 01214670
KB = 317344614065121564244652757381460991962.
synthesisKey = 317344614065121564244652757381460991962.
AES_KEY = O薶b(鳞jwKxY绢摹
synthesisKey = 317344614065121564244652757381460991962.
AES_KEY = O薶b(鳞jwKxY绢摹
16
A original plain = I'am y0uqh, from WZ, ZJ
A.cipher  = mDcdn!8蛑收榎櫵6滃
O攉$h
B.cipher = mDcdn!8蛑收榎櫵6滃
O攉$h
B get plain = I'am y0uqh, from WZ, ZJ
attack Secret = O薶b(鳞jwKxY绢烫烫烫烫S+裚t輮蠪I麕3烫烫世-筘
set key = p - 1, attack plain = I'am y0uqh, from WZ, ZJ


-------------------------------------------
p = 206590446842013531924718030582197885203.
---------condition: g = p - 1--------------
produce key = 00652630
KA = 206590446842013531924718030582197885202.
produce key = 00652670
KB = 1.
synthesisKey = 1.
AES_KEY = 
synthesisKey = 1.
AES_KEY = 
16
A original plain = I'am y0uqh, from WZ, ZJ
A.cipher  = 溉
?騒苦"Q-禃W.oI??裭
B.cipher = 溉
?騒苦"Q-禃W.oI??裭
B get plain = I'am y0uqh, from WZ, ZJ
attack Secret = 檿MKU該倀,~n遦浱烫烫烫獭暓3s?贋t?烫烫翈m螋鵚
attack Secret = 
set key = 1, attack plain = I'am y0uqh, from WZ, ZJ


*/