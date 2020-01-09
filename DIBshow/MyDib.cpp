// MyDib.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyDib.h"
#include <math.h>


MyDib::MyDib ( LPCTSTR dibName )
: DibBits ( NULL ), DibInfo ( NULL )
{
	//����CFile����
	CFile fp ( dibName, CFile::modeRead | CFile::typeBinary );

	BITMAPFILEHEADER bmfileheader;
	BITMAPINFOHEADER bmheader;

	ULONGLONG headpos;
	int PaletteSize = 0;
	int ret, cbHeaderSize;
	//��ȡ�ļ�ָ���λ��
	headpos = fp.GetPosition();
	//��ȡBMP�ļ�ͷ
	ret = fp.Read ( &bmfileheader, sizeof(BITMAPFILEHEADER) );
	//����ļ����ͱ�ͷ���ǡ�0x4d42������ʾ���ļ�����BMP�����ļ�
	//����ʾ�����˳���ע�⡰0x4d42�����ַ�������ǡ�BM��
	if ( bmfileheader.bfType != 0x4d42) 
	{
		AfxMessageBox(L"Error!");
		return;
	}

	//��ȡBMP�ļ���Ϣͷ	
	ret = fp.Read ( &bmheader, sizeof(BITMAPINFOHEADER) );
	//����RGBQUAD�Ĵ�С
	switch ( bmheader.biBitCount )
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
	}

	//ΪBITMAPINFO�ṹ�����ڴ�
	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize*sizeof ( RGBQUAD );
	DibInfo = (BITMAPINFO*) new char [ cbHeaderSize ];
	DibInfo->bmiHeader = bmheader;

	if ( PaletteSize )
	{	
		ret = fp.Read ( &(DibInfo->bmiColors[0]), PaletteSize*sizeof ( RGBQUAD ) );
		if ( ret != int( PaletteSize*sizeof ( RGBQUAD ) ) )
		{
			delete[] DibInfo;
			DibInfo = NULL;
			return;
		}
	}

	//Ϊ�����������ռ䣬��С��GetBodySize()����
	DibBits = (void*) new char[GetBodySize()];
	//���ļ�ָ���ƶ���DIBͼ������
	fp.Seek ( headpos + bmfileheader.bfOffBits, CFile::begin );

	ret = fp.Read ( DibBits, GetBodySize() );
	if ( ret != int ( GetBodySize() ) )
	{
		delete[] DibInfo;
		delete[] DibBits;
		DibInfo = NULL;
		DibBits = NULL;
	}
	//�ر��ļ�
	fp.Close();
}

