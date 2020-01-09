
// shiyan2View.h: Cshiyan2View 类的接口
//

#pragma once

#include "MyDIB.h"
#include <algorithm>
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
	//unsigned char * m_pImage;			//指向图像数据区指针
	//int m_Nsize, m_Msize;				//图像长宽
	//DWORD ndwSizeRaw;					//图像大小
	BYTE gs(unsigned char ** rgb, int i, int j, int p);
	BYTE zh(unsigned char ** rgb, int i, int j, int p);
	int gs3[3][3] = { {1,2,1},{2,4,2},{1,2,1} };
	int gs5[5][5] = { {1, 4, 7, 4, 1}, {4, 16, 26, 16, 4}, {7, 26, 41, 26, 7}, {4, 16, 26, 16, 4}, {1, 4, 7, 4, 1} };
	unsigned char arr[25];
};

#ifndef _DEBUG  // shiyan2View.cpp 中的调试版本
inline Cshiyan2Doc* Cshiyan2View::GetDocument() const
   { return reinterpret_cast<Cshiyan2Doc*>(m_pDocument); }
#endif

