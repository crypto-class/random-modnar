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

#define SECURELEN 1024
//FILE *fo = freopen("result.out", "w", stdout);
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
};

Integer p, g, KS, KC, k, A, B, V, u;

AutoSeededRandomPool rnd;
/*
过程分析：
1、首先协商好 N, g = 2, k = 3, email and password
2、server 随机生成salt和密文进行连接后hash = x
利用hash值生成可传递的 v = g**x % N， 保存下来v用来以后对比
3、client 随机生成a, DH协议生成 A = g ** a %N
4、server 把salt给client， 顺便生成B = kv + g**b % N
5、对A|B进行hash
6、client得到salt后跟自己输入的password串起来后hash = x
计算 S = (B - k * g**x)**(a + u * x) % N
生成 K = SHA256(S)
发送 HMAC-SHA256(K, salt)
7、server计算 S = (A * v**u) ** b % N
生成 K = SHA256(S)
生成HMAC-SHA256(K, salt) 和 client的进行比较返回是否相同

server的公式 S = (A * v**u) ** b % N = ( (g**a) * (g**x)**u ) ** b % N = g**(a+ux)**b % N
client的公式 S = (B - k * g**x)**(a + u * x) % N = (k*(g**x) + g**b - k* g**x )**(a + u**x)) % N
= (g**b)**(a + u*x) % N = g**b**(a+ux) % N

代码看函数名很清晰的~\(≧▽≦)/~啦啦啦

*/
void DHAgreementSC(int len, User &S)
{
	S.email = "y0uqh@y0uqh.com";
	S.passwd = "Cryptopals";
	g = 2;
	k = 3;
	DH dh;
	dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, len);
	p = dh.GetGroupParameters().GetModulus();
	cout << "email = " << S.email << endl;
	cout << "passwd = " << S.passwd << endl;
	cout << " p = " << p << endl;
	cout << " g = " << g << endl;
	cout << " k = " << k << endl;
}

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

void step4S2C(User &C, User &S)
{

	C.salt = S.salt;

	SecByteBlock key(0x00, SECURELEN / BYTELEN);
	rnd.GenerateBlock(key, key.size());
	cout << "produce Server'b = " << key << endl;
	int len = SECURELEN / BYTELEN;

	S.k = Byte2Inter(key, len);

	B = k*V + powMod(g, S.k, p);
	cout << "B = " << B << endl;
}

void step5SC()
{
	string ab = Inter2Str(A) + Inter2Str(B);
	SHA256 hash;
	string uH;
	StringSource s(ab, true, new HashFilter(hash, new HexEncoder(new StringSink(uH))));
	u = Str2Inter(uH, 16);
}

void step6C(User &S, User &C)
{
	string ab = Inter2Str(C.salt) + C.passwd;
	SHA256 hash;
	string xH;
	StringSource s(ab, true, new HashFilter(hash, new HexEncoder(new StringSink(xH))));
	Integer x = Str2Inter(xH, 16);
	cout << "xH = " << xH << endl;
	cout << "x = " << x << endl;
	Integer tmp1 = (B - k*powMod(g, x, p));
	C.S = powMod(tmp1, C.k + u*x, p);
	string ss = Inter2Str(C.S);
	cout << " CS = " << ss << endl;
	StringSource s2(ss, true, new HashFilter(hash, new HexEncoder(new StringSink(C.K))));
	cout << "CK = " << C.K << endl;
}

void step7S(User &S)
{
	Integer tmp1 = A*powMod(V, u, p);
	S.S = powMod(tmp1, S.k, p);
	SHA256 hash;
	string ss = Inter2Str(S.S);
	cout << " CS = " << ss << endl;
	StringSource s2(ss, true, new HashFilter(hash, new HexEncoder(new StringSink(S.K))));
	cout << "CK = " << S.K << endl;
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
	}
	else {
		cout << "FAILED" << endl;
	}

	return 0;
}

User client, server;
int main()
{
	srand(time(0));

	client.email = "y0uqh@y0uqh.com";
	//client.passwd = "ILOVECHINA";
	client.passwd = "Cryptopals";

	cout << "----------C & S Agree on N = [NIST Prime]---------" << endl;
	cout << "----------g = 2, k = 3, I(email), P(password)----------" << endl;
	DHAgreementSC(SECURELEN, server);

	cout << "---------Server    Initialization----------" << endl;
	ServerInit(server);

	cout << "---------C->S Send I, A = g**a % N(a la Diffie Hellman)----------" << endl;
	step3C2S(client);

	cout << "---------S->C Send salt, B = kv + g**b % N----------" << endl;
	step4S2C(client, server);

	cout << "---------S, C Compute string uH = SHA256(A | B), u = integer of uH----------" << endl;
	step5SC();

	cout << "---------C-------------" << endl;
	cout << "Generate string xH = SHA256(salt | password)" << endl;
	cout << "Convert xH to integer x somehow(put 0x on hexdigest)" << endl;
	cout << "Generate S = (B - k * g**x)**(a + u * x) % N" << endl;
	cout << "Generate K = SHA256(S)----------" << endl;
	step6C(server, client);

	cout << "---------S------------" << endl;
	cout << "Generate S = (A * v**u) ** b % N" << endl;
	cout << "Generate K = SHA256(S)----------" << endl;
	step7S(server);

	cout << "---------Let's validate----------" << endl;
	Validates(server, client);

	system("pause");
	return 0;
}

/*


RESULT

----------C & S Agree on N = [NIST Prime]---------
----------g = 2, k = 3, I(email), P(password)----------
email = y0uqh@y0uqh.com
passwd = Cryptopals
p = 132820415773862160216700520962414582683992259081190042797182215598608611777828382771476044741377150403017906406681242539517082066570711661157787922073990683863443523035419408370806724043338225034032464467045187967141252484125702514773536039956621981429881628980367700561772528425154059703941199148349425002043.
g = 2.
k = 3.
---------Server    Initialization----------
salt = 28438491653133678642289915067024420243967077195912857716189442094456391758049363544272493820293192824851797451829390551334817114148767676663549483004962608325159743139298011520491375731876820068458727653542379550143073487119718633863276798631679113244683938361333040462938285301710879965078571644685034348364.
salt|passwd = 28438491653133678642289915067024420243967077195912857716189442094456391758049363544272493820293192824851797451829390551334817114148767676663549483004962608325159743139298011520491375731876820068458727653542379550143073487119718633863276798631679113244683938361333040462938285301710879965078571644685034348364Cryptopals
xH = 79CB72CA526068C3315E7F938A5EE787FE8B1DFFEEB454F589B1C978A34400A8
x = 55089316886393274273778006097514581123922061515221609863042436102959030075560.
v = 14868586228666821290297730236570984122227676092841712929938555953616467519769760178314517343474987445831459973009029201926745562283232529402256561035893605277203763833091393486701425817936312092118876571722620013815294082699792019752961854324317638040342506790613464454522116578015198291322341328142237375155.
---------C->S Send I, A = g**a % N(a la Diffie Hellman)----------
C send I = y0uqh@y0uqh.com
produce Client'a = 0055A5C8
A = 82604442164003682215916259823578032964051570416574258616040748802374765215586690815360150628222981869523240555146446820242559254753119034607306873320657013356556965266999886091081068519975223533823344332004255529305542483426416296878960409042910715928396195445944143771830290351924027077315654783029089836145.
---------S->C Send salt, B = kv + g**b % N----------
produce Server'b = 0055A5C8
B = 129322912713601338248199005424717586348088182533105549997520895851922972351083374212566093594481372534436422313020690627511540729662708057877609068402753784596160385982999696389244475221123934709977154430562999818341111530868263330434626629492815594090515112505106691432859088736043183978339834513489370041887.
---------S, C Compute string uH = SHA256(A | B), u = integer of uH----------
---------C-------------
Generate string xH = SHA256(salt | password)
Convert xH to integer x somehow(put 0x on hexdigest)
Generate S = (B - k * g**x)**(a + u * x) % N
Generate K = SHA256(S)----------
xH = 79CB72CA526068C3315E7F938A5EE787FE8B1DFFEEB454F589B1C978A34400A8
x = 55089316886393274273778006097514581123922061515221609863042436102959030075560.
CS = 76538800158668365608933602574641906196046871898029757559665439514110744772087639725764788227943268817304193285771997326964335224045430200971702810944518170599135308308413690246735464846262669985543848747711983644717107688149799750865386409061724981825645827253275277282715854671905493065972208126793503289611
CK = AA00931A9CA17C2F35D7138BF84AE0B6A950FFE59BA6B8FABEE98D8FC0EFB75F
---------S------------
Generate S = (A * v**u) ** b % N
Generate K = SHA256(S)----------
CS = 76538800158668365608933602574641906196046871898029757559665439514110744772087639725764788227943268817304193285771997326964335224045430200971702810944518170599135308308413690246735464846262669985543848747711983644717107688149799750865386409061724981825645827253275277282715854671905493065972208126793503289611
CK = AA00931A9CA17C2F35D7138BF84AE0B6A950FFE59BA6B8FABEE98D8FC0EFB75F
---------Let's validate----------
keyC = 0056B930
keyS = 0056B9E0
HC = êD²påáoš÷é‚DZ
+­×yUmVÊ3þÁZ`)&9
HS = êD²påáoš÷é‚DZ
+­×yUmVÊ3þÁZ`)&9
Success!!


*/