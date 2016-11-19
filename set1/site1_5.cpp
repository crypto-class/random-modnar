#include <string>
#include "fun.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/des.h"
#include "cryptopp/base64.h"
#include "cryptopp/md5.h"
#include "cryptopp/hex.h"
#include "cryptopp/files.h"
using namespace CryptoPP;
using namespace std;
#define STOP system("pause")
int main()
{
	string plain1 = "Burning 'em, if you ain't quick and nimble";
	string plain2 = "I go crazy when I hear a cymbal";
	string plain3 = plain1 + '\n' + plain2;
	string key = "ICE";

	string cipher1, cipher2, cipher3;
	cipher1 = AXORWord(plain1, key);
	cipher1 = str2hex(cipher1);
	cout << cipher1 << endl;

	cipher2 = AXORWord(plain2, key);
	cipher2 = str2hex(cipher2);
	cout << cipher2 << endl;

	cipher3 = AXORWord(plain3, key);
	cipher3 = str2hex(cipher3);
	cout << cipher3 << endl;


	system("pause");
	return 0;
}