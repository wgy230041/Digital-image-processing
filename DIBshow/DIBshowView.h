
// DIBshowView.h : CDIBshowView ��Ľӿ�
//

#pragma once

class CDIBshowView : public CScrollView
{
protected: // �������л�����
	CDIBshowView();
	DECLARE_DYNCREATE(CDIBshowView)

// ����
public:
	CDIBshowDoc* GetDocument() const;

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
	virtual ~CDIBshowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DIBshowView.cpp �еĵ��԰汾
inline CDIBshowDoc* CDIBshowView::GetDocument() const
   { return reinterpret_cast<CDIBshowDoc*>(m_pDocument); }
#endif

