// program_baliseView.cpp : Cprogram_baliseView 类的实现
//

#include "stdafx.h"
#include "program_balise.h"
#include "A4ReadDlg.h"
#include "A5_Write.h"
#include "S_Send.h"
#include "BtmDlg.h"
#include "program_baliseDoc.h"
#include "program_baliseView.h"
#include "Comm_Balise_ts.h"
#include "program_baliseview.h"
#include "WriteOperationMode.h"
#include "ReadManufacture.h"
#include "BaliseCMeas.h"
#include "LeuCRead.h"
#include "SRead_SC.h"
#include "LeuWriteDefaultTele.h"
#include ".\program_baliseview.h"
#include "SErase_Msg.h"
#include "SWrite_FT.h"
#include "LeuWriteDefaultTele_Bx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cprogram_baliseView

IMPLEMENT_DYNCREATE(Cprogram_baliseView, CView)

BEGIN_MESSAGE_MAP(Cprogram_baliseView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_A4READ, OnA4read)
	ON_COMMAND(ID_A5WRITE, OnA5write)
//	ON_WM_CLOSE()
	ON_COMMAND(ID_WRITE_OPERATIONMODE, OnWriteOperationmode)
	ON_COMMAND(ID_PRODUCE_INFO, OnProduceInfo)
	ON_COMMAND(ID_LEU_S_SEND, OnLeuSSend)
	ON_COMMAND(ID_BTM, OnBtm)
	ON_COMMAND(ID_LEU_READ_C, OnLeuReadC)
	ON_COMMAND(ID_BALISE_C, OnBaliseC)
	ON_COMMAND(ID_LEU_READ_SC, OnLeuReadSc)
	ON_COMMAND(ID_LEU_WRITETELE, OnLeuWritetele)
	ON_COMMAND(ID_LEU_ERASE_BTM, OnLeuEraseBtm)
	ON_COMMAND(ID_LEU_WRITE_FT, OnLeuWriteFt)
	ON_COMMAND(ID_LEU_WRITETELE_BX, OnLeuWriteteleBx)
END_MESSAGE_MAP()

// Cprogram_baliseView 构造/销毁

Cprogram_baliseView::Cprogram_baliseView()
{
	// TODO: 在此处添加构造代码

}

Cprogram_baliseView::~Cprogram_baliseView()
{
}

BOOL Cprogram_baliseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// Cprogram_baliseView 绘制

void Cprogram_baliseView::OnDraw(CDC* /*pDC*/)
{
	Cprogram_baliseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: 在此处为本机数据添加绘制代码
}


// Cprogram_baliseView 打印

BOOL Cprogram_baliseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cprogram_baliseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void Cprogram_baliseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// Cprogram_baliseView 诊断

#ifdef _DEBUG
void Cprogram_baliseView::AssertValid() const
{
	CView::AssertValid();
}

void Cprogram_baliseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cprogram_baliseDoc* Cprogram_baliseView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cprogram_baliseDoc)));
	return (Cprogram_baliseDoc*)m_pDocument;
}
#endif //_DEBUG


// Cprogram_baliseView 消息处理程序

void Cprogram_baliseView::OnA4read()
{
	// TODO: 在此添加命令处理程序代码
	CA4ReadDlg ddd;

	ddd.DoModal();
}



void Cprogram_baliseView::OnA5write()
{
	// TODO: 在此添加命令处理程序代码
	CA5_Write ddd;
	ddd.DoModal();
}


void Cprogram_baliseView::OnWriteOperationmode()
{
	// TODO: 在此添加命令处理程序代码
	CWriteOperationMode ddd;

	ddd.DoModal();

}

void Cprogram_baliseView::OnProduceInfo()
{
	// TODO: 在此添加命令处理程序代码
	CReadManufacture dd;

	dd.DoModal();
}

void Cprogram_baliseView::OnLeuSSend()
{
	// TODO: 在此添加命令处理程序代码
	CS_Send ddd;
	ddd.DoModal();

}

void Cprogram_baliseView::OnBtm()
{
	// TODO: 在此添加命令处理程序代码
	CBtmDlg ddd;
	ddd.DoModal();

}

void Cprogram_baliseView::OnLeuReadC()
{
	// TODO: 在此添加命令处理程序代码
	CLeuCRead ddd;
	ddd.DoModal();
}

void Cprogram_baliseView::OnBaliseC()
{
	// TODO: 在此添加命令处理程序代码
	CBaliseCMeas ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuReadSc()
{
	// TODO: 在此添加命令处理程序代码
	CSRead_SC ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuWritetele()
{
	// TODO: 在此添加命令处理程序代码
	CLeuWriteDefaultTele ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuEraseBtm()
{
	// TODO: 在此添加命令处理程序代码
	CSErase_Msg ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuWriteFt()
{
	// TODO: 在此添加命令处理程序代码
	CSWrite_FT ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuWriteteleBx()
{
	// TODO: 在此添加命令处理程序代码
	CLeuWriteDefaultTele_Bx ddd;

	ddd.DoModal();
}
