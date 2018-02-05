/* ******************************************************************** **
** @@ Abstract Transformator
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "trans_abstract.h"

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ Transformator::Transformator()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

Transformator::Transformator()
{
   _pIn          = NULL;
   _dwInSize     = 0;
   _dwInCursor   = 0;
   _dwInByteRead = 0;
   _bInAttached  = false;

   _pOut           = NULL;
   _dwOutSize      = 0;
   _dwOutCursor    = 0;
   _dwOutByteWrite = 0;
   _bOutAttached   = false;
}

/* ******************************************************************** **
** @@ Transformator::~Transformator()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

Transformator::~Transformator()
{
}

/* ******************************************************************** **
** @@ Transformator::AttachIn()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Transformator::AttachIn
(
   void*       pBuf,
   DWORD       dwSize
)
{
   VERIFY(!_bInAttached);

   if (_bInAttached)
   {
      return;
   }
   
   _pIn          = (BYTE*)pBuf;
   _dwInSize     = dwSize;
   _dwInCursor   = 0;
   _dwInByteRead = 0;
   _bInAttached  = true;
}

/* ******************************************************************** **
** @@ Transformator::DetachIn()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void* Transformator::DetachIn(DWORD& rdwSize)
{
   VERIFY(_bInAttached);

   if (!_bInAttached)
   {
      rdwSize = 0;
      return NULL;
   }

   rdwSize       = _dwInCursor;
   _dwInSize     = 0;
   _dwInCursor   = 0;
   _dwInByteRead = 0;
   _bInAttached  = false;

   return _pIn;
}

/* ******************************************************************** **
** @@ Transformator::AttachOut()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Transformator::AttachOut
(
   void*       pBuf,
   DWORD       dwSize
)
{
   VERIFY(!_bOutAttached);

   if (_bOutAttached)
   {
      return;
   }
   
   _pOut           = (BYTE*)pBuf;
   _dwOutSize      = dwSize;
   _dwOutCursor    = 0;
   _dwOutByteWrite = 0;
   _bOutAttached   = true;
}

/* ******************************************************************** **
** @@ Transformator::DetachOut()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void* Transformator::DetachOut(DWORD& rdwSize)
{
   VERIFY(_bOutAttached);

   if (!_bOutAttached)
   {
      rdwSize = 0;
      return NULL;
   }

   rdwSize         = _dwOutCursor;
   _dwOutSize      = 0;
   _dwOutCursor    = 0;
   _dwOutByteWrite = 0;
   _bOutAttached   = false;

   return _pOut;
}

/* ******************************************************************** **
** @@ Transformator::GetC()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int Transformator::GetC()
{
   if (!_pIn || (_dwInCursor > (DWORD)(_dwInSize - 1)) || IsBadReadPtr(_pIn,_dwInCursor + 1))
   {
      return -1;
   }

   return (int)_pIn[_dwInCursor++];
}

/* ******************************************************************** **
** @@ Transformator::PutC()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Transformator::PutC(int iByte)
{
   if (!_pOut || (_dwOutCursor > (DWORD)(_dwOutSize - 1)) || IsBadWritePtr(_pOut,_dwOutCursor + 1))
   {
      // Error 
      return;
   }

   _pOut[_dwOutCursor++] = (BYTE)iByte;
}

/* ******************************************************************** **
** @@ Transformator::Rewrite()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Transformator::Rewind()
{
   _dwInCursor = 0;
}

/* ******************************************************************** **
** @@ Transformator::Reset()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Transformator::Reset()
{
   _dwOutCursor = 0;
}

/* ******************************************************************** **
** @@ Transformator::UngetC()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Transformator::UngetC()
{
   _dwInCursor = max(0,(_dwInCursor - 1));
}

/* ******************************************************************** **
** @@ Transformator::Realloc()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void* Transformator::Realloc(void* pBuf,DWORD dwOldSize,DWORD dwNewSize)    
{
   if (!dwNewSize)
   {
      if (pBuf)
      {
         delete pBuf;
         pBuf = NULL;
      }

      return NULL;
   }

   BYTE*    _pTemp = new BYTE[dwNewSize];

   ASSERT(_pTemp);

   if (!_pTemp)
   {
      // Error !
      return NULL;
   }

   if (!dwOldSize)
   {
      return _pTemp;
   }
   
   memcpy(_pTemp,pBuf,dwOldSize);

   delete pBuf;
   pBuf = NULL;

   return _pTemp;
}

/* ******************************************************************** **
** @@ Transformator::Read()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD Transformator::Read
(
   void*       pBuf,
   DWORD       dwSize
)
{
   DWORD    dwRealSize = min(dwSize,_dwInSize - _dwInCursor);

   if (!dwRealSize)
   {
      return 0;
   }

   if (!_pIn || ((_dwInCursor + dwRealSize) > _dwInSize) || IsBadReadPtr(_pIn + _dwInCursor,dwRealSize))
   {
      return 0;
   }

   if (!pBuf || IsBadWritePtr(pBuf,dwRealSize))
   {
      return 0;
   }

   memcpy(pBuf,_pIn + _dwInCursor,dwRealSize);

   _dwInCursor += dwRealSize;

   return dwRealSize;
}

/* ******************************************************************** **
** @@ Transformator::Write()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD Transformator::Write
(
   const void* const       pBuf,
   DWORD                   dwSize
)
{
   DWORD    dwRealSize = min(dwSize,_dwOutSize - _dwOutCursor);

   if (!dwRealSize)
   {
      return 0;
   }

   if (!_pOut || ((_dwOutCursor + dwRealSize) > _dwOutSize) || IsBadWritePtr(_pOut + _dwOutCursor,dwRealSize))
   {
      return 0;
   }

   if (!pBuf || IsBadReadPtr(pBuf,dwRealSize))
   {
      return 0;
   }

   memcpy(_pOut + _dwOutCursor,pBuf,dwRealSize);

   _dwOutCursor += dwRealSize;

   return dwRealSize;
}

/* ******************************************************************** **
** End of File
** ******************************************************************** */
