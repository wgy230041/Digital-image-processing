
// DIBshowDoc.h : CDIBshowDoc ��Ľӿ�
//


#pragma once

#include "MyDib.h"
#include "Histogram.h"

class CDIBshowDoc : public CDocument
{
protected: // �������л�����
	CDIBshowDoc();
	DECLARE_DYNCREATE(CDIBshowDoc);

// ����
public:
	MyDib* mDib;

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CDIBshowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
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
