#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <set>
#include <typeinfo>
#include <chrono>

/* User defined include */
#include "Common.h"
#include "Disease.h"
#include "Person.h"
#include "Population.h"

using namespace std;

/* Population main function */
void Population:: Population_Runnable(void){

    this -> Population_PeopleInteraction();

    this -> Population_ProgressAllPeople();

	_ASSERT_( (bool)
			((this -> Population_CountInfected() +
			this -> Population_CountRecovered() +
			this -> Population_CountSusceptible() +
			this -> Population_CountVaccinated())==
					this -> Population_GetSize())
			, (std::string) "Error in tracking population \n");

}


Population::Population(Population_uint32 PopSize,
						Population_float VRate,
						Population_bool StartInfected,
						Population_string PopName,
						DiseasePtr DPtr):
		PopulationSize(PopSize),
		VaccinationRate(VRate),
		PatientZero(StartInfected),
		PopulationName(PopName),
		NumOfInfected(0U),
		NumOfVaccinated(0U),
		NumOfRecovered(0U),
		NumOfSusceptible(PopSize),
		Population_FullInfection(false),
		Population_IntraInteraction(6.0),
		/* Allocate vector of Person class of population size */
		PopulationVector(PopulationSize, Person(this)){

	/* Have a vector of random numbers generated from the population */
	Population_SetInt RandomSet;

	/* Hold the number of to be infected to a certain value for
	 * intra-population interaction where people from the same
	 * population interact with each other.
	 * */
	Population_ToBeInfected = ceil (DPtr -> Disease_GetChanceOfTransmission() *
							 	 	 Population_IntraInteraction)	;

	srand((unsigned) time(NULL));

	/* Calculate size of random numbers to be generated. */
	Population_uint32 SizeOfRandomNums = (Population_uint32) ceil(VaccinationRate * PopulationSize) +
										 (Population_uint32) PatientZero;

	_ASSERT_((bool) (SizeOfRandomNums <= PopulationSize), (std::string) "Logical error in loaded configuration\n");

	/* Get random numbers generated, the amount of numbers depend mainly
	 * on whether you have a patient zero or not and also the
	 * vaccination rate for a population to start with.
	 * */
	Population_GenerateRandomNumbers(RandomSet, SizeOfRandomNums, NULL);

	/* Vaccinate these random people */
	Population_RandomVaccination(RandomSet);

	/* Infect this random Person */
	Population_RandomInfection(RandomSet, DPtr);

    _DEBUG_PRINT_("Population " + PopulationName + " initialized");

#ifndef _DEPLOYMENT_MODE_
				cout << "Population " + PopulationName + " initialized\n";
#endif
}


void Population:: Population_GenerateRandomNumbers(Population_SetInt &RandomSet,
													Population_uint32 RandomSize,
													PersonPtr RestrictedPerson){

	/* WARNING: This function needs more optimization and a better approach. */

	Population_uint32 TmpIndex = 0U;

	if(RandomSize > PopulationSize){

		/* Corner case for expanding infection. */

	}
	else{

	    /* Get a total of: Number of random vaccinated people + whether we start with
	     * a random infectious person or not.
	     * */
		while (RandomSet.size() < RandomSize) {

			TmpIndex = rand() % PopulationSize;

			if(&(PopulationVector[TmpIndex]) == RestrictedPerson){
				continue;
			}
			else{
				RandomSet.insert(TmpIndex);
			}

		}

	}

}


void Population::Population_RandomInfection(Population_SetInt &RandomSet, DiseasePtr DPtr){

    if(PatientZero){

    	/* Get the last index from the vector as it should have included
    	 * this part in the generation phase
    	 * */

    	PopulationVector[*(RandomSet.rbegin())].Person_DiseaseGetInfected(DPtr);

    }
    else{

    	/* Don't start with an infected person */

    }
}


void Population::Population_RandomVaccination(Population_SetInt &RandomSet){

	Population_uint32 NumberOfVacc = (Population_uint32) ceil(VaccinationRate * PopulationSize);
	int i = 0;

    for (auto it = RandomSet.begin(); it != RandomSet.end() && i < NumberOfVacc; it++, i++) {

    	PopulationVector[*(it)].Person_DiseaseGetVaccinated();

    }

    _ASSERT_((bool)(NumberOfVacc == (RandomSet.size() - (Population_uint32) PatientZero)),
    		(std::string) "Error in vector size");

}



void Population:: Population_SearchForInfectedPerson(PersonPtr *PPtr){

	if(PopulationQuarantine_Set.size()){

		/* Return the key for the first element in this map, which
		 * refers to a person address in memory where you will find
		 * that this person is sick.
		 * */
		*PPtr = *PopulationQuarantine_Set.begin();

		_ASSERT_((bool) ((*PPtr) -> Person_GetCurrentStatus() == Person_Sick),
				(std::string) "This person isn't infected");

	}
	else{
		/* No elements in this map. force a NULL address.
		 * This just for security reasons, PPtr might have
		 * garbage in the address it refers to.
		 * */
		*PPtr = NULL;
	}

}


