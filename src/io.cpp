#include "io.h"

void read_file(struct options_t* args, int* n_vals, vector<Body>& vec) {
  std::ifstream in;
	in.open(args->inputfilename);
	// Get num vals
	in >> *n_vals;
  for (int i = 0; i < *n_vals; ++i) {
    Body newBody;
    in >> newBody.idx;
    in >> newBody.xpos;
    in >> newBody.ypos;
    in >> newBody.mass;
    in >> newBody.xvel;
    in >> newBody.yvel;
    vec.push_back(newBody);
  }
}