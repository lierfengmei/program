// program_baliseView.h : Cprogram_baliseView 类的接口
//


#pragma once


class Cprogram_baliseView : public CView
{
protected: // 仅从序列化创建
	Cprogram_baliseView();
	DECLARE_DYNCREATE(Cprogram_baliseView)

// 属性
public:
	Cprogram_baliseDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cprogram_baliseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // program_baliseView.cpp 的调试版本
inline Cprogram_baliseDoc* Cprogram_baliseView::GetDocument() const
   { return reinterpret_cast<Cprogram_baliseDoc*>(m_pDocument); }
#endif

