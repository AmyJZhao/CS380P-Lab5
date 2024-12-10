#include "io.h"

void read_file(struct options_t* args, int *n_vals, Position** position, Velocity** ivelocity, double** mass) {
  std::ifstream in;
	in.open(args->inputfilename);
	// Get num vals
	in >> *n_vals;
  *mass = (double*) malloc(*n_vals * sizeof(double));
  *position = (Position *) malloc(*n_vals * sizeof(Position));
  *ivelocity = (Velocity *) malloc(*n_vals * sizeof(Velocity));
  for (int i = 0; i < *n_vals; ++i) {
    int idx;
    in >> idx;
    in >> (*position)[i].px;
    in >> (*position)[i].py;
    in >> (*mass)[i];
    in >> (*ivelocity)[i].vx;
    in >> (*ivelocity)[i].vy;
  }
}
/*
void write_file(struct options_t* args, vector<Body> vec) {
  std::ofstream out;
	out.open(args->outputfilename, std::ofstream::trunc);
  out << vec.size() << std::endl;
  for(int i = 0; i < vec.size(); i++) {
    out << vec[i].idx << " " << vec[i].xpos << " " << vec[i].ypos << 
        " " << vec[i].mass << " " << vec[i].xvel << " " << vec[i].yvel << std::endl;
  }

}

void printBodies(vector<Body> bodies) {
  for (int i = 0; i < bodies.size(); i++) {
      printf("Index: %d, X Pos: %lf, Y Pos: %lf, Mass: %lf, X Vel: %lf, Y Vel: %lf\n", 
            bodies[i].idx,
            bodies[i].xpos,
            bodies[i].ypos,
            bodies[i].mass,
            bodies[i].xvel,
            bodies[i].yvel);
      
  }
}
*/