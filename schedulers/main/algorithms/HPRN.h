#ifndef HPRN_H
#define HPRN_H

#include "scheduler/scheduler.h"

/*
	requires a global scheduler in order to use its own version of ready which 
	differs from the basic setup in RR and FCFS

	for more information look at HPRN.cpp
*/

scheduler hp;

void HPRN(const vector<int*>& data, int v);
void hp_ready();

#endif