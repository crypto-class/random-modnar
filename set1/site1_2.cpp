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
/*
异或即可。。。
*/
int main()
{
	string plain = "1c0111001f010100061a024b53535009181c";
	plain = hex2str(plain);

	string key = "686974207468652062756c6c277320657965";
	key = hex2str(key);

	string cipher = AXORB(plain, key);
	cipher = str2hex(cipher);
	cout << cipher << endl;

	system("pause");
	return 0;
}