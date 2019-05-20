
// [MFC] BitMapView.cpp : CMFCBitMapView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCBitMapView ����/�Ҹ�

CMFCBitMapView::CMFCBitMapView()
{
	//_CrtSetBreakAlloc(239);
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}
// CMFCBitMapView �׸���

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

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

}


// CMFCBitMapView �μ�

BOOL CMFCBitMapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFCBitMapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFCBitMapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFCBitMapView ����

#ifdef _DEBUG
void CMFCBitMapView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCBitMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCBitMapDoc* CMFCBitMapView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCBitMapDoc)));
	return (CMFCBitMapDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCBitMapView �޽��� ó����


void CMFCBitMapView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages
	CDC MemDC;
	BITMAP bmpInfo;

	// ȭ�� DC�� ȣȯ�Ǵ� �޸� DC�� ����
	MemDC.CreateCompatibleDC(&dc);

	// ��Ʈ�� ���ҽ� �ε�
	
	pOldBmp = NULL;
	bmp.LoadBitmapW(IDB_BITMAP1);

	// �ε��� ��Ʈ�� ���� Ȯ��
	bmp.GetBitmap(&bmpInfo);

	// �޸� DC�� ����
	pOldBmp = MemDC.SelectObject(&bmp);

	// �޸� DC�� ��� �ִ� ��Ʈ���� ȭ�� DC�� �����Ͽ� ���
	
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	ClearReDO();
	MouseCommand * pNew = new MouseCommand(point);
	m_UnDo.push(pNew);
	pNew->Execute(this);
	InvalidateRect(NULL);
	CView::OnLButtonDown(nFlags, point);
}

void CMFCBitMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
