#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <fstream>


/* 				User defined header files 				*/
#include "mpi.h"
#include "Common.h"
#include "INIReader.h"
#include "Disease.h"
#include "Population.h"
#include "Simulation.h"
#include "MPI_MultiPopulation.h"





/* 				Class implementation 					*/
using namespace std;

MPISimulation:: ~MPISimulation(void){

	_DEBUG_PRINT_((std::string) "MPI simulation de-constructor");

	Simulation_CleanExit();

    MPI_Finalize();

}


void MPISimulation:: Simulation_InitConfirmation(void){

	/* Just confirm that MPI simulation is initialized */;

}


MPISimulation:: MPISimulation(std::string in_file){

	/* Design choice:
	 * - "All processes are executing the simulation by dividing number
	 *   of population between them. "
	 *
	 * - "At no-point in the entire lifetime of the simulation, no process
	 *   is going to communicate with another process, no dependency allowed. "
	 *
	 * 1. Initialize MPI.
	 * 2. All processes are going to read the configuration file.
	 * 3. All processes are using a sorted map data structure to
	 *    save key-value pair where the key is population.
	 *
	 * */

	_DEBUG_PRINT_((std::string) "MPI simulation");

	int Procrank = 0, NumOfProc = 0;
	INIReader Reader(ABSL_PATH + in_file);
    std::multimap<Sim_uint32, std::string> MapOfPopulation_Sorted;


	MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &Procrank);
    MPI_Comm_size(MPI_COMM_WORLD, &NumOfProc);

    disease_details_csv = "Disease_Details_" + to_string(Procrank) + ".csv";
	disease_stats_csv = "Disease_Stats_" + to_string(Procrank) + ".csv";

	/* All processes are going to allocate the following:
	 * 1. Disease configuration
	 * 2. Population configuration vector [dependent on a certain
	 * 		work-distribution criterion.
	 * 3. Other configuration information for the simulation.
	 * */
	DiseaseCfg = new Disease(Reader.GetInteger((std::string )"disease", (std::string)"duration", -1),
							 Reader.GetFloat((std::string )"disease", (std::string)"transmissability", -1),
							 Reader.Get((std::string )"disease", (std::string)"name", (std::string)""));

	simulation_name = Reader.Get((std::string) "global", (std::string)"simulation_name", (std::string)"");
	num_populations = Reader.GetInteger((std::string )"global", (std::string)"num_populations", 0);
	simulation_runs = Reader.GetInteger((std::string) "global", (std::string)"simulation_runs", 0);

	if(num_populations > NumOfProc){

		/* Every process is going this marathon - calculate
		 * number of populations per process.
		 * */
		PerProcessPopulations = num_populations / NumOfProc;

		if(Procrank == NumOfProc - 1){

			PerProcessPopulations += num_populations % NumOfProc;

		}
		else{

			/* Nothing to do for other processes. */

		}
	}

	else if(num_populations < NumOfProc){

		/* only certain processes are going to work while the others
		 * are idle (load imbalance issue - known bug)
		 * */

		if(Procrank < num_populations){

			/* Only these processes are working. i.e. in a 2 population
			 * and 4 processes -> Only process 0 and 1 are working.
			 * The others are idle (Load imbalance issue - known but)
			 * */
			PerProcessPopulations = 1U;

		}
		else{

			/* These processes are going to be idle. (Load imbalance
			 * issue - known but).
			 * */
			PerProcessPopulations = 0U;
		}
	}
	else{

		/* Number of population is equal to number of processes.
		 * Each process is taking a population.
		 * */

		PerProcessPopulations = 1U;

	}

	for(int i = 1; i <= num_populations; i++){

		MapOfPopulation_Sorted.insert(
				{
					Reader.GetInteger((std::string )("population_" + std::to_string(i)),
								  (std::string)"size",
								  0),
					std::to_string(i)
				}
		);
	}

	int i_iter = MapOfPopulation_Sorted.size();
	int i_StartIndex = num_populations - Procrank * (num_populations / NumOfProc);
	int i_EndIndex = i_StartIndex - PerProcessPopulations;


	_DEBUG_PRINT_("Process: " + to_string(Procrank) +
								" starts at " + to_string(i_StartIndex) +
								" and ends at: " + to_string(i_EndIndex) + '\n');

    for(auto it = MapOfPopulation_Sorted.rbegin();
		it != MapOfPopulation_Sorted.rend();
		++it){

    	if((i_iter <= i_StartIndex) && (i_iter > i_EndIndex)){

    		PopulationCfg.push_back(
    				new Population(
    						Reader.GetInteger((std::string )("population_" + it -> second),
    										  (std::string)"size",
    										  0),
    						Reader.GetFloat((std::string )("population_" + it -> second),
    										(std::string)"vaccination_rate",
    										0),

    						Reader.GetBoolean((std::string )("population_" + it -> second),
    										 (std::string)"patient_0",
    										 0),

    						Reader.Get((std::string )("population_" + it -> second),
    									(std::string)"name",
    									(std::string)""),

    						DiseaseCfg)
    				);

    		_DEBUG_PRINT_("Process: " + to_string(Procrank) +
							" has population of size: " + to_string(it->first) +
							" -> Population called: " +
							PopulationCfg.back() -> Population_GetName()+ '\n');

    	}
    	else{

    		/* Do nothing */
    	}

    	i_iter--;
    }

    std::cout << "Initialization for " + to_string(Procrank) + " is done \n";

}

