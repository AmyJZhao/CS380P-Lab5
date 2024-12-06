#include <iostream>
#include <argparse.h>
#include <io.h>
#include <vector>
#include "body.h"
using namespace std;

int main(int argc, char **argv)
{
    // Parse args
    struct options_t opts;
    get_opts(argc, argv, &opts);
    vector<body_t> bodies;
    int num_bodies;
    read_file(&opts, &num_bodies, bodies);
    printf("Input file: %s\nOutput file: %s\nSteps: %d\nTheta: %f\nTimestep: %f\nVisual: %s\n", 
            opts.inputfilename, opts.outputfilename, opts.steps, opts.theta, opts.timestep, opts.visual?"true":"false");
    int max = min(static_cast<int>(bodies.size()), 100);
    for (int i = 0; i < max; i++) {
        cout << "Index: " << bodies[i].idx <<
               ", X Pos: " << bodies[i].xpos <<
               ", Y Pos: " << bodies[i].ypos <<
               ", Mass: " << bodies[i].mass << 
               ", X Vel: " << bodies[i].xvel <<
               ", Y Vel: " << bodies[i].yvel << endl;
    }
}