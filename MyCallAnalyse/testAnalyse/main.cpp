#include "Comm_Balise_ts.h"
#include <iostream>
#include <string.h>
#include <assert.h>


using namespace std;

bool isLegalChar(const char ch);
int toXNum(const char ch);
void PrintHex(const byte* data,const int length);
/*int AnalyseOfCpp(byte user_tele_ori[]);

extern "C"
{
  int Analyse(byte user_tele_ori[])
  {
    AnalyseOfCpp(user_tele_ori);
  }
}
*/

int main()
{
//用户报文数据
byte user_tele_ori[]="90 24 7E 63 44 02 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF ";
  
//用户报文长度
int len_usertele = 104;
  
//删除user_tele_ori的空格
  int i=0,j=0;
  byte user_tele[300];
//  while(user_tele_ori[i]!='\0')
 while(j<104)
 {
    if((user_tele_ori[i]>='0'&&user_tele_ori[i]<='9')||(user_tele_ori[i]>='A'&&user_tele_ori[i]<='F'))
    {
     user_tele[j++] = user_tele_ori[i++];
     cout<<user_tele[j-1];
    }
    else i++;
  } 
  user_tele[j]='\0';

  cout<<"对以下用户报文进行解析:"<<endl;
  cout<<user_tele<<endl;
    
  cout<<"解析结果:"<<endl;
  Analyse_Usertele_Info(user_tele,len_usertele);

  return 0;
}

bool isLegalChar(const char ch)
{
  if((ch>='0'&&ch<='9')||(ch>='A'&&ch<='F')||(ch>='a'&&ch<='f'))
    return true;
  else
    return false;
}

int toXNum(const char ch)
{
  assert((ch>='0'&&ch<='9')||(ch>='A'&&ch<='F')||(ch>='a'&&ch<='f'));
  if(ch>='0'&&ch<='9') return(ch-'0');
  if(ch>='A'&&ch<='F') return(ch-'A'+10);
  if(ch>='a'&&ch<='f') return(ch-'a'+10);
}

void PrintHex(const byte* data,const int length)
{
  int i=0;
  int a=0,b=0;
  char des_data[300];
  assert(length*2<300);

  int j=0;
  for(i=0;i<length;i++)
  {
    a = (data[i]>>4)&0x0f;
    b = data[i]&0x0f;
    des_data[j++]=(a>=10)?(a+'A'-10):(a+'0');
    des_data[j++]=(b>=10)?(b+'A'-10):(b+'0'); 
  }
  des_data[j]='\0';

  cout<<des_data<<endl;
}

//A4读出的报文和A5读出的报文有区别吗？
//BOOL Analyse_Usertele_Info(int read_interface,byte usertele[],int len_usertele,byte tele[],int len_tele)
