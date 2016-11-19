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

#define SECURELEN 128
FILE *fo = freopen("result.out", "w", stdout);
struct User
{
	Integer k;
	string plain, cipher;
	byte keys[AES::DEFAULT_KEYLENGTH];
	Integer SynthesisKey;
	Integer salt;
	Integer S;
	string K;
	string passwd;
	string email;
	Integer u;
};

Integer p, g, KS, KC, k, A, B, V, u;

AutoSeededRandomPool rnd;
int dict_num = 6;
/*
字典攻击
跑字典就好了
意思一下。。。
*/
string dict[100] = {
	"password",
	"email",
	"success",
	"Cryptopals",
	"6666666",
	"###rrrppp"
};

void DHAgreementSC(int len, User &S)
{
	S.email = "y0uqh@y0uqh.com";
	g = 2;
	k = 3;
	DH dh;
	dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, len);
	p = dh.GetGroupParameters().GetModulus();
	cout << "email = " << S.email << endl;

	cout << " p = " << p << endl;
	cout << " g = " << g << endl;
	cout << " k = " << k << endl;
}

/*
x = SHA256(salt|password)
v = g**x % n
*/
void ServerInit(User &S)
{
	SHA256 hash;
	SecByteBlock salt(0x00, SECURELEN / BYTELEN);
	rnd.GenerateBlock(salt, SECURELEN / BYTELEN);
	S.salt = Byte2Inter(salt, SECURELEN / BYTELEN);
	string saltPass = Inter2Str(S.salt);
	saltPass += S.passwd;
	string xH;
	StringSource s(saltPass, true, new HashFilter(hash, new HexEncoder(new StringSink(xH))));
	cout << "salt = " << S.salt << endl;
	cout << "salt|passwd = " << saltPass << endl;
	cout << "xH = " << xH << endl;

	Integer x = Str2Inter(xH, 16);
	cout << "x = " << x << endl;
	V = powMod(g, x, p);
	cout << "v = " << V << endl;
}
/*
I, A = g**a % n
*/
void step3C2S(User &C)
{
	cout << "C send I = " << C.email << endl;
	SecByteBlock key(0x00, SECURELEN / BYTELEN);
	rnd.GenerateBlock(key, key.size());
	cout << "produce Client'a = " << key << endl;
	int len = SECURELEN / BYTELEN;

	C.k = Byte2Inter(key, len);
	Integer KC = powMod(g, C.k, p);
	A = KC;
	cout << "A = " << A << endl;
}
/*
S->C
salt, B = g**b % n, u = 128 bit random number
*/
void step4S2C(User &C, User &S)
{

	C.salt = S.salt;

	SecByteBlock key(0x00, SECURELEN / BYTELEN);
	rnd.GenerateBlock(key, key.size());
	cout << "produce Server'b = " << key << endl;
	int len = SECURELEN / BYTELEN;

	S.k = Byte2Inter(key, len);

	SecByteBlock keyu(0x00, SECURELEN / BYTELEN);
	rnd.GenerateBlock(keyu, keyu.size());

	S.u = Byte2Inter(keyu, len);

	cout << "u =  " << S.u << endl;
	C.u = S.u;

	B = powMod(g, S.k, p);
	cout << "B = " << B << endl;
}
/*
C
x = SHA256(salt|password)
S = B**(a + ux) % n
K = SHA256(S)
*/
void step6C(User &C)
{
	string ab = Inter2Str(C.salt) + C.passwd;
	cout << "client ab = " << ab << endl;
	SHA256 hash;
	string xH;
	StringSource s(ab, true, new HashFilter(hash, new HexEncoder(new StringSink(xH))));
	Integer x = Str2Inter(xH, 16);
	cout << "xH = " << xH << endl;
	cout << "x = " << x << endl;

	C.S = powMod(B, C.k + C.u*x, p);
	string ss = Inter2Str(C.S);
	cout << " CS = " << ss << endl;
	string testC;
	StringSource s22(ss, true, new HashFilter(hash, new HexEncoder(new StringSink(testC))));
	C.K = testC;
	cout << "CK = " << C.K << endl;
}
/*
S
S = (A * v ** u)**b % n
K = SHA256(S)
*/
void step7S(User &S)
{
	Integer tmp1 = A*powMod(V, S.u, p);
	S.S = powMod(tmp1, S.k, p);
	SHA256 hash;
	string ss = Inter2Str(S.S);
	cout << " SS = " << ss << endl;
	string testS;
	StringSource s22(ss, true, new HashFilter(hash, new HexEncoder(new StringSink(testS))));
	S.K = testS;
	cout << "SK = " << S.K << endl;
}

bool Validates(User &S, User &C)
{
	string HC, HS;
	SecByteBlock keyC(SECURELEN / BYTELEN);
	Inter2Byte(C.salt, keyC, SECURELEN / BYTELEN);
	cout << "keyC = " << keyC << endl;
	try
	{
		HMAC< SHA256 > hmac(keyC, keyC.size());

		StringSource ss(C.K, true,
			new HashFilter(hmac,
				new StringSink(HC)
				) // HashFilter      
			); // StringSource
	}
	catch (Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	SecByteBlock keyS(SECURELEN / BYTELEN);
	Inter2Byte(S.salt, keyS, SECURELEN / BYTELEN);
	cout << "keyS = " << keyS << endl;
	try
	{
		HMAC< SHA256 > hmac(keyS, keyS.size());

		StringSource ss(S.K, true,
			new HashFilter(hmac,
				new StringSink(HS)
				) // HashFilter      
			); // StringSource
	}
	catch (Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	cout << "HC = " << HC << endl;
	cout << "HS = " << HS << endl;

	if (HS == HC)
	{
		cout << "Success!!" << endl;
		return 1;
	}
	else {
		cout << "FAILED" << endl;
		return 0;
	}
	return 0;
}

User client, server, eve;
int main()
{
	srand(time(0));

	client.email = "y0uqh@y0uqh.com";
	//client.passwd = "ILOVECHINA";
	client.passwd = "Cryptopals";

	cout << "----------C & S Agree on N = [NIST Prime]---------" << endl;
	cout << "----------g = 2, k = 3, I(email), P(password)----------" << endl;

	DHAgreementSC(SECURELEN, server);
	server.passwd = "Cryptopals";

	ServerInit(server);


	step3C2S(client);

	step4S2C(client, server);

	step6C(client);


	step7S(server);

	cout << "---------Let's validate----------" << endl;
	Validates(server, client);

	cout << "----------Let's attack-------------" << endl;

	bool success = 0;
	for (int i = 0;i < dict_num;i++)////////////字典
	{

		cout << "----------try times------ " << i << endl;
		DHAgreementSC(SECURELEN, eve);

		eve.passwd = dict[i];
		cout << "passwd = " << eve.passwd << endl;

		ServerInit(eve);

		step3C2S(client);

		step4S2C(client, eve);

		step6C(client);

		step7S(eve);

		success = Validates(eve, client);

		if (success)
		{
			cout << "success!!!" << endl;
			cout << "the password = " << eve.passwd << endl;
			break;
		}
	}
	if (!success)
	{
		cout << "sorry, there is not a password in your dictionary" << endl;
	}


	//system("pause");
	return 0;
}

/*
Now, run the protocol as a MITM attacker: pose as the server and use arbitrary values for b, B, u, and salt.

Crack the password from A's HMAC-SHA256(K, salt).

bruteforce to attach password!!!

*/