
// DIBshowDoc.h : CDIBshowDoc 类的接口
//


#pragma once

#include "MyDib.h"
#include "Histogram.h"

class CDIBshowDoc : public CDocument
{
protected: // 仅从序列化创建
	CDIBshowDoc();
	DECLARE_DYNCREATE(CDIBshowDoc);

// 特性
public:
	MyDib* mDib;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CDIBshowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnHist();
	afx_msg void OnHistave();
	afx_msg void OnStrechLog();
	afx_msg void OnReverse();
	afx_msg void OnStrechLinear();
	afx_msg void OnStrechExp();
	afx_msg void OnHistmatch();
	afx_msg void Onmedfilter3x3();
	afx_msg void Onmedfilter5x5();
	afx_msg void Onavgfilter3x3();
	afx_msg void Onavgfilter5x5();
	afx_msg void OnFft();
	afx_msg void OnIfft();
};
