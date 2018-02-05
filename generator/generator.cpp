/* ******************************************************************** **
** @@ Quiz Test Generator
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

#include <stdio.h>
#include <time.h>

#include "..\shared\hash_md5.h"
#include "..\shared\trans_abstract.h"
#include "..\shared\prng_chisquare.h"

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */
   
/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

static void PRNG_String(char* pszStr,int iSize)
{
   for (int ii = 0; ii < iSize; ++ii)
   {
      pszStr[ii] = (char)('a' + (char)RandomLong('z' - 'a'));
   }

   pszStr[ii] = 0;  // ASCIIZ
}

/* ******************************************************************** **
** @@ ShowHelp()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void ShowHelp()
{
   const char  pszCopyright[] = "-*-   Test Generator *  Copyright (c) Gazlan 2013   -*-";
   const char  pszDescript [] = "Markup Quiz Text Test Generator";
   const char  pszE_Mail   [] = "complains_n_suggestions direct to gazlan@yandex.ru";

   printf("%s\n",pszCopyright);
   printf("\n%s\n",pszDescript);
   printf("\nUsage: genertor.exe [> Test.txt]\n");
   printf("\n%s\n\n",pszE_Mail);
}

/* ******************************************************************** **
** @@ main()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool main(int argc,char** argv)
{
   if ((argc == 2) && argv[1] && *argv[1])
   {
      if ((!strcmp(argv[1],"?")) || (!strcmp(argv[1],"/?")) || (!strcmp(argv[1],"-?")) || (!stricmp(argv[1],"/h")) || (!stricmp(argv[1],"-h")))
      {
         ShowHelp();
         return 0;
      }
   }

   RandomInit(time(NULL));

   char     pszQuestion[MAX_PATH + 1];

   MD5         Hash;

   for (int ii = 0; ii < 10000; ++ii)
   {
      int   iStrLen = 5 + RandomLong(125);

      memset(pszQuestion,0,sizeof(pszQuestion));

      PRNG_String(pszQuestion,iStrLen);

      Hash.Reset();
      Hash.Update((BYTE*)pszQuestion,strlen(pszQuestion));
      Hash.Finalize();

      printf("<simple>\n");
      printf("<question>%s</question>\n",pszQuestion);
      printf("<answer>%s</answer>\n",Hash.Signature());
      printf("</simple>\n\n");
   }

   return true;
}

/* ******************************************************************** **
**                That's All
** ******************************************************************** */
