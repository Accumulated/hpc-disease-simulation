#include <iostream>
#include "Disease.h"
#include "Common.h"

using namespace std;


Disease_IntType Disease::Disease_GetSicknessLength(void){

    return this -> Disease_NumOfRecovDays;

}


Disease_FloatType Disease:: Disease_GetChanceOfTransmission(void){

	return this -> Disease_ChanceOfTransmission;
}


Disease::Disease(Disease_IntType RecovDays,
				Disease_FloatType TransmissionProb,
				Disease_StrType Name):
		/* Initialize internal private variables */
		Disease_NumOfRecovDays(RecovDays),
		Disease_ChanceOfTransmission(TransmissionProb),
		Diease_Name(Name){

	_DEBUG_PRINT_((std::string) "Disease " + Name + " has been initialized");

}


Disease:: ~Disease(void){

	_DEBUG_PRINT_((std::string) "Disease ceased to exist");

}

