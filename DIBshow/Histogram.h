#pragma once
class Histogram
{
public:
	//直方图统计数据
	int m_histArray[256];

	//均衡化后的像素数据
	int m_histAveArray[256];

public:
	//不带参数的构造函数
	Histogram();

	//析构函数
	~Histogram();

	//统计灰度图像的直方图数据
	void computeHist(BYTE *DibBits, int size);

	//直方图均衡化计算
	void computeHistAve(BYTE *DibBits, int size);
};

