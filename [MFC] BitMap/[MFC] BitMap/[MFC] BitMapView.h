
// [MFC] BitMapView.h : CMFCBitMapView Ŭ������ �������̽�
//

#pragma once
#include "Command.h"
#include <stack>
class CMFCBitMapDoc;
class CMFCBitMapView : public CView
{

private:
	POINT pt;
	std::stack<Command *> m_ReDo;
	std::stack<Command *> m_UnDo;
	CBitmap bmp;
	CBitmap* pOldBmp;

protected: // serialization������ ��������ϴ�.
	CMFCBitMapView();
	DECLARE_DYNCREATE(CMFCBitMapView)
	

// Ư���Դϴ�.
public:
	CMFCBitMapDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	void SetPoint(POINT _pt);	
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFCBitMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void ClearReDO();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // [MFC] BitMapView.cpp�� ����� ����
inline CMFCBitMapDoc* CMFCBitMapView::GetDocument() const
   { return reinterpret_cast<CMFCBitMapDoc*>(m_pDocument); }
#endif

