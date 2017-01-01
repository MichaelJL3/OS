#ifndef LCFS_H
#define LCFS_H

#include "scheduler/scheduler.h"

/*
	requires a global scheduler in order to use its own version of ready which 
	differs from the basic setup in RR and FCFS

	for more information look at LCFS.cpp
*/

scheduler lc;

void LCFS(const vector<int*>& data, int v);
void lc_ready();

#endif