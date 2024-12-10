#include "bodies.h"

double compute_euclidean_dist(Position a, Position b){
    return sqrt(pow((a.px - b.px), 2.0) +
               pow((a.py - b.py), 2.0));
}

int find_region(QuadTreeNode* node, int idx, Position *position) {
  double x = position[idx].px;
  double y = position[idx].py;
  if(x >= node->regions[1]->lowerLeftX) {
    // must be either NE or SE
    if(y >= node->regions[1]->lowerLeftY) {
      return 1;
    } else {
      return 3;
    }
  } else {
    if(y >= node->regions[0]->lowerLeftY) {
      return 0;
    } else {
      return 2;
    }
  }
}

QuadTreeNode* create_quadTree_node(double lowerLeftX, double lowerLeftY, double length) {
  QuadTreeNode* node = (QuadTreeNode*) malloc(sizeof(QuadTreeNode));
  node->mass = 0;
  node->n_children = 0;
  node->index = -1;
  node->cx = 0;
  node->cy = 0;
  node->lowerLeftX = lowerLeftX;
  node->lowerLeftY = lowerLeftY;
  node->length = length;   
  return node;
}

void subdivide(QuadTreeNode* node) {
  node->n_children = 4;
  node->regions[0] = create_quadTree_node(node->lowerLeftX, node->lowerLeftY + node->length / 2, node->length / 2);
  node->regions[1] = create_quadTree_node(node->lowerLeftX + node->length / 2, node->lowerLeftY + node->length / 2, node->length / 2);
  node->regions[2] = create_quadTree_node(node->lowerLeftX, node->lowerLeftY, node->length / 2);
  node->regions[3] = create_quadTree_node(node->lowerLeftX + node->length / 2, node->lowerLeftY, node->length / 2);
}

void add_body(QuadTreeNode* node, int idx, Position *position) {
  if(node->index == -1) {
    node->index = idx;
    return;
  }
  subdivide(node);
  int r1 = find_region(node, node->index, position);
  node->regions[r1]->index = node->index;
  int r2 = find_region(node, idx, position);
  if(r1 == r2) {
    add_body(node->regions[r1], idx, position);
  } else {
    node->regions[r2]->index = idx;
  }
}

QuadTreeNode* create_quadTree(int num_bodies, Position *position, double *mass) {
  QuadTreeNode* root = create_quadTree_node(0.0, 0.0, 4.0);
  int j = 0;
  while(j < num_bodies) {
    if(mass[j] != -1) {
      root->index = j;
      return root;
    }
    j++;
  }
  for(int i = j + 1; i < num_bodies; i++) {
    if(mass[i] != -1) {
      QuadTreeNode* node = root;
      while(node->n_children != 0) {
        int region = find_region(node, i, position);
        node = node->regions[region];
      }
      add_body(node, i, position);
    }
    
  }
  return root;
}

QuadTreeNode* update_properties(QuadTreeNode* node, Position *position, double *mass) {
  if(node->n_children == 0) {
    if(node->index != -1) {
      node->mass = mass[node->index];
      return node;
    }
  } else {
    double px = 0, py = 0;
    for(int i = 0; i < node->n_children; i++) {
      QuadTreeNode* ch = update_properties(node->regions[i], position, mass);
      if(ch != NULL) {
        node->mass += ch->mass;
        px += position[ch->index].px * ch->mass;
        py += position[ch->index].py * ch->mass;
      }
    }
    node->cx = px / node->mass;
    node->cy = py / node->mass;
  }
  return NULL;
}

void calculate_force_single_body(QuadTreeNode* node, int idx, int pidx, Force* force, Position* position, double* mass) {
  double d = compute_euclidean_dist(position[idx], position[node->index]);
  if(d < rlimit) {
    d = rlimit;
  }
  double force_x = (G * mass[idx] * mass[node->index] * (position[node->index].px - position[idx].px)) / (d * d);
  force[idx - pidx].fx += force_x / d;
  double force_y = (G * mass[idx] * mass[node->index] * (position[node->index].py - position[idx].py)) / (d * d);
  force[idx - pidx].fy += force_y / d;
}

void calculate_force_for_body(double theta, QuadTreeNode* node, int idx, int pidx, Force* force, Position* position, double* mass) {
  if(node->n_children == 0) {
    if(node->index != -1 && node->index != idx && node->mass != -1) {
      calculate_force_single_body(node, idx, pidx, force, position, mass);
    }
  } else {
    double d = compute_euclidean_dist(position[idx], position[node->index]);
    if((node->length / d) < theta) {
      calculate_force_single_body(node, idx, pidx, force, position, mass);
    } else {
      for (int i = 0; i < node->n_children; i++) {
          calculate_force_for_body(theta, node->regions[i], idx, pidx, force, position, mass);
      }
    }
  }
}

void calculate_force(double theta, int num_proc_bodies, int pidx, QuadTreeNode* node, Force *force, Position *position, double *mass) {
  for(int i = 0; i < num_proc_bodies; i++) {
    force[i].fx = 0.0;
    force[i].fy = 0.0;
    calculate_force_for_body(theta, node, i + pidx, pidx, force, position, mass);
  }
}

void free_quadTree(QuadTreeNode* node) {
  if(node->n_children == 0) {
    free(node);
    return;
  }
  for(int i = 0; i < node->n_children; i++) {
    free_quadTree(node->regions[i]);
  }
  free(node);
}