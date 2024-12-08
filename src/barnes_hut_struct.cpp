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

void QuadTree::insert(Body b) {
  if(body == nullptr) {
    body = new Body(b);
    return;
  } else {
    if(body->idx == -1) {
      Body updateBody = Body::add(*body, b);
      body = new Body(updateBody);
      if (quad.NW().contains(b.xpos, b.ypos)) {
          NW->insert(b);
      } else if (quad.NE().contains(b.xpos, b.ypos)) {
          NE->insert(b);
      } else if (quad.SW().contains(b.xpos, b.ypos)) {
          SW->insert(b);
      } else if (quad.SE().contains(b.xpos, b.ypos)) {
          SE->insert(b);
      }
    } else {
      NW = new QuadTree(quad.NW());
      NE = new QuadTree(quad.NE());
      SW = new QuadTree(quad.SW());
      SE = new QuadTree(quad.SE());
      if (quad.NW().contains(b.xpos, b.ypos)) {
          NW->insert(b);
      } else if (quad.NE().contains(b.xpos, b.ypos)) {
          NE->insert(b);
      } else if (quad.SW().contains(b.xpos, b.ypos)) {
          SW->insert(b);
      } else if (quad.SE().contains(b.xpos, b.ypos)) {
          SE->insert(b);
      }
      Body copy = *body;
      if (quad.NW().contains(copy.xpos, copy.ypos)) {
          NW->insert(copy);
      } else if (quad.NE().contains(copy.xpos, copy.ypos)) {
          NE->insert(copy);
      } else if (quad.SW().contains(copy.xpos, copy.ypos)) {
          SW->insert(copy);
      } else if (quad.SE().contains(copy.xpos, copy.ypos)) {
          SE->insert(copy);
      }
      Body updateBody = Body::add(copy, b);
      body = new Body(updateBody);
    }
  }
}

void QuadTree::printTree(int depth) {
  for (int i = 0; i < depth; ++i) std::cout << "  ";

  // Print the body of the current node (if exists)
  if (body != nullptr) {
      std::cout << "Idx " << body->idx << ", Body at (" << body->xpos << ", " << body->ypos << ") with mass " << body->mass << std::endl;
  } else {
      // If no body exists (internal node), print "Internal node"
      std::cout << "No body exists" << std::endl;
  }

  // If this is an internal node, print details for its 4 children (NW, NE, SW, SE)
  if (NW != nullptr) {
      for (int i = 0; i < depth + 1; ++i) std::cout << "  "; // Indentation
      std::cout << "NW: ";
      NW->printTree(depth + 1); // Recursively print NW child
  }

  if (NE != nullptr) {
      for (int i = 0; i < depth + 1; ++i) std::cout << "  "; // Indentation
      std::cout << "NE: ";
      NE->printTree(depth + 1); // Recursively print NE child
  }

  if (SW != nullptr) {
      for (int i = 0; i < depth + 1; ++i) std::cout << "  "; // Indentation
      std::cout << "SW: ";
      SW->printTree(depth + 1); // Recursively print SW child
  }

  if (SE != nullptr) {
      for (int i = 0; i < depth + 1; ++i) std::cout << "  "; // Indentation
      std::cout << "SE: ";
      SE->printTree(depth + 1); // Recursively print SE child
  }

}