// program_baliseDoc.h :  Cprogram_baliseDoc ��Ľӿ�
//


#pragma once

class Cprogram_baliseDoc : public CDocument
{
protected: // �������л�����
	Cprogram_baliseDoc();
	DECLARE_DYNCREATE(Cprogram_baliseDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~Cprogram_baliseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


