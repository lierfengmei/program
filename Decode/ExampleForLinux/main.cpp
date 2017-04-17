#include "Decode.h"
#include <iostream>
#include <string.h>
#include <assert.h>


using namespace std;

bool isLegalChar(const char ch);
int toXNum(const char ch);
void PrintHex(const byte* data,const int length);

int main()
{
//解析报文
//  Analyse_Usertele_Info(ANALYSE_USERTELE_BALISE_A4,user_tele,len_usertele,tele,len_tele);
  
  //解码报文，将1024位报文解码成用户报文.
  
//分析数据得到报文和用户报文
//原始数据在scr_data[]中
//scr_data[0~127]

//最后报文存到des_tele[]数组中，
//des_tele[0]:报文数据长度
//des_tele[1] ~ [128]: 报文数据

//user_p_tele：用户报文数组
//long_or_short:输出的长短用户标识 0 长报文，1 短报文

//返回值：0 正常；非0 ：错误


int i=0,j=0;

char tele_ori[]=" 98 3B E6 32 7B 23 75 ED 96 19 46 9A 3D 0E F2 A6 3D 2D 7C 37 88 CD F7 77  C3 DD EB D1 82 65 A7 F5 22 D4 BB D4 75 3A DC 4D 34 2F 5E 63 91 C7 B3 92 96 BA 7D 7B EC DB 14 2F 24 5C 87 F8 EA 7D 3E 0D 2B F6 F2 F1 AB 99 5B 7E DF 45 3C 41 3C AE 77 C2 3B E9 7C 47 5A 7D F2 C5 5D 49 AA F3 30 6774 FC4A C7 59 F2 D9 ED 5E F9 13 E2 E6 17 85 92 CD 7B 0F D9 10 1B 51 67 29 2F  B5 DF 89 B8 AC DE DA 7C";

byte src_data[130];
byte temp=0;
int src_data_length = 128;

// get src_data,16进制
i=0;
for(j=0;j<128;j++)
{
  while(!isLegalChar(tele_ori[i])) i++;
  temp+=toXNum(tele_ori[i])*16;
  cout<<tele_ori[i];
  i++;
  
  while(!isLegalChar(tele_ori[i])) i++;
  temp+=toXNum(tele_ori[i]);
  cout<<tele_ori[i];
  i++;
  
  src_data[j]=temp;
  temp = 0;
}

byte des_tele[300];//最后的报文
byte user_tele[300];//用户报文数组
UINT long_or_short;//输出长报文还是短报文

UINT* pLongShorFlag = &long_or_short;

int result = Analyse_Tele_Data_Get_TeleAndUsertele(src_data,des_tele,user_tele,pLongShorFlag);

if(result==0) 
{
cout<<"解析结果正常"<<endl;

if(long_or_short==0) cout<<"这是长报文"<<endl;
else if(long_or_short==1) cout<<"这是短报文"<<endl;
else cout<<"既不是长报文，也不是短报文，该解码出现了错误"<<endl;

cout<<"报文数据长度为　"<<int(des_tele[0])<<endl;
cout<<"报文数据为：　"<<endl;
PrintHex(des_tele+1,int(des_tele[0]));

cout<<endl;
cout<<"用户报文数组为: "<<endl;
PrintHex(user_tele,104);
}
else
{
    cout<<"result is "<<result<<endl;  
    cout<<"解析结果错误"<<endl;
}

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
