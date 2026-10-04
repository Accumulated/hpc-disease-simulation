#pragma once

#include <iostream>

/* User defined include */
#include "Common.h"
#include "Disease.h"
#include "Population.h"

using namespace std;

/* This is just an interface - Class which has only functions as members */
class PopulationTestHook : public Population{

public:

	PopulationTestHook(Population_uint32 PopSize,
						Population_float VRate,
						Population_bool StartInfected,
						Population_string PopName,
						Disease *DPtr);

	Population_uint32 PopulationTestHook_CountInfected(void);

	Population_uint32 PopulationTestHook_CountVaccinated(void);

	Population_uint32 PopulationTestHook_GetPopulationSize(void);

	Population_uint32 PopulationTestHook_GetNumOfVaccinated(void);

	Population_float PopulationTestHook_GetVaccinationRate(void);

	Population_bool PopulationTestHook_GetPatientZero(void);

	Population_string PopulationTestHook_GetPopulationName(void);

	Population_bool PopulationTestHook_SanityCheck(void);


};


