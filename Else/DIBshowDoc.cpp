
// DIBshowDoc.cpp : CDIBshowDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DIBshow.h"
#endif

#include "DIBshowDoc.h"
#include "Hist.h"
#include "Histogram.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDIBshowDoc

IMPLEMENT_DYNCREATE(CDIBshowDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIBshowDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CDIBshowDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CDIBshowDoc::OnFileSave)
	ON_COMMAND(ID_Hist, &CDIBshowDoc::OnHist)
	ON_COMMAND(ID_HistAve, &CDIBshowDoc::OnHistave)
	ON_COMMAND(ID_Strech_Log, &CDIBshowDoc::OnStrechLog)
	ON_COMMAND(ID_Reverse, &CDIBshowDoc::OnReverse)
	ON_COMMAND(ID_Strech_Linear, &CDIBshowDoc::OnStrechLinear)
	ON_COMMAND(ID_Strech_Exp, &CDIBshowDoc::OnStrechExp)
	ON_COMMAND(ID_HistMatch, &CDIBshowDoc::OnHistmatch)
	ON_COMMAND(ID_medFilter3x3, &CDIBshowDoc::Onmedfilter3x3)
	ON_COMMAND(ID_medFilter5x5, &CDIBshowDoc::Onmedfilter5x5)
	//ON_COMMAND(ID_avgFilter3x3, &CDIBshowDoc::Onavgfilter3x3)
	//ON_COMMAND(ID_avgFilter5x5, &CDIBshowDoc::Onavgfilter5x5)
	ON_COMMAND(ID_OnGsph, &CDIBshowDoc::OnGsph)
	ON_COMMAND(ID_OnGsphColor, &CDIBshowDoc::OnGsphColor)
END_MESSAGE_MAP()


// CDIBshowDoc 构造/析构

CDIBshowDoc::CDIBshowDoc()
: mDib(NULL)
{
	// TODO:  在此添加一次性构造代码

}

CDIBshowDoc::~CDIBshowDoc()
{
	if (mDib != NULL)
		delete mDib;
}

BOOL CDIBshowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDIBshowDoc 序列化

void CDIBshowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDIBshowDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CDIBshowDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CDIBshowDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDIBshowDoc 诊断

#ifdef _DEBUG
void CDIBshowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIBshowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDIBshowDoc 命令


void CDIBshowDoc::OnFileOpen()
{
	// TODO:  在此添加命令处理程序代码
	LPCTSTR lpszFilter = LPCTSTR(L"BMP Files(*.bmp)|*.bmp|任何文件|*.*||");
	CFileDialog dlg1(TRUE, lpszFilter, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, NULL);
	CString filename;

	//打开文件对话框
	if (dlg1.DoModal() == IDOK)
	{
		filename = dlg1.GetPathName();
		if (mDib != NULL)
		{
			delete mDib;
			mDib = NULL;
		}
		mDib = new MyDib(filename);
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::OnFileSave()
{
	// TODO:  在此添加命令处理程序代码
	LPCTSTR lpszFilter = LPCTSTR(L"BMP Files(*.bmp)|*.bmp|任何文件|*.*||");
	CFileDialog dlg1(FALSE, lpszFilter, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, NULL);
	CString filename;

	//打开文件对话框
	if (dlg1.DoModal() == IDOK)
	{
		filename = dlg1.GetPathName();
		if (mDib != NULL)
		{
			mDib->SaveDIB(filename);
		}
	}
}


void CDIBshowDoc::OnHist()
{
	CHist dlg;
	Histogram hist;

	if (mDib != NULL)
	{
		hist.computeHist((BYTE *)mDib->GetBits(), mDib->GetBodySize());
		dlg.setHist(hist);
		dlg.DoModal();
	}
}


void CDIBshowDoc::OnHistave()
{
	Histogram hist;

	if (mDib != NULL)
	{
		hist.computeHistAve((BYTE *)mDib->GetBits(), mDib->GetBodySize());
		mDib->histAve(hist.m_histAveArray);
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::OnStrechLog()
{
	if (mDib != NULL)
	{
		mDib->LogStrech(0, 0.033, 2);
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::OnReverse()
{
	if (mDib != NULL)
	{
		mDib->Minus();
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::OnStrechLinear()
{
	if (mDib != NULL)
	{
		mDib->LinearStrech(50, 200, 100, 150);
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::OnStrechExp()
{
	if (mDib != NULL)
	{
		mDib->ExpStrech(128, 2, 0.6);
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::OnHistmatch()
{
	MyDib* objDib = NULL;

	LPCTSTR lpszFilter = LPCTSTR(L"BMP Files(*.bmp)|*.bmp|任何文件|*.*||");
	CFileDialog dlg1(TRUE, lpszFilter, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, NULL);
	CString filename;

	//打开文件对话框
	if (dlg1.DoModal() == IDOK)
	{
		filename = dlg1.GetPathName();
		if (objDib != NULL)
		{
			delete objDib;
			objDib = NULL;
		}
		objDib = new MyDib(filename);
	}

	Histogram sourceHist ,objHist;
	if (mDib != NULL)
		sourceHist.computeHist((BYTE *)mDib->GetBits(), mDib->GetBodySize());
	if (objDib != NULL)
		objHist.computeHist((BYTE *)objDib->GetBits(), objDib->GetBodySize());

	mDib->histMatch(sourceHist.m_histArray, objHist.m_histArray);
	UpdateAllViews(NULL, 0, NULL);
}


void CDIBshowDoc::Onmedfilter3x3()
{
	if (mDib != NULL)
	{
		//mDib->medianFilter(5);
		mDib->avgFilter(5);
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::Onmedfilter5x5()
{
	if (mDib != NULL)
	{
		mDib->medianFilter(5);
		//mDib->avgFilter(5);
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::Onavgfilter3x3()
{
	if (mDib != NULL)
	{
		mDib->avgFilter(3);
		UpdateAllViews(NULL, 0, NULL);
	}
}


void CDIBshowDoc::Onavgfilter5x5()
{
	if (mDib != NULL)
	{
		mDib->avgFilter(5);
		UpdateAllViews(NULL, 0, NULL);
	}
}

void CDIBshowDoc::OnGsph()
{
	if (mDib != NULL)
	{
		mDib->OnGsph(3);
		UpdateAllViews(NULL, 0, NULL);
	}
}

void CDIBshowDoc::OnGsphColor()
{
	if (mDib != NULL)
	{
		mDib->OnGsphColor(3);
		UpdateAllViews(NULL, 0, NULL);
	}
}