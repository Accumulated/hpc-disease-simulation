#pragma once

#include <string>
#include <vector>
#include "Disease.h"
#include "Population.h"
#include "Simulation.h"


class SequentialSimulation : public Simulation {

public:

  ~SequentialSimulation();
  SequentialSimulation(std::string in_file = "disease_in.ini");
  virtual void Simulation_InitConfirmation(void);

};
