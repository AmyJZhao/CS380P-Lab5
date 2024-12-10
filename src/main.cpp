#include <iostream>
#include <argparse.h>
#include <io.h>
#include <vector>
//#include "barnes_hut_struct.h"
#include "bodies.h"
#include <mpi.h>
#include <cstring>
using namespace std;
MPI_Datatype MPI_POSITION;
MPI_Datatype MPI_VELOCITY;
void print_position(int rank, int num_bodies, Position* position){
   int i;
   for (i = 0; i < num_bodies; i++)
      printf("Rank=%d, px=%lf, py=%lf\n", rank, position[i].px, position[i].py);
   printf("\n");
}

void print_velocity(int rank, int part_size, Velocity *velocity){
   int i;
   for (i = 0; i < part_size; i++)
      printf("Rank=%d, vx=%lf, vy=%lf\n", rank, velocity[i].vx, velocity[i].vy);
   printf("\n");
}

void print_ivelocity(int rank, int num_bodies, Velocity *ivelocity){
   int i;
   for (i = 0; i < num_bodies; i++)
      printf("Rank=%d, vx=%lf, vy=%lf\n", rank, ivelocity[i].vx, ivelocity[i].vy);
   printf("\n");
}

void print_mass(int rank, int num_bodies, double *mass){
   int i;
   for (i = 0; i < num_bodies; i++)
      printf("Rank=%d, mass=%lf\n", rank, mass[i]);
   printf("\n");
}

// ./nbody -i input/nb-10.txt -o output/nb-10-parallel.txt -s 10 -t 0.005 -d 0.005
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Type_contiguous(2, MPI_DOUBLE, &MPI_POSITION);
    MPI_Type_contiguous(2, MPI_DOUBLE, &MPI_VELOCITY);
    MPI_Type_commit(&MPI_POSITION);
    MPI_Type_commit(&MPI_VELOCITY);
    // Parse args
    struct options_t opts;
    get_opts(argc, argv, &opts);
    int num_total_bodies = 0;
    Position* position;   // Current positions for all particles
    Velocity* ivelocity;  // Initial velocity for all particles
    Velocity* velocity;   // Velocity of particles in current processor
    double* mass;         // Mass of each particle
    Force* force;         // Force experienced by all particles
    read_file(&opts, &num_total_bodies, &position, &ivelocity, &mass);
    int num_proc_bodies = num_total_bodies / num_procs;
    
    int pidx = rank * num_proc_bodies;
    velocity = (Velocity *) malloc(num_proc_bodies * sizeof(Velocity));
    for (int i = 0; i < num_proc_bodies; i++){
      velocity[i].vx = 0;
      velocity[i].vy = 0;
    }
    force = (Force *) malloc(num_proc_bodies * sizeof(Force));

    double start = MPI_Wtime();
    MPI_Bcast(mass, num_total_bodies, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(position, num_total_bodies, MPI_POSITION, 0, MPI_COMM_WORLD);
    MPI_Scatter(ivelocity, num_proc_bodies, MPI_VELOCITY, velocity, num_proc_bodies, MPI_VELOCITY, 0, MPI_COMM_WORLD);
    
    for(int x = 0; x < opts.steps; x++) {
      QuadTreeNode* quadTree = create_quadTree(num_total_bodies, position, mass);
      update_properties(quadTree, position, mass);
      calculate_force(opts.theta, num_proc_bodies, pidx, quadTree, force, position, mass);
      free_quadTree(quadTree);
      for (int i = 0; i < num_proc_bodies; i++) {
          position[i + pidx].px += velocity[i].vx * opts.timestep + 0.5 * (force[i].fx / mass[i + pidx]) * opts.timestep * opts.timestep;
          position[i + pidx].py += velocity[i].vy * opts.timestep + 0.5 * (force[i].fy / mass[i + pidx]) * opts.timestep * opts.timestep;
          velocity[i].vx += (force[i].fx / mass[i + pidx]) * opts.timestep;
          velocity[i].vy += (force[i].fy / mass[i + pidx]) * opts.timestep;
          if(position[i + pidx].px < 0 || position[i + pidx].px > 4.0 || position[i + pidx].py < 0 || position[i + pidx].py > 4.0) {
            mass[i + pidx] = -1;
          }
      }
      Position *tempPosition = (Position*) malloc(num_total_bodies * sizeof(Position));
      MPI_Allgather(position + (rank * num_proc_bodies), num_proc_bodies, MPI_POSITION, tempPosition, num_proc_bodies, MPI_POSITION, MPI_COMM_WORLD);
      memcpy(position, tempPosition, num_total_bodies * sizeof(Position));
      MPI_Allgather(velocity, num_proc_bodies, MPI_VELOCITY, ivelocity, num_proc_bodies, MPI_VELOCITY, MPI_COMM_WORLD);
    }
    if(rank == 0) {
      double end = MPI_Wtime();
      std::cout << end - start << std::endl;
      write_file(&opts, num_total_bodies, position, ivelocity, mass);
    }
    MPI_Finalize();
    
}
