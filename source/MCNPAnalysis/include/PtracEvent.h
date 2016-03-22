/**
 * \class    PtracEvent
 * \ingroup  MCNPAnalysis
 *
 * \brief    Store event information from PTRAC file
 *
 * This class can be used to store MCNP event information from
 * PTRAC file. It is remodified from ParsNIP (Parser for PTRAC files 
 * produced by MCNP) project http://ptracparser.sourceforge.net/ .
 *
 * \author   Dang Nguyen Phuong (dnphuong1984@gmail.com)
 * \version  0.1
 * \date     30-03-2015
 *
 * \file     PtracEvent.h
 * 
 */

//////////////////////////////////////////////////////////
// This class is a modification of ParsNIP project
// Copyright (c) 2014, Tanya Hutton
// http://ptracparser.sourceforge.net/
//////////////////////////////////////////////////////////

#include <iostream>

#ifndef __PtracEvent__
#define __PtracEvent__

class PtracEvent {

public:

	/// \brief Class constructor, initialize counters
	PtracEvent() : event_ctr(0), nps_ctr(0), hist_ctr(0) {};
	
	/// \brief Class destructor
	~PtracEvent() {};

	/// \brief Initialize values
	void initialize();

	int event_ctr;	///< counter for number of events in file
	int nps_ctr;	///< events in history
	int hist_ctr;	///< counter for number of histories

	int nps;		///< history number
	int s_event;	///< initial event type of history
	int nxt_event;	///< next event type                  
	int node;		///< nodes in track from source
	int nsr;		///< source type
	int nxs;		///< zzaaa of event
	int ntyn;		///< reaction type (MT)
	int nsf;		///< closest surface
	int ang;		///< angle with surface normal
	int nter;		///< termination type
	int branch;		///< branch number for history
	int ipt;		///< particle type
	int ncl;		///< problem cell number
	int mat;		///< problem material number
	int ncp;		///< number of collisions per track

	/** xxx, yyy, zzz of event */
	//@{
	double xxx, yyy, zzz;
	//@}
	/** uuu, vvv, www of direction after event */
	//@{
	double uuu, vvv, www; 
	//@}
	double erg;				///< outgoing energy
	double wgt;				///< outgoing weight
	double tme;				///< time at position xxx, yyy, zzz

	int type;

};

#endif
