#pragma once

class MyDib : public CObject
{
	/*成员变量*/
private:
	//DIB文件
	BITMAPINFO*	DibInfo;
	//像素数组
	void*		DibBits;	

	/*成员函数*/
public:

	//构造函数
	MyDib ( LPCTSTR  dibName);//从文件中读入DIB，参数dibName代表文件全路径名
	MyDib ( CBitmap& ddb, CPalette* palette );//从DDB中获得DIB
	MyDib ( int Width, int Height, int BitsPerPixel );//创建新的DIB

	//析构函数
	~MyDib ( );

	//返回像素数组
	void*		GetBits();
	//返回每个像素的位数
	int			GetBitsPerPixel ( );
	//获得图像的高
	long		GetWidth();
	//获得图像的宽
	long		GetHeight();
	//获得图像每行扫描线所需的字节数
	long		BytesPerLine();
	//获得调色板
	CPalette*	GetPalette();
	//复制调色板
	void		CopyPalette(CPalette&);
	//获得调色板的大小
	int	GetPaletteCount();
	//获得像素数组的大小
	long		GetBodySize();
	//在指定DC上缩放输出
	int			StretchToDC ( CDC& dc, CRect& src, CRect& dst, DWORD rop = SRCCOPY );
	//在指定DC上输出，不改变大小
	int			SetToDC ( CDC& dc, CRect& src, CPoint& dst );
	//保存位图，参数dibName代表文件全路径名
	void		SaveDIB ( LPCTSTR dibName );
	//位图取反运算
	void		Minus();
	//位图直方图均衡化
	void		histAve(int m_histAveArray[]);
	//位图直方图匹配
	void		histMatch(int sourceDist[], int objDist[]);
	//对数对比度拉伸
	void		LogStrech(double a, double b, double c);
	//线性对比度拉伸
	void		LinearStrech(double a, double b, double c, double d);
	//指数对比度拉伸
	void		ExpStrech(double a, double b, double c);
	//查找数组中的中值
	unsigned char findMedianValue(unsigned char *pTemp, int tempLen);
	//中值滤波
	void medianFilter(int N);
	//均值滤波
	void avgFilter(int N);
	//高斯模糊
	void OnGsph(int N);
	void OnGsphColor(int N);
};
