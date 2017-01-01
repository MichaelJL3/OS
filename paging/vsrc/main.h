#ifndef MAIN_H //define guard
#define MAIN_H

#include <cstdlib>
#include <string>
#include "simulator/driver.cpp"

#define A 0
#define B 1
#define C 2

#define MACH 0
#define PAGE 1
#define SIZE 2
#define REFS 3
#define MAXF 4
#define PROC 5

using namespace std;

double** status;

double** setup(int n);
int preprocess(int job);
int algorithm(string s);
void output(int process);

#endif