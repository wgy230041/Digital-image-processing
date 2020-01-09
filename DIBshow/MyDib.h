#pragma once

class MyDib : public CObject
{
	/*��Ա����*/
private:
	//DIB�ļ�
	BITMAPINFO*	DibInfo;
	//��������
	void*		DibBits;	

	/*��Ա����*/
public:

	//���캯��
	MyDib ( LPCTSTR  dibName);//���ļ��ж���DIB������dibName�����ļ�ȫ·����
	MyDib ( CBitmap& ddb, CPalette* palette );//��DDB�л��DIB
	MyDib ( int Width, int Height, int BitsPerPixel );//�����µ�DIB

	//��������
	~MyDib ( );

	//������������
	void*		GetBits();
	//����ÿ�����ص�λ��
	int			GetBitsPerPixel ( );
	//���ͼ��ĸ�
	long		GetWidth();
	//���ͼ��Ŀ�
	long		GetHeight();
	//���ͼ��ÿ��ɨ����������ֽ���
	long		BytesPerLine();
	//��õ�ɫ��
	CPalette*	GetPalette();
	//���Ƶ�ɫ��
	void		CopyPalette(CPalette&);
	//��õ�ɫ��Ĵ�С
	int	GetPaletteCount();
	//�����������Ĵ�С
	long		GetBodySize();
	//��ָ��DC���������
	int			StretchToDC ( CDC& dc, CRect& src, CRect& dst, DWORD rop = SRCCOPY );
	//��ָ��DC����������ı��С
	int			SetToDC ( CDC& dc, CRect& src, CPoint& dst );
	//����λͼ������dibName�����ļ�ȫ·����
	void		SaveDIB ( LPCTSTR dibName );
	//λͼȡ������
	void		Minus();
	//λͼֱ��ͼ���⻯
	void		histAve(int m_histAveArray[]);
	//λͼֱ��ͼƥ��
	void		histMatch(int sourceDist[], int objDist[]);
	//�����Աȶ�����
	void		LogStrech(double a, double b, double c);
	//���ԶԱȶ�����
	void		LinearStrech(double a, double b, double c, double d);
	//ָ���Աȶ�����
	void		ExpStrech(double a, double b, double c);
	//���������е���ֵ
	unsigned char findMedianValue(unsigned char *pTemp, int tempLen);
	//��ֵ�˲�
	void medianFilter(int N);
	//��ֵ�˲�
	void avgFilter(int N);
	//��˹ģ��
	void OnGsph(int N);
	void OnGsphColor(int N);
};
