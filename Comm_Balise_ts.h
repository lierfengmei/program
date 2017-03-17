
//FILE FOR COMMUCATION HEADER FILE 
//date:2002.12.18

#include "afxmt.h"

#define MAXWRITEBUF 500 //9-28-99f add
#define MAXREADBUF 2048
#define MAXRECIBUF 2000 

#define ANALYSE_USERTELE_BALISE_A4     1
#define ANALYSE_USERTELE_BALISE_A5     2
#define ANALYSE_USERTELE_LEU_C1        3
#define ANALYSE_USERTELE_LEU_C2        4
#define ANALYSE_USERTELE_LEU_C3        5
#define ANALYSE_USERTELE_LEU_C4        6

#define ORDER_A4_READ_TELE            0x01

#define ORDER_A5_WRITE_TELE           0x10
#define ORDER_A5_WRITE_MODE           0x11
#define ORDER_A5_READ_TELE            0x12
#define ORDER_A5_READ_MANU            0x13

#define ORDER_LEU_SEND_TSD            0x20
#define ORDER_LEU_STOP_TSD            0x21

#define ORDER_LEU_READ                0x22
#define ORDER_LEU_READ_N              0x23
#define ORDER_LEU_ERASE               0x24
#define ORDER_LEU_WRITE_DEFAULTTELE   0x25
#define ORDER_LEU_WRITE_FT            0x26

#define ORDER_LEU_TSE            0x00
#define ORDER_LEU_TPC            0x01
#define ORDER_LEU_TE1            0x02
#define ORDER_LEU_TE2            0x03
#define ORDER_LEU_TE3            0x04
#define ORDER_LEU_TE4            0x05
#define ORDER_LEU_TE5            0x06
#define ORDER_LEU_TE6            0x07
#define ORDER_LEU_THM            0x08
#define ORDER_LEU_THE            0x09
#define ORDER_LEU_SC             0x0a
#define ORDER_LEU_FT             0x0b


#define ORDER_START_BTM               0x30
#define ORDER_STOP_BTM                0x31

#define ORDER_C_SEND_TELE             0x40
#define ORDER_LEU_C_READ_TELE         0x50

#define READLEN_A4_READ_TELE             9+234
#define READLEN_A5_WRITE_TELE            9+1
#define READLEN_A5_WRITE_MODE		     9+1
#define READLEN_A5_READ_TELE             9+513
#define READLEN_A5_READ_MANU             9+513
#define READLEN_LEU_SEND_TSD             9+1
#define READLEN_LEU_STOP_TSD             9+1

#define READLEN_LEU_READ_SC              9+231
#define READLEN_LEU_READ_THM             9+231
#define READLEN_LEU_READ_THE             9+231
#define READLEN_LEU_WRITE_DEFAULTTELE    9+7
#define READLEN_LEU_READ_TPC             9+231
#define READLEN_LEU_READ_TE1             9+231
#define READLEN_LEU_READ_TSE             9+231

#define READLEN_LEU_ERASE                9+1
#define READLEN_LEU_WRITE_FT             9+1


#define READLEN_START_BTM                9+129
#define READLEN_STOP_BTM                 9+1
#define READLEN_C_SEND_TELE              9+130
#define READLEN_LEU_C_READ_TELE          9+132


extern HANDLE hCom;
extern DWORD ReadBufLen;
extern byte Read_Tele[100000];
extern DWORD Read_Tele_Len;
extern BOOL Read_Tele_End_Flag;

extern CStdioFile logfile;

BOOL InitComm();
BOOL CloseComm(HANDLE &hCom);
BOOL SendCommand(int len,byte send[]);
BOOL ReceiveData(int readlen);
BOOL ArrangeData(byte order);
void ClearData();
int strtodata(unsigned char *str, unsigned char *data, int len, int flag);
int strtodata(CString source_str,  DWORD *data, int len,int flag);

BOOL InitTelegram(CString ff,byte send[]);
void left_shift_bits(byte data[],int len_data,int dup_mul,int len_shift_bits,int* total_len);
unsigned int cal_crc(unsigned char *ptr,unsigned int len);
ULONG cal_crc32(byte* data,ULONG len);

BOOL Open_LogFile();
BOOL Close_LogFile();

BOOL Analyse_Usertele_Info(int h_flag,byte usertele[],int len_usertele,byte tele[],int len_tele);
DWORD Get_Data_From_Byte(byte usertele[],int len_array,int get_bit_len);
void Analyse_Usertele_Header(CStdioFile* file,byte usertele[],int len_array);
void Print_Log(CStdioFile* ptr_file,CString ss);
CString UserTelegram_Description(int list_serial,int pos);

void Make_Xor(unsigned int *scr_buff,unsigned int *var_buff1,int num);
void Analyse_Usertele_Header(CListBox* list,byte usertele[],int len_array);

int Analyse_Tele_Data_Get_TeleAndUsertele(byte* scr_data,byte* des_tele,byte* user_tele,UINT* long_or_short_flag);

BOOL Start_ResetUsbDev(BOOL start_stop);
BOOL Get_ComDev();

void Copy_UserTele(byte des[],byte source[],int len_array);