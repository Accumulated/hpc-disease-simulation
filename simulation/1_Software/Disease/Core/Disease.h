#pragma once

#include <string>

typedef unsigned int Disease_IntType;
typedef float Disease_FloatType;
typedef std::string Disease_StrType;


class Disease{

    private:
		Disease_IntType Disease_NumOfRecovDays;
		Disease_FloatType Disease_ChanceOfTransmission;
		Disease_StrType Diease_Name;

    public:

        Disease(Disease_IntType RecovDays,
				Disease_FloatType TransmissionProb,
				Disease_StrType Name);

        ~Disease();

        Disease_IntType Disease_GetSicknessLength(void);

        Disease_FloatType Disease_GetChanceOfTransmission(void);

};



typedef Disease * DiseasePtr;
