#include "bodies.h"

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
  node->no_children = 0;
  node->index = -1;
  node->cx = 0;
  node->cy = 0;
  node->lowerLeftX = lowerLeftX;
  node->lowerLeftY = lowerLeftY;
  node->length = length;   
  return node;
}

void subdivide(QuadTreeNode* node) {
  node->no_children = 4;
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
      while(node->no_children != 0) {
        int region = find_region(node, i, position);
        node = node->regions[region];
      }
      add_body(node, i, position);
    }
    
  }
  return root;
}

QuadTreeNode* update_properties(QuadTreeNode* node, Position *position, double *mass) {
  if(node->no_children == 0) {
    if(node->index != -1) {

    }
  }
}