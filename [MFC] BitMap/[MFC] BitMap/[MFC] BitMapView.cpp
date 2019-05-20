
// [MFC] BitMapView.cpp : CMFCBitMapView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "[MFC] BitMap.h"
#include "MouseCommand.h"
#include "KeyBoardCommand.h"
#endif

#include "[MFC] BitMapDoc.h"
#include "[MFC] BitMapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCBitMapView

IMPLEMENT_DYNCREATE(CMFCBitMapView, CView)

BEGIN_MESSAGE_MAP(CMFCBitMapView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCBitMapView 생성/소멸

CMFCBitMapView::CMFCBitMapView()
{
	//_CrtSetBreakAlloc(239);
	// TODO: 여기에 생성 코드를 추가합니다.
	pt.x = 0;
	pt.y = 0;

	MouseCommand * pNew = new MouseCommand(pt);
	m_UnDo.push(pNew);
}

CMFCBitMapView::~CMFCBitMapView()
{
	ClearReDO();

	for (int i = 0; i < m_UnDo.size(); i++)
	{
		delete(m_UnDo.top());
		m_UnDo.pop();
	}
}

BOOL CMFCBitMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}
// CMFCBitMapView 그리기

void CMFCBitMapView::SetPoint(POINT _pt)
{
	pt.x = _pt.x;
	pt.y = _pt.y;
}

void CMFCBitMapView::OnDraw(CDC* /*pDC*/)
{
	CMFCBitMapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

}


// CMFCBitMapView 인쇄

BOOL CMFCBitMapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCBitMapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCBitMapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCBitMapView 진단

#ifdef _DEBUG
void CMFCBitMapView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCBitMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCBitMapDoc* CMFCBitMapView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCBitMapDoc)));
	return (CMFCBitMapDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCBitMapView 메시지 처리기


void CMFCBitMapView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages
	CDC MemDC;
	BITMAP bmpInfo;

	// 화면 DC와 호환되는 메모리 DC를 생성
	MemDC.CreateCompatibleDC(&dc);

	// 비트맵 리소스 로딩
	
	pOldBmp = NULL;
	bmp.LoadBitmapW(IDB_BITMAP1);

	// 로딩된 비트맵 정보 확인
	bmp.GetBitmap(&bmpInfo);

	// 메모리 DC에 선택
	pOldBmp = MemDC.SelectObject(&bmp);

	// 메모리 DC에 들어 있는 비트맵을 화면 DC로 복사하여 출력
	
	dc.TransparentBlt(pt.x, pt.y, bmpInfo.bmWidth,bmpInfo.bmHeight, &MemDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, RGB(255, 0, 255));
	MemDC.SelectObject(pOldBmp);
}

void CMFCBitMapView::ClearReDO()
{
	for (int i = 0; i < m_ReDo.size(); i++)
	{
		delete (m_ReDo.top());
		m_ReDo.pop();
	}
}

void CMFCBitMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ClearReDO();
	MouseCommand * pNew = new MouseCommand(point);
	m_UnDo.push(pNew);
	pNew->Execute(this);
	InvalidateRect(NULL);
	CView::OnLButtonDown(nFlags, point);
}

void CMFCBitMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KeyBoardCommand * pNew = new KeyBoardCommand(pt);
	m_UnDo.push(pNew);

	switch (nChar)
	{
	case VK_RIGHT:
		ClearReDO();
		pt.x += 30;
		pNew->SetPt(pt);
		break;

	case VK_LEFT:
		ClearReDO();
		if (pt.x >= 30)
			pt.x -= 30;
		pNew->SetPt(pt);
		break;

	case VK_UP:
		ClearReDO();
		if (pt.y >= 30)
			pt.y -= 30;
		pNew->SetPt(pt);
		break;

	case VK_DOWN:
		ClearReDO();
		pt.y += 30;
		pNew->SetPt(pt);
		break;

	case 0x59: // y
		delete(m_UnDo.top());
		m_UnDo.pop();
		if (m_ReDo.size() < 1)
			break;

		m_UnDo.push(m_ReDo.top());
		m_ReDo.pop();
		m_UnDo.top()->Execute(this);

		break;
	case 0x5A: //z
		delete(m_UnDo.top());
		m_UnDo.pop();
		if (m_UnDo.size() <= 1)
			break;
		m_ReDo.push(m_UnDo.top());
		m_UnDo.pop();
		m_UnDo.top()->Execute(this);
		break;
	}
	


	InvalidateRect(NULL);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
