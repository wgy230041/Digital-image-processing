
// DIBshow.h : DIBshow Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDIBshowApp:
// �йش����ʵ�֣������ DIBshow.cpp
//

class CDIBshowApp : public CWinAppEx
{
public:
	CDIBshowApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDIBshowApp theApp;
