/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: prng_chisquare.h $
* Version:		$Revision: 1.2 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Header file for random number module
*
* $Id: prng_chisquare.h 1.2 2011/06/26 13:36:22Z admin Exp $
*
* Revision History:
* -----------------
*
* $Log: prng_chisquare.h $
* Revision 1.2  2011/06/26 13:36:22Z  admin
* Revision 1.1  2011/05/19 18:23:02Z  admin
* Initial revision
* Revision 1.1  2011/03/16 18:26:50Z  admin
* Initial revision
* Revision 1.1  2010/10/26 22:13:49Z  Admin
* Initial revision
* Revision 1.3  2008/06/25 18:47:33Z  Administrator
* Revision 1.2  2007/11/19 08:00:18Z  Administrator
* Revision 1.1  2007/06/09 13:50:06Z  _____________
* Initial revision
* Revision 1.2  91/09/27  03:11:08  kjb
* Added compatibility with C++.
* 
* Revision 1.1  91/08/16  10:54:55  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#ifndef	__RANDOM_H
#define	__RANDOM_H

/*----------------------- Function Prototypes -----------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void     RandomInit(long s);
long     RandomLong(long range);
double   randomfloat(void);
double   chisquare(int N,int r,long s);

#ifdef __cplusplus
}
#endif

#endif