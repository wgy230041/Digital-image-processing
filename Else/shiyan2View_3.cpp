
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
		}
		else if (m_dib->m_BMPinfoheader.biBitCount == 8)
		{
			unsigned char grey;
			int width = m_dib->m_width;
			int height = m_dib->m_height;
			int real_width = width + m_dib->padding;
			unsigned char * m_pImage = m_dib->m_BMPdata;
			RGBQuad * quad = NULL;

			unsigned char greyMin=255, greyMax=0;

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j);
					quad = m_dib->Quad + grey;
					pDC->SetPixelV(10 + j, 10 + height - i, RGB(quad->rgbRed, quad->rgbGreen, quad->rgbBlue));
					r[quad->rgbRed]++;
					if (greyMax < quad->rgbRed) greyMax = quad->rgbRed;
					if (greyMin > quad->rgbRed) greyMin = quad->rgbRed;
				}
			}

			long long sum = width * height;
			double p_sum = 0;
			for (int i = 0;i < 256;i++)
			{
				pr[i] = r[i] * 1.0 / sum;
				p_sum += pr[i];
				ps[i] = p_sum;
			}

			int x0 = 10, y0 = 300, x1 = 350, y1 = 600; //定义视口的顶点在窗口中的坐标
			int N = 256, dx = (x1 - x0) / N, dy = y1-y0;//数据的个数及条图的宽度
			CBrush br;
			int i, x;
			srand((unsigned)time(NULL));
			pDC->Rectangle(CRect(x0, y0, x1, y1));
			for (i = 0, x = x0;i < N;++i, x += dx)
			{
				br.CreateHatchBrush(rand() % 6, RGB(rand() % 200, rand() % 200, rand() % 200));

				pDC->SelectObject(&br);
				pDC->Rectangle(x, y1 - pr[i] * dy, x + dx, y1);
				pDC->SelectStockObject(WHITE_BRUSH);
				br.DeleteObject();
			}

			int temp;
			for (int i = 0;i < 256;i++)
			{
				r_s[i] = (int)(ps[i] * 255);
			}
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j);
					quad = m_dib->Quad + grey;
					temp = r_s[quad->rgbRed];
					pDC->SetPixelV(400 + j, 10 + height - i, RGB(temp, temp, temp));
					s[temp]++;
				}
			}

			x0 = 400, y0 = 300, x1 = 750, y1 = 600; //定义视口的顶点在窗口中的坐标
			dx = (x1 - x0) / N, dy = y1 - y0;//数据的个数及条图的宽度
			srand((unsigned)time(NULL));
			pDC->Rectangle(CRect(x0, y0, x1, y1));
			for (i = 0, x = x0;i < N;++i, x += dx)
			{
				br.CreateHatchBrush(rand() % 6, RGB(rand() % 200, rand() % 200, rand() % 200));

				pDC->SelectObject(&br);
				pDC->Rectangle(x, y1 - s[i] * dy * 1.0/sum, x + dx, y1);
				pDC->SelectStockObject(WHITE_BRUSH);
				br.DeleteObject();
			}

			//设置规格化标准频率和
			for (int i = 0;i < 128;i++)
			{
				pz[i] = 0;
			}
			for (int i = 128;i < 256;i++)
			{
				pz[i] = (i-127)*2.0/256;
			}

			double tmp,tmp2;
			int j;
			for (int i = 0;i < 256;i++)
			{
				tmp = fabs(ps[i] - pz[0]);
				for (j = 1;j < 256;j++)
				{
					tmp2 = fabs(ps[i] - pz[j]);
					if (tmp >= tmp2)
					{
						tmp = tmp2;
					}
					else
					{
						r_z[i] = j - 1;
						break;
					}
				}
				if (j == 256)
				{
					r_z[i] = 255;
				}
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j);
					quad = m_dib->Quad + grey;
					temp = r_z[quad->rgbRed];
					pDC->SetPixelV(800 + j, 10 + height - i, RGB(temp, temp, temp));
					z[temp]++;
				}
			}

			x0 = 800, y0 = 300, x1 = 1150, y1 = 600; //定义视口的顶点在窗口中的坐标
			dx = (x1 - x0) / N, dy = y1 - y0;//数据的个数及条图的宽度
			srand((unsigned)time(NULL));
			pDC->Rectangle(CRect(x0, y0, x1, y1));
			for (i = 0, x = x0;i < N;++i, x += dx)
			{
				br.CreateHatchBrush(rand() % 6, RGB(rand() % 200, rand() % 200, rand() % 200));

				pDC->SelectObject(&br);
				pDC->Rectangle(x, y1 - z[i] * dy*1.0 / sum, x + dx, y1);
				pDC->SelectStockObject(WHITE_BRUSH);
				br.DeleteObject();
			}

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j);
					quad = m_dib->Quad + grey;
					temp = quad->rgbRed;
					if (temp < 90)
						temp = 0;
					else
						temp = 255;
					pDC->SetPixelV(1100 + j, 10 + height - i, RGB(temp,temp,temp));
					r[quad->rgbRed]++;
				}
			}
			//printf("1\n");
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
		memset(r, 0, sizeof(r));
		memset(s, 0, sizeof(s));
	}	memset(z, 0, sizeof(z));

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
