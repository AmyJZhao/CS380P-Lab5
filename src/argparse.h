#ifndef _ARGPARSE_H
#define _ARGPARSE_H

#include <getopt.h>
#include <stdlib.h>
#include <iostream>

struct options_t {
    char *inputfilename;
    char *outputfilename;
    int steps;
    double theta;
    double timestep;
    bool visual;
};

void get_opts(int argc, char **argv, struct options_t *opts);
#endif
