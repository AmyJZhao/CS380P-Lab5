#ifndef _IO_H
#define _IO_H

#include <argparse.h>
#include "bodies.h"
#include <iostream>
#include <fstream>
using namespace std;

void read_file(struct options_t* args, int *n_vals, Position** position, Velocity** ivelocity, double** mass);

//void write_file(struct options_t* args, vector<Body> vec);

//void printBodies(vector<Body> vec);
#endif