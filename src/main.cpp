#include <iostream>
#include <argparse.h>
#include <io.h>
#include <vector>
#include "barnes_hut_struct.h"
#include <mpi.h>
using namespace std;


// ./nbody -i input/nb-10.txt -o output/nb-10-parallel.txt -s 10 -t 0.005 -d 0.005
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    // Parse args
    struct options_t opts;
    get_opts(argc, argv, &opts);
    vector<Body> bodies;
    int num_bodies = 0;
    if(rank == 0) {
      read_file(&opts, &num_bodies, bodies);
    }

    /*
    printf("Input file: %s\nOutput file: %s\nSteps: %d\nTheta: %f\nTimestep: %f\nVisual: %s\n", 
            opts.inputfilename, opts.outputfilename, opts.steps, opts.theta, opts.timestep, opts.visual?"true":"false");
    */
    MPI_Bcast(&num_bodies, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int bodies_per_proc = num_bodies / num_procs;
    vector<Body> local_bodies(bodies_per_proc);

    MPI_Scatter(bodies.data(), bodies_per_proc * sizeof(Body), MPI_BYTE,
                local_bodies.data(), bodies_per_proc * sizeof(Body), MPI_BYTE,
                0, MPI_COMM_WORLD);

    
    for(int s = 0; s < opts.steps; s++) {
      QuadTree tree(Quad(4.0, 0.0, 0.0));
      if(rank == 0) {
        for (int i = 0; i < bodies.size(); i++) {
          if(bodies[i].mass >= 0) {
            tree.insert(bodies[i]);
          }
        }
      }
      broadcastQuadTree(&tree, rank, MPI_COMM_WORLD);
      
      vector<pair<double, double>> local_forces(local_bodies.size());
      for(int i = 0; i < local_bodies.size(); i++) {
        if(local_bodies[i].mass >= 0) {
          auto force = tree.calculateNetForce(bodies[i], opts.theta);
          double ax = force.first / bodies[i].mass;
          double ay = force.second / bodies[i].mass;
          double px = bodies[i].xpos + bodies[i].xvel * opts.timestep + 0.5 * ax * opts.timestep * opts.timestep;
          double py = bodies[i].ypos + bodies[i].yvel * opts.timestep + 0.5 * ay * opts.timestep * opts.timestep;
          double vx = bodies[i].xvel + ax * opts.timestep;
          double vy = bodies[i].yvel + ay * opts.timestep;
          bodies[i].xpos = px;
          bodies[i].ypos = py;
          bodies[i].xvel = vx;
          bodies[i].yvel = vy;
          if(px < 0 || px > 4 || py < 0 || py > 4) {
            bodies[i].mass = -1;
          }
        }
      }
      if(rank == 0) {
        MPI_Gather(local_bodies.data(), bodies_per_proc * sizeof(Body), MPI_BYTE,
               bodies.data(), bodies_per_proc * sizeof(Body), MPI_BYTE,
               0, MPI_COMM_WORLD);
    
        // Broadcast the updated bodies to all processes
        MPI_Bcast(bodies.data(), num_bodies * sizeof(Body), MPI_BYTE, 0, MPI_COMM_WORLD);
      } else {
        // For non-root processes, send the updated local_bodies back to the root process
        MPI_Gather(local_bodies.data(), bodies_per_proc * sizeof(Body), MPI_BYTE,
               nullptr, 0, MPI_BYTE, 0, MPI_COMM_WORLD);
    
        // Broadcast updated bodies to all processes
        MPI_Bcast(local_bodies.data(), bodies_per_proc * sizeof(Body), MPI_BYTE, 0, MPI_COMM_WORLD);
      }     
    }   
    if(rank == 0) {
      write_file(&opts, bodies);
    }

    MPI_Finalize();
    
}
