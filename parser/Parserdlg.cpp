/* ******************************************************************** **
** @@ Parser
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Dscr   :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include <malloc.h>
#include <string.h>

#include "..\shared\vector.h"
#include "..\shared\vector_sorted.h"
#include "..\shared\file.h"
#include "..\shared\text.h"

#include "parser.h"
#include "parserdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif 

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

#define TITLE_START_MARKER             "<question>"
#define TITLE_END_MARKER               "</question>"

#define SIGNATURE_START_MARKER         "<answer>"
#define SIGNATURE_END_MARKER           "</answer>"

#define RECORD_START_MARKER            "<simple>"
#define RECORD_END_MARKER              "</simple>"

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

HANDLE   _hReject = INVALID_HANDLE_VALUE;

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ CMP_Num()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static int CMP_Num(const void** const pKey1,const void** const pKey2)
{
   ListLine**  p1 = (ListLine**)pKey1;
   ListLine**  p2 = (ListLine**)pKey2;

   return (*p1)->_iNum - (*p2)->_iNum;
}

/* ******************************************************************** **
** @@ CMP_Title()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static int CMP_Title(const void** const pKey1,const void** const pKey2)
{
   ListLine**  p1 = (ListLine**)pKey1;
   ListLine**  p2 = (ListLine**)pKey2;

   int      iRes = stricmp((*p1)->_pszTitle,(*p2)->_pszTitle);

   if (iRes)
   {
      return iRes;
   }
   else
   {
      return (*p1)->_iNum - (*p2)->_iNum;
   }
}

/* ******************************************************************** **
** @@ CMP_Signature()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int CMP_Signature(const void** const pKey1,const void** const pKey2)
{
   ListLine**  p1 = (ListLine**)pKey1;
   ListLine**  p2 = (ListLine**)pKey2;

   return stricmp((char*)((*p1)->_pszSignature),(char*)((*p2)->_pszSignature));
}

/* ******************************************************************** **
** @@ CParserDlg::CParserDlg()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

CParserDlg::CParserDlg(CWnd* pParent /*=NULL*/)
:  CDialog(CParserDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParserDlg)
   //}}AFX_DATA_INIT
   // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
          
   _pImageList = NULL;

   _sFind = _T("");

   _iCurrent = 0;
   _iTotal   = 0;

   _bOpen   = false;
   _bOwner  = false;
   _bClosed = false;
   _bReject = false;

   LOGFONT     LF;

   memset(&LF,0,sizeof(LOGFONT));            // zero out structure
  
   LF.lfHeight = 16;                         // request a 16-pixel-height font
   strcpy(LF.lfFaceName,"Courier New");      // request a face name "Courier New"

   VERIFY(_Font.CreateFontIndirect(&LF));    // create the font
}

/* ******************************************************************** **
** @@ CParserDlg::DoDataExchange()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParserDlg)
   DDX_Control(pDX, IDC_SPIN_GO, m_Spin);
   DDX_Control(pDX, IDC_EDT_FIND, m_Find);
   DDX_Control(pDX, IDC_RB_NAME, m_Name);
   DDX_Control(pDX, IDC_RB_HASH, m_Hash);
   DDX_Control(pDX, IDC_LIST, m_List);
   //}}AFX_DATA_MAP
}

/* ******************************************************************** **
** @@ CParserDlg::MESSAGE_MAP()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

BEGIN_MESSAGE_MAP(CParserDlg, CDialog)
   //{{AFX_MSG_MAP(CParserDlg)
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   ON_EN_CHANGE(IDC_EDT_FILE, OnChangeEdtFile)
   ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
   ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
   ON_BN_CLICKED(IDC_BTN_FIRST, OnBtnFirst)
   ON_BN_CLICKED(IDC_BTN_LAST, OnBtnLast)
   ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
   ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
   ON_EN_CHANGE(IDC_EDT_FIND, OnChangeEdtFind)
   ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetdispinfoList)
   ON_NOTIFY(HDN_ITEMCLICK,0,OnHdnItemclickLst)
   ON_WM_CLOSE()
   ON_WM_DESTROY()
   ON_WM_SYSCOMMAND()
   ON_BN_CLICKED(IDC_RB_NAME, OnRbName)
   ON_BN_CLICKED(IDC_RB_HASH, OnRbHash)
   ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
   ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
   ON_EN_SETFOCUS(IDC_EDT_FIND, OnSetfocusEdtFind)
   ON_BN_CLICKED(IDC_BTN_GO, OnBtnGo)
   ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, OnKeydownList)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* ******************************************************************** **
** @@ CParserDlg::OnInitDialog()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

BOOL CParserDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   // Set the icon for this dialog.  The framework does this automatically
   //  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);       // Set big icon
   SetIcon(m_hIcon, FALSE);      // Set small icon
   
   CenterWindow();

   m_Spin.SetBase(10);

   _pSysMenu = GetSystemMenu(FALSE);

   if (_pSysMenu)
   {
      _pSysMenu->AppendMenu(MF_SEPARATOR);
   }

   m_Name.SetCheck(1);
   
   m_List.InsertColumn(0,"##",       LVCFMT_LEFT, 50,0);
   m_List.InsertColumn(1,"Title",    LVCFMT_LEFT,600,1);
   m_List.InsertColumn(2,"Signature",LVCFMT_LEFT,300,2);

   m_List.SetFont(&_Font,FALSE);

   DWORD    dwStyle = m_List.GetExtendedStyle();

   dwStyle |= LVS_OWNERDATA | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_FLATSB | LVS_EX_GRIDLINES;
   
   m_List.SetExtendedStyle(dwStyle);

   char     pszTemp[MAX_PATH];

   wsprintf(pszTemp,"[ %3d / %3d ]",_iCurrent,_iTotal);

   SetDlgItemText(IDC_STT_COUNTER,pszTemp);

   GetDlgItem(IDC_BTN_FIRST)->   EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_LAST)->    EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_PREV)->    EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_NEXT)->    EnableWindow(FALSE);
   GetDlgItem(IDC_RB_NAME)->     EnableWindow(FALSE);
   GetDlgItem(IDC_RB_HASH)->     EnableWindow(FALSE);
   GetDlgItem(IDC_EDT_FIND)->    EnableWindow(FALSE);
   GetDlgItem(IDC_EDT_GO)->      EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_GO)->      EnableWindow(FALSE);
   GetDlgItem(IDC_SPIN_GO)->     EnableWindow(FALSE);

   _bClosed = false;
   _bReject = false;

   _DB.Resize(1024);
   _DB.Delta(128);

   _DB.SetSorter(CMP_Signature);
   _DB.QSort(); 
   
   ModifyHeaderItems(2);

   return TRUE;  // return TRUE  unless you set the focus to a control
}
      
/* ******************************************************************** **
** @@ CParserDlg::OnSysCommand()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
/*
   switch (nID & 0xFFF0)
   {
   }
*/

   CDialog::OnSysCommand(nID,lParam);
}

