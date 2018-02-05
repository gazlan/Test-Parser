/* ******************************************************************** **
** @@ Transformator Hdr File
** @  Copyrt : 
** @  Author : Aleph
** @  Modify :
** @  Update : 11 Jun 2007
** @  Notes  : Abstract class
** ******************************************************************** */

#ifndef _TRANSFORMATOR_HPP_
#define _TRANSFORMATOR_HPP_

#if _MSC_VER > 1000
#pragma once
#endif

/* ******************************************************************** **
** @@ internal defines
** ******************************************************************** */

#ifndef QWORD
typedef unsigned __int64   QWORD;
#endif

/* ******************************************************************** **
** @@ internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@ external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@ static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@ Prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@ class Transformator
** @  Copyrt : 
** @  Author : Aleph
** @  Modify :
** @  Update : 31 May 2007
** @  Notes  : Abstract Transformator
** ******************************************************************** */

class Transformator
{
   protected:

      // IN
      BYTE*       _pIn;
      DWORD       _dwInSize;
      DWORD       _dwInCursor;
      DWORD       _dwInByteRead;
      bool        _bInAttached;
      // OUT
      BYTE*       _pOut;
      DWORD       _dwOutSize;
      DWORD       _dwOutCursor;
      DWORD       _dwOutByteWrite;
      bool        _bOutAttached;

   public:

               Transformator();
      virtual ~Transformator();
                     
      virtual bool   Entrans() = 0;
      virtual bool   Detrans() = 0;
     
      DWORD    InSize();
      DWORD    OutSize();

      // Common
      // IN
      void     Rewind();
      int      SeekIn(int iOfs,int iMode);

      // Common
      // OUT
      void     Reset();
      int      SeekOut(int iOfs,int iMode);

      // Memfile functions
      // IN
      void     AttachIn(void* pBuf,DWORD dwSize);
      void*    DetachIn(DWORD& rdwSize);
      DWORD    Read(void* pBuf,DWORD dwSize);

      // Memfile functions
      // OUT
      void     AttachOut(void* pBuf,DWORD dwSize);
      void*    DetachOut(DWORD& rdwSize);
      DWORD    Write(const void* const pBuf,DWORD dwSize);
      void*    Realloc(void* pBuf,DWORD dwOldSize,DWORD dwNewSize);    

      // Stream functions
      // IN
      int      GetB();
      void     UngetB(); 
      
      DWORD    GetBits(int iCnt);
      void     UngetBits(int iCnt); 
      
      int      GetC();
      void     UngetC(); 
      
      WORD     GetW();
      void     UngetW(); 
      
      DWORD    GetDW();
      void     UngetDW(); 
      
      QWORD    GetQW();
      void     UngetQW(); 
      
      // Stream functions
      // OUT
      void     PutB (int iByte);
      void     PutBits(int iByte);
      void     PutC (int iByte);
      void     PutW (WORD wWord);
      void     PutDW(DWORD dwDWord);
      void     PutQW(QWORD qwQWord);
      void     Flush();
};

#endif

/* ******************************************************************** **
** End of File
** ******************************************************************** */
