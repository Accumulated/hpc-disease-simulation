#pragma once

#include <string>
#include <vector>
#include "Disease.h"
#include "Population.h"
#include "Simulation.h"




class MPISimulation : public Simulation {

private:
	Sim_uint32 PerProcessPopulations = 0U;

public:
  ~MPISimulation();
  MPISimulation(std::string in_file = "disease_in.ini");
  virtual void Simulation_InitConfirmation(void);


};
