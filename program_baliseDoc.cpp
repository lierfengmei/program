// program_baliseDoc.cpp :  Cprogram_baliseDoc ���ʵ��
//

#include "stdafx.h"
#include "program_balise.h"

#include "program_baliseDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cprogram_baliseDoc

IMPLEMENT_DYNCREATE(Cprogram_baliseDoc, CDocument)

BEGIN_MESSAGE_MAP(Cprogram_baliseDoc, CDocument)
END_MESSAGE_MAP()


// Cprogram_baliseDoc ����/����

Cprogram_baliseDoc::Cprogram_baliseDoc()
{
	// TODO���ڴ����һ���Թ������

}

Cprogram_baliseDoc::~Cprogram_baliseDoc()
{
}

BOOL Cprogram_baliseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO���ڴ�������³�ʼ������
	// ��SDI �ĵ������ø��ĵ���

	return TRUE;
}




// Cprogram_baliseDoc ���л�

void Cprogram_baliseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO���ڴ���Ӵ洢����
	}
	else
	{
		// TODO���ڴ���Ӽ��ش���
	}
}


// Cprogram_baliseDoc ���

#ifdef _DEBUG
void Cprogram_baliseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cprogram_baliseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cprogram_baliseDoc ����
