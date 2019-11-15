
// MFC_basicView.h : interface of the CMFCbasicView class
//

#pragma once


class CMFCbasicView : public CView
{
protected: // create from serialization only
	CMFCbasicView() noexcept;
	DECLARE_DYNCREATE(CMFCbasicView)

// Attributes
public:
	CMFCbasicDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMFCbasicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MFC_basicView.cpp
inline CMFCbasicDoc* CMFCbasicView::GetDocument() const
   { return reinterpret_cast<CMFCbasicDoc*>(m_pDocument); }
#endif

