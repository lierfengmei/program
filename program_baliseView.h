// program_baliseView.h : Cprogram_baliseView ��Ľӿ�
//


#pragma once


class Cprogram_baliseView : public CView
{
protected: // �������л�����
	Cprogram_baliseView();
	DECLARE_DYNCREATE(Cprogram_baliseView)

// ����
public:
	Cprogram_baliseDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Cprogram_baliseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnA4read();
	afx_msg void OnA5write();
	afx_msg void OnA5read();
//	afx_msg void OnClose();
	afx_msg void OnWriteOperationmode();
	afx_msg void OnProduceInfo();
	afx_msg void OnLeuSSend();
	afx_msg void OnBtm();
	afx_msg void OnLeuReadC();
	afx_msg void OnBaliseC();
	afx_msg void OnLeuReadSc();
	afx_msg void OnLeuWritetele();
	afx_msg void OnLeuEraseBtm();
	afx_msg void OnLeuWriteFt();
	afx_msg void OnLeuWriteteleBx();
};

#ifndef _DEBUG  // program_baliseView.cpp �ĵ��԰汾
inline Cprogram_baliseDoc* Cprogram_baliseView::GetDocument() const
   { return reinterpret_cast<Cprogram_baliseDoc*>(m_pDocument); }
#endif

