#ifndef _BODIES_H
#define _BODIES_H
#include <cstdlib>
typedef struct {

   double px, py;

} Position;


/* Components of velocity of particle */
typedef struct {

   double vx, vy;

} Velocity;


/* Components of force experienced by particle */
typedef struct {

   double fx, fy;

} Force;

typedef struct QuadTreeNode {
  int index;
  int no_children;
  double mass;
  double lowerLeftX, lowerLeftY;
  double length;
  double cx, cy; // center of mass
  struct QuadTreeNode* regions[4]; // 0 - NW, 1 - NE, 2 - SW, 3 - SE
} QuadTreeNode;

QuadTreeNode* create_quadTree_node(double lowerLeftX, double lowerLeftY, double length);

QuadTreeNode* create_quadTree(int num_bodies, Position *position, double *mass);

int find_region(QuadTreeNode* node, int idx, Position *position);

void add_body(QuadTreeNode* node, int idx, Position *position);

void subdivide(QuadTreeNode* node);

#endif