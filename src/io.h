#ifndef _IO_H
#define _IO_H

#include <argparse.h>
#include "barnes_hut_struct.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void read_file(struct options_t* args, int* n_vals, vector<Body>& vec);

#endif