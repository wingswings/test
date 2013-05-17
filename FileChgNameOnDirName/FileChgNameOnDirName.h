// FileChgNameOnDirName.h : main header file for the FILECHGNAMEONDIRNAME application
//

#if !defined(AFX_FILECHGNAMEONDIRNAME_H__2D9EC938_6BE8_434A_ACE0_9B5088C2BDB0__INCLUDED_)
#define AFX_FILECHGNAMEONDIRNAME_H__2D9EC938_6BE8_434A_ACE0_9B5088C2BDB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFileChgNameOnDirNameApp:
// See FileChgNameOnDirName.cpp for the implementation of this class
//

class CFileChgNameOnDirNameApp : public CWinApp
{
public:
	CFileChgNameOnDirNameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileChgNameOnDirNameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFileChgNameOnDirNameApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILECHGNAMEONDIRNAME_H__2D9EC938_6BE8_434A_ACE0_9B5088C2BDB0__INCLUDED_)
