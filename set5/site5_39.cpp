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

#define SECURELEN 256
//FILE *fo = freopen("result.out", "w", stdout);




Integer p, q, e, d, et, n, m, tmp, c, plain;
/*
RSA
生成俩素数 p,q
令N = p*q
phi(N) = (p-1)(q-1)
public key = e, gcd(e,phi) = 1
private key = d, e*d = 1 mod N

cipher = plain^e
plain = cipher^d
*/
int main()
{
	srand(time(0));

	AutoSeededRandomPool rng;

	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, SECURELEN);
	p = params.GetPrime1();
	q = params.GetPrime2();
	cout << "p = " << p << endl;
	cout << "q = " << q << endl;

	n = p*q;
	et = (p - 1)*(q - 1);
	e = 3;
	exgcd(e, et, d, tmp);
	d = (d%et + et) % et;


	cout << "public key e = " << e << endl;
	cout << "private key d = " << d << endl;
	m = 42;
	cout << "message = " << m << endl;
	c = powMod(m, e, n);
	cout << "cipher = " << c << endl;
	plain = powMod(c, d, n);
	cout << "plain = " << plain << endl;


	system("pause");
	return 0;
}
