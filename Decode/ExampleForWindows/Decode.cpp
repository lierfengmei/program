//该文件的功能：解码应答器报文


#include "Decode.h"
#include "Const.h"

typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef bool BOOL;
typedef unsigned char byte;
typedef unsigned long ULONG;


using namespace std;
//分析数据得到报文和用户报文
//原始数据在scr_data[]中
//scr_data[0~127]

//最后报文存到des_tele[]数组中，
//des_tele[0]:报文数据长度
//des_tele[1] ~ [128]: 报文数据

//user_p_tele：用户报文数组
//long_or_short:输出的长短用户标识 0 长报文，1 短报文

//返回值：0 正常；非0 ：错误
int Analyse_Tele_Data_Get_TeleAndUsertele(byte* scr_data,byte* des_tele,byte* user_tele,UINT* long_or_short_flag)
{
	unsigned long l_temp;
	unsigned int i,j,len,tel_start,temp,temp1,zm_or_fm,sb_temp,s_temp;
	unsigned int tel_buff[128],user_p_tele[256];
	unsigned int flx[2]={0xdb,0xe0};	//
	unsigned int fsx[2]={0xb5,0x60};	//
	
	unsigned int glx[10]={0xb8,0x87,0x39,0xa7,0xa2,0xed,0x52,0x3b,0xa1,0x30};//last 4 bit no use
	unsigned int gsx[10]={0x9f,0x79,0x0c,0x2f,0xef,0x7c,0xa4,0xa3,0xc4,0xb0};//last 4 bit no use
	
	int tel_error;
	UINT long_or_short;

	len=128;
	des_tele[0]=(BYTE)(len & 0xff); //???????

	//分析报文
	tel_error=0;
	for(i=0;i<128;i++) tel_buff[i]=scr_data[i];
	tel_buff[127]&=~0x01;

	for(i=0;i<1013;i++)
	{
		if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,flx,2);
		left_shift_onebit(tel_buff,128);
	}

	tel_start=tel_buff[0]*256+tel_buff[1];
	tel_start>>=6;
	
	if(tel_start!=0)
	{			//长报文
		long_or_short=0;
		if(tel_start==0x1ee)
		{		//长码不正确
			tel_error=1;
			return tel_error;
		}
		tel_start=slfx[tel_start];
		for(i=0;i<128;i++) tel_buff[i]=scr_data[i];
		if(tel_start!=0)
		{
			tel_start=1023-tel_start;

			if((tel_buff[127]&0x01)!=0) temp=1;
			else temp=0;
			for(i=0;i<tel_start;i++)
			{
				if((tel_buff[0] & 0x80)!=0) tel_buff[127]|=0x01;
				else tel_buff[127]&=~0x01;
				left_shift_onebit(tel_buff,128);
			}
			if(temp!=0) tel_buff[127]|=0x01;
		}
		for(i=0;i<128;i++) des_tele[i+1]=tel_buff[i];
		tel_buff[127]&=~0x01;

		//检查是否可被g(x)整除
		for(i=0;i<948;i++)
		{
			if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,glx,10);
			left_shift_onebit(tel_buff,128);
		}
		temp=0;
		for(i=0;i<10;i++) temp+=tel_buff[i];
		if(temp!=0)
		{			//长码不正确
			tel_error=2;
			return tel_error;
		}

		//检查11位有效
		for(i=0;i<128;i++) tel_buff[i]=des_tele[i+1];
		tel_buff[127]&=~0x01;

		for(i=0;i<93;i++)
		{
			temp=(i*11)/8;
			if(i!=92) temp=(tel_buff[temp]<<16)+(tel_buff[temp+1]<<8)+tel_buff[temp+2];
			else temp=(tel_buff[temp]<<16)+(tel_buff[temp+1]<<8);
			temp=temp<<((i*11)%8);
			temp>>=13;
			temp&=0x7ff;
			if(bit11_to_10[temp]>=1024)
			{				//长码不正确
				tel_error=3;
				return tel_error;
			}
			if(i<83)
			{
				user_p_tele[i]=bit11_to_10[temp];
			}
			else if(i==83)
			{
				if((temp & 0x0700)==0x0100) zm_or_fm=0;
				else if((temp & 0x0700)==0x0600) zm_or_fm=1;
				else
				{					//长码不正确
					tel_error=4;
					return tel_error;
				}
				sb_temp=temp & 0xff;
				sb_temp<<=4;
			}
			else if(i==84) sb_temp+=(temp>>7);
		}

	}
	else
	{			//短报文
		long_or_short=1;

		for(i=0;i<128;i++) tel_buff[i]=scr_data[i];
		tel_buff[42]&=~0x07;

		for(i=0;i<331;i++)
		{
			if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,fsx,2);
			left_shift_onebit(tel_buff,43);
		}

		tel_start=tel_buff[0]*256+tel_buff[1];
		tel_start>>=6;

		tel_start=ssfx[tel_start];
		if(tel_start>=341)
		{
			tel_error=0x11;
			return tel_error;
		}

		for(i=0;i<128;i++) tel_buff[i]=scr_data[i];
		if(tel_start!=0)
		{
			tel_start=1023-tel_start;

			if((tel_buff[127]&0x01)!=0) temp=1;
			else temp=0;
			for(i=0;i<tel_start;i++)
			{
				if((tel_buff[0] & 0x80)!=0) tel_buff[127]|=0x01;
				else tel_buff[127]&=~0x01;
				left_shift_onebit(tel_buff,128);
			}
			if(temp!=0) tel_buff[127]|=0x01;
		}
		for(i=0;i<128;i++) des_tele[i+1]=tel_buff[i];
		for(i=0;i<341;i++)
		{
			temp=tel_buff[i/8];
			temp=(temp<<(i%8)&0x80);

			j=i+341;
			temp1=tel_buff[j/8];
			temp1=(temp1<<(j%8)&0x80);
			if(temp1!=temp)
			{
				tel_error=0x15;
				return tel_error;
			}
			j=i+682;
			temp1=tel_buff[j/8];
			temp1=(temp1<<(j%8)&0x80);
			if(temp1!=temp)
			{
				tel_error=0x15;
				return tel_error;
			}
		}
		tel_buff[42]&=~0x07;

		//检查是否可被g(x)整除
		for(i=0;i<266;i++)
		{
			if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,gsx,10);
			left_shift_onebit(tel_buff,43);
		}
		temp=0;
		for(i=0;i<10;i++) temp+=tel_buff[i];
		if(temp!=0)
		{			//短码不正确
			tel_error=0x12;
			return tel_error;
		}

		//检查11位有效
		for(i=0;i<128;i++) tel_buff[i]=des_tele[i+1];
		tel_buff[42]&=~0x07;

		for(i=0;i<31;i++)
		{
			temp=(i*11)/8;
			if(i!=30) temp=(tel_buff[temp]<<16)+(tel_buff[temp+1]<<8)+tel_buff[temp+2];
			else temp=(tel_buff[temp]<<16)+(tel_buff[temp+1]<<8);
			temp=temp<<((i*11)%8);
			temp>>=13;
			temp&=0x7ff;
			if(bit11_to_10[temp]>=1024)
			{				//短码不正确
				tel_error=0x13;
				return tel_error;
			}
			if(i<21)
			{
				user_p_tele[i]=bit11_to_10[temp];
			}
			else if(i==21)
			{
				if((temp & 0x0700)==0x0100) zm_or_fm=0;
				else if((temp & 0x0700)==0x0600) zm_or_fm=1;
				else
				{					//短码不正确
					tel_error=0x14;
					return tel_error;
				}
				sb_temp=temp & 0xff;
				sb_temp<<=4;
			}
			else if(i==22) sb_temp+=(temp>>7);
		}
	}

	if(zm_or_fm!=0)
	{
		sb_temp^=0xfff;
		for(i=0;i<83-long_or_short*62;i++) user_p_tele[i]^=0x3ff;
	}

	//解扰
	l_temp=2801775573;
	l_temp*=sb_temp;
	l_temp&=0xffffffff;
	s_temp=l_temp;


	for(i=0;i<83-long_or_short*62;i++)
	{
		temp=0;
		temp1=user_p_tele[i];
		for(j=0;j<10;j++)
		{
			temp<<=1;
			if((s_temp & 0x80000000)!=0)
			{
				if((temp1 & 0x0200)==0) temp|=0x01;
			}
			else
			{
				if((temp1 & 0x0200)!=0) temp|=0x01;
			}
			s_temp<<=1;
			if((temp1 & 0x0200)!=0)
			{
				s_temp^=0xea000001;
			}
			s_temp&=0xffffffff;
			temp1<<=1;
		}
		user_p_tele[i]=temp;
	}
	//恢复第一个10位
	temp=0;
	for(i=1;i<83-long_or_short*62;i++) temp+=user_p_tele[i];
	temp=user_p_tele[0]-temp;
	user_p_tele[0]=temp&0x3ff;

	for(i=0;i<83;i++)
	{
		user_p_tele[103-i]=user_p_tele[82-i];
	}
	for(i=0;i<104;i++) user_p_tele[i]&=0x3ff;


	for(i=0;i<104;i++)
	{
		temp=i*8;
		j=temp/10;
		temp=temp%10;
		if(j==82) j=user_p_tele[j+21]<<10;
		else j=(user_p_tele[j+21]<<10)+user_p_tele[j+21+1];
		j>>=(12-temp);
		user_p_tele[i]=(j&0xff);
	}
	if(long_or_short==0) user_p_tele[103]&=0xfc;
	else
	{
		user_p_tele[26]&=0xc0;
		for(i=27;i<104;i++) user_p_tele[i]=0;
	}

	*(long_or_short_flag)=long_or_short;
	for(i=0;i<104;i++) user_tele[i]=user_p_tele[i] & 0xff;
	return 0;
}

