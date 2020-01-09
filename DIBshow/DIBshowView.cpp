
// DIBshowView.cpp : CDIBshowView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDIBshowView 构造/析构

CDIBshowView::CDIBshowView()
{
	// TODO:  在此处添加构造代码
}

CDIBshowView::~CDIBshowView()
{
}

BOOL CDIBshowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CDIBshowView 绘制

void CDIBshowView::OnDraw(CDC* pDC)
{
	CDIBshowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
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
	// TODO:  计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CDIBshowView 打印

BOOL CDIBshowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDIBshowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CDIBshowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CDIBshowView 诊断

#ifdef _DEBUG
void CDIBshowView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDIBshowView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDIBshowDoc* CDIBshowView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIBshowDoc)));
	return (CDIBshowDoc*)m_pDocument;
}
#endif //_DEBUG


// CDIBshowView 消息处理程序


