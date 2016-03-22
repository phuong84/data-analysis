/**
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     PtracEvent.cxx
 * 
 */

#include "PtracEvent.h"

/***************************************************************************/
/**
 * This method initialzes event values.
 */
void PtracEvent::initialize()
{
	s_event = 0;
	nxt_event = 0;
	node = 0;
	nsr = 0;
	nxs = 0;
	ntyn = 0;
	nsf = 0;
	ang = 0;
	nter = 0;
	branch = 0;
	ipt = 0;
	ncl = 0;
	mat = 0;
	ncp = 0;

	xxx = 0.0;
	yyy = 0.0;
	zzz = 0.0;
	uuu = 0.0;
	vvv = 0.0;
	www = 0.0;
	erg = 0.0;
	wgt = 0.0;
	tme = 0.0;
}

