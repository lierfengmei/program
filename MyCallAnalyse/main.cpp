#include <iostream>
//#include "Comm_Balise_ts.h"
#include "test.h"

using namespace std;


int main()
{
  /*
  //长报文
  byte user_tele_ori[]="90247E634402FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FFFFFFFFFF";

int len_usertele = 104;
  byte tele_ori[]="0006752CF5B207A329E36FC7D651B9569E8702D752410FB053 5C 8B 02 CF 82 38 AD 51 AC B9 4E 2B F8 66 9E E9 68 2C 67 F2 F6 57 11 40 D4 34 51 E3 B0 74 D1 84 2F 81 DA FB C6 7B F5 8A 19 3F 57 FB 1A 19 2B 60 40 67 96 16 8D 41 60 AC 2F E6 7D D3 FB F3 76 BB 56 94 44 FA 29 4B 8C 2E 71 79 C3 89 96 58 79 2D EA EC 7D 79 BA 5C 5B EC 78 10 16 1C E6 2D F0 A5 F0 45 4A CC 3E D2 1C";
int len_tele = 128; 

  //删除user_tele_ori的空格
  int i=0,j=0;
  byte user_tele[120];
  while(user_tele_ori[i!='\0'])
  {
    if((user_tele_ori[i]>='0'&&user_tele_ori[i]<='9')||(user_tele_ori[i]>='A'&&user_tele_ori[i]<='F'))
      user_tele[j++] = user_tele_ori[i++];
    else i++;
  } 
  user_tele[j]='\0';

  i = 0;j=0;
  byte tele[150];
  while(tele_ori[i]!='\0')
  {
    if(tele_ori[i]>='0'&&tele_ori<='9')||(tele_ori[i]>='A'&&tele_ori<='F') 
    {
      tele[j++]=tele_ori[i++];
    }
    else i++;
  }
  tele[j]='\0';


  Analyse_Usertele_Info(ANALYSE_USERTELE_BALISE_A4,usertele,len_usertele,tele,len_tele);
 */
 
  cout<<"intput 4 and 5,sum is :"<<Add(4,5)<<endl;
 
  return 0;
}

//A4读出的报文和A5读出的报文有区别吗？
//BOOL Analyse_Usertele_Info(int read_interface,byte usertele[],int len_usertele,byte tele[],int len_tele)
