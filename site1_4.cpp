#include <iostream>
#include "fun.h"
using namespace std;
#include <cryptopp/aes.h>
using namespace CryptoPP;

FILE *fi = freopen("site1_4.in","r",stdin);
FILE *fo = freopen("site1_4.txt","w",stdout);
vector<string>cipher;
vector<string>alpha;
void assert(int i)
{
   printf("%d__ok\n",i);
   system("pause");
}

bool check(string s)
{
   int cnt = 0;
   for(int i=0;i<(int)s.length();i++)
   {
       int c = s[i];
       if(!(c>=0x20 && c<=0x7e))
           return 0;
   }
   return 1;
}

int main()
{
   cout<<"hello world"<<endl;
   string s;
   int cnt = 0;

	while(cin>>s)
   {
       cnt++;
       string ss = hex2str(s);
       cipher.pb(ss);
       s.clear();
       ss.clear();
   }
   cout<<cnt<<endl;
//    cout<<cipher[0]<<endl;


   int len = cipher[0].length();
   for(int i=0;i<255;i++)
   {
       string a;a.clear();
       char c = i;
       for(int j=0;j<len;j++)
           a+=c;
       alpha.pb(a);
   }

//    cout<<alpha[0]<<endl;

   string pa = "the";
   int tag = -1;
   for(int i=0;i<255;i++)
   {
//        printf("\n*****\n");
//        cout<<alpha[i]<<endl;
//        puts("*******");
       int flag = 0;
       for(int j=0;j<cnt;j++)
       {
           string ne = AXORB(cipher[j],alpha[i]);
           string ::iterator it, it2;
           for(it = ne.begin();it!=ne.end();)
           {
               it2 = it;
               if((*it2) == '\n')
               {
                   it--;
                   ne.erase(it2);
               }
               else{
                   it++;
               }
           }
           if(check(ne))
           {
               flag =1;
               cout<<ne<<endl;
               string hex = str2hex(ne);
               cout<<"hex = "<<hex<<endl;
           }
//            if(kmp_count(pa,pa.length(),ne,ne.length()));
//            cout<<ne<<endl;
       }
       if(flag)
       {

           tag = i,cout<<i<<"Bingo  = " <<(char)i<<endl;

       }
   }
   cout<<"tag = "<<tag<<endl;
	return 0;
}
