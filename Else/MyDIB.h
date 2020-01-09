#pragma once
#pragma pack(2) 
struct BMPFileHeader
{
	//����BMP�ļ�ͷ
	WORD bfType;		//�ļ����ͣ��������ַ�����BM��
	DWORD bfSize;		//ָ���ļ���С
	WORD bfReserved1;	//�����֣�������
	WORD bfReserved2;	//�����֣�������
	DWORD bfOffBits;	//���ļ�ͷ��λͼ���ݵ�ƫ���ֽ���
};

struct BMPInfoHeader
{
	//����BMP��Ϣͷ
	DWORD biSize;			//�ýṹ�ĳ��ȣ�40���ֽ�
	LONG biWidth;			//ͼ��Ŀ�ȣ���λ������
	LONG biHeight;			//ͼ��ĸ߶ȣ���λ������
	WORD biPlanes;			//������1
	WORD biBitCount;		//��ɫλ������1��4��8��24
	DWORD biCompression;	//ѹ�����ͣ���BI_RPG,BI_RLE4
	DWORD biSizeImage;		//ʵ��λͼ����ռ���ֽ���
	LONG biXPelsPerMeter;	//ˮƽ�ֱ���
	LONG biYPelsPerMeter;	//��ֱ�ֱ���
	DWORD biClrUsed;		//ʵ��ʹ�õ���ɫ
	DWORD biClrImportant;	//��Ҫ����ɫ��
};

struct RGBQuad
{
	//�����ɫ��
	BYTE rgbBlue;		//����ɫ����ɫ����
	BYTE rgbGreen;		//����ɫ����ɫ����
	BYTE rgbRed;		//����ɫ�ĺ�ɫ����
	BYTE rgbReserved;	//����ֵ��������
};

class MyDIB
{
public:
	BMPFileHeader m_BMPfileheader;	//λͼͷ�ļ�
	BMPInfoHeader m_BMPinfoheader;	//λͼ��Ϣͷ
	RGBQuad * Quad;					//��ɫ��
	unsigned char * m_BMPdata;		//λͼ����ʵͼ������

	int m_width;					//ͼ����
	int m_height;					//ͼ��߶�
	int real_size;					//ͼ�������ֽڳ���
	int real_width;					//ÿ�е��ֽ�����4�ı�����
	int padding;					//Ϊ����ÿ��4���ֽڵ����

	unsigned char *m_pImage;
	DWORD ndwSizeRaw;

	MyDIB();
	~MyDIB();

	void Read(const CString& filename);
	void Write(const CString& filename);
};
