/* Header Guard */
#pragma once

#include <vector>

#include "Disease.h"
#include "Common.h"

typedef unsigned int Person_uint32;
typedef std::string Person_string;

typedef enum{
    
    Person_Sick = 0U,
    Person_Susceptible,
    Person_Recovered,
    Person_Vaccinated,

}PersonStatus;


typedef enum{
    
    Person_FullyProne = 0U,
    Person_FullyImmune = 1U,

}PersonImmunity;

class Population;

class Person{


protected:

	/* Personal details */
	PersonStatus Person_CurrentStatus;
	Person_uint32 Person_NumOfInfectDays;
	Disease *DPtr;

	PersonImmunity Person_EvaluatePersonStatus(void);

	void Person_HandleStateTransition(PersonStatus CurrentState, PersonStatus NextState);

private:

	Population *POPULATION_RTE_INSTANCE;



public:

	Person(Population *POPULATION_INSTANCE);

	~Person();

	/* Vaccinate the person against the disease */
	void Person_DiseaseGetVaccinated(void);

	/* Infect a person with a certain disease. */
	void Person_DiseaseGetInfected(Disease *);

	/* Getting in touch with people */
	void Person_PersonTouch(Person *PPtr);

	/* Return the current status of a person */
	PersonStatus Person_GetCurrentStatus(void);

	/* Move a person to live for another day. */
	void Person_ForOneMoreDay(void);

	Person_uint32 Person_GetNumberToBeInfected(Disease_IntType NumOfPeople);


};


typedef Person * PersonPtr;

