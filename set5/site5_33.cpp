#include <string>
#include "fun.h"

using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)

void DHIntTest()
{

	Integer p("0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024"
		"e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd"
		"3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec"
		"6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f"
		"24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361"
		"c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552"
		"bb9ed529077096966d670c354e4abc9804f1746c08ca237327fff"
		"fffffffffffff");
	Integer g("0x2");
	Integer a("0x6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f");
	a = a%p;
	Integer b("0xbb9ed529077096966d670c354e4abc9804f1746c08ca237327fff");
	b = b%p;
	cout << p << endl;
	cout << g << endl;
	cout << "a = " << a << endl;
	Integer A = powMod(g, a, p);
	cout << "A = " << A << endl;


	cout << "b = " << b << endl;
	Integer B = powMod(g, b, p);
	cout << "B = " << B << endl;

	cout << "CA = " << powMod(B, a, p) << endl;
	cout << "CB = " << powMod(A, b, p) << endl;

}
int main()
{
	DHIntTest();
	STOP;
	return 0;
}