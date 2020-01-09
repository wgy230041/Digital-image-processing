
// shiyan2View.h: Cshiyan2View 类的接口
//

#pragma once

#include "MyDIB.h"

class Cshiyan2View : public CView
{
private:
	MyDIB * m_dib;
protected: // 仅从序列化创建
	Cshiyan2View() noexcept;
	DECLARE_DYNCREATE(Cshiyan2View)

// 特性
public:
	Cshiyan2Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cshiyan2View();
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
	long long r[256];	//存储原图灰度值的频数
	double pr[256];		//存储原图灰度值的频率
	int r_s[256];		//存储原图到均衡化图像的映射
	long long s[256];	//存储均衡化图像的灰度值的频数
	double ps[256];		//存储原图的灰度值的频率和
	long long z[256];	//存储规格化图像的灰度值的频数
	double pz[256];		//存储标准图像的灰度值的频率
	int r_z[256];		//存储原图到规格化图像的映射
};

#ifndef _DEBUG  // shiyan2View.cpp 中的调试版本
inline Cshiyan2Doc* Cshiyan2View::GetDocument() const
   { return reinterpret_cast<Cshiyan2Doc*>(m_pDocument); }
#endif

