// program_baliseDoc.cpp :  Cprogram_baliseDoc 类的实现
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


// Cprogram_baliseDoc 构造/销毁

Cprogram_baliseDoc::Cprogram_baliseDoc()
{
	// TODO：在此添加一次性构造代码

}

Cprogram_baliseDoc::~Cprogram_baliseDoc()
{
}

BOOL Cprogram_baliseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO：在此添加重新初始化代码
	// （SDI 文档将重用该文档）

	return TRUE;
}




// Cprogram_baliseDoc 序列化

void Cprogram_baliseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO：在此添加存储代码
	}
	else
	{
		// TODO：在此添加加载代码
	}
}


// Cprogram_baliseDoc 诊断

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


// Cprogram_baliseDoc 命令
