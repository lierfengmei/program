#pragma once

#include <iostream>
#include <stdlib.h>

typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef bool BOOL;
typedef unsigned char byte;
typedef unsigned long ULONG;
typedef unsigned char BYTE;

int Analyse_Tele_Data_Get_TeleAndUsertele(byte* scr_data,byte* des_tele,byte* user_tele,UINT* long_or_short_flag);
void Make_Xor(unsigned int *scr_buff,unsigned int *var_buff1,int num);
//void left_shift_bit(byte data[],int len_data,int len_shift_bit);
void left_shift_bits(byte data[],int len_data,int dup_mul,int len_shift_bits,int* total_len);
void left_shift_onebit(unsigned int *temp,int num);
void left_shift_bit(byte data[],int len_data,int len_shift_bit);
