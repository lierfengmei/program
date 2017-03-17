// BmpStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "BmpStatic.h"


// CBmpStatic

IMPLEMENT_DYNAMIC(CBmpStatic, CStatic)
CBmpStatic::CBmpStatic()
{
}

CBmpStatic::~CBmpStatic()
{
}


BEGIN_MESSAGE_MAP(CBmpStatic, CStatic)
END_MESSAGE_MAP()



// CBmpStatic 消息处理程序

void CBmpStatic::SetResultBmp(int color)
{
	switch(color)
	{
	case 0:
		SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_BACK)));
		break;
	case 1:
		SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_FUNCTIONAL)));
		break;
	case 2:
		SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_SILENT)));
		break;
	}

}


