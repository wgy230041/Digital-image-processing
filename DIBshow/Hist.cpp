// Hist.cpp : 实现文件
//

#include "stdafx.h"
#include "DIBshow.h"
#include "Hist.h"
#include "afxdialogex.h"
#include "Histogram.h"
#include "DIBshowDoc.h"


// CHist 对话框

IMPLEMENT_DYNAMIC(CHist, CDialogEx)

CHist::CHist(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHist::IDD, pParent)
{

}

CHist::~CHist()
{
}

void CHist::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHist, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CHist::setHist(Histogram given_hist)
{
	hist = given_hist;
}

// CHist 消息处理程序

void CHist::OnPaint()
{
	CPaintDC dc(this); 

	CDialog::OnInitDialog();

	CRect clientRect;
	GetDlgItem(IDC_STATIC_HistRect)->GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);

	dc.SelectStockObject(NULL_BRUSH);

	int max = 0;
	for (int i = 0; i < 256; ++i)
	{
		if (hist.m_histArray[i] > max)
			max = hist.m_histArray[i];
	}
	DWORD bufTemp[256];
	double x = clientRect.Height();
	for (int i = 0; i < 256; ++i)
		bufTemp[i] = (unsigned long)((float)hist.m_histArray[i] * x / max);

	dc.MoveTo(clientRect.left, clientRect.bottom);
	dc.LineTo(clientRect.left, clientRect.bottom - x - 1);
	dc.MoveTo(clientRect.left, clientRect.bottom);
	dc.LineTo(clientRect.left + 257, clientRect.bottom);
	for (int i = clientRect.left; i <= clientRect.left + 256; ++i)
	{
		dc.MoveTo(i, clientRect.bottom);
		dc.LineTo(i, clientRect.bottom - bufTemp[i - clientRect.left]);
	}
}
