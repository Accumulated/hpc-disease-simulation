#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"
#include "INIReader.h"
#include "Disease.h"
#include "Population.h"
#include "Simulation.h"
#include "SequentialSimulation.h"
#include <filesystem>

using namespace std;


void SequentialSimulation:: Simulation_InitConfirmation(void){

	/* Just confirm that sequential simulation is initialized */;

}


SequentialSimulation:: ~SequentialSimulation(void){

	Simulation_CleanExit();

}


SequentialSimulation:: SequentialSimulation(std::string in_file){


	INIReader Reader(in_file);
	cout << "Current directory is: " << std::filesystem::current_path() << endl;
	
	
	
	
	DiseaseCfg = new Disease(Reader.GetInteger((std::string )"disease", (std::string)"duration", -1),
							 Reader.GetFloat((std::string )"disease", (std::string)"transmissability", -1),
							 Reader.Get((std::string )"disease", (std::string)"name", (std::string)""));

	simulation_name = Reader.Get((std::string) "global", (std::string)"simulation_name", (std::string)"");
	num_populations = Reader.GetInteger((std::string )"global", (std::string)"num_populations", 0);
	simulation_runs = Reader.GetInteger((std::string) "global", (std::string)"simulation_runs", 0);

    disease_details_csv = ABSL_PATH + "Disease_Details.csv";
	disease_stats_csv = ABSL_PATH + "Disease_Stats.csv";

	for(int i = 1; i <= num_populations; i++){

		PopulationCfg.push_back(
				new Population(
						Reader.GetInteger((std::string )("population_" + std::to_string(i)),
										  (std::string)"size",
										  0),
						Reader.GetFloat((std::string )("population_" + std::to_string(i)),
										(std::string)"vaccination_rate",
										0),

						Reader.GetBoolean((std::string )("population_" + std::to_string(i)),
										 (std::string)"patient_0",
										 0),

						Reader.Get((std::string )("population_" + std::to_string(i)),
									(std::string)"name",
									(std::string)""),

						DiseaseCfg)
				);
	}

}
