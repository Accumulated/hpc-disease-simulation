#include <iostream>

/* User defined include */
#include "Common.h"
#include "Disease.h"
#include "Population.h"
#include "PopulationTestHook.h"

using namespace std;

PopulationTestHook:: PopulationTestHook(
						Population_uint32 PopSize,
						Population_float VRate,
						Population_bool StartInfected,
						Population_string PopName,
						Disease *DPtr) :
								Population(PopSize, VRate, StartInfected, PopName, DPtr){

	/* Just initialize the population normally */
	_DEBUG_PRINT_((std::string) "PopulationTestHook - constructor call");

}

Population_uint32 PopulationTestHook:: PopulationTestHook_CountInfected(void){

	return this -> Population_CountInfected();
}


Population_uint32 PopulationTestHook:: PopulationTestHook_CountVaccinated(void){

	return this -> Population_CountVaccinated();

}


Population_uint32 PopulationTestHook:: PopulationTestHook_GetPopulationSize(void){

	return this -> PopulationSize;

}


Population_float PopulationTestHook:: PopulationTestHook_GetVaccinationRate(void){

	return this -> VaccinationRate;
}


Population_bool PopulationTestHook:: PopulationTestHook_GetPatientZero(void){

	return this -> PatientZero;
}


Population_string PopulationTestHook:: PopulationTestHook_GetPopulationName(void){

	return this -> PopulationName;
}


Population_uint32 PopulationTestHook:: PopulationTestHook_GetNumOfVaccinated(void){

	return this -> NumOfVaccinated;

}

Population_bool PopulationTestHook:: PopulationTestHook_SanityCheck(void){

	Population_uint32 Local_NumOfInfected = 0U;
	Population_uint32 Local_NumOfVacc = 0U;
	Population_bool RetVal = true;

	for(auto& vec : this -> PopulationVector){

		if(vec.Person_GetCurrentStatus() == Person_Vaccinated){

			Local_NumOfVacc++;

		}
		else if(vec.Person_GetCurrentStatus() == Person_Sick){
			Local_NumOfInfected++;
		}
		else{
			/* Nothing to do here */
		}
	}

	if((Local_NumOfVacc != this -> PopulationTestHook_CountVaccinated()) ||
		(Local_NumOfInfected != this -> PopulationTestHook_CountInfected())){

		RetVal = false;
	}


	return RetVal;
}

