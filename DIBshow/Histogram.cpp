#include "stdafx.h"
#include "Histogram.h"
#include "MyDib.h"
#include "Hist.h"

Histogram::Histogram()
{
	memset(m_histArray, 0, 256 * sizeof(int));
}


Histogram::~Histogram()
{
}

void Histogram::computeHist(BYTE *DibBits, int size)
{
	memset(m_histArray, 0, 256 * sizeof(int));
	for (int i = 0; i < size; ++i)
		m_histArray[(int)DibBits[i]] += 1;
}

void Histogram::computeHistAve(BYTE *DibBits, int size)
{
	memset(m_histAveArray, 0, 256 * sizeof(int));
	computeHist(DibBits, size);

	int sum = 0;
	for (int i = 0; i < 256; ++i)
	{
		sum += m_histArray[i];
		m_histAveArray[i] = (double)sum * 255 / size + 0.5;
	}
}
