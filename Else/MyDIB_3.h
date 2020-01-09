#pragma once
#pragma pack(2) 
struct BMPFileHeader
{
	//定义BMP文件头
	WORD bfType;		//文件类型，必须是字符串“BM”
	DWORD bfSize;		//指定文件大小
	WORD bfReserved1;	//保留字，不考虑
	WORD bfReserved2;	//保留字，不考虑
	DWORD bfOffBits;	//从文件头到位图数据的偏移字节数
};

struct BMPInfoHeader
{
	//定义BMP信息头
	DWORD biSize;			//该结构的长度，40个字节
	LONG biWidth;			//图像的宽度，单位是像素
	LONG biHeight;			//图像的高度，单位是像素
	WORD biPlanes;			//必须是1
	WORD biBitCount;		//颜色位数，如1，4，8，24
	DWORD biCompression;	//压缩类型，如BI_RPG,BI_RLE4
	DWORD biSizeImage;		//实际位图数据占用字节数
	LONG biXPelsPerMeter;	//水平分辨率
	LONG biYPelsPerMeter;	//垂直分辨率
	DWORD biClrUsed;		//实际使用的颜色
	DWORD biClrImportant;	//重要的颜色数
};

struct RGBQuad
{
	//定义调色板
	BYTE rgbBlue;		//该颜色的蓝色分量
	BYTE rgbGreen;		//该颜色的绿色分量
	BYTE rgbRed;		//该颜色的红色分量
	BYTE rgbReserved;	//保留值，不考虑
};

class MyDIB
{
public:
	BMPFileHeader m_BMPfileheader;	//位图头文件
	BMPInfoHeader m_BMPinfoheader;	//位图信息头
	RGBQuad * Quad;					//调色板
	unsigned char * m_BMPdata;		//位图的真实图像数据

	int m_width;					//图像宽度
	int m_height;					//图像高度
	int real_size;					//图像数据字节长度
	int real_width;					//每行的字节数（4的倍数）
	int padding;					//为满足每行4个字节的填充

	unsigned char *m_pImage;
	DWORD ndwSizeRaw;

	MyDIB();
	~MyDIB();

	void Read(const CString& filename);
	void Write(const CString& filename);
};
