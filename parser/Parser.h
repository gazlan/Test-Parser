#ifndef  _AFX_PARSER_H_
#define  _AFX_PARSER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"      // main symbols

#define REJECT_LIST_FILENAME           "reject.lst"

/* ******************************************************************** **
** @@ struct ListLine
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

struct ListLine
{
   int            _iNum;
   bool           _bInserted;
   char           _pszTitle    [MAX_PATH + 1];
   char           _pszSignature[MAX_PATH + 1];
};

class CParserApp : public CWinApp
{
   private:

      HANDLE      _hMutext;

   public:

         CParserApp();

   // Overrides
      // ClassWizard generated virtual function overrides
      //{{AFX_VIRTUAL(CParserApp)
      public:
      virtual BOOL   InitInstance();
      virtual int    ExitInstance();
      //}}AFX_VIRTUAL

   // Implementation

      //{{AFX_MSG(CParserApp)
         // NOTE - the ClassWizard will add and remove member functions here.
         //    DO NOT EDIT what you see in these blocks of generated code !
      //}}AFX_MSG
      DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
