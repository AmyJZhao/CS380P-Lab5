#include <iostream>
#include <argparse.h>
#include <io.h>
#include <vector>
#include "barnes_hut_struct.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>
#include <GL/glut.h>
using namespace std;

// ./nbody -i input/nb-10.txt -o out.txt -s 10 -t 0.00004 -d 0.5
int main(int argc, char **argv)
{
    // Parse args
    struct options_t opts;
    get_opts(argc, argv, &opts);
    vector<Body> bodies;
    int num_bodies;
    read_file(&opts, &num_bodies, bodies);
    printf("Input file: %s\nOutput file: %s\nSteps: %d\nTheta: %f\nTimestep: %f\nVisual: %s\n", 
            opts.inputfilename, opts.outputfilename, opts.steps, opts.theta, opts.timestep, opts.visual?"true":"false");
    int max = min(static_cast<int>(bodies.size()), 100);
    for (int i = 0; i < max; i++) {
        cout << "Index: " << bodies[i].idx <<
               ", X Pos: " << bodies[i].xpos <<
               ", Y Pos: " << bodies[i].ypos <<
               ", Mass: " << bodies[i].mass << 
               ", X Vel: " << bodies[i].xvel <<
               ", Y Vel: " << bodies[i].yvel << endl;
    }
    Quad domain(4.0, 0.0, 0.0);
    QuadTree tree(domain);
    for (int i = 0; i < max; i++) {
        tree.insert(bodies[i]);
    }
    tree.printTree();
    /**
    // OpenGL window dims
    int width=600, height=600;
    GLFWwindow* window;
    if( !glfwInit() ){
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( width, height, "Simulation", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    **/
}