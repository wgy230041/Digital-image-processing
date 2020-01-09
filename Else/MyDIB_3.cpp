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
	ndwSizeRaw = (DWORD)file.GetLength();			//��ȡ�ļ���С
	m_pImage = new unsigned char[ndwSizeRaw];		//����ָ��
	file.Read(m_pImage, ndwSizeRaw);
	file.Close();

	file.Open(filename, CFile::modeRead);
	file.Read(&m_BMPfileheader, 14);				//��ȡ�ļ����ݵ�ָ����
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
		if (padding)					//������ÿ��4���ֽڵı���
		{
			padding = 4 - padding;		//ÿ�е�����ֽ���
		}
		real_size = m_height * (m_width * 3 + padding);
	}
	else
	{
		int len = (4 * 8) / m_BMPinfoheader.biBitCount;
		padding = m_width % len;
		if (padding)					//������ÿ��4���ֽڵı���
		{
			padding = len - padding;	//ÿ�е����������
		}
		real_size = m_height * (m_width + padding) * m_BMPinfoheader.biBitCount / 8;
	}

	m_BMPdata = new unsigned char[real_size];
	file.Read(m_BMPdata, real_size);
	file.Close();									//�ر��ļ�
}

void MyDIB::Write(const CString & filename)
{
	CFile RawFile;
	RawFile.Open(filename, CFile::modeCreate | CFile::modeWrite);

	try {
		RawFile.Write((LPVOID)m_pImage, ndwSizeRaw);		//������д���ļ���ʵ�֡����Ϊ��Ч����
	}
	catch (CException * pe)
	{
		pe->Delete();
		AfxMessageBox(_T("write error"));
		RawFile.Close();
	}
	RawFile.Close();							//�ر��ļ�
}