MyDib::MyDib ( CBitmap& ddb, CPalette* palette )
: DibBits ( NULL ), DibInfo ( NULL )
{
	BITMAP ddbinfo;
	//���DDB�������Ϣ
	ddb.GetBitmap ( &ddbinfo );	
	/*�������ͼ��ĸ߶ȿ�ȵ���Ϣ����дDIB�������BITMAPINFO*/
	int w = ddbinfo.bmWidth;
	int h = ddbinfo.bmHeight;
	int BitsPerPixel = ddbinfo.bmPlanes*ddbinfo.bmBitsPixel;

	int i;
	int PaletteSize = 0, cbHeaderSize;

	//����ÿ������Ҫ����bit������RGBQUAD����Ĵ�С
	switch ( BitsPerPixel )	
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
		//����8λ��DIB����Ҫ��ɫ��
	case 15:
	case 16:
		BitsPerPixel = 16;
		break;
	}

	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize * sizeof ( RGBQUAD );
	if ( DibInfo ) delete[] DibInfo;
	//ΪBITMAPINFO�Ķ���DibInfo����һ���ڴ�ռ�
	DibInfo = (BITMAPINFO*) new char [ cbHeaderSize ]; 
	//��дBITMAPINFO
	DibInfo->bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );
	DibInfo->bmiHeader.biWidth = w;
	DibInfo->bmiHeader.biHeight = h;
	DibInfo->bmiHeader.biPlanes = 1;
	DibInfo->bmiHeader.biBitCount = short(BitsPerPixel);
	DibInfo->bmiHeader.biCompression = BI_RGB;
	DibInfo->bmiHeader.biSizeImage = BytesPerLine() * DibInfo->bmiHeader.biHeight;
	DibInfo->bmiHeader.biXPelsPerMeter =
		DibInfo->bmiHeader.biYPelsPerMeter = 3780;
	DibInfo->bmiHeader.biClrUsed = PaletteSize;
	DibInfo->bmiHeader.biClrImportant = PaletteSize;

	if ( PaletteSize )
	{
		//��RGBQUAD�����е�ÿ��Ԫ��ȫ����0
		for ( i = 0 ; i < PaletteSize ; i ++ )
		{
			DibInfo->bmiColors[i].rgbRed = 0;
			DibInfo->bmiColors[i].rgbGreen = 0;
			DibInfo->bmiColors[i].rgbBlue = 0;
			DibInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	//ΪDIB�����������ռ䣬��С��GetBodySize()����
	DibBits = (void*) new char[GetBodySize()];
	//�ж��Ƿ��ɵ�ɫ�壬�������palette�е�entry
	//���Ƶ�BITMAPINFO��RGBQUAD������
	if ( palette ) CopyPalette ( *palette );

	// ��DDB�е�ͼ����Ϣ���Ƶ�DIB��
	// GetDesktopWindow()�������洰�ڵľ��
	CWnd DesktopWnd;	
	DesktopWnd.Attach ( ::GetDesktopWindow() );
	CWindowDC dtpDC ( &DesktopWnd );
	// ��ɴ�DDB��DIB��ת������
	::GetDIBits( dtpDC.m_hDC,		
		HBITMAP ( ddb ),	 
		0,		  
		ddbinfo.bmHeight,	
		LPVOID ( DibBits ),	
		DibInfo,
		DIB_RGB_COLORS  
		);
	DesktopWnd.Detach();
}

MyDib::~MyDib()
{
	delete[] DibInfo;
	delete[] DibBits;
}

void* MyDib::GetBits() 
{
	return DibBits; 
}

int MyDib::GetBitsPerPixel () 
{ 
	if( !DibInfo )return 0;
	return DibInfo->bmiHeader.biBitCount; 
}

long MyDib::GetWidth() 
{ 
	return DibInfo->bmiHeader.biWidth; 
}

long MyDib::GetHeight() 
{ 
	return DibInfo->bmiHeader.biHeight; 
}

long MyDib::GetBodySize() 
{ 
	return BytesPerLine() * DibInfo->bmiHeader.biHeight; 
}

int MyDib::GetPaletteCount()
{
	int PaletteSize = 0;
	if (!DibInfo) return 0;
	switch ( DibInfo->bmiHeader.biBitCount )
	{
	case 1:
		PaletteSize = 2;
		break;
	case 4:
		PaletteSize = 16;
		break;
	case 8:
		PaletteSize = 256;
		break;
	}
	return PaletteSize;
}

long MyDib::BytesPerLine() 
{ 
	return ((((DibInfo->bmiHeader.biWidth * GetBitsPerPixel())+31)/32)*4);
}

int MyDib::SetToDC ( CDC& dc, CRect& src, CPoint& dst )
{
	return ::SetDIBitsToDevice(
		dc.m_hDC,	
		dst.x,	
		dst.y,	 
		src.Width(),	 
		src.Height(),	 
		src.left,	 
		src.top,	 
		0,	 
		DibInfo->bmiHeader.biHeight,	 
		DibBits,	 
		DibInfo,	 
		DIB_RGB_COLORS	 
		);	
}

int MyDib::StretchToDC ( CDC& dc, CRect& src, CRect& dst, DWORD rop )
{
	return ::StretchDIBits (
		dc.m_hDC,	
		dst.left,	 
		dst.top,	 
		dst.Width(),	 
		dst.Height(),	 
		src.left,	 
		src.top,	 
		src.Width(),	 
		src.Height(),	 
		DibBits,	 
		DibInfo,	
		DIB_RGB_COLORS,	 
		rop		
		);	
}

void MyDib::SaveDIB ( LPCTSTR dibName )
{
	//��DIBΪ������ʾ���󲢷���
	if(!DibInfo)
	{
		AfxMessageBox(L"DIB��Ϣ�����ڣ�");
		return;
	}

	//ͨ���ļ�������CFile����
	//���ļ���ʧ������ʾ���󲢷���
	CFile file;
	if(!file.Open(dibName, CFile::modeCreate | CFile::modeReadWrite))
	{
		AfxMessageBox(L"�����ļ�ʧ�ܣ�");
		return;
	}

	//Bitmap�ļ�ͷ
	BITMAPFILEHEADER bmfileheader;

	long size = BytesPerLine() * DibInfo->bmiHeader.biHeight;
	int PaletteSize = GetPaletteCount();

	//��дBMP�ļ�������
	bmfileheader.bfType = 0x4d42;//���ͱ�ʾ"BM"
	bmfileheader.bfReserved1 = bmfileheader.bfReserved2 = 0;//������0
	bmfileheader.bfOffBits = 
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+PaletteSize*sizeof( RGBQUAD );
	bmfileheader.bfSize = bmfileheader.bfOffBits + size;

	//���ļ�ͷ��λͼ��Ϣ����������д������ļ�
	file.Write ( &bmfileheader, sizeof(BITMAPFILEHEADER) );
	file.Write ( DibInfo, sizeof(BITMAPINFOHEADER)+PaletteSize*sizeof( RGBQUAD ) );
	file.Write ( DibBits, size );
	//�ر��ļ�
	file.Close();	
}

void MyDib::CopyPalette( CPalette& palette )
{
	int i;
	PALETTEENTRY *pe;

	if ( !DibInfo->bmiHeader.biBitCount ) return;
	if ( palette.GetEntryCount() != DibInfo->bmiHeader.biBitCount ) return;

	pe = new PALETTEENTRY[palette.GetEntryCount()];
	palette.GetPaletteEntries( 0, palette.GetEntryCount(), pe );

	for ( i = 0 ; i < palette.GetEntryCount() ; i ++ )
	{
		DibInfo->bmiColors[i].rgbRed = pe[i].peRed;
		DibInfo->bmiColors[i].rgbGreen = pe[i].peGreen;
		DibInfo->bmiColors[i].rgbBlue = pe[i].peBlue;
		DibInfo->bmiColors[i].rgbReserved = 0;
	}

	delete[] pe;
}

void MyDib::Minus()
{
	if (GetBitsPerPixel() <= 8)
	{
		for (int i = 0; i < GetPaletteCount(); ++i)
		{
			DibInfo->bmiColors[i].rgbRed = 255 - DibInfo->bmiColors[i].rgbRed;
			DibInfo->bmiColors[i].rgbGreen = 255 - DibInfo->bmiColors[i].rgbGreen;
			DibInfo->bmiColors[i].rgbBlue = 255 - DibInfo->bmiColors[i].rgbBlue;
			DibInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	else
	{
		BYTE *p_bmpBit = (BYTE *)DibBits;
		for (int i = 0; i < GetBodySize(); ++i)
		{
			p_bmpBit[i] = 255 - p_bmpBit[i];
		}
	}
}

void MyDib::histAve(int m_histAveArray[])
{
	BYTE *p_bmpBit = (BYTE *)DibBits;
	for (int i = 0; i < GetBodySize(); ++i)
	{
		p_bmpBit[i] = m_histAveArray[p_bmpBit[i]];
	}
}

void MyDib::histMatch(int sourceDist[], int objDist[])
{
	for (int i = 1; i < 256; ++i)
	{
		sourceDist[i] = sourceDist[i - 1] + sourceDist[i];
		objDist[i] = objDist[i - 1] + objDist[i];
	}

	int min, temp;
	int index[256] = { 0 };
	for (int i = 0; i < 256; ++i)
	{
		min = 0x3f3f3f3f;
		for (int j = 0; j < 256; ++j)
		{
			temp = abs(sourceDist[i] - objDist[j]);
			if (temp < min)
			{
				min = temp;
				index[i] = j;
			}
		}
	}

	BYTE *p_bmpBit = (BYTE *)DibBits;
	for (int i = 0; i < GetBodySize(); ++i)
	{
		p_bmpBit[i] = index[p_bmpBit[i]];
	}
}

void MyDib::LogStrech(double a, double b, double c)
{
	if (GetBitsPerPixel() <= 8)
	{
		int temp;
		for (int i = 0; i < GetPaletteCount(); ++i)
		{
			temp = (int)(a + log(DibInfo->bmiColors[i].rgbBlue + 1.0) / (b * log(c)));
			DibInfo->bmiColors[i].rgbBlue = (temp <= 255 ? temp : 255);
			temp = (int)(a + log(DibInfo->bmiColors[i].rgbGreen + 1.0) / (b * log(c)));
			DibInfo->bmiColors[i].rgbGreen = (temp <= 255 ? temp : 255);
			temp = (int)(a + log(DibInfo->bmiColors[i].rgbRed + 1.0) / (b * log(c)));
			DibInfo->bmiColors[i].rgbRed = (temp <= 255 ? temp : 255);
			DibInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	else
	{
		BYTE *p_bmpBit = (BYTE *)DibBits;
		int temp;
		for (int i = 0; i < GetBodySize(); ++i)
		{
			temp = (int)(a + log(p_bmpBit[i] + 1.0) / (b * log(c)));
			p_bmpBit[i] = (temp <= 255 ? temp : 255);
		}
	}
}

void MyDib::LinearStrech(double a, double b, double c, double d)
{
	if (GetBitsPerPixel() <= 8)
	{
		for (int i = 0; i < GetPaletteCount(); ++i)
		{
			if (DibInfo->bmiColors[i].rgbBlue <= a)
				DibInfo->bmiColors[i].rgbBlue = (int)((c / a) * DibInfo->bmiColors[i].rgbBlue);
			else if (a < DibInfo->bmiColors[i].rgbBlue && DibInfo->bmiColors[i].rgbBlue <= b)
				DibInfo->bmiColors[i].rgbBlue = (int)((d - c) / (b - a) * (DibInfo->bmiColors[i].rgbBlue - a) + c);
			else
				DibInfo->bmiColors[i].rgbBlue = (int)((255 - d) / (255 - b) * (DibInfo->bmiColors[i].rgbBlue - b) + d);
			
			if (DibInfo->bmiColors[i].rgbGreen <= a)
				DibInfo->bmiColors[i].rgbGreen = (int)((c / a) * DibInfo->bmiColors[i].rgbGreen);
			else if (a < DibInfo->bmiColors[i].rgbGreen && DibInfo->bmiColors[i].rgbGreen <= b)
				DibInfo->bmiColors[i].rgbGreen = (int)((d - c) / (b - a) * (DibInfo->bmiColors[i].rgbGreen - a) + c);
			else
				DibInfo->bmiColors[i].rgbGreen = (int)((255 - d) / (255 - b) * (DibInfo->bmiColors[i].rgbGreen - b) + d);

			if (DibInfo->bmiColors[i].rgbRed <= a)
				DibInfo->bmiColors[i].rgbRed = (int)((c / a) * DibInfo->bmiColors[i].rgbRed);
			else if (a < DibInfo->bmiColors[i].rgbRed && DibInfo->bmiColors[i].rgbRed <= b)
				DibInfo->bmiColors[i].rgbRed = (int)((d - c) / (b - a) * (DibInfo->bmiColors[i].rgbRed - a) + c);
			else
				DibInfo->bmiColors[i].rgbRed = (int)((255 - d) / (255 - b) * (DibInfo->bmiColors[i].rgbRed - b) + d);

			DibInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	else
	{
		BYTE *p_bmpBit = (BYTE *)DibBits;
		for (int i = 0; i < GetBodySize(); ++i)
		{
			if (p_bmpBit[i] <= a)
				p_bmpBit[i] = (int)((c / a) * p_bmpBit[i]);
			else if (a < p_bmpBit[i] && p_bmpBit[i] <= b)
				p_bmpBit[i] = (int)((d - c) / (b - a) * (p_bmpBit[i] - a) + c);
			else
				p_bmpBit[i] = (int)((255 - d) / (255 - b) * (p_bmpBit[i] - b) + d);
		}
	}
}

void MyDib::ExpStrech(double a, double b, double c)
{
	int temp;
	if (GetBitsPerPixel() <= 8)
	{
		for (int i = 0; i < GetPaletteCount(); ++i)
		{
			temp = (int)(pow(b, c * (DibInfo->bmiColors[i].rgbRed - a)) - 1);
			DibInfo->bmiColors[i].rgbRed = (temp <= 255 ? temp : 255);
			temp = (int)(pow(b, c * (DibInfo->bmiColors[i].rgbGreen - a)) - 1);
			DibInfo->bmiColors[i].rgbGreen = (temp <= 255 ? temp : 255);
			temp = (int)(pow(b, c * (DibInfo->bmiColors[i].rgbBlue - a)) - 1);
			DibInfo->bmiColors[i].rgbBlue = (temp <= 255 ? temp : 255);
			DibInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	else
	{
		BYTE *p_bmpBit = (BYTE *)DibBits;
		int temp;
		for (int i = 0; i < GetBodySize(); ++i)
		{
			temp = (int)(pow(b, c * (p_bmpBit[i] - a)) - 1);
			p_bmpBit[i] = (temp <= 255 ? temp : 255);
		}
	}
}

unsigned char MyDib::findMedianValue(unsigned char *pTemp, int tempLen)
{
	unsigned char medianValue;

	for (int j = 0; j < tempLen; ++j)
	{
		for (int i = j+1; i < tempLen; ++i)
		{
			if (pTemp[j] > pTemp[i])
			{
				medianValue = pTemp[i];
				pTemp[i] = pTemp[j];
				pTemp[j] = medianValue;
			}
		}
	}
	if ((tempLen & 1) > 0)
		medianValue = pTemp[(tempLen + 1) / 2];
	else
		medianValue = (pTemp[tempLen / 2] + pTemp[(tempLen + 1) / 2]);

	return medianValue;
}

void MyDib::medianFilter(int N)
{
	int modelCenterX = (N - 1) / 2;	//ģ������Ԫ��X����
	int modelCenterY = (N - 1) / 2;	//ģ������Ԫ��Y����
	int imgHeight = GetHeight();
	int imgWidth = GetWidth();
	unsigned char *tempBits = NULL;
	unsigned char *p_bmpBit = (BYTE *)DibBits;
	unsigned char *value;			//ָ���˲�������
	unsigned char *pDst;
	int linebyte = BytesPerLine();

	tempBits = new unsigned char[GetBodySize()];
	for (int i = 0; i < GetBodySize(); ++i)
	{
		tempBits[i] = p_bmpBit[i];
	}

	value = new unsigned char[N * N];
	for (int i = modelCenterY; i < imgHeight - N + modelCenterY + 1; ++i)
	{
		for (int j = modelCenterX; j < imgWidth - N + modelCenterX + 1; ++j)
		{
			pDst = &tempBits[(imgHeight - 1 - i) * linebyte + j];
			for (int k = 0; k < N; ++k)
			{
				for (int l = 0; l < N; ++l)
				{
					value[k * N + l] = tempBits[(imgHeight - l - i + modelCenterY - k) * linebyte + (j - modelCenterX + l)];
				}
			}
			*pDst = findMedianValue(value, N * N);
		}
	}

	for (int i = 0; i < GetBodySize(); ++i)
	{
		p_bmpBit[i] = tempBits[i];
	}

	delete tempBits;
	delete value;
}

void MyDib::avgFilter(int N)
{
	int modelCenterX = (N - 1) / 2;	//ģ������Ԫ��X����
	int modelCenterY = (N - 1) / 2;	//ģ������Ԫ��Y����
	int imgHeight = GetHeight();
	int imgWidth = GetWidth();
	unsigned char *tempBits = NULL;
	unsigned char *p_bmpBit = (BYTE *)DibBits;
	double value;
	unsigned char *pDst;
	int linebyte = BytesPerLine();

	tempBits = new unsigned char[GetBodySize()];
	for (int i = 0; i < GetBodySize(); ++i)
	{
		tempBits[i] = p_bmpBit[i];
	}

	for (int i = modelCenterY; i < imgHeight - N + modelCenterY + 1; ++i)
	{
		for (int j = modelCenterX; j < imgWidth - N + modelCenterX + 1; ++j)
		{
			pDst = &tempBits[(imgHeight - 1 - i) * linebyte + j];
			value = 0;
			for (int k = 0; k < N; ++k)
			{
				for (int l = 0; l < N; ++l)
				{
					value += tempBits[(imgHeight - l - i + modelCenterY - k) * linebyte + (j - modelCenterX + l)];
				}
			}
			*pDst = value / (N * N);
		}
	}

	for (int i = 0; i < GetBodySize(); ++i)
	{
		p_bmpBit[i] = tempBits[i];
	}

	delete tempBits;
}

void MyDib::OnGsph(int N)
{
	int modelCenterX = (N - 1) / 2;	//ģ������Ԫ��X����
	int modelCenterY = (N - 1) / 2;	//ģ������Ԫ��Y����
	int imgHeight = GetHeight();
	int imgWidth = GetWidth();
	unsigned char *tempBits = NULL;
	unsigned char *p_bmpBit = (BYTE *)DibBits;
	double value;
	unsigned char *pDst;
	int linebyte = BytesPerLine();

	tempBits = new unsigned char[GetBodySize()];
	for (int i = 0; i < GetBodySize(); ++i)
	{
		tempBits[i] = p_bmpBit[i];
	}

	for (int i = modelCenterY; i < imgHeight - N + modelCenterY + 1; ++i)
	{
		for (int j = modelCenterX; j < imgWidth - N + modelCenterX + 1; ++j)
		{
			pDst = &tempBits[(imgHeight - 1 - i) * linebyte + j];
			value = 0;
			for (int k = 0; k < N; ++k)
			{
				for (int l = 0; l < N; ++l)
				{
					value += tempBits[(imgHeight - l - i + modelCenterY - k) * linebyte + (j - modelCenterX + l)];
				}
			}
			*pDst = value / (N * N);
		}
	}

	for (int i = 0; i < GetBodySize(); ++i)
	{
		p_bmpBit[i] = tempBits[i];
	}

	delete tempBits;
}

void MyDib::OnGsphColor(int N)
{
	int modelCenterX = (N - 1) / 2;	//ģ������Ԫ��X����
	int modelCenterY = (N - 1) / 2;	//ģ������Ԫ��Y����
	int imgHeight = GetHeight();
	int imgWidth = GetWidth();
	unsigned char *tempBits = NULL;
	unsigned char *p_bmpBit = (BYTE *)DibBits;
	double value;
	unsigned char *pDst;
	int linebyte = BytesPerLine();

	tempBits = new unsigned char[GetBodySize()];
	for (int i = 0; i < GetBodySize(); ++i)
	{
		tempBits[i] = p_bmpBit[i];
	}

	int temp;
	if (GetBitsPerPixel() <= 8)
	{
		for (int i = 1; i < GetPaletteCount() - 1; ++i)
		{
			DibInfo->bmiColors[i].rgbRed = (DibInfo->bmiColors[i - 3].rgbRed + DibInfo->bmiColors[i].rgbRed + DibInfo->bmiColors[i + 3].rgbRed) / 3;
			DibInfo->bmiColors[i].rgbGreen = (DibInfo->bmiColors[i - 3].rgbGreen + DibInfo->bmiColors[i].rgbGreen + DibInfo->bmiColors[i + 3].rgbGreen) / 3;
			DibInfo->bmiColors[i].rgbGreen = (DibInfo->bmiColors[i - 3].rgbBlue + DibInfo->bmiColors[i].rgbBlue + DibInfo->bmiColors[i + 3].rgbBlue) / 3;
		}
	}
	else
	{
		BYTE *p_bmpBit = (BYTE *)DibBits;
		int temp;
		for (int i = 0; i < GetBodySize(); ++i)
		{
			p_bmpBit[i] = (p_bmpBit[i - 3] + p_bmpBit[i] + p_bmpBit[i + 3]) / 3;
		}

	}


	delete tempBits;
}