#pragma once

#include "stdafx.h"
#include <complex>

using namespace std;

void FFT_1D(complex<double> *pCTData, complex<double> *pCFData, int nLevel);

void IFFT_1D(complex<double> *pCFData, complex<double> *pCTData, int nLevel);

void DIBFFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> *pCFData);

void IFFT_2D(complex<double> * pCFData, complex<double> *pCTData, int nWidth, int nHeight);