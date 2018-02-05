#include "stdafx.h"

#include "..\shared\text.h"
#include "..\shared\vector.h"
#include "..\shared\vector_sorted.h"

#include "parser.h"
#include "parserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CParserApp        theApp;

extern DWORD      dwKeepError = 0;

BEGIN_MESSAGE_MAP(CParserApp,CWinApp)
   //{{AFX_MSG_MAP(CParserApp)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   // DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG
   ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/* ******************************************************************** **
** @@ CParserApp::CParserApp()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

CParserApp::CParserApp()
{
}

/* ******************************************************************** **
** @@ CParserApp::InitInstance()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

BOOL CParserApp::InitInstance()
{
   // Only One Instance of this App is available !
   _hMutext = CreateMutex(NULL,TRUE,"QUIZ_PARSER");

   if (!_hMutext)
   {
      return FALSE;
   }

   if (GetLastError() == ERROR_ALREADY_EXISTS)
   {
      return FALSE;
   }

   // Standard initialization
   // If you are not using these features and wish to reduce the size
   // of your final executable, you should remove from the following
   // the specific initialization routines you do not need.

   #ifdef _AFXDLL
   Enable3dControls();        // Call this when using MFC in a shared DLL
   #else
   Enable3dControlsStatic();  // Call this when linking to MFC statically
   #endif

   CParserDlg     dlg;

   m_pMainWnd = &dlg;
   
   dlg.DoModal();

   // Since the dialog has been closed, return FALSE so that we exit the
   // application, rather than start the application's message pump.
   return FALSE;
}

/* ******************************************************************** **
** @@ CParserApp::ExitInstance
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

int CParserApp::ExitInstance()
{
   CloseHandle(_hMutext);
   _hMutext = INVALID_HANDLE_VALUE;

   return CWinApp::ExitInstance();
}

/* ******************************************************************** **
** @@                   End of File
** ******************************************************************** */
