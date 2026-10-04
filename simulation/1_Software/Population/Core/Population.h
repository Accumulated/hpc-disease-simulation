/* Header Guard */
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <random>
#include <set>

/* User defined include */
#include "Common.h"
#include "Disease.h"
#include "Person.h"


using namespace std;

typedef unsigned int Population_uint32;
typedef float Population_float;
typedef std::string Population_string;
typedef bool Population_bool;
typedef vector<Person> Population_Vector;
typedef std::unordered_set<PersonPtr> Population_Set;
typedef vector<unsigned int> Population_VectorInt;
typedef std::set<Population_uint32> Population_SetInt;
typedef Population *	PopulationPtr;



class Population {


protected:
	/* Update the following parameters from the configuration */
	Population_uint32 PopulationSize;
	Population_float VaccinationRate;
	Population_bool PatientZero;
	Population_string PopulationName;

	/* Keep track of number of the following */
	Population_uint32 NumOfInfected;
	Population_uint32 NumOfRecovered;
	Population_uint32 NumOfVaccinated;
	Population_uint32 NumOfSusceptible;

	/* Vector to store all people inside a population */
	Population_Vector PopulationVector;

	/* Set of Quarantined people - A set data structure is used
	 * to simulate a dictionary key-value pair where the key
	 * is the person address in memory, the value is redundant -
	 * defaults to 0U.
	 * */
	Population_Set PopulationQuarantine_Set;


private:

	/* Boolean variable that should be initialized to false, and only
	 * set to true once the whole population is infected.
	 * This restricts redundant searching for people to infect.
	 * */
	Population_bool Population_FullInfection;

	/* As per the requirement: Each infected person is going
	 * to try to infect certain number of random people.
	 * This value is hard coded in the constructor based on
	 * disease configuration and a value dependent on the
	 * requirements.
	 * */
	Population_uint32 Population_ToBeInfected;

	/* This value is hard coded in the constructor. */
	const Population_float Population_IntraInteraction;

	void Population_GenerateRandomNumbers(Population_SetInt & RandomSet,
											Population_uint32 RandomSize,
											PersonPtr RestrictedPerson);

	void Population_RandomInfection(Population_SetInt &RandomSet, Disease *DPtr);

	void Population_RandomVaccination(Population_SetInt &RandomSet);

	void Population_SearchForInfectedPerson(PersonPtr *);

	void Population_PeopleInteraction(void);

	void Population_ProgressAllPeople(void);


public:

	Population(Population_uint32 PopSize,
				Population_float VRate,
				Population_bool StartInfected,
				Population_string PopName,
				DiseasePtr DPtr);

	~Population();

	Population_uint32 Population_CountInfected(void);

	Population_uint32 Population_CountVaccinated(void);

	Population_uint32 Population_CountRecovered(void);

	Population_uint32 Population_CountSusceptible(void);

	Population_uint32 Population_GetSize(void);

	Population_string Population_GetName(void);

	void Population_Runnable(void);

	void Population_IncrementNumOfInfected(void);

	void Population_DecrementNumOfSusceptible(void);

	void Population_IncrementNumOfRecovered(void);

	void Population_DecrementNumOfInfected(void);

	void Population_IncrementNumOfVaccinated(void);

	void Population_QuarantinePerson(PersonPtr PPtr);

	Population_Set::iterator Population_DeQuarantinePerson(PersonPtr PPtr);

};





