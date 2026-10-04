#include <iostream>
#include <vector>
#include <fstream>

#include "Common.h"
#include "Simulation.h"
#include "Population.h"


Simulation:: ~Simulation(void){

	_DEBUG_PRINT_((std::string) "Calling Basic simulation class destructor \n");

}


Simulation:: Simulation(void){

	_DEBUG_PRINT_((std::string) "Calling basic simulation constructor \n");
}


void Simulation:: Simulation_CleanExit(void){

	/* Population pointer contains info per process. */
	for (auto& populationPtr : PopulationCfg) {
			delete populationPtr;
	}

	delete DiseaseCfg;

	PopulationCfg.clear();

}


void Simulation:: start() {

	std::cout << "Starting simulation..." << std::endl;

	Sim_bool SimulationTermination = true;

	while(1){


		/* reset the condition flag */
		SimulationTermination = true;

		Simulation_GetDetails();

		/* Check whether to terminate the simulation or not */
		for(Sim_uint32 i = 0; i < PopulationCfg.size(); i++){

			/* Terminate only when you have no more */
			SimulationTermination &= PopulationCfg[i] -> Population_CountInfected() > 0 ? false: true;

		}

		Sim_TimeSteps++;

		if(SimulationTermination){

			Simulation_PrintRuntimeInfo_Summary();
			Simulation_SaveDetails();
			break;
		}


		for(Sim_uint32 i = 0; i < PopulationCfg.size(); i++){

			PopulationCfg[i] -> Population_Runnable();

		}

	}

	std::cout << "Simulation terminated ..." << std::endl;


}


void Simulation:: Simulation_SaveDetails(void){

	  std::ofstream file((std::string) disease_details_csv, std::ios::out);

	if (!file.is_open()) {
		std::cerr << "Error: Unable to open file for writing." << std::endl;
		return;
	}
	else{

	}


	for (const auto& D : RunTime_Data){
		file << D;
		file << std::endl;
	}

}


void Simulation:: Simulation_GetDetails(void){

  	for(int i = 0; i < PopulationCfg.size(); i++){

  		RunTime_Data.push_back(
  				PopulationCfg[i] -> Population_GetName() + ", " +
  				std::to_string(PopulationCfg[i] -> Population_CountInfected()) + ", " +
  				std::to_string(PopulationCfg[i] -> Population_CountRecovered()) + ", " +
  				std::to_string(PopulationCfg[i] -> Population_CountSusceptible()) + ", " +
  				std::to_string(PopulationCfg[i] -> Population_CountVaccinated()));
  	}

}


void Simulation:: Simulation_SaveStats(Sim_long SuscPersons,
  										Sim_long RecovPerson,
  										Sim_long VaccPersons){

  	std::vector<std::string> Data;
  	std::ofstream file(disease_stats_csv);

  	Data.push_back("total_steps, " + std::to_string(Sim_TimeSteps));
  	Data.push_back("susceptiple_persons, " + std::to_string(SuscPersons));
  	Data.push_back("recovered_persons, " + std::to_string(RecovPerson));
  	Data.push_back("vaccinated_persons, " + std::to_string(VaccPersons));

  	if (!file.is_open()) {
  		std::cerr << "Error: Unable to open file for writing." << std::endl;
  		return;
  	}

  	for (const auto& D : Data){
  		file << D;
  		file << std::endl;
  	}

}


void Simulation:: Simulation_PrintRuntimeInfo_Summary(void){

  	Sim_long Sim_TotalVaccinated = (Sim_long)0;

  	Sim_long Sim_TotalRecovered = (Sim_long)0;

  	Sim_long Sim_TotalSusceptible = (Sim_long)0;

  	Sim_long Sim_TotalSize = (Sim_long)0;


  	std::cout << "Number of steps taken: " << Sim_TimeSteps << std::endl;


  	for(int i = 0; i < PopulationCfg.size(); i++){

  		Sim_TotalVaccinated += PopulationCfg[i] -> Population_CountVaccinated();
  		Sim_TotalRecovered += PopulationCfg[i] -> Population_CountRecovered();
  		Sim_TotalSusceptible += PopulationCfg[i] -> Population_CountSusceptible();

  	}

  	std::cout << "Number of Vaccinated: " << Sim_TotalVaccinated << "\n";

  	std::cout << "Number of recovered: " << Sim_TotalRecovered << "\n";

  	std::cout << "Number of Susceptible: " << Sim_TotalSusceptible << "\n";

  	Simulation_SaveStats(Sim_TotalSusceptible,
  							Sim_TotalRecovered,
  							Sim_TotalVaccinated);
}
