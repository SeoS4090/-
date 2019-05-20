
// [MFC] BitMapView.h : CMFCBitMapView 클래스의 인터페이스
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

protected: // serialization에서만 만들어집니다.
	CMFCBitMapView();
	DECLARE_DYNCREATE(CMFCBitMapView)
	

// 특성입니다.
public:
	CMFCBitMapDoc* GetDocument() const;

// 작업입니다.
public:
	void SetPoint(POINT _pt);	
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCBitMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void ClearReDO();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // [MFC] BitMapView.cpp의 디버그 버전
inline CMFCBitMapDoc* CMFCBitMapView::GetDocument() const
   { return reinterpret_cast<CMFCBitMapDoc*>(m_pDocument); }
#endif