//数据向左移位
//data[]:数据缓冲区
//len_data:要移动的数据个数
//len_shift_bit:每个字节移动的位数;0~8
void left_shift_bit(byte data[],int len_data,int len_shift_bit)
{
	int i;
	byte temp;

	if(len_shift_bit<0) len_shift_bit=0;
	else if(len_shift_bit>8) len_shift_bit=8;

	for(i=0;i<len_data;i++)
	{
		temp=data[i]<<len_shift_bit;

		temp&=0xff;
		data[i]=temp;
	}
}

//数据向左移位，并复制数组
//data[]:数据源数组
//len_data:数据源数组长度
//dup_mul:目标数组是源数组的倍数，即复制 dup_mul-1倍数据。
//len_shift_bits:每个字节移动的位数;0~8
//total_len:返回数组有效数据的总长度。
void left_shift_bits(byte data[],int len_data,int dup_mul,int len_shift_bits,int* total_len)
{
	int j,k;
	byte temp,temp1;
	int del_byte=0;
	byte last_data;

	if(len_shift_bits<0) len_shift_bits=0;
	else if(len_shift_bits>8) len_shift_bits=8;

	//保存源数组最后一个数。
	last_data=data[len_data-1];

	for(k=1;k<dup_mul;k++)
	{
		//复制
		for(j=0;j<len_data;j++)
		{
			data[len_data*k+j-del_byte]=data[j];
		}
		data[len_data*(k+1)-1-del_byte]=last_data;
	
		//处理:第一个数有多少位移到上次最后一个数据
		temp1=data[len_data*k-del_byte];
		temp1>>=(8-(len_shift_bits*k)%8);
		
		//上次最后一个数据留下的位
		temp=0xff;
		temp<<=(len_shift_bits*k)%8;//移出
		data[len_data*k-del_byte-1] &=temp;

		//得到上次最后一个数据
		data[len_data*k-del_byte-1] |=temp1;

		//后边所有数据移位
		left_shift_bit(&data[len_data*k-del_byte],len_data,(len_shift_bits*k)%8);

		del_byte=(k*len_shift_bits)/8;
	}

	del_byte=(dup_mul*len_shift_bits)/8;
	(*total_len)=len_data*dup_mul-del_byte;
}


//对数组每字节进行异或
void Make_Xor(unsigned int *scr_buff,unsigned int *var_buff1,int num)
{
	int i;
	for(i=0;i<num;i++) scr_buff[i]^=var_buff1[i];
}

//对num个数向左移一位
void left_shift_onebit(unsigned int *temp,int num)
{
	int i;
	unsigned int temp1;

	for(i=0;i<num;i++)
	{
		temp1=temp[i]<<1;
		if(i!=num-1) temp1+=(temp[i+1]>>7);
		temp1&=0xff;
		temp[i]=temp1;
	}
}


