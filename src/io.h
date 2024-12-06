#ifndef _IO_H
#define _IO_H

#include <argparse.h>
#include <body.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void read_file(struct options_t* args, int* n_vals, vector<body_t>& vec);

#endif