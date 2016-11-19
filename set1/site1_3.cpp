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
	string cipher = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	cipher = hex2str(cipher);

	int len = cipher.length();
	string pa = "the";
	/*
	遍历可能字母看看异或后是否有频率词the出现
	*/
	for (int i = 0;i < 255; ++i)
	{
		string  key;
		key.clear();
		for (int j = 0;j < len;++j)key += (char)i;
		string plain = AXORB(key, cipher);
		cout << plain <<"      key = "<<(char)i << endl;
		int flag = kmp_count(pa, pa.length(), plain, len);
		if (flag)
		{
			cout << plain << endl;
			cout << (char)i << endl;
			STOP;
		}
	}

	system("pause");
	return 0;
}