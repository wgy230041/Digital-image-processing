#pragma once
#include "resource.h"
#include "Histogram.h"
#include "DIBshowDoc.h"

// CHist �Ի���

class CHist : public CDialogEx
{
	DECLARE_DYNAMIC(CHist)

public:
	CHist(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHist();
	void setHist(Histogram given_hist);

// �Ի�������
	enum { IDD = IDD_DIALOG_Hist };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	Histogram hist;
};
