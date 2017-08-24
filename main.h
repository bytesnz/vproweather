/****************************************************************************/
/*  vproweather 1.0                                                         */
/*  A program for acquiring data from a Davis Vantage Pro Weather Station.  */
/*                                                                          */
/*  Thanks to Paul Davis for his 'remserial' program, and Aaron Logue's     */
/*  'weastat' program.                                                      */
/*                                                                          */
/* (c)2004 Joe Jaworski    email: jj@joejaworski.com                        */
/* VPROWEATHER is covered under the GNU general license. See the attached   */
/* LICENSE file for details.                                                */
/*                                                                          */
/* This software is provided as-is, without any expressed or implied        */
/* warranties, including, without limitation, the implied warranties of     */
/* merchantibility and fitness for any particular purpose.                  */
/*                                                                          */
/*                                                                          */
/* main.h.h - main header file                                              */
/* tabs every 4 places                                                      */
/*                                                                          */
/****************************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
#define extern "C" {        /* respect c++ callers */
#endif

#define LOBYTE(w)   ((uint8_t)(w))
#define HIBYTE(w)   ((uint8_t)(((uint16_t)(w) >> (uint8_t)8) & 0xff))

#ifndef PACKED
#define PACKED __attribute__ ((__packed__))
#endif // PACKED


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __MAIN_H__ */
