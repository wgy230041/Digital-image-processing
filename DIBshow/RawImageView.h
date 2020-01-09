
// RawImageView.h : CRawImageView ��Ľӿ�
//

#pragma once


class CRawImageView : public CScrollView
{
protected: // �������л�����
	CRawImageView();
	DECLARE_DYNCREATE(CRawImageView)

// ����
public:
	CRawImageDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CRawImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // RawImageView.cpp �еĵ��԰汾
inline CRawImageDoc* CRawImageView::GetDocument() const
   { return reinterpret_cast<CRawImageDoc*>(m_pDocument); }
#endif

