#ifndef _BODY_H
#define _BODY_H
#include <iostream>

class Quad {
  double length;
  double lowerLeftX;
  double lowerLeftY;
  public:
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
  Body* body = nullptr;
  Quad quad;
  QuadTree* NW = nullptr;
  QuadTree* NE = nullptr;
  QuadTree* SW = nullptr;
  QuadTree* SE = nullptr;
  public:
    QuadTree(Quad q) : quad(q) {};
    void insert(Body b);
    void updateForce(Body b);
    void printTree(int depth = 0);
};


#endif