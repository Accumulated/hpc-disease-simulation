#include <iostream>
#include "Common.h"
#include "Person.h"
#include "Population.h"

using namespace std;


class PersonTestHook : public Person{

public:


	Population *POPULATION_RTE_INSTANCE_HOOK;

	PersonTestHook();

	Person_uint32 PersonTHook_GetNumOfInfectDays(void);

	PersonImmunity PersonTHook_EvaluatePersonStatus(void);

	void PersonTHook_HandleStateTransition(PersonStatus CurrentState, PersonStatus NextState);

};


