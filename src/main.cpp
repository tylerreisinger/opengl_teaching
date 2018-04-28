#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include "Simulation.h"

int main(int argc, char** argv) {
    Simulation sim;
    sim.run();

    return 0;
}
