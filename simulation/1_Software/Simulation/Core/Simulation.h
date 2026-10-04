#pragma once

#include <iostream>
#include <vector>
#include "Population.h"
#include "Common.h"


typedef unsigned int Sim_uint32;
typedef unsigned long Sim_long;
typedef bool Sim_bool;
typedef std::vector<std::string> Simulation_String;
typedef std::vector<Population *> Simulation_VecOfPops;


/* Abstract class for an IS-A relation */

class Simulation {


protected:
	std::string input_file = (std::string) "disease_in.ini";
	std::string simulation_name;
	std::string disease_details_csv;
	std::string disease_stats_csv;
	Sim_uint32 num_populations = 0U;
	Sim_uint32 simulation_runs = 0U;
	Sim_uint32 Sim_TimeSteps = 0U;
	DiseasePtr DiseaseCfg = NULL;
	Simulation_VecOfPops PopulationCfg;
	Simulation_String RunTime_Data;


public:

  virtual void start(void);

  void Simulation_CleanExit(void);

  Simulation(void);

  virtual ~Simulation();

  void Simulation_SaveDetails(void);

  void Simulation_GetDetails(void);

  void Simulation_SaveStats(Sim_long SuscPersons,
							Sim_long RecovPerson,
							Sim_long VaccPersons);

  void Simulation_PrintRuntimeInfo_Summary(void);

  /* Protect against making objects from this class, only inherit. */
  virtual void Simulation_InitConfirmation() = 0;

};
