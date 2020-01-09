
// shiyan2View.cpp: Cshiyan2View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "shiyan2.h"
#endif

#include "shiyan2Doc.h"
#include "shiyan2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cshiyan2View

IMPLEMENT_DYNCREATE(Cshiyan2View, CView)

BEGIN_MESSAGE_MAP(Cshiyan2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &Cshiyan2View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &Cshiyan2View::OnFileSaveAs)
END_MESSAGE_MAP()

// Cshiyan2View 构造/析构

Cshiyan2View::Cshiyan2View() noexcept
{
	// TODO: 在此处添加构造代码
	m_dib = NULL;
}

Cshiyan2View::~Cshiyan2View()
{
	if (m_dib != NULL)
	{
		delete m_dib;
		m_dib = NULL;
	}
	//if (m_pImage != NULL)
	//{
	//	delete[] m_pImage;
	//}
}

BOOL Cshiyan2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cshiyan2View 绘图

void Cshiyan2View::OnDraw(CDC* pDC)
{
	Cshiyan2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	if (m_dib)
	{
		if (m_dib->m_BMPinfoheader.biBitCount == 24)
		{
			unsigned char r, g, b;
			int width = m_dib->m_width;
			int height = m_dib->m_height;
			int real_width = width * 3 + m_dib->padding;
			unsigned char * m_pImage = m_dib->m_BMPdata;

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					b = *(m_pImage + real_width * i + j * 3);
					g = *(m_pImage + real_width * i + j * 3 + 1);
					r = *(m_pImage + real_width * i + j * 3 + 2);
					pDC->SetPixelV(10 + j, 10 + height - i, RGB(r, g, b));
				}
			}

			unsigned char ** rgbr = new unsigned char *[height + 4];
			unsigned char ** rgbg = new unsigned char *[height + 4];
			unsigned char ** rgbb = new unsigned char *[height + 4];

			for (int i = 0;i < height + 4;i++)
			{
				rgbr[i] = new unsigned char[width + 4];
				rgbg[i] = new unsigned char[width + 4];
				rgbb[i] = new unsigned char[width + 4];
			}

			memset(rgbr[0], 0, sizeof(rgbr[0]));
			memset(rgbr[1], 0, sizeof(rgbr[1]));
			memset(rgbr[height + 2], 0, sizeof(rgbr[height + 2]));
			memset(rgbr[height + 3], 0, sizeof(rgbr[height + 3]));
			memset(rgbg[0], 0, sizeof(rgbg[0]));
			memset(rgbg[1], 0, sizeof(rgbg[1]));
			memset(rgbg[height + 2], 0, sizeof(rgbg[height + 2]));
			memset(rgbg[height + 3], 0, sizeof(rgbg[height + 3]));
			memset(rgbb[0], 0, sizeof(rgbb[0]));
			memset(rgbb[1], 0, sizeof(rgbb[1]));
			memset(rgbb[height + 2], 0, sizeof(rgbb[height + 2]));
			memset(rgbb[height + 3], 0, sizeof(rgbb[height + 3]));

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					rgbr[i + 2][j + 2] = *(m_pImage + real_width * i + j * 3 + 2);
					rgbg[i + 2][j + 2] = *(m_pImage + real_width * i + j * 3 + 1);
					rgbb[i + 2][j + 2] = *(m_pImage + real_width * i + j * 3);
				}
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = gs(rgbr, i + 1, j + 1, 3);
					g = gs(rgbg, i + 1, j + 1, 3);
					b = gs(rgbb, i + 1, j + 1, 3);
					pDC->SetPixelV(550 + j, 10 + height - i, RGB(r, g, b));
				}
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = gs(rgbr, i, j, 5);
					g = gs(rgbg, i, j, 5);
					b = gs(rgbb, i, j, 5);
					pDC->SetPixelV(1100 + j, 10 + height - i, RGB(r, g, b));
				}
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = zh(rgbr, i + 1, j + 1, 3);
					g = zh(rgbg, i + 1, j + 1, 3);
					b = zh(rgbb, i + 1, j + 1, 3);
					pDC->SetPixelV(550 + j, 550 + height - i, RGB(r, g, b));
				}
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = zh(rgbr, i, j, 5);
					g = zh(rgbg, i, j, 5);
					b = zh(rgbb, i, j, 5);
					pDC->SetPixelV(1100 + j, 550 + height - i, RGB(r, g, b));
				}
			}

			delete[] rgbr;
			delete[] rgbg;
			delete[] rgbb;
		}
		else if (m_dib->m_BMPinfoheader.biBitCount == 8)
		{
			unsigned char grey;
			int width = m_dib->m_width;
			int height = m_dib->m_height;
			int real_width = width + m_dib->padding;
			unsigned char * m_pImage = m_dib->m_BMPdata;
			RGBQuad * quad = NULL;

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j);
					quad = m_dib->Quad + grey;
					pDC->SetPixelV(10 + j, 10 + height - i, RGB(quad->rgbRed, quad->rgbGreen, quad->rgbBlue));
				}
			}

			unsigned char ** rgbr = new unsigned char *[height + 4];
			unsigned char ** rgbg = new unsigned char *[height + 4];
			unsigned char ** rgbb = new unsigned char *[height + 4];

			for (int i = 0;i < height + 4;i++)
			{
				rgbr[i] = new unsigned char[width + 4];
				rgbg[i] = new unsigned char[width + 4];
				rgbb[i] = new unsigned char[width + 4];
			}

			memset(rgbr[0], 0, sizeof(rgbr[0]));
			memset(rgbr[1], 0, sizeof(rgbr[1]));
			memset(rgbr[height + 2], 0, sizeof(rgbr[height + 2]));
			memset(rgbr[height + 3], 0, sizeof(rgbr[height + 3]));
			memset(rgbg[0], 0, sizeof(rgbg[0]));
			memset(rgbg[1], 0, sizeof(rgbg[1]));
			memset(rgbg[height + 2], 0, sizeof(rgbg[height + 2]));
			memset(rgbg[height + 3], 0, sizeof(rgbg[height + 3]));
			memset(rgbb[0], 0, sizeof(rgbb[0]));
			memset(rgbb[1], 0, sizeof(rgbb[1]));
			memset(rgbb[height + 2], 0, sizeof(rgbb[height + 2]));
			memset(rgbb[height + 3], 0, sizeof(rgbb[height + 3]));

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j);
					quad = m_dib->Quad + grey;
					rgbr[i + 2][j + 2] = quad->rgbRed;
					rgbg[i + 2][j + 2] = quad->rgbGreen;
					rgbb[i + 2][j + 2] = quad->rgbBlue;
				}
			}

			BYTE r, g, b;
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = gs(rgbr, i + 1, j + 1, 3);
					g = gs(rgbg, i + 1, j + 1, 3);
					b = gs(rgbb, i + 1, j + 1, 3);
					pDC->SetPixelV(550 + j, 10 + height - i, RGB(r, g, b));
				}
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = gs(rgbr, i, j, 5);
					g = gs(rgbg, i, j, 5);
					b = gs(rgbb, i, j, 5);
					pDC->SetPixelV(1100 + j, 10 + height - i, RGB(r, g, b));
				}
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = zh(rgbr, i + 1, j + 1, 3);
					g = zh(rgbg, i + 1, j + 1, 3);
					b = zh(rgbb, i + 1, j + 1, 3);
					pDC->SetPixelV(550 + j, 550 + height - i, RGB(r, g, b));
				}
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = zh(rgbr, i, j, 5);
					g = zh(rgbg, i, j, 5);
					b = zh(rgbb, i, j, 5);
					pDC->SetPixelV(1100 + j, 550 + height - i, RGB(r, g, b));
				}
			}

			delete[] rgbr;
			delete[] rgbg;
			delete[] rgbb;
		}
		else if (m_dib->m_BMPinfoheader.biBitCount == 4)
		{
			unsigned char grey;
			int width = m_dib->m_width;
			int height = m_dib->m_height;
			int real_width = (width + m_dib->padding) / 2;
			unsigned char * m_pImage = m_dib->m_BMPdata;
			RGBQuad * quad = NULL;

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j / 2);
					if (!j % 2)
					{
						grey = grey >> 4;
					}
					else
					{
						grey = grey & 15;
					}
					quad = m_dib->Quad + grey;
					pDC->SetPixelV(10 + j, 10 + height - i, RGB(quad->rgbRed, quad->rgbGreen, quad->rgbBlue));
				}
			}
		}
		else if (m_dib->m_BMPinfoheader.biBitCount == 1)
		{
			unsigned char grey;
			int width = m_dib->m_width;
			int height = m_dib->m_height;
			int real_width = (width + m_dib->padding) / 8;
			unsigned char * m_pImage = m_dib->m_BMPdata;
			RGBQuad * quad = NULL;

			int temp;
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j / 8);
					temp = 7 - j % 8;
					grey = (grey & (1 << temp)) >> temp;
					quad = m_dib->Quad + grey;
					pDC->SetPixelV(10 + j, 10 + height - i, RGB(quad->rgbRed, quad->rgbGreen, quad->rgbBlue));
				}
			}
		}
	}
}


// Cshiyan2View 打印

BOOL Cshiyan2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cshiyan2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Cshiyan2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Cshiyan2View 诊断

#ifdef _DEBUG
void Cshiyan2View::AssertValid() const
{
	CView::AssertValid();
}

void Cshiyan2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cshiyan2Doc* Cshiyan2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cshiyan2Doc)));
	return (Cshiyan2Doc*)m_pDocument;
}
#endif //_DEBUG


// Cshiyan2View 消息处理程序


void Cshiyan2View::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	//读取新图像前，先清空旧图像数据
	if (m_dib)
	{
		delete m_dib;
		m_dib = NULL;
	}

	CFileDialog dlg(TRUE);	//读取任何文件

	if (dlg.DoModal() == IDOK)
	{
		CString filename = dlg.GetPathName();	//获取文件完整路径

		m_dib = new MyDIB;	//分配内存
		m_dib->Read(filename);
	}

	Invalidate();
}


void Cshiyan2View::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	if (m_dib == NULL)		//如果没有图像，则直接返回
		return;

	CFileDialog dlg(FALSE);		//保存文件对话框
	if (dlg.DoModal() == IDOK)
	{
		CString filename = dlg.GetPathName();	//获取文件完整路径
		m_dib->Write(filename);
	}

	Invalidate();
}

BYTE Cshiyan2View::gs(unsigned char ** rgb, int i, int j, int p)
{
	int x, y;
	long long temp = 0;
	if (p == 3)
	{
		for (x = 0;x < p;x++)
		{
			for (y = 0;y < p;y++)
			{
				temp += gs3[x][y] * rgb[i + x][j + y];
			}
		}
		temp = temp / 16;
	}
	else if (p == 5)
	{
		for (x = 0;x < p;x++)
		{
			for (y = 0;y < p;y++)
			{
				temp += gs5[x][y] * rgb[i + x][j + y];
			}
		}
		temp = temp / 273;
	}
	
	return (BYTE)temp;
}

BYTE Cshiyan2View::zh(unsigned char ** rgb, int i, int j, int p)
{
	int x, y;
	for (x = 0;x < p;x++)
	{
		for (y = 0;y < p;y++)
		{
			arr[x*p + y] = rgb[i + x][j + y];
		}
	}
	p = p * p;
	std::sort(arr, arr + p);
	p = p >> 1;
	return (BYTE)arr[p];
}