/* ******************************************************************** **
** @@ CParserDlg::OnPaint()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.

void CParserDlg::OnPaint() 
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      CDialog::OnPaint();
   }
}

/* ******************************************************************** **
** @@ CParserDlg::OnQueryDragIcon()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

// The system calls this to obtain the cursor to display while the user drags
// the minimized window.

HCURSOR CParserDlg::OnQueryDragIcon()
{
   return (HCURSOR) m_hIcon;
}

/* ******************************************************************** **
** @@ CParserDlg::OnChangeEdtFile()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnChangeEdtFile() 
{
   if (_bOwner)
   {
      _bOwner = false;
      return;
   }

   GetDlgItemText(IDC_EDT_FILE,_pszUserDB,sizeof(_pszUserDB) - 1);

   if (*_pszUserDB == '"')
   {
      CString     sTemp = _pszUserDB;

      sTemp.Remove('"');

      strcpy(_pszUserDB,(LPCTSTR)sTemp);
   }

   Fetch();
}

/* ******************************************************************** **
** @@ CParserDlg::OnBtnFile()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnBtnFile() 
{
   DWORD    dwFlags =   OFN_ENABLESIZING     |
                        OFN_FILEMUSTEXIST    |
                        OFN_HIDEREADONLY     |
                        OFN_NOREADONLYRETURN |
                        OFN_PATHMUSTEXIST;
                                    
   char     pszFilter[MAX_PATH] = "TXT (test.txt)|test.txt|"
                                  "TXT (*.txt)|*.txt|"
                                  "ALL (*.*)|*.*||";

   CFileDialog    UserDB(TRUE,NULL,"test.txt",dwFlags,pszFilter);

   if (UserDB.DoModal() == IDOK)
   {
      _bOwner = true;  

      strcpy(_pszUserDB,(LPCTSTR)UserDB.GetPathName());
      SetDlgItemText(IDC_EDT_FILE,_pszUserDB);

      Fetch();
   }
}

/* ******************************************************************** **
** @@ CParserDlg::OnClickList()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   if (!_DB.Count())
   {
      return;
   }

   NM_LISTVIEW*   pNMListView = (NM_LISTVIEW*)pNMHDR;
   
   _iCurrent = pNMListView->iItem;

   UpdateCounter();

   *pResult = 0;
}

/* ******************************************************************** **
** @@ CParserDlg::OnBtnKill()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnBtnKill() 
{
   if (!_DB.Count())
   {
      return;
   }

   int      iSelected = m_List.GetSelectedCount();

   ASSERT(iSelected != -1);

   if (iSelected <= 0)
   {
      MessageBox("Nothing to do ...","No entry selected !",MB_OK | MB_ICONSTOP);
      return;         
   }

   POSITION    Pos = m_List.GetFirstSelectedItemPosition();

   if (!Pos)
   {
      MessageBox("Nothing to do ...","No entry selected !",MB_OK | MB_ICONSTOP);
      return;         
   }
   
   char     pszTemp[MAX_PATH];
         
   wsprintf(pszTemp,"Do you confirm deletion of %d selected item(s) ?",iSelected);

   if (MessageBox(pszTemp,"Last chance !",MB_OK | MB_YESNO | MB_ICONQUESTION) != IDYES)
   {
      return;        
   }

   int*  pRemArr = new int[iSelected];

   if (!pRemArr)
   {
      // Error !
      return;
   }

   memset(pRemArr,0,sizeof(int) * iSelected);

   for (int kk = 0; Pos; ++kk)
   {
      pRemArr[kk] = m_List.GetNextSelectedItem(Pos);
   }

   // Delete in Reverse order
   for (int ii = (iSelected - 1); ii >= 0; --ii)
   {
      int   iIdx = pRemArr[ii];

      ListLine*      pItem = (ListLine*)_DB.At(iIdx);

      if (pItem)
      {
         _DB.RemoveAt(iIdx);
         delete pItem;
         pItem = NULL;
      }
   }

   delete pRemArr;
   pRemArr = NULL;

   Refresh();
}

/* ******************************************************************** **
** @@ CParserDlg::OnBtnFirst()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnBtnFirst() 
{
   if (!_DB.Count())
   {
      return;
   }

   _iCurrent = 0;

   Go();
}

/* ******************************************************************** **
** @@ CParserDlg::OnBtnLast()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnBtnLast() 
{
   if (!_DB.Count())
   {
      return;
   }

   _iCurrent = _DB.Count() - 1;

   Go();
}

/* ******************************************************************** **
** @@ CParserDlg::OnBtnPrev()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnBtnPrev() 
{
   if (!_DB.Count())
   {
      return;
   }

   if (_iCurrent <= 0)
   {
      MessageBeep(0);
      return;
   }

   --_iCurrent;

   Go();
}

/* ******************************************************************** **
** @@ CParserDlg::OnBtnNext()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnBtnNext() 
{
   if (!_DB.Count())
   {
      return;
   }

   if (_iCurrent >= (int)(_DB.Count() - 1))
   {
      MessageBeep(0);
      return;
   }

   ++_iCurrent;

   Go();
}

/* ******************************************************************** **
** @@ CParserDlg::OnChangeEdtFind()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnSetfocusEdtFind() 
{
   OnChangeEdtFind();
}

/* ******************************************************************** **
** @@ CParserDlg::OnChangeEdtFind()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnChangeEdtFind() 
{
   ClearSelection();

   GetDlgItemText(IDC_EDT_FIND,_sFind);

   if (_sFind.IsEmpty())
   {
      return;
   }
   
   if (!_DB.Count())
   {
      return;
   }
}

/* ******************************************************************** **
** @@ CParserDlg::Reject()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::RejectReport(const ListLine* const pItem)
{
   if (_hReject == INVALID_HANDLE_VALUE)
   {
      // Error !
      return;
   }
}

/* ******************************************************************** **
** @@ CParserDlg::Fetch()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::Fetch()
{
   GetDlgItem(IDC_EDT_GO)-> EnableWindow(FALSE);
   GetDlgItem(IDC_SPIN_GO)->EnableWindow(FALSE);

   m_List.DeleteAllItems();
   m_List.SetItemCountEx(0,LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);

   if (!_pszUserDB || !*_pszUserDB)
   {
      return;
   }

   FILE*    pIn = fopen(_pszUserDB,"rt");

   if (!pIn)
   {
      // Error
      return;
   }

   char*    pTextBuf = new char[MAX_PATH + 1];

   if (!pTextBuf)
   {
      // Error !
      return;
   }

   // Set properly sort order !
   _DB.SetSorter(CMP_Signature);
   _DB.QSort();

   ModifyHeaderItems(2);

   ListLine*      pItem = NULL;

   CWaitCursor    Waiter;

   _bReject = false;

   _hReject = CreateFile(REJECT_LIST_FILENAME,CREATE_ALWAYS,0);

   while (fgets(pTextBuf,MAX_PATH,pIn))
   {
      pTextBuf[MAX_PATH - 1] = 0; // ASCIIZ

      DWORD    dwEOL = strcspn((char*)pTextBuf,"\r\n");

      pTextBuf[dwEOL] = 0;  // Remove EOL chars

      if (!strlen(pTextBuf))
      {
         // Empty string !
         continue;
      }

      if (!strncmp(pTextBuf,RECORD_START_MARKER,sizeof(RECORD_START_MARKER) - 1))
      {
         // First Line
         if (pItem && !pItem->_bInserted)
         {
            // Prevent orphaned items
            _bReject = true;
            RejectReport(pItem);
  
            delete pItem;
            pItem = NULL;
         }
         
         pItem = new ListLine;

         if (!pItem)
         {
            // Error
            delete pTextBuf;
            pTextBuf = NULL;
            Waiter.Restore();
            return;
         }

         memset(pItem,0,sizeof(ListLine));
      }
      if (!strncmp(pTextBuf,RECORD_END_MARKER,sizeof(RECORD_END_MARKER) - 1))
      {
         pItem->_iNum = ++_iTotal;

         if (_DB.Insert(pItem) == -1)
         {
            delete pItem;
            pItem = NULL;
            --_iTotal;
         }

         if (pItem)
         {
            pItem->_bInserted = true;
         }
      }
      else if (!strnicmp(pTextBuf,TITLE_START_MARKER,sizeof(TITLE_START_MARKER) - 1))
      {
         char*    pTitleStart = strstr(pTextBuf,TITLE_START_MARKER);

         if (pTitleStart)
         {
            char*    pTitleEnd = strstr(pTextBuf,TITLE_END_MARKER);

            if (pTitleEnd)
            {
               int   iLen = pTitleEnd - pTitleStart - (sizeof(TITLE_START_MARKER) - 1);

               ASSERT(iLen > 0);

               iLen = min(iLen,MAX_PATH);

               if (iLen > 0)
               {
                  strncpy(pItem->_pszTitle,pTitleStart + sizeof(TITLE_START_MARKER) - 1,iLen);
                  pItem->_pszTitle[iLen] = 0; // Ensure ASCIIZ !
               }
            }
         }
      }
      else if (!strncmp(pTextBuf,SIGNATURE_START_MARKER,sizeof(SIGNATURE_START_MARKER) - 1))
      {
         char*    pSignatureStart = strstr(pTextBuf,SIGNATURE_START_MARKER);

         if (pSignatureStart)
         {
            char*    pSignatureEnd = strstr(pTextBuf,SIGNATURE_END_MARKER);

            if (pSignatureEnd)
            {
               int   iLen = pSignatureEnd - pSignatureStart - (sizeof(TITLE_START_MARKER) - 1);

               ASSERT(iLen > 0);

               iLen = min(iLen,MAX_PATH);

               if (iLen > 0)
               {
                  strncpy(pItem->_pszSignature,pSignatureStart + sizeof(TITLE_START_MARKER) - 1,iLen);
                  pItem->_pszSignature[iLen] = 0; // Ensure ASCIIZ !
               }
            }
         }
      }
   }

   if (pItem && !pItem->_bInserted)
   {
      // Prevent last orphaned items
      _bReject = true;
      RejectReport(pItem);

      delete pItem;
      pItem = NULL;
   }

   if (_hReject != INVALID_HANDLE_VALUE)
   {
      CloseHandle(_hReject);
      _hReject = INVALID_HANDLE_VALUE;
   }

   delete pTextBuf;
   pTextBuf = NULL;

   delete pTextBuf;
   pTextBuf = NULL;

   fclose(pIn);
   pIn = NULL;

   Waiter.Restore();
   Refresh();

   if (_bReject)
   {
      char     pszTemp[MAX_PATH];
      
      wsprintf(pszTemp,"Some incomplete records was rejected.\nSee [%s] file for more detailes.",REJECT_LIST_FILENAME);

      MessageBox(pszTemp,"Fetch Info",MB_OK | MB_ICONSTOP);
   }
   else
   {
      DeleteFile(REJECT_LIST_FILENAME);
   }
}

/* ******************************************************************** **
** @@ CParserDlg::OnGetdispinfoList()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   LV_DISPINFO*      pDispInfo = (LV_DISPINFO*)pNMHDR;

   LV_ITEM*          pItem = &(pDispInfo)->item;

   int      iIndex = pItem->iItem;

   if (pItem->mask & LVIF_TEXT)
   {
      ListLine*      pEntry = (ListLine*)_DB.At(iIndex);

      if (pEntry)
      {
         switch (pItem->iSubItem)
         {
            case 0: 
            {
               wsprintf(pItem->pszText,"%5ld",pEntry->_iNum);
               break;
            }
            case 1: 
            {
               strncpy(pItem->pszText,pEntry->_pszTitle,MAX_PATH);
               pItem->pszText[MAX_PATH] = 0;
               break;
            }
            case 2: 
            {
               // Fucking M$ Limits about 700 chars
               strncpy(pItem->pszText,pEntry->_pszSignature,MAX_PATH);
               pItem->pszText[MAX_PATH] = 0;
               break;
            }
         }
      }
   }
   
   *pResult = 0;
}

/* ******************************************************************** **
** @@ CParserDlg::OnItemclickList()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::OnHdnItemclickLst(NMHDR* pNMHDR, LRESULT* pResult) 
{
   // Q250614  !  HOWTO: Sort Items in a CListCtrl in Report View
   //
   // MFC in Visual C++ 6.0 has a problem with header notifications for the ListView control. Although a handler can be added, in the
   // current version it isn't called.
   //
   // The problem here is that the notification doesn't actually originate from the ListView control; instead, the Header control
   // created by the ListView sends the notification. The message map entry listed above does not work. The fix is simple, however,
   // since the Header control always has an ID of 0, the macro can be edited to work correctly

   *pResult = 0;

   if (!_DB.Count())
   {
      return;
   }

   NM_LISTVIEW*      pNMListView = (NM_LISTVIEW*)pNMHDR;

   switch(pNMListView->iItem)
   {
      case 0:
      {
         _DB.SetSorter(CMP_Num); 
         _DB.QSort(); 
         ModifyHeaderItems(0);
         break;
      }
      case 1:
      {
         _DB.SetSorter(CMP_Title); 
         _DB.QSort(); 
         ModifyHeaderItems(1);
         break;
      }
      case 2:
      {
         _DB.SetSorter(CMP_Signature); 
         _DB.QSort(); 
         ModifyHeaderItems(2);
         break;
      }
   }
   
   m_List.RedrawItems(0,_DB.Count());

   ResetSelection();
}

/* ******************************************************************** **
** @@ CParserDlg::Go()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::Go()
{
   m_List.SetItemState (_iCurrent,LVIS_SELECTED,LVIS_SELECTED);
   m_List.EnsureVisible(_iCurrent,FALSE);

   m_List.SetSelectionMark(_iCurrent);

   UpdateCounter();
}

/* ******************************************************************** **
** @@ CParserDlg::OnDestroy()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::OnDestroy()
{
   _bClosed = true;

   CDialog::OnDestroy();
   Cleanup();
}

/* ******************************************************************** **
** @@ CParserDlg::OnClose()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::OnClose()
{
   _bClosed = true;

   Cleanup();
   CDialog::OnClose();
}

/* ******************************************************************** **
** @@ CParserDlg::Cleanup()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::Cleanup()
{
   if (_pImageList)
   {
      delete _pImageList;
      _pImageList = NULL;
   }

   int   iCnt = _DB.Count();

   for (int ii = iCnt - 1; ii >= 0; --ii)
   {
      ListLine*      pItem = (ListLine*)_DB.At(ii);

      if (pItem)
      {
         _DB.RemoveAt(ii);
         delete pItem;
         pItem = NULL;
      }
   }

   // Reinitialize !
   _DB.Resize(1024);
   _DB.Delta(128);

   _DB.SetSorter(CMP_Signature);
   _DB.QSort(); 

   ModifyHeaderItems(2);
}

/* ******************************************************************** **
** @@ CParserDlg::OnBtnMake()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update : 15 Apr 2006
** @  Notes  :
** ******************************************************************** */

