#include "stdafx.h"
#include "Fourier.h"
#include <complex>
#include <math.h>
#include <memory.h>

using namespace std;

void FFT_1D(complex<double> *pCTData, complex<double> *pCFData, int nLevel)
{
	int i, j, k;
	double PI = 3.1415926;
	int nCount = (int)pow(2, nLevel);
	int nBtFlyLen = 0;
	double dAngle;

	complex<double> *pCW;
	pCW = new complex<double>[nCount / 2];

	for (i = 0; i < nCount / 2; ++i)
	{
		dAngle = -2 * PI * i / nCount;
		pCW[i] = complex<double>(cos(dAngle), sin(dAngle));
	}

	complex<double> *pCWork1, *pCWork2;

	pCWork1 = new complex<double>[nCount];
	pCWork2 = new complex<double>[nCount];

	complex<double> *pCTmp;

	memcpy(pCWork1, pCTData, sizeof(complex<double>) * nCount);

	int nInter = 0;

	for (k = 0; k < nLevel; ++k)
	{
		for (j = 0; j < (int)pow(2, k); ++j)
		{
			nBtFlyLen = (int)pow(2, (nLevel - k));

			for (i = 0; i < nBtFlyLen / 2; ++i)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] = pCWork1[i + nInter] + pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] = (pCWork1[i + nInter] - pCWork1[i + nInter + nBtFlyLen / 2]) * pCW[(int)(i * pow(2, k))];

			}
		}

		pCTmp = pCWork1;
		pCWork1 = pCWork2;
		pCWork2 = pCTmp;
	}

	for (j = 0; j < nCount; ++j)
	{
		nInter = 0;
		for (i = 0; i < nLevel; ++i)
		{
			if (j & (1 << i))
				nInter += 1 << (nLevel - i - 1);
		}
		pCFData[j] = pCWork1[nInter];
	}
	
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW = NULL;
	pCWork1 = NULL;
	pCWork2 = NULL;
}

void IFFT_1D(complex<double> *pCFData, complex<double> *pCTData, int nLevel)
{
	int i;
	int nCount = (int)pow(2, nLevel);
	complex<double> *pCWork;
	pCWork = new complex<double>[nCount];

	memcpy(pCWork, pCTData, nLevel);

	FFT_1D(pCWork, pCTData, nLevel);

	for (i = 0; i < nCount; ++i)
		pCTData[i] = complex<double>(pCTData[i].real() / nCount, -pCTData[i].imag() / nCount);
	
	delete pCWork;
	pCWork = NULL;
}

void DIBFFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> *pCFData)
{
	int x, y;
	double dTmpOne, dTmpTwo;
	int nTransWidth, nTransHeight;

	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	int nXLev, nYLev;

	nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
	nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

	for (y = 0; y < nTransHeight; y++)
	{
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev);
	}

	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x];
		}
	}

	for (x = 0; x < nTransWidth; x++)
	{
		FFT_1D(&pCTData[x * nTransHeight], &pCFData[x * nTransHeight], nYLev);
	}

	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransWidth * y + x] = pCFData[nTransHeight * x + y];
		}
	}
	memcpy(pCTData, pCFData, sizeof(complex<double>) * nTransHeight * nTransWidth);
}

void IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight)
{
	int x;
	int y;
	int a, b;

	double dTmpOne;
	double dTmpTwo;

	int nTransWidth;
	int nTransHeight;

	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	complex<double> *pCWork = new complex<double>[nTransWidth * nTransHeight];
	
	complex<double> *pCTmp;
	a = nTransWidth;
	b = nTransHeight;
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCFData[nTransWidth * y + x];
			pCWork[nTransWidth * y + x] = complex<double>(pCTmp->real(), -pCTmp->imag());
			
			//¹ýÂËµôµÍÆµ
			if ((x < a / 16 && y < b / 16) || (x > 15 * a / 16 && y < b / 16) || (x < a / 16 && y > 15 * b / 16) || (x > 15 * a / 16 && y > 15 * b / 16))
				pCWork[nTransWidth * y + x] = 0;

			//¹ýÂËµô¸ßÆµ
			/*if ((x > a/16 && x < 15*a/16) && (y > b/16 && y < 15*b/16))
				pCWork[nTransWidth * y + x] = 0;*/
		}
	}

	::DIBFFT_2D(pCWork, nWidth, nHeight, pCTData);

	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCTData[nTransWidth * y + x];
			pCTData[nTransWidth * y + x] =
				complex<double>(pCTmp->real() / (nTransWidth*nTransHeight),
				-pCTmp->imag() / (nTransWidth*nTransHeight));
		}
	}
	delete pCWork;
	pCWork = NULL;
}