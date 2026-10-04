#include <iostream>
#include <cmath>

/* User defined include */
#include "Common.h"
#include "Population.h"
#include "Person.h"
#include "Disease.h"

using namespace std;


PersonStatus Person::Person_GetCurrentStatus(void){

    return this -> Person_CurrentStatus;

}


void Person::Person_ForOneMoreDay(void){

    /* Check whether the number of days are through or not */
    if(this -> Person_NumOfInfectDays == (Person_uint32) 0){

        /* Nothing to do, the person has already recovered */

    }

    else{

        /* Most probably he is still sick, keep him going for another day */
        this -> Person_NumOfInfectDays--;

        /* This logic should be replaced with another more solid logic
         * for checking over mutually exclusive/inclusive states.
         */
        if(this -> Person_NumOfInfectDays == (Person_uint32) 0){
            
            /* This person has successfully recovered, update his status. */
            this -> Person_HandleStateTransition(this -> Person_CurrentStatus,
            										Person_Recovered);


            /* Exit from Quarantine area is done by the population itself.
             * A person should never make such a decision.
             * */

        }
        else{

            /* Nothing new here, the person is still sick. */

        }
    }
}


void Person::Person_DiseaseGetVaccinated(void){

    /* Getting vaccinated as per the requirements mean 2 things:
    * 1. The person can never get sick again for the same disease.
    * 2. The person recovers instantly with 0 days left in his sickness time.
    * */
   this -> Person_HandleStateTransition(this -> Person_CurrentStatus,
		   	   	   	   	   	   	   	   	   Person_Vaccinated);

   this -> Person_NumOfInfectDays = 0U;
}


void Person::Person_DiseaseGetInfected(Disease *D_Ptr){

    if(this -> Person_EvaluatePersonStatus() == Person_FullyProne){

        /* In this status, the person is quite prone to be infected 
        * and indeed will get infected in this case.
        */

        /* Update number of days needed by the patient to recover. */
        this -> Person_NumOfInfectDays = 
            D_Ptr -> Disease_GetSicknessLength();

        /* Update the person status to be sick. */
        this -> Person_HandleStateTransition(this -> Person_CurrentStatus,
        										Person_Sick);

        /* Get the disease details -
         * WARNING: This only works for a single disease in the
         * simulation. Multiple diseases will overlap each other
         * */
        this -> DPtr = D_Ptr;


        /* Go to quarantine area */
        this -> POPULATION_RTE_INSTANCE -> Population_QuarantinePerson(this);

    }

    else{

        /* The person is totally immune to the disease. */
    }

}


void Person:: Person_PersonTouch(Person *PPtr){

	/* Check if any of both is already prone to be infected */
	if((this -> Person_EvaluatePersonStatus() == Person_FullyImmune) ||
		(PPtr -> Person_EvaluatePersonStatus() == Person_FullyImmune)){

		/* If any of which is immune, no chance of infection exists */

	}
	else{

		/* Definitely someone is getting sick */
		if((this -> Person_GetCurrentStatus()) !=
			(PPtr -> Person_GetCurrentStatus())){

			/* In this case: one of which is actually sick */
			if(this -> Person_GetCurrentStatus() == Person_Sick){

				/* I'm sick, I'm going to infect the person pointer */
				PPtr -> Person_DiseaseGetInfected(this -> DPtr);

				_ASSERT_((bool) ((PPtr -> Person_GetCurrentStatus()) == (this -> Person_GetCurrentStatus()))
						, (std::string) "Invalid touch");
			}

			else{

				/* I'm not sick, I'm getting infected, make sure to initialize
				 * the person sickness correctly.
				 * */
				this -> Person_DiseaseGetInfected(PPtr -> DPtr);

				_ASSERT_((bool) ((PPtr -> Person_GetCurrentStatus()) == (this -> Person_GetCurrentStatus()))
						, (std::string) "Invalid touch");
			}

		}
		else{

			/* Nothing to be done here, both of which might be sick at the
			 * same time or susceptible at the same time.
			 * */

		}

	}
}

/* 
* Private functions:
* These functions are specific for internal operations for the Person class
* itself only. Not intended for public or inherited operations.
* */

/* Evaluate the status of a person. */
PersonImmunity Person::Person_EvaluatePersonStatus(void){

    /* Based upon the following conditions - The status shall change.
    * If a person is sick or healthy - return false.
    * If a person is vaccinated or Recovered - return true.
    * 
    * The function returns true in case the current person is fully immune
    * for the disease (Vaccinated or Recovered).
    * 
    */

    PersonImmunity RetVal = Person_FullyProne;

    if((this -> Person_CurrentStatus == Person_Vaccinated) ||
        (this -> Person_CurrentStatus == Person_Recovered)){

        /* The person is quite immune as per the requirements */
        RetVal = Person_FullyImmune;

    }

    else{

        /* Nothing to do, the person can get infected. */

    }
    
    return RetVal;
}


Person_uint32 Person:: Person_GetNumberToBeInfected(Disease_IntType NumOfPeople){

	return ceil(this -> DPtr ->  Disease_GetChanceOfTransmission() * NumOfPeople);

}

/* Construction has no return - this function is designed to
 * take nothing for now. It can be overloaded later.
 * */
Person:: Person(Population *POPULATION_INSTANCE):
		/* Using member initializers to initialize private variables
		 * The person is assume to be healthy at first with no
		 * needed days to recover from anything. Like a newborn baby.
		 * */
		Person_CurrentStatus(Person_Susceptible),
		Person_NumOfInfectDays(0U),
		DPtr(NULL),
		POPULATION_RTE_INSTANCE(POPULATION_INSTANCE){

	_DEBUG_PRINT_((std::string)"A person entered the simulation.");

}


Person:: ~Person(void){

	_DEBUG_PRINT_((std::string)"Person died");

}


void Person:: Person_HandleStateTransition(PersonStatus CurrentState, PersonStatus NextState){

	switch(CurrentState){

	case Person_Susceptible:

		if(NextState == Person_Sick){

			/* Increment number of infected people to be 1. */
			this -> POPULATION_RTE_INSTANCE -> Population_IncrementNumOfInfected();
			this -> POPULATION_RTE_INSTANCE -> Population_DecrementNumOfSusceptible();

		}
		else if(NextState == Person_Vaccinated){

			/* Increment number of vaccinated by 1. */
			this -> POPULATION_RTE_INSTANCE -> Population_IncrementNumOfVaccinated();
			this -> POPULATION_RTE_INSTANCE -> Population_DecrementNumOfSusceptible();

		}
		else{

			/* Nothing to do */
			_ASSERT_(true, (std::string)"ERROR in transition \n");
		}
		break;

	case Person_Sick:

		if(NextState == Person_Recovered){

			/* Decrement number of infected people to be 1. */
			this -> POPULATION_RTE_INSTANCE -> Population_DecrementNumOfInfected();
			this -> POPULATION_RTE_INSTANCE -> Population_IncrementNumOfRecovered();

		}
		else if(NextState == Person_Vaccinated){

			/* Decrement number of infected people to be 1. */
			/* Increment number of vaccinated by 1. */
			this -> POPULATION_RTE_INSTANCE -> Population_IncrementNumOfVaccinated();
			this -> POPULATION_RTE_INSTANCE -> Population_DecrementNumOfInfected();

		}
		else{

			/* Nothing to do - You should never be here */
			_ASSERT_(true, (std::string)"ERROR in transition \n");
		}
		break;

	default:

		/* Nothing to do - You should never be here */
		_ASSERT_(true, (std::string)"ERROR in transition \n");
		break;
	}

	/* Update next state */
	this -> Person_CurrentStatus = NextState;

	_ASSERT_((bool)
			((this -> POPULATION_RTE_INSTANCE -> Population_CountInfected() +
			this -> POPULATION_RTE_INSTANCE -> Population_CountRecovered() +
			this -> POPULATION_RTE_INSTANCE -> Population_CountSusceptible() +
			this -> POPULATION_RTE_INSTANCE -> Population_CountVaccinated())==
					this -> POPULATION_RTE_INSTANCE -> Population_GetSize())
			, (std::string) "Error in tracking population \n");

}
