#include "barnes_hut_struct.h"

Quad::Quad(double l, double x, double y) {
  length = l;
  lowerLeftX = x;
  lowerLeftY = y;
}

bool Quad::contains(double x, double y) {
  return (x >= lowerLeftX && x <= lowerLeftX + length) &&
           (y >= lowerLeftY && y <= lowerLeftY + length);
}

double Quad::len() {
  return length;
}

Quad Quad::NW() {
  return Quad(length / 2, lowerLeftX, lowerLeftY + length / 2);
}

Quad Quad::NE() {
  return Quad(length / 2, lowerLeftX + length / 2, lowerLeftY + length / 2);
}

Quad Quad::SW() {
  return Quad(length / 2, lowerLeftX, lowerLeftY);
} 

Quad Quad::SE() {
  return Quad(length / 2, lowerLeftX + length / 2, lowerLeftY);
}

bool Body::in(Quad q) {
  return q.contains(xpos, ypos);
}

Body Body::add(Body a, Body b) {
  Body total;
  total.mass = a.mass + b.mass;
  total.xpos = (a.xpos * a.mass + b.xpos * b.mass) / (a.mass + b.mass);
  total.ypos = (a.ypos * a.mass + b.ypos * b.mass) / (a.mass + b.mass);
  return total;
}