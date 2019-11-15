
// MFC_basic.h : main header file for the MFC_basic application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCbasicApp:
// See MFC_basic.cpp for the implementation of this class
//

class CMFCbasicApp : public CWinApp
{
public:
	CMFCbasicApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCbasicApp theApp;
