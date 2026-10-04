#include <iostream>
#include "Simulation.h"
#include "SequentialSimulation.h"
#include "MPI_MultiPopulation.h"

int main(void) {

    std::cout << "Loading Disease Simulation..." << std::endl;

#ifdef _DEPLOYMENT_MODE_
    Simulation *Sim = new MPISimulation();

#else

    Simulation *Sim = new SequentialSimulation();;

#endif

    Sim -> start();

	delete Sim;

    return 0;

}
