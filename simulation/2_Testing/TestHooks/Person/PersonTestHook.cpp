#include <iostream>
#include "Common.h"
#include "Person.h"
#include "PersonTestHook.h"
#include "Population.h"
#include "Disease.h"

using namespace std;

/* Stubbed variables */
Disease DiseaseDetails(10, 0.7, (std::string) "TmpName");

Population DummyPopulation(1, 0.0, 0, (Population_string) "NA", &DiseaseDetails);


PersonImmunity PersonTestHook:: PersonTHook_EvaluatePersonStatus(void){

	return this -> Person_EvaluatePersonStatus();

}

void PersonTestHook:: PersonTHook_HandleStateTransition(PersonStatus CurrentState, PersonStatus NextState){

	this -> Person_HandleStateTransition(CurrentState, NextState);

}


PersonTestHook:: PersonTestHook(): Person(&DummyPopulation){

	POPULATION_RTE_INSTANCE_HOOK = &DummyPopulation;

	_DEBUG_PRINT_((std::string) "PersonTestHook - Default constructor call");

}

Person_uint32 PersonTestHook::PersonTHook_GetNumOfInfectDays(void){

	return this -> Person_NumOfInfectDays;

}