void Population:: Population_PeopleInteraction(void){

	PersonPtr InfectedPerson_Running = NULL;
	Population_uint32 ToBeInfected = 0U;
	Population_uint32 NumOfInfected = PopulationQuarantine_Set.size();

	Population_SetInt RandomSet;

	this -> Population_SearchForInfectedPerson(&InfectedPerson_Running);

	if(NumOfInfected == this -> PopulationSize){
		/* All population are infected, redundant function call.
		 * You should do nothing. Setting the Population_FullInfection
		 * to be true is necessary to avoid wasting time for useless
		 * search for other people to infect.
		 *
		 * WARNING: OOD design violation here, forcing this kind of
		 * behavior in this class should be abstracted by another
		 * class that actually define how people interact.
		 * */
		this -> Population_FullInfection = true;
	}
	else{

		for(Population_uint32 it = 0;
			(it < NumOfInfected) && (this -> Population_FullInfection == false);
			it++){

			/* WARNING: There is a bug in this function call, a person can meet himself.
			 * Such a non-sense :) -> Refactor needed (To be considered when integrating
			 * MPI solution).
			 * SOLUTION: Temporary solution is to send the address of the current infected
			 * person and make sure that whatever index is generated by the random generator
			 * doesn't reflect to the address of the same person. (EXPENSIVE SOLUTION)
			 * */
			Population_GenerateRandomNumbers(RandomSet,
											this -> Population_ToBeInfected,
											InfectedPerson_Running);


			/* Connect the infected person with all these 6 people. 6 is
			 * hard coded. (REQ)
			 * */
			for(auto &i : RandomSet){

				this -> PopulationVector[i].Person_PersonTouch(InfectedPerson_Running);

			}

			RandomSet.clear();

		}
	}
}


void Population:: Population_QuarantinePerson(PersonPtr PPtr){

	PopulationQuarantine_Set.insert(PPtr);

}


Population_Set::iterator Population:: Population_DeQuarantinePerson(PersonPtr PPtr){

	Population_Set::iterator iter = PopulationQuarantine_Set.find(PPtr);
	Population_Set::iterator nextIter;

	if (iter != (PopulationQuarantine_Set.end())) {

		nextIter = std::next(iter);

		/* Erase method should return the next iterator only if you gave
		 * it a certain type of input.
		 * HINT: Don't give a key and expect an iterator in return. Give an
		 * iterator and expect the same in return; what an ungratful method user.
		 * */
		PopulationQuarantine_Set.erase(PPtr);

		iter = nextIter;
	}
	else {

		iter = PopulationQuarantine_Set.end();

		_ASSERT_(true, (std::string) "Attempted to De-Quarantine a person");

	}

	return iter;
}


void Population:: Population_ProgressAllPeople(void){

	for (Population_Set::iterator it = PopulationQuarantine_Set.begin();
			it != PopulationQuarantine_Set.end();
			/* The value for the next it is decided within
			 * the loop.
			 * */
			){

		(*it) -> Person_ForOneMoreDay();

		if((*it) -> Person_GetCurrentStatus() != Person_Sick){

			/* Feel free to remove him from Quarantine. */
			it = Population_DeQuarantinePerson((*it));

		}
		else{

			/* Just go on with the next sick person. */
			it++;
		}
	}
}


void Population:: Population_DecrementNumOfInfected(void){

	this -> NumOfInfected--;
}


void Population:: Population_DecrementNumOfSusceptible(void){

	this -> NumOfSusceptible--;
}


void Population:: Population_IncrementNumOfInfected(void){

	this -> NumOfInfected++;

}


void Population:: Population_IncrementNumOfRecovered(void){

	this -> NumOfRecovered++;

}


void Population:: Population_IncrementNumOfVaccinated(void){

	this -> NumOfVaccinated++;

}


Population_uint32 Population::Population_GetSize(void){

	return this -> PopulationSize;

}


Population_string Population::Population_GetName(void){

	return this -> PopulationName;

}


Population_uint32 Population::Population_CountInfected(void){

	return this -> NumOfInfected;
}


Population_uint32 Population::Population_CountVaccinated(void){

	return this -> NumOfVaccinated;
}


Population_uint32 Population::Population_CountRecovered(void){

	return this -> NumOfRecovered;
}


Population_uint32 Population::Population_CountSusceptible(void){

	return this -> NumOfSusceptible;
}


Population::~Population(void){

	_DEBUG_PRINT_((std::string) "Population died");

}
