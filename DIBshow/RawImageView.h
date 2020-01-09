
// RawImageView.h : CRawImageView 类的接口
//

#pragma once


class CRawImageView : public CScrollView
{
protected: // 仅从序列化创建
	CRawImageView();
	DECLARE_DYNCREATE(CRawImageView)

// 特性
public:
	CRawImageDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CRawImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	BOOL WriteRawFile(LPVOID lpvBits, DWORD ndwSizeRaw);
	unsigned char*m_pImage;
	int m_Nsize,m_Msize;
	DWORD ndwSizeRaw;
};

#ifndef _DEBUG  // RawImageView.cpp 中的调试版本
inline CRawImageDoc* CRawImageView::GetDocument() const
   { return reinterpret_cast<CRawImageDoc*>(m_pDocument); }
#endif

