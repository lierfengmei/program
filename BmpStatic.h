#pragma once


// CBmpStatic

class CBmpStatic : public CStatic
{
	DECLARE_DYNAMIC(CBmpStatic)

public:
	CBmpStatic();
	virtual ~CBmpStatic();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetResultBmp(int color);
};


