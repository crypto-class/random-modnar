#include <string>
#include "fun.h"
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/asn.h>

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

#define SECURELEN 32
//FILE *fo = freopen("result.out", "w", stdout);

struct Point
{
	Point() {};
	Integer x, y;
	Point(Integer _x, Integer _y)
	{
		x = _x;
		y = _y;
	}
	bool operator == (const Point &b)const
	{
		return x == b.x && y == b.y;
	}
	void display()
	{
		cout << "x = " << x << " , y = " << y << endl;
	}
};
Point O(0, 1);

Point InvInCurve(Point P, Integer p)
{
	return Point(P.x,(-P.y+p)%p);
}

Integer a, b;	

Point AddInCurve(Point P1, Point P2, Integer p)
{
	if (P1 == O)
		return P2;
	if (P2 == O)
		return P1;
	if (P1 == InvInCurve(P2, p))
		return O;
	//Integer dx = P1.x - P2.x; dx = (dx % p + p) % p;
	//Integer dy = P2.y - P2.y; dy = (dy % p + p) % p;
	//Integer lamda = dy*(calInv(dx, p)) % p;
	//Integer xr = lamda*lamda - P1.x - P2.x; xr = (xr%p + p) % p;
	//Integer yr = lamda*(P1.x - xr) - P1.y; yr = (yr%p + p) % p;
	//Point ans(xr, yr);
	//ans.display();
	//return ans;


	Point p1 = P1;
	Point p2 = P2;
	Integer m;
	if (P1 == P2)
		m = (3 * p1.x * p1.x + a) % p * calInv(2 * p1.y, p) % p;
	else
		m = (p2.y - p1.y) % p*calInv(p2.x - p1.x, p)%p;
	Point p3;
	p3.x = (m*m - p1.x - p2.x) % p;
	p3.y = (m * (p1.x - p3.x) - p1.y) % p;
	//p3.display();
	return p3;
}

Point MulInCurve(Integer n, Point P, Integer p)
{
	Point ans = O;
	for (Integer i = 0;i < n;i++)
		ans = AddInCurve(ans, P,p);
	return ans;
}

int main()
{
	//y^2 = x^3 + a*x + b
	//a = 324;
	//b = 1287;
	//Integer na = 1194, nb = 1759;
	//Point P(920, 303);
	//Integer p = 3851;

	//Point test = MulInCurve(5, P, p);
	a = -95051;
	b = 11279326;
	Integer na = 562;
	Integer nb = 667;
	Integer px("182");
	Integer py("85518893674295321206118380980485522083");
	Point P(px, py);
	Integer p("233970423115425145524320034830162017933");

	Point QA = MulInCurve(na, P, p);
	Point QB = MulInCurve(nb, P, p);
	cout << "QA = " << endl;
	QA.display();
	cout << "QB = " << endl;
	QB.display();
	Point naQB = MulInCurve(na, QB, p);
	Point nbQA = MulInCurve(nb, QA, p);
	cout << "naQB = " << endl;
	naQB.display();
	cout << "nbQA = " << endl;
	nbQA.display();


	STOP;
	return 0;
}