
// DIBshowView.cpp : CDIBshowView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DIBshow.h"
#endif

#include "DIBshowDoc.h"
#include "DIBshowView.h"
#include "MyDib.h"
#include "Hist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDIBshowView

IMPLEMENT_DYNCREATE(CDIBshowView, CScrollView)

BEGIN_MESSAGE_MAP(CDIBshowView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDIBshowView ����/����

CDIBshowView::CDIBshowView()
{
	// TODO:  �ڴ˴���ӹ������
}

CDIBshowView::~CDIBshowView()
{
}

BOOL CDIBshowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CDIBshowView ����

void CDIBshowView::OnDraw(CDC* pDC)
{
	CDIBshowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	MyDib* hDIB = pDoc->mDib;

	if (hDIB != NULL)
	{
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = hDIB->GetWidth();
		rcDIB.bottom = hDIB->GetHeight();

		hDIB->StretchToDC(*pDC, rcDIB, rcDIB);
	}
}

void CDIBshowView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO:  �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CDIBshowView ��ӡ

BOOL CDIBshowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDIBshowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDIBshowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CDIBshowView ���

#ifdef _DEBUG
void CDIBshowView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDIBshowView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDIBshowDoc* CDIBshowView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIBshowDoc)));
	return (CDIBshowDoc*)m_pDocument;
}
#endif //_DEBUG


// CDIBshowView ��Ϣ�������


