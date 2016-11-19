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
16进制转字符串
字符串base64解码
*/
int main()
{
	string cipher, hexstr = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	string plain = hex2str(hexstr);
	cout << plain << endl;
	//	cin >> plain;
	cipher = Base64Enc(plain);
	cout << cipher << endl;
	string attach = Base64Dec(cipher);
	cout << attach << endl;
	system("pause");
	return 0;
}
