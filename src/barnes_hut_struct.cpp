#include "barnes_hut_struct.h"
#include <cmath>

Quad::Quad(double l, double x, double y) {
  length = l;
  lowerLeftX = x;
  lowerLeftY = y;
}

bool Quad::contains(double x, double y) {
  return (x >= lowerLeftX && x < lowerLeftX + length) &&
           (y >= lowerLeftY && y < lowerLeftY + length);
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

std::pair<double, double> QuadTree::calculateNetForce(Body b, double theta) {
  double Fx = 0.0;
  double Fy = 0.0;
  if(body != nullptr && body->idx != -1 && body->idx != b.idx) {
    double dx = abs(body->xpos - b.xpos);
    double dy = abs(body->ypos - b.ypos);
    double d = sqrt(dx * dx + dy * dy);
    if(d < rlimit) {
      d = rlimit;
    }
    double force = G * b.mass * body->mass / (d * d * d);
    Fx += force * dx;
    Fy += force * dy;
    return std::make_pair(Fx, Fy);
  }
  if(body != nullptr && body->idx == -1) {
    double dx = abs(body->xpos - b.xpos);
    double dy = abs(body->ypos - b.ypos);
    double d = sqrt(dx * dx + dy * dy);
    if(d < rlimit) {
      d = rlimit;
    }
    double s = quad.len();
    double ratio = s / d;
    if(ratio < theta) {
      double force = G * b.mass * body->mass / (d * d * d);
      Fx += force * dx;
      Fy += force * dy;
    } else {
      if (NW != nullptr) {
          auto childForce = NW->calculateNetForce(b, theta);
          Fx += childForce.first;
          Fy += childForce.second;
      }
      if (NE != nullptr) {
          auto childForce = NE->calculateNetForce(b, theta);
          Fx += childForce.first;
          Fy += childForce.second;
      }
      if (SW != nullptr) {
          auto childForce = SW->calculateNetForce(b, theta);
          Fx += childForce.first;
          Fy += childForce.second;
      }
      if (SE != nullptr) {
          auto childForce = SE->calculateNetForce(b, theta);
          Fx += childForce.first;
          Fy += childForce.second;
      }
    }
    
  }
  return std::make_pair(Fx, Fy);
}

void broadcastQuadTree(QuadTree *tree, int rank, MPI_Comm comm) {
  std::vector<char> buffer;
    if (rank == 0) {
        // Root process serializes the tree
        serializeTreeNode(tree, buffer);
    }
    // Broadcast the serialized tree structure and body data to all other processes
    int buffer_size = buffer.size();
    MPI_Bcast(&buffer_size, 1, MPI_INT, rank, comm);
    MPI_Bcast(buffer.data(), buffer_size, MPI_BYTE, rank, comm);

    // Deserializing the tree into the QuadTree structure for all processes
    size_t offset = 0;
    deserializeTreeNode(tree, buffer, offset);

}

void serializeTreeNode(const QuadTree *node, std::vector<char> &buffer) {
    if (node == nullptr) {
        return;
    }
  
    // Serialize the current node's Quad data
    buffer.push_back(static_cast<char>(node->body != nullptr)); // 1 byte to indicate if this is a leaf node

    if (node->body) {
        // Serialize Body attributes: idx, mass, xpos, ypos, xvel, yvel
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&node->body->idx), reinterpret_cast<const char*>(&node->body->idx) + sizeof(node->body->idx));
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&node->body->mass), reinterpret_cast<const char*>(&node->body->mass) + sizeof(node->body->mass));
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&node->body->xpos), reinterpret_cast<const char*>(&node->body->xpos) + sizeof(node->body->xpos));
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&node->body->ypos), reinterpret_cast<const char*>(&node->body->ypos) + sizeof(node->body->ypos));
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&node->body->xvel), reinterpret_cast<const char*>(&node->body->xvel) + sizeof(node->body->xvel));
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&node->body->yvel), reinterpret_cast<const char*>(&node->body->yvel) + sizeof(node->body->yvel));
    }
    
    // Serialize the Quad data (bounds)
    buffer.insert(buffer.end(), reinterpret_cast<const char*>(&node->quad), reinterpret_cast<const char*>(&node->quad) + sizeof(Quad));

    // Recursively serialize the children if they exist
    if (node->NW) {
        serializeTreeNode(node->NW, buffer);
    } else {
        buffer.push_back(0); // No NW child
    }
    
    if (node->NE) {
        serializeTreeNode(node->NE, buffer);
    } else {
        buffer.push_back(0); // No NE child
    }

    if (node->SW) {
        serializeTreeNode(node->SW, buffer);
    } else {
        buffer.push_back(0); // No SW child
    }

    if (node->SE) {
        serializeTreeNode(node->SE, buffer);
    } else {
        buffer.push_back(0); // No SE child
    }
}

void deserializeTreeNode(QuadTree *node, const std::vector<char> &buffer, size_t &offset) {
    if (offset >= buffer.size()) {
        return;
    }
  
    // Deserialize the body flag and body index
    bool has_body = buffer[offset++];
    if (has_body) {
        node->body = new Body(); // Create a new Body object
        node->body->idx = *reinterpret_cast<const int*>(&buffer[offset]);
        offset += sizeof(int);
        node->body->mass = *reinterpret_cast<const double*>(&buffer[offset]);
        offset += sizeof(double);
        node->body->xpos = *reinterpret_cast<const double*>(&buffer[offset]);
        offset += sizeof(double);
        node->body->ypos = *reinterpret_cast<const double*>(&buffer[offset]);
        offset += sizeof(double);
        node->body->xvel = *reinterpret_cast<const double*>(&buffer[offset]);
        offset += sizeof(double);
        node->body->yvel = *reinterpret_cast<const double*>(&buffer[offset]);
        offset += sizeof(double);
    } else {
        node->body = nullptr;
    }
    
    // Deserialize the quad data
    node->quad = *reinterpret_cast<const Quad*>(&buffer[offset]);
    offset += sizeof(Quad);

    // Deserialize the child nodes recursively
    if (buffer[offset] == 1) {
        node->NW = new QuadTree(node->quad.NW());
        offset++;
        deserializeTreeNode(node->NW, buffer, offset);
    } else {
        offset++; // Skip
    }

    if (buffer[offset] == 1) {
        node->NE = new QuadTree(node->quad.NE());
        offset++;
        deserializeTreeNode(node->NE, buffer, offset);
    } else {
        offset++; // Skip
    }

    if (buffer[offset] == 1) {
        node->SW = new QuadTree(node->quad.SW());
        offset++;
        deserializeTreeNode(node->SW, buffer, offset);
    } else {
        offset++; // Skip
    }

    if (buffer[offset] == 1) {
        node->SE = new QuadTree(node->quad.SE());
        offset++;
        deserializeTreeNode(node->SE, buffer, offset);
    } else {
        offset++; // Skip
    }
}