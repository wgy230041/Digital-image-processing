#pragma once
class Histogram
{
public:
	//ֱ��ͼͳ������
	int m_histArray[256];

	//���⻯�����������
	int m_histAveArray[256];

public:
	//���������Ĺ��캯��
	Histogram();

	//��������
	~Histogram();

	//ͳ�ƻҶ�ͼ���ֱ��ͼ����
	void computeHist(BYTE *DibBits, int size);

	//ֱ��ͼ���⻯����
	void computeHistAve(BYTE *DibBits, int size);
};

