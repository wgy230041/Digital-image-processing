/* RawImageView.cpp : CRawImageView ���ʵ��
ID:22920172204299
NAME: Zhao Xiaoxuan
DATE: 2019/10/11
*/

#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "RawImage.h"
#endif

#include "RawImageDoc.h"
#include "RawImageView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CRawImageView
IMPLEMENT_DYNCREATE(CRawImageView, CScrollView)

BEGIN_MESSAGE_MAP(CRawImageView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CRawImageView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &CRawImageView::OnFileSaveAs)
END_MESSAGE_MAP()

// CRawImageView ����/����
CRawImageView::CRawImageView()
: m_pImage(NULL)
, m_Nsize(0)
{
	// TODO:  �ڴ˴���ӹ������
	m_pImage = NULL;
}

CRawImageView::~CRawImageView()
{
	if (m_pImage != NULL)
		delete[] m_pImage;
}

BOOL CRawImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CRawImageView ����
void CRawImageView::OnDraw(CDC* pDC)
{
	CRawImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	unsigned char grey = 0, r = 0, g = 0, b = 0;
	/*for (int i = 0; i<m_Msize; i++)
		for (int j = 0; j<m_Nsize; j++)
		{
			r = *(m_pImage + m_Nsize*i*3 + j*3);
			g = *(m_pImage + m_Nsize*i*3 + j*3 + 1);
			b = *(m_pImage + m_Nsize*i*3 + j*3 + 2);
			pDC->SetPixelV(10 + j, 10 + i, RGB(r, g, b));
		}*/
	for (int i = 0; i<m_Msize; i++)
		for (int j = 0; j<m_Nsize; j++)
		{
			grey = *(m_pImage + m_Nsize*i + j);
			pDC->SetPixelV(10 + j, 10 + i, RGB(grey, grey, grey));
		}
}

void CRawImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CSize sizeTotal;
	// TODO:  �������ͼ�ĺϼƴ�С
//	sizeTotal.cx = sizeTotal.cy = 100;
//	SetScrollSizes(MM_TEXT, sizeTotal);
	CSize sizeTotal(3000, 4000); // 3-by-4 cm
	CSize sizeLine = CSize(sizeTotal.cx / 100, sizeTotal.cy / 100);
	SetScrollSizes(MM_TEXT, sizeTotal, sizeTotal, sizeLine);
	return; 
}

// CRawImageView ��ӡ
BOOL CRawImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CRawImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CRawImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}

// CRawImageView ���
#ifdef _DEBUG
void CRawImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CRawImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CRawImageDoc* CRawImageView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRawImageDoc)));
	return (CRawImageDoc*)m_pDocument;
}
#endif //_DEBUG

void CRawImageView::OnFileOpen()
{
	// TODO:  �ڴ���������������
	CFileDialog dlg(TRUE,_T("raw"), _T("*.raw"));
	CFile file;
	if (dlg.DoModal() == IDOK)
	{
		VERIFY(file.Open(dlg.GetPathName(), CFile::modeRead));
	}
	ndwSizeRaw = (DWORD)file.GetLength();
	m_pImage = new unsigned char[ndwSizeRaw];
	file.Read(m_pImage, ndwSizeRaw);
	file.Close();

	m_Msize = m_Nsize = (int)sqrt(double(ndwSizeRaw));
	//m_Msize = 600;
	//m_Nsize = 752;
	TRACE("N=%d\n", m_Nsize);

	Invalidate();
}

void CRawImageView::OnFileSaveAs()
{
	// TODO:  �ڴ���������������
	WriteRawFile(m_pImage, ndwSizeRaw);
	Invalidate();

	return;
}

BOOL CRawImageView::WriteRawFile(LPVOID lpvBits, DWORD ndwSizeRaw)
{
	CFileDialog dlg(FALSE, _T("raw"), _T("*.raw"));
	CFile RawFile;
	if (dlg.DoModal() == IDOK)
	{
		VERIFY(RawFile.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite));
	}
	try 
	{
		RawFile.Write((LPVOID)lpvBits, ndwSizeRaw);
	}
	catch (CException* pe) 
	{
		pe->Delete();

		char chars[12] = "write error";
		AfxMessageBox(_T("write error)"));
		RawFile.Close();
		return FALSE;
	}
	RawFile.Close();
	return TRUE;
}
