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

string Md5(string strData)
{
	string digest;
	Weak1::MD5 md5;
	StringSource(strData, true, new HashFilter(md5, new HexEncoder(new StringSink(digest))));
	return digest;
}

string Base64Enc(string strSrc)
{
	string strOut;
	CryptoPP::StringSink* sink = new CryptoPP::StringSink(strOut);
	Base64Encoder *base64Enc = new Base64Encoder(sink);
	int iLen = strSrc.size();
	StringSource source(strSrc, true, base64Enc);

	int iPos = -1;
	iPos = strOut.find("\n", 0);
	while (iPos >= 0)
	{
		strOut = strOut.erase(iPos, 1);
		iPos = strOut.find("\n", 0);
	}

	return strOut;
}
string Base64Dec(string strSrc)
{
	string strOut;
	StringSink *sink = new StringSink(strOut);
	Base64Decoder *baseDec = new Base64Decoder(sink);
	StringSource dst(strSrc, true, baseDec);
	return strOut;
}
int main()
{
	string cipher, hexstr="49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
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
