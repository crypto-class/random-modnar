#include <string>
#include "fun.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

using namespace CryptoPP;
using namespace std;
#define STOP system("pause")
char testwait;
#define WAIT testwait = getchar()
#pragma warning (disable:4996)


AutoSeededRandomPool rnd;
/*
中国剩余定理
*/
Integer ChineseRemainderTheory(vector<Integer> Ni, vector<Integer> Ai, int n)
{
	Integer M = 1;
	Integer ans = 0;
	for (int i = 0;i < n;i++)
		M *= Ni[i];
	for (int i = 0;i < n;i++)
	{
		Integer mi, tmp;
		Integer Mi = M / Ni[i];
		exgcd(Mi, Ni[i], mi, tmp);
		ans = (ans + Ai[i] * mi*Mi) % M;
	}
	return ans;
}

bool IsNoPrime[MAXN] = {1,1};
int Prime[MAXN];
int prime_num = 0;
/*
素数筛
*/
void findPrime()
{
	for (int i = 2;i < MAXN;i++)
	{
		if (!IsNoPrime[i])
			Prime[prime_num++] = i;
		for (int j = 0;j < prime_num && i*Prime[j] < MAXN;j++)
		{
			IsNoPrime[i*Prime[j]] = 1;
			if (!(i%Prime[j]))
				break;
		}
	}
}

Integer calInv(Integer a, Integer p)
{
	Integer x, y;
	
	Integer d = exgcd(a, p, x, y);
	if (d != 1)
		return -1;
	x = (x%p + p) % p;
	return x;
}

struct factor
{
	Integer base;
	Integer exponent;
};
/*
质因数分解
*/
vector<factor> findFactor(Integer n)
{
	vector<factor> ans;
	for (int i = 0;i < prime_num;i++)
	{
		factor f = { Prime[i],0 };
		while (n % Prime[i] == 0) {
			n /= Prime[i];
			f.exponent++;
		}
		if (f.exponent == 0)
			continue;
		cout << "(" << f.base << "^" << f.exponent << ")*" << endl;
		if (f.exponent >= 1)
			ans.push_back(f);
		if (n == 1)
			return ans;
	}
	ans.push_back({ n,-1 });
	return ans;
}

Integer pow(Integer a, Integer n)
{
	Integer ans = 1;
	while (n != 0)
	{
		if (n % 2 == 1)
			ans = (ans*a);
		a = (a*a);
		n /= 2;
	}
	return ans;
}
/*
Take one of the small factors j. Call it r. We want to find an element h of order r. To find it, do:

h := rand(1, p)^((p-1)/r) mod p

If h = 1, try again.

*/
Integer findH(Integer p, Integer r)
{

	while (true)
	{

		Integer rd;
		rd.Randomize(rnd, 1, p - 1);
		Integer h = powMod(rd, (p - 1) / r, p);
		if (h != 1)
			return h;
	}
	return -1;
}
/*
Remember how we saw that g^x starts repeating when x > q? h has the same property with r.
This means there are only r possible values of K that Bob could have generated. We can recover K by doing a brute-force search over these values until t = MAC(K, m).
*/
Integer findBi(Integer K, Integer r, Integer h,Integer p)
{
	for (Integer bi = 0;bi < r;bi++)
	{
		if (powMod(h, bi, p) == K)
			return bi;
	}
	return -1;
}
/*
Repeat steps 1 through 4 many times. Eventually you will know:

x = b1 mod r1
x = b2 mod r2
x = b3 mod r3
...

Once (r1*r2*...*rn) > q, you'll have enough information to reassemble Bob's secret key using the Chinese Remainder Theorem.

*/
Integer PHSolve(Integer p, Integer q, Integer g, Integer x)
{
	Integer j = (p - 1) / q;
	cout << "j = " << j << endl;
	vector<factor>fact = findFactor(j);
	vector<Integer>Ni, Ai;
	Integer pending = 1;
	for (int i = 0;i < (int)fact.size();i++)
	{
		Integer r = fact[i].base;
		Integer h = findH(p, r);
		cout << "r = " << r << endl;
		cout << "h = " << h << endl;
		Integer K = powMod(h, x, p);
		Integer bi = findBi(K, r, h, p);
		cout << "b = " << bi << endl;
		pending *= r;
		Ni.push_back(r);
		Ai.push_back(bi);
	}
	cout << "peding > q?" << (pending > q) << endl;
	return ChineseRemainderTheory(Ni, Ai, Ai.size());
}


//loga(b) mod p
//Integer solve(Integer a, Integer b, Integer p)
//{
//	cout << " p = " << p << endl;
//	Integer phi = p - 1;
//	Integer a_inv = calInv(a, p);
//	vector<Integer>Ai, Ni;
//	vector<factor> facts = findFactor(phi);
//	Integer Pending = 1;
//	for (int i = 0;i < (int)facts.size();i++)
//	{
//		Integer q = facts[i].base;
//		Integer c = facts[i].exponent;
//		Integer x = 0;
//		Integer bj = b;
//		for (Integer j = 0;j < c;j++)
//		{
//			//cout << "phi/pow = " << phi << "  /  " << pow(q, j + 1) << endl;
//			Integer d = powMod(bj, phi / pow(q, j + 1), p);
//			//cout << "d = " << d << endl;
//			Integer aj;
//			for (aj = 0; aj < q;aj++)
//			{
//				if (d == powMod(a, aj*(phi / pow(q, 1)), p))
//					break;
//			}
//			x = (x + aj*pow(q, j));
//			bj = bj*powMod(a_inv, aj*pow(q, j), p) % p;
//		}
//		Ai.push_back(x);
//		Integer ni = pow(q, c);
//		Pending *= ni;
//		Ni.push_back(ni);
//		//if (Pending > qq)
//		//	break;
//	}
//	//cout << Pending << " grate? = " << (Pending > qq) << endl;
//	return ChineseRemainderTheory(Ni, Ai, Ai.size());
//}

int main()
{
	findPrime();
	Integer p("7199773997391911030609999317773941274322764333428698921736339643928346453700085358802973900485592910475480089726140708102474957429903531369589969318716771");
	Integer g("4565356397095740655436854503483826832136106141639563487732438195343690437606117828318042418238184896212352329118608100083187535033402010599512641674644143 ");
	Integer q("236234353446506858198510045061214171961");
	Integer bb("6661");

	cout << " p = " << p << endl;
	//cout << "q = " << endl;
	cout << "g = " << g << endl;
	cout << "b = " << bb << endl;
	//Integer p = 353;
	//Integer g = 3;
	//Integer bb = 97;
	//Integer B = powMod(g, bb, p);
	//cout << "B = " << B << endl;
	Integer ans = PHSolve(p, q, g, bb);
	//Integer ans = solve(g, B, p)%p;
	cout << "ans = " << ans << endl;

	//vector<Integer>Ai, Ni;
	//Ai.pb(2);Ai.pb(3);Ai.pb(2);
	//Ni.push_back(3);
	//Ni.push_back(5);
	//Ni.push_back(7);
	//cout << ChineseRemainderTheory(Ni, Ai, Ai.size()) << endl;

	STOP;
	return 0;
}