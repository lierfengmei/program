#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//产生１０个１００以内的随机数并输出。

int main()
{
  char str[100];

  int offset = 0;
  int i=0;
  srand(time(0)); //随机种子
  for(i=0;i<10;i++)
  {
    offset += sprintf(str+offset,"%d, ",rand()%100);
  }
//  str[offset-1]='\n';
  puts(str);
  return 0;
}
