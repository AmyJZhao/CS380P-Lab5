#ifndef _BODY_H
#define _BODY_H
#include <iostream>
#include <mpi.h>
#include <vector>
const double G = 0.0001;
const double rlimit = 0.03;

class Quad {
  public:
    double length;
    double lowerLeftX;
    double lowerLeftY;
    Quad(double l, double x, double y);
    bool contains(double x, double y);
    double len();
    Quad NW(), NE(), SW(), SE();
};

class Body {
  public:
    int idx = -1;
    double mass = 0.0;
    double xpos = 0.0;
    double ypos = 0.0;
    double xvel = 0.0;
    double yvel = 0.0;
    bool in(Quad q);
    static Body add(Body a, Body b);
};

class QuadTree {
  public:
    Body* body = nullptr;
    Quad quad;
    QuadTree* NW = nullptr;
    QuadTree* NE = nullptr;
    QuadTree* SW = nullptr;
    QuadTree* SE = nullptr;
    QuadTree(Quad q) : quad(q) {};
    void insert(Body b);
    std::pair<double, double> calculateNetForce(Body b, double theta);
    void printTree(int depth = 0);
};

void broadcastQuadTree(QuadTree *tree, int rank, MPI_Comm comm);
void serializeTreeNode(const QuadTree *node, std::vector<char> &buffer);
void deserializeTreeNode(QuadTree *node, const std::vector<char> &buffer, size_t &offset);

#endif