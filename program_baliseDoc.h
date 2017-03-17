// program_baliseDoc.h :  Cprogram_baliseDoc 类的接口
//


#pragma once

class Cprogram_baliseDoc : public CDocument
{
protected: // 仅从序列化创建
	Cprogram_baliseDoc();
	DECLARE_DYNCREATE(Cprogram_baliseDoc)

// 属性
public:

// 操作
public:

// 重写
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~Cprogram_baliseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