/* ******************************************************************** **
** @@ CParserDlg::OnRbName()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::OnRbName() 
{
}

/* ******************************************************************** **
** @@ CParserDlg::OnRbHash()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::OnRbHash() 
{
}

/* ******************************************************************** **
** @@ CParserDlg::Refresh()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::Refresh()
{
   _iTotal = _DB.Count();

   m_Spin.SetRange32(1,_iTotal);
   m_Spin.SetPos(_iCurrent);

   m_List.SetItemCountEx(_iTotal,LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
   m_List.RedrawWindow();

   char     pszTemp[MAX_PATH];

   wsprintf(pszTemp,"[ %3d / %3d ]",_iCurrent,_iTotal);

   SetDlgItemText(IDC_STT_COUNTER,pszTemp);

//   OnBtnFirst();
    
   if (_iTotal)
   {
      GetDlgItem(IDC_BTN_FIRST)->  EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_LAST)->   EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_PREV)->   EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_NEXT)->   EnableWindow(TRUE);
      GetDlgItem(IDC_RB_NAME)->    EnableWindow(TRUE);
      GetDlgItem(IDC_EDT_GO)->     EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_GO)->     EnableWindow(TRUE);
      GetDlgItem(IDC_SPIN_GO)->    EnableWindow(TRUE);
      GetDlgItem(IDC_EDT_FIND)->   EnableWindow(TRUE);
   }
   else
   {
      GetDlgItem(IDC_BTN_FIRST)->  EnableWindow(FALSE);
      GetDlgItem(IDC_BTN_LAST)->   EnableWindow(FALSE);
      GetDlgItem(IDC_BTN_PREV)->   EnableWindow(FALSE);
      GetDlgItem(IDC_BTN_NEXT)->   EnableWindow(FALSE);
      GetDlgItem(IDC_RB_NAME)->    EnableWindow(FALSE);
      GetDlgItem(IDC_RB_HASH)->   EnableWindow(FALSE);
      GetDlgItem(IDC_EDT_GO)->     EnableWindow(FALSE);
      GetDlgItem(IDC_BTN_GO)->     EnableWindow(FALSE);
      GetDlgItem(IDC_SPIN_GO)->    EnableWindow(FALSE);
      GetDlgItem(IDC_EDT_FIND)->   EnableWindow(FALSE);
   }
}

/* ******************************************************************** **
** @@ CParserDlg::ClearSelection()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::ClearSelection()
{
   int      iCount = m_List.GetSelectedCount();
   int      iIndex = -1;

   // Update all of the selected items.
   if (iCount > 0)
   {
      for (int ii = 0; ii < iCount; ++ii)
      {
         iIndex = m_List.GetNextItem(iIndex,LVNI_SELECTED);
         ASSERT(iIndex != -1);
         m_List.SetItemState(iIndex,0,LVIS_SELECTED);
      }
   }

   ResetSelection();
}

/* ******************************************************************** **
** @@ CParserDlg::ResetSelection()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::ResetSelection()
{
   if (!_DB.Count())
   {
      return;
   }

   UpdateCounter();
}

/* ******************************************************************** **
** @@ CParserDlg::UpdateCounter()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::UpdateCounter()
{
   int      iNum = m_List.GetSelectionMark();

   if (iNum == -1)
   {
      GetDlgItem(IDC_EDT_GO)->     EnableWindow(FALSE);
      GetDlgItem(IDC_SPIN_GO)->    EnableWindow(FALSE);
      return;
   }

   GetDlgItem(IDC_EDT_GO)->     EnableWindow(TRUE);
   GetDlgItem(IDC_SPIN_GO)->    EnableWindow(TRUE);

   ListLine*      pEntry = (ListLine*)_DB.At(iNum);

   if (pEntry)
   {
      CString  sNum = _T("");
   
      _iTotal = _DB.Count();

      sNum.Format("%d / %d",pEntry->_iNum,_iTotal);
   
      m_Spin.SetRange32(1,_iTotal);
      m_Spin.SetPos(pEntry->_iNum);

      SetDlgItemText(IDC_STT_COUNTER,(LPCTSTR)sNum);
   }
}

/* ******************************************************************** **
** @@ CParserDlg::SearchName()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::SearchName()
{
   char     pszPattern[MAX_PATH + 1];

   strncpy(pszPattern,(LPCTSTR)_sFind,MAX_PATH);

   strupr(pszPattern);

   int   iCnt = _DB.Count();

   for (int ii = 0; ii < iCnt; ++ii)
   {
      ListLine*      pItem = (ListLine*)_DB.At(ii);

      if (pItem)
      {
         char     pszTitle[MAX_PATH + 1];

         strncpy(pszTitle,pItem->_pszTitle,MAX_PATH);

         strupr(pszTitle);

         char*    pStr = strstr(pszTitle,pszPattern);

         if (pStr)
         {
            _iCurrent = ii;
            Go();
            return;
         }
      }
   }

   MessageBeep(MB_ICONEXCLAMATION);
   _sFind.SetAt(_sFind.GetLength() - 1,0);

   // Dirty trick to set Cursor beyond of the text !
   m_Find.SetSel(0,-1,FALSE);
   m_Find.ReplaceSel((LPCTSTR)_sFind,TRUE);
}

/* ******************************************************************** **
** @@ CParserDlg::SearchStart()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::SearchStart()
{
   char     pszPattern[MAX_PATH + 1];

   strncpy(pszPattern,(LPCTSTR)_sFind,MAX_PATH);

   strupr(pszPattern);

   int   iCnt = _DB.Count();

   for (int ii = 0; ii < iCnt; ++ii)
   {
      ListLine*      pItem = (ListLine*)_DB.At(ii);

      if (pItem)
      {
         int      iSize = strlen(pszPattern);

         if (!strncmp((char*)pItem->_pszSignature,pszPattern,iSize))
         {
            _iCurrent = ii;
            Go();
            return;
         }
      }
   }

   MessageBeep(MB_ICONEXCLAMATION);
   _sFind.SetAt(_sFind.GetLength() - 1,0);

   // Dirty trick to set Cursor beyond of the text !
   m_Find.SetSel(0,-1,FALSE);
   m_Find.ReplaceSel((LPCTSTR)_sFind,TRUE);
}


/* ******************************************************************** **
** @@ CParserDlg::OnRclickList()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#pragma warning(disable: 4100)   // 'pNMHDR' : unreferenced formal parameter
void CParserDlg::OnRclickList(NMHDR* pNMHDR,LRESULT* pResult) 
{
   int      iNum = m_List.GetSelectionMark();

   if (iNum == -1)
   {
      return;
   }

   CMenu    Menu;

   if (!Menu.CreatePopupMenu())
   {
      return;
   }

   Menu.AppendMenu(MF_SEPARATOR);
   Menu.AppendMenu(MF_STRING,IDM_KILL,"Kill");

   POINT    Point; 
   
   GetCursorPos(&Point);

   Menu.TrackPopupMenu(TPM_CENTERALIGN,Point.x,Point.y,this,NULL);

   *pResult = 0;
}

/* ******************************************************************** **
** @@ CParserDlg::OnCommand()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BOOL CParserDlg::OnCommand(WPARAM wParam,LPARAM lParam) 
{
   switch (wParam)
   {
      case IDM_KILL:
      {
         OnBtnKill();
         break;
      }
   }
   
   return CDialog::OnCommand(wParam, lParam);
}

/* ******************************************************************** **
** @@ CParserDlg::OnDblclkList()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#pragma warning(disable: 4100)   // 'pNMHDR' : unreferenced formal parameter
void CParserDlg::OnDblclkList(NMHDR* pNMHDR,LRESULT* pResult) 
{
   *pResult = 0;

   if (!_DB.Count())
   {
      return;
   }
}

/* ******************************************************************** **
** @@ CParserDlg::CopyData()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CParserDlg::CopyData(const BYTE* const pBuf,DWORD dwSize)
{
   if (!OpenClipboard())
   { 
      return FALSE; 
   }

   EmptyClipboard(); 

   // Allocate a global memory object for the text. 
   HGLOBAL     hMem = GlobalAlloc(GMEM_MOVEABLE,dwSize); 

   if (!hMem) 
   { 
      CloseClipboard(); 
      return false; 
   } 

   // Lock the handle and copy the text to the buffer. 
   LPVOID      pMem = GlobalLock(hMem); 

   memcpy(pMem,pBuf,dwSize); 
   
   GlobalUnlock(hMem); 

   // Place the handle on the clipboard. 
   SetClipboardData(CF_TEXT,hMem); 

   CloseClipboard(); 
   GlobalFree(hMem);

   return true;
}

/* ******************************************************************** **
** @@ CParserDlg::ModifyHeaderItems()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CParserDlg::ModifyHeaderItems(int iArrowPos)
{
   if (_bClosed)
   {
      // Don't create image list !
      return;
   }

   // create image list for header items
   if (!_pImageList)
   {
      _pImageList = new CImageList();

      ASSERT(_pImageList != NULL); 

      if (!_pImageList)
      {
         return;
      }

      CParserApp*    pApp = (CParserApp*)AfxGetApp();

      _pImageList->Create(8,16,ILC_COLOR8,2,0);
      _pImageList->Add(pApp->LoadIcon(IDI_ARROW));
      _pImageList->Add(pApp->LoadIcon(IDI_EMPTY));

      // Retrieve embedded header control
      CHeaderCtrl*      pHdrCtrl = m_List.GetHeaderCtrl();

      pHdrCtrl->SetImageList(_pImageList);
   }

   CHeaderCtrl*      pHdrCtrl = m_List.GetHeaderCtrl();

   // Set Icon to each header item
   int      iCnt = pHdrCtrl->GetItemCount();

   HD_ITEM     Item;

   TCHAR    pszText[MAX_PATH + 1];

   Item.mask       = HDI_TEXT |  HDI_IMAGE | HDI_FORMAT;
   Item.pszText    = pszText;
   Item.cchTextMax = MAX_PATH;

   for (int ii = 0; ii < iCnt; ++ii)
   {
      pHdrCtrl->GetItem(ii,&Item);

      Item.iImage = ii == iArrowPos  ?  0  :  1;
      Item.fmt    = HDF_LEFT | HDF_BITMAP_ON_RIGHT | HDF_BITMAP | HDF_IMAGE | HDF_STRING;
      
      pHdrCtrl->SetItem(ii,&Item);
   }
}

/* ******************************************************************** **
** @@ CParserDlg::OnBtnGo()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::OnBtnGo() 
{
   _DB.SetSorter(CMP_Num); 
   _DB.QSort(); 
   
   ModifyHeaderItems(0);

   int   iCnt = _DB.Count();

   char     pszTemp[MAX_PATH + 1];

   GetDlgItemText(IDC_EDT_GO,pszTemp,MAX_PATH);

   int   iPos = atoi(pszTemp);

   iPos = max(1,iPos);

   ClearSelection();

   for (int ii = 0; ii < iCnt; ++ii)
   {
      ListLine*      pItem = (ListLine*)_DB.At(ii);

      if (pItem)
      {
         if (pItem->_iNum == iPos)
         {
            _iCurrent = ii;
            Go();
            return;
         }
      }
   }

   m_List.RedrawItems(0,_DB.Count());
   ResetSelection();
}

/* ******************************************************************** **
** @@ CParserDlg::OnKeydownList()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CParserDlg::OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;

   LV_KEYDOWN*    pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

   switch (pLVKeyDow->wVKey)  
   {
      case VK_INSERT:
      {
         OnBtnFile();
         break;
      }
      case VK_DELETE:
      {
         OnBtnKill();
         break;
      }
   }
}

/* ******************************************************************** **
**                That's All
** ******************************************************************** */
