
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
			long long N = width * height;

			fftw_complex * inRed, *outRed, *inGreen, *outGreen, *inBlue, *outBlue;
			inRed = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);
			outRed = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);
			inGreen = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);
			outGreen = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);
			inBlue = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);
			outBlue = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*width*height);

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					b = *(m_pImage + real_width * i + j * 3);
					g = *(m_pImage + real_width * i + j * 3 + 1);
					r = *(m_pImage + real_width * i + j * 3 + 2);
					pDC->SetPixelV(10 + j, 10 + height - i, RGB(r, g, b));

					inRed[i*width + j][0] = r;
					inRed[i*width + j][1] = 0;
					inGreen[i*width + j][0] = g;
					inGreen[i*width + j][1] = 0;
					inBlue[i*width + j][0] = b;
					inBlue[i*width + j][1] = 0;
				}
			}
			
			fftw_plan planRed = fftw_plan_dft_2d(height, width, inRed, outRed, FFTW_FORWARD, FFTW_ESTIMATE);
			fftw_plan planGreen = fftw_plan_dft_2d(height, width, inGreen, outGreen, FFTW_FORWARD, FFTW_ESTIMATE);
			fftw_plan planBlue = fftw_plan_dft_2d(height, width, inBlue, outBlue, FFTW_FORWARD, FFTW_ESTIMATE);

			fftw_plan planRedBack = fftw_plan_dft_2d(height, width, outRed, outRed, FFTW_BACKWARD, FFTW_ESTIMATE);
			fftw_plan planGreenBack = fftw_plan_dft_2d(height, width, outGreen, outGreen, FFTW_BACKWARD, FFTW_ESTIMATE);
			fftw_plan planBlueBack = fftw_plan_dft_2d(height, width, outBlue, outBlue, FFTW_BACKWARD, FFTW_ESTIMATE);

			fftw_execute(planRed);
			fftw_execute(planGreen);
			fftw_execute(planBlue);

			/*double Max = 0;
			for (int i = 0; i < N;i++)
			{
				Max = max(Max, sqrt(outRed[i][0] * outRed[i][0] + outRed[i][1] * outRed[i][1]));
			}
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = (unsigned char)(sqrt(outRed[i*width + j][0] * outRed[i*width + j][0] + outRed[i*width + j][1] * outRed[i*width + j][1]) / Max);
					pDC->SetPixelV(10 + j, 530 + height - i, RGB(r, r, r));
				}
			}*/

			fftw_execute(planRedBack);
			fftw_execute(planGreenBack);
			fftw_execute(planBlueBack);
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = (unsigned char)(outRed[i*width + j][0]/N);
					g = (unsigned char)(outGreen[i*width + j][0]/N);
					b = (unsigned char)(outBlue[i*width + j][0]/N);
					pDC->SetPixelV(550 + j, 10 + height - i, RGB(r, g, b));
				}
			}

			fftw_execute(planRed);
			fftw_execute(planGreen);
			fftw_execute(planBlue);

			//x0 = 1100, y0 = 530, x1 = 1100 + width, y1 = 530 + height; //定义视口的顶点在窗口中的坐标
			//x_center = (x0 + x1) / 2, y_center = (y0 + y1) / 2;
			//pDC->Rectangle(CRect(x0, y0, x1, y1));
			//double fliter= 20000;
			//for (int i = 0; i < N; i++)
			//{
			//	if (fabs(outRed[i][0]) > fliter || fabs(outRed[i][1]) > fliter)
			//	{
			//		outRed[i][0] = 0;
			//		outRed[i][1] = 0;
			//	}
			//	else
			//	{
			//		x = outRed[i][0] * 512 / 200000;
			//		y = outRed[i][1] * 512 / 200000;
			//		pDC->SetPixelV(x_center + int(x), y_center + int(y), RGB(255, 0, 0));
			//	}
			//	if (fabs(outGreen[i][0]) > fliter || fabs(outGreen[i][1]) > fliter)
			//	{
			//		outGreen[i][0] = 0;
			//		outGreen[i][1] = 0;
			//	}
			//	else
			//	{
			//		x = outGreen[i][0] * 512 / 200000;
			//		y = outGreen[i][1] * 512 / 200000;
			//		pDC->SetPixelV(x_center + int(x), y_center + int(y), RGB(0, 255, 0));
			//	}
			//	if (fabs(outBlue[i][0]) > fliter || fabs(outBlue[i][1]) > fliter)
			//	{
			//		outBlue[i][0] = 0;
			//		outBlue[i][1] = 0;
			//	}
			//	else
			//	{
			//		x = outBlue[i][0] * 512 / 200000;
			//		y = outBlue[i][1] * 512 / 200000;
			//		pDC->SetPixelV(x_center + int(x), y_center + int(y), RGB(0, 0, 255));
			//	}
			//}
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					if (i > height * 1 / 8 && i < height * 7 / 8 ||
						j > width * 1 / 8 && j < width * 7 / 8)
					{
						outRed[i*width + j][0] = 0;
						outRed[i*width + j][1] = 0;
						outGreen[i*width + j][0] = 0;
						outGreen[i*width + j][1] = 0;
						outBlue[i*width + j][0] = 0;
						outBlue[i*width + j][1] = 0;
					}
				}
			}

			fftw_execute(planRedBack);
			fftw_execute(planGreenBack);
			fftw_execute(planBlueBack);
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = (unsigned char)(sqrt((outRed[i*width + j][0])*(outRed[i*width + j][0]) + (outRed[i*width + j][1])*(outRed[i*width + j][1])) / N);
					g = (unsigned char)(sqrt((outGreen[i*width + j][0])*(outGreen[i*width + j][0]) + (outGreen[i*width + j][1])*(outGreen[i*width + j][1])) / N);
					b = (unsigned char)(sqrt((outBlue[i*width + j][0])*(outBlue[i*width + j][0]) + (outBlue[i*width + j][1])*(outBlue[i*width + j][1])) / N);
					//r = (unsigned char)(outRed[i*width + j][0] / N);
					//g = (unsigned char)(outGreen[i*width + j][0] / N);
					//b = (unsigned char)(outBlue[i*width + j][0] / N);
					pDC->SetPixelV(1100 + j, 10 + height - i, RGB(r, g, b));
				}
			}

			fftw_execute(planRed);
			fftw_execute(planGreen);
			fftw_execute(planBlue);
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					if ( i < height * 1 / 8 && ( j < width * 1 / 8 || j> width * 7 / 8)  || 
						i > height * 7 / 8 && (j < width * 1 / 8 || j> width * 7 / 8) )
					{
						outRed[i*width + j][0] = 0;
						outRed[i*width + j][1] = 0;
						outGreen[i*width + j][0] = 0;
						outGreen[i*width + j][1] = 0;
						outBlue[i*width + j][0] = 0;
						outBlue[i*width + j][1] = 0;
					}
				}
			}

			fftw_execute(planRedBack);
			fftw_execute(planGreenBack);
			fftw_execute(planBlueBack);
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					r = (unsigned char)(sqrt((outRed[i*width + j][0])*(outRed[i*width + j][0]) + (outRed[i*width + j][1])*(outRed[i*width + j][1])) / N);
					g = (unsigned char)(sqrt((outGreen[i*width + j][0])*(outGreen[i*width + j][0]) + (outGreen[i*width + j][1])*(outGreen[i*width + j][1])) / N);
					b = (unsigned char)(sqrt((outBlue[i*width + j][0])*(outBlue[i*width + j][0]) + (outBlue[i*width + j][1])*(outBlue[i*width + j][1])) / N);
					//r = (unsigned char)(outRed[i*width + j][0] / N);
					//g = (unsigned char)(outGreen[i*width + j][0] / N);
					//b = (unsigned char)(outBlue[i*width + j][0] / N);
					pDC->SetPixelV(1100 + j, 530 + height - i, RGB(r, g, b));
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

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					grey = *(m_pImage + i * real_width + j);
					quad = m_dib->Quad + grey;
					pDC->SetPixelV(10 + j, 10 + height - i, RGB(quad->rgbRed, quad->rgbGreen, quad->rgbBlue));
				}
			}
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
