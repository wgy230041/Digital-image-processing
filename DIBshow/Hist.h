#pragma once
#include "resource.h"
#include "Histogram.h"
#include "DIBshowDoc.h"

// CHist 对话框

class CHist : public CDialogEx
{
	DECLARE_DYNAMIC(CHist)

public:
	CHist(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHist();
	void setHist(Histogram given_hist);

// 对话框数据
	enum { IDD = IDD_DIALOG_Hist };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	Histogram hist;
};
