
// MFC_basicView.cpp : implementation of the CMFCbasicView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFC_basic.h"
#endif

#include "MFC_basicDoc.h"
#include "MFC_basicView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCbasicView

IMPLEMENT_DYNCREATE(CMFCbasicView, CView)

BEGIN_MESSAGE_MAP(CMFCbasicView, CView)
END_MESSAGE_MAP()

// CMFCbasicView construction/destruction

CMFCbasicView::CMFCbasicView() noexcept
{
	// TODO: add construction code here

}

CMFCbasicView::~CMFCbasicView()
{
}

BOOL CMFCbasicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCbasicView drawing

void CMFCbasicView::OnDraw(CDC* /*pDC*/)
{
	CMFCbasicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMFCbasicView diagnostics

#ifdef _DEBUG
void CMFCbasicView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCbasicView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCbasicDoc* CMFCbasicView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCbasicDoc)));
	return (CMFCbasicDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCbasicView message handlers
