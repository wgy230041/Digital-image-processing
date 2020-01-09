#include "stdafx.h"
#include "MyDIB.h"


MyDIB::MyDIB()
{
}


MyDIB::~MyDIB()
{
}

void MyDIB::Read(const CString & filename)
{
	CFile file;
	file.Open(filename, CFile::modeRead);
	ndwSizeRaw = (DWORD)file.GetLength();			//读取文件大小
	m_pImage = new unsigned char[ndwSizeRaw];		//数据指针
	file.Read(m_pImage, ndwSizeRaw);
	file.Close();

	file.Open(filename, CFile::modeRead);
	file.Read(&m_BMPfileheader, 14);				//读取文件数据到指针中
	file.Read(&m_BMPinfoheader, 40);
	if (m_BMPinfoheader.biBitCount == 24)
	{
		Quad = NULL;
	}
	else
	{
		int pow = 1, n = m_BMPinfoheader.biBitCount;
		while (n--)
		{
			pow *= 2;
		}
		Quad = new RGBQuad[pow];
		file.Read(Quad, sizeof(RGBQuad)*pow);
	}

	m_width = m_BMPinfoheader.biWidth;
	m_height = m_BMPinfoheader.biHeight;

	if (m_BMPinfoheader.biBitCount == 24)
	{
		padding = m_width * 3 % 4;
		if (padding)					//不满足每行4个字节的倍数
		{
			padding = 4 - padding;		//每行的填充字节数
		}
		real_size = m_height * (m_width * 3 + padding);
	}
	else
	{
		int len = (4 * 8) / m_BMPinfoheader.biBitCount;
		padding = m_width % len;
		if (padding)					//不满足每行4个字节的倍数
		{
			padding = len - padding;	//每行的填充像素数
		}
		real_size = m_height * (m_width + padding) * m_BMPinfoheader.biBitCount / 8;
	}

	m_BMPdata = new unsigned char[real_size];
	file.Read(m_BMPdata, real_size);
	file.Close();									//关闭文件
}

void MyDIB::Write(const CString & filename)
{
	CFile RawFile;
	RawFile.Open(filename, CFile::modeCreate | CFile::modeWrite);

	try {
		RawFile.Write((LPVOID)m_pImage, ndwSizeRaw);		//将数据写入文件（实现“另存为”效果）
	}
	catch (CException * pe)
	{
		pe->Delete();
		AfxMessageBox(_T("write error"));
		RawFile.Close();
	}
	RawFile.Close();							//关闭文件
}

