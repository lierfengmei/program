// program_baliseView.cpp : Cprogram_baliseView ���ʵ��
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
	// ��׼��ӡ����
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

// Cprogram_baliseView ����/����

Cprogram_baliseView::Cprogram_baliseView()
{
	// TODO: �ڴ˴���ӹ������

}

Cprogram_baliseView::~Cprogram_baliseView()
{
}

BOOL Cprogram_baliseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// Cprogram_baliseView ����

void Cprogram_baliseView::OnDraw(CDC* /*pDC*/)
{
	Cprogram_baliseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Cprogram_baliseView ��ӡ

BOOL Cprogram_baliseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Cprogram_baliseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void Cprogram_baliseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// Cprogram_baliseView ���

#ifdef _DEBUG
void Cprogram_baliseView::AssertValid() const
{
	CView::AssertValid();
}

void Cprogram_baliseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cprogram_baliseDoc* Cprogram_baliseView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cprogram_baliseDoc)));
	return (Cprogram_baliseDoc*)m_pDocument;
}
#endif //_DEBUG


// Cprogram_baliseView ��Ϣ�������

void Cprogram_baliseView::OnA4read()
{
	// TODO: �ڴ���������������
	CA4ReadDlg ddd;

	ddd.DoModal();
}



void Cprogram_baliseView::OnA5write()
{
	// TODO: �ڴ���������������
	CA5_Write ddd;
	ddd.DoModal();
}


void Cprogram_baliseView::OnWriteOperationmode()
{
	// TODO: �ڴ���������������
	CWriteOperationMode ddd;

	ddd.DoModal();

}

void Cprogram_baliseView::OnProduceInfo()
{
	// TODO: �ڴ���������������
	CReadManufacture dd;

	dd.DoModal();
}

void Cprogram_baliseView::OnLeuSSend()
{
	// TODO: �ڴ���������������
	CS_Send ddd;
	ddd.DoModal();

}

void Cprogram_baliseView::OnBtm()
{
	// TODO: �ڴ���������������
	CBtmDlg ddd;
	ddd.DoModal();

}

void Cprogram_baliseView::OnLeuReadC()
{
	// TODO: �ڴ���������������
	CLeuCRead ddd;
	ddd.DoModal();
}

void Cprogram_baliseView::OnBaliseC()
{
	// TODO: �ڴ���������������
	CBaliseCMeas ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuReadSc()
{
	// TODO: �ڴ���������������
	CSRead_SC ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuWritetele()
{
	// TODO: �ڴ���������������
	CLeuWriteDefaultTele ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuEraseBtm()
{
	// TODO: �ڴ���������������
	CSErase_Msg ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuWriteFt()
{
	// TODO: �ڴ���������������
	CSWrite_FT ddd;

	ddd.DoModal();
}

void Cprogram_baliseView::OnLeuWriteteleBx()
{
	// TODO: �ڴ���������������
	CLeuWriteDefaultTele_Bx ddd;

	ddd.DoModal();
}
