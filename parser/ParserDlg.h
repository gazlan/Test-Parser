#ifndef  _AFX_PARSERDLG_H_
#define  _AFX_PARSERDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* ******************************************************************** **
** @@ class CParserDlg : public CDialog
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

class CParserDlg : public CDialog
{
   private:

      CString                    _sFind;
      CFont                      _Font;
      CMenu*                     _pSysMenu;
      int                        _iCurrent;
      bool                       _bOpen;
      char                       _pszUserDB[MAX_PATH + 1];
      bool                       _bOwner;
      bool                       _bClosed;
      bool                       _bReject;

   public:

      int                        _iTotal;
      SortedVector               _DB;

   // Construction
   public:

      CParserDlg(CWnd* pParent = NULL);   // standard constructor

      void  ModifyHeaderItems(int iArrowPos);
      void  Refresh();

   // Dialog Data
      //{{AFX_DATA(CParserDlg)
   enum { IDD = IDD_PARSER };
   CSpinButtonCtrl   m_Spin;
   CEdit m_Find;
   CButton  m_Name;
   CButton  m_Hash;
   CListCtrl   m_List;
   //}}AFX_DATA

      // ClassWizard generated virtual function overrides
      //{{AFX_VIRTUAL(CParserDlg)
   protected:
      virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
   //}}AFX_VIRTUAL

   // Implementation
   protected:
   
      HICON m_hIcon;
      CImageList*       _pImageList;

      // Generated message map functions
      //{{AFX_MSG(CParserDlg)
      virtual BOOL OnInitDialog();
      afx_msg void OnPaint();
      afx_msg HCURSOR OnQueryDragIcon();
      afx_msg void OnChangeEdtFile();
      afx_msg void OnBtnFile();
      afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg void OnBtnKill();
      afx_msg void OnBtnFirst();
      afx_msg void OnBtnLast();
      afx_msg void OnBtnPrev();
      afx_msg void OnBtnNext();
      afx_msg void OnChangeEdtFind();
      afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg void OnHdnItemclickLst(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg void OnClose();
      afx_msg void OnDestroy();
      afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
      afx_msg void OnRbName();
      afx_msg void OnRbHash();
      afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
      afx_msg void OnSetfocusEdtFind();
      afx_msg void OnBtnGo();
      afx_msg void OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult);
   //}}AFX_MSG
      DECLARE_MESSAGE_MAP()

   private: 

      void ClearSelection();
      bool CopyData(const BYTE* const pBuf,DWORD dwSize);

      void SearchName();
      void SearchStart();

      void UpdateCounter();
      void ResetSelection();

      void  Cleanup();
      void  Go();
      void  Fetch();
      void  RejectReport(const ListLine* const pItem);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
