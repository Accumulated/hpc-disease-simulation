#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <chrono>
#include "doctest.h"
#include "Simulation.h"
#include "SequentialSimulation.h"
#include "MPI_MultiPopulation.h"
#include "Person.h"
#include "Disease.h"

/* test hooks for unit testing */
#include "PersonTestHook.h"
#include "PopulationTestHook.h"

TEST_CASE("SIMULATION - Sequential") {

    Simulation *sim = new SequentialSimulation();

    auto start = std::chrono::high_resolution_clock::now();

    sim -> start();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Simulation execution time: " << elapsed.count() << " seconds" << std::endl;

    delete sim;

}

TEST_CASE("SIMULATION - MPI") {

    Simulation *sim = new MPISimulation();

    auto start = std::chrono::high_resolution_clock::now();

    sim -> start();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Simulation execution time: " << elapsed.count() << " seconds" << std::endl;

    delete sim;

}

/* Person test cases */
TEST_CASE("Test Person class - Constructor functionality") {

	/* Test purpose:
	 * Test constructor functionality to initialize internal variabls
	 * correctly.
	 *
	 * Test steps:
	 * 1. Implement an instance of Person class. This is done implicitly
	 * inside PersonTestHook.
	 *
	 * 2. Check for initial conditions for the person himself.
	 *
	 * */
	PersonTestHook PInstance;

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0U);

}

TEST_CASE("Test Person class - Person_ForOneMoreDay function"){

	/* Cover all corner cases for Person_ForOneMoreDay function */
	PersonTestHook PInstance;

	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	/* Move for one more day */
	PInstance.Person_ForOneMoreDay();

	/* Check that the function does nothing if the person isn't
	 * sick
	 * */
	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Infect this person with a disease */
	PInstance.Person_DiseaseGetInfected(&DInstance);

	/* First time sick */
	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 20);


	/* Move for one more day */
	PInstance.Person_ForOneMoreDay();

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 19);


	/* Help him recover */
	while(PInstance.Person_GetCurrentStatus() == Person_Sick){

		/* Move for one more day */
		PInstance.Person_ForOneMoreDay();

	}

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Recovered);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0);


}

TEST_CASE("Test Person class - Person_DiseaseGetVaccinated function "){

	/* Cover all corner cases for Person_ForOneMoreDay function */
	PersonTestHook PInstance;

	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	/* Move for one more day */
	PInstance.Person_ForOneMoreDay();

	/* Check that the function does nothing if the person isn't
	 * sick
	 * */
	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Infect this person with a disease */
	PInstance.Person_DiseaseGetInfected(&DInstance);

	/* First time sick */
	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 20);

	PInstance.Person_DiseaseGetVaccinated();

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0);

}

TEST_CASE("Test Person class - Person_DiseaseGetInfected function "){

	/* Experiment with a fully prone person - default at initialization */
	PersonTestHook PInstance;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	/* Move for one more day */
	PInstance.Person_ForOneMoreDay();

	/* Check that the function does nothing if the person isn't
	 * sick
	 * */
	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Infect this person with a disease */
	PInstance.Person_DiseaseGetInfected(&DInstance);

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 20);

	/* Recover */
	while(PInstance.Person_GetCurrentStatus() == Person_Sick){
		PInstance.Person_ForOneMoreDay();
	}

	/* Infect again - you are immune */

	/* Infect this person with a disease */
	PInstance.Person_DiseaseGetInfected(&DInstance);

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Recovered);
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0U);

}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A prone, B immune") {

	/* A is prone while B is immune 0 1*/

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	/* A is prone */
	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 0U);

	PInstance_A.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	/* B is immune */
	PInstance_B.Person_DiseaseGetVaccinated();
	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Touching function */
	PInstance_A.Person_PersonTouch(&PInstance_B);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A immune, B immune") {

	/* A is immune while B is immune 1 1*/

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;

	PInstance_A.Person_DiseaseGetVaccinated();
	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 0U);

	/* B is immune */
	PInstance_B.Person_DiseaseGetVaccinated();
	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Touching function */
	PInstance_A.Person_PersonTouch(&PInstance_B);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);

}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A immune, B prone") {

	/* A is immune while B is prone 1 0 */

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	PInstance_A.Person_DiseaseGetVaccinated();

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 0U);

	/* B is sick */
	PInstance_B.Person_DiseaseGetInfected(&DInstance);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

	/* Touching function */
	PInstance_A.Person_PersonTouch(&PInstance_B);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 0U);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A prone, B prone") {

	/* A is prone while B is prone 0 0 - Both are Susceptible */

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 0U);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Touching function */
	PInstance_A.Person_PersonTouch(&PInstance_B);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 0U);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);
}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A prone, B prone") {

	/* A is prone while B is prone 0 0 - Both are sick */

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	PInstance_A.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	PInstance_B.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

	/* Touching function */
	PInstance_A.Person_PersonTouch(&PInstance_B);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A prone, B prone") {

	/* A is prone while B is prone 0 0 - one of them is sick while the other is not */
	/* A is sick, A is touching B ->  B is getting infected */

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	PInstance_A.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Touching function */
	PInstance_A.Person_PersonTouch(&PInstance_B);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A prone, B prone") {

	/* A is prone while B is prone 0 0 - one of them is sick while the other is not */
	/* A is sick, B is touching A ->  B is getting infected */

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	PInstance_A.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Touching function */
	PInstance_B.Person_PersonTouch(&PInstance_A);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A prone, B prone") {

	/* A is prone while B is prone 0 0 - one of them is sick while the other is not */
	/* B is sick, A is touching B ->  B is getting infected */

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	PInstance_B.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Touching function */
	PInstance_A.Person_PersonTouch(&PInstance_B);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

}


TEST_CASE("Test Person class - Person_PersonTouch - Immune corner case - A prone, B prone") {

	/* A is prone while B is prone 0 0 - one of them is sick while the other is not */
	/* B is sick, B is touching A ->  B is getting infected */

	PersonTestHook PInstance_A;
	PersonTestHook PInstance_B;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	PInstance_B.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Susceptible);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Touching function */
	PInstance_B.Person_PersonTouch(&PInstance_A);

	REQUIRE(PInstance_A.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_A.PersonTHook_GetNumOfInfectDays() == 20);

	REQUIRE(PInstance_B.Person_GetCurrentStatus() == Person_Sick);
	REQUIRE(PInstance_B.PersonTHook_GetNumOfInfectDays() == 20);

}


TEST_CASE("Test Person class - Sickness functionality") {

	/* Recovered - full immunity */
	PersonTestHook PInstance_A;
	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");

	REQUIRE(PInstance_A.PersonTHook_EvaluatePersonStatus() == Person_FullyProne);

	PInstance_A.Person_DiseaseGetInfected(&DInstance);

	while(PInstance_A.Person_GetCurrentStatus() == Person_Sick){

		PInstance_A.Person_ForOneMoreDay();

	}

	REQUIRE(PInstance_A.PersonTHook_EvaluatePersonStatus() == Person_FullyImmune);

}


TEST_CASE("Test Person class - Sickness functionality") {

	/* Vaccination - full immunity */
	PersonTestHook PInstance_A;

	REQUIRE(PInstance_A.PersonTHook_EvaluatePersonStatus() == Person_FullyProne);

	PInstance_A.Person_DiseaseGetVaccinated();

	REQUIRE(PInstance_A.PersonTHook_EvaluatePersonStatus() == Person_FullyImmune);

}


TEST_CASE("Test Person class - Sickness functionality") {

	/* Test purpose:
	 * - Basic functionality of ability to get infected and
	 * accurately monitor internal defined variables for the person.
	 *
	 * Test steps:
	 * 1. Configure a disease with a certain capabilities.
	 * 2. Infect a person with that disease.
	 * 3. Check person data after infection to match the details
	 * of the Disease of number of infection days needed and check
	 * the status of this person to be sick.
	 *
	 * */
	PersonTestHook PInstance;


	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");


	PInstance.Person_DiseaseGetInfected(&DInstance);

	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 20U);

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Sick);

}

TEST_CASE("Test Person class - Sickness functionality") {

	/* Test purpose:
	 * - Check recover functionality for an infected person.
	 *
	 * Test steps:
	 * 1. Configure a Disease with certain capabilities
	 * 2. Infect a person with that disease
	 * 3. Recover from the disease after number of infected days
	 * 4. Check that the status of a person is recovered
	 * 5. Try to infect him again
	 * 6. Check both status and number of infection days remaining to be
	 * accurate.
	 * */
	PersonTestHook PInstance;


	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");


	PInstance.Person_DiseaseGetInfected(&DInstance);

	for(int i = 0 ; i < 20; i++){
		PInstance.Person_ForOneMoreDay();
	}
	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0U);

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Recovered);
	PInstance.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Recovered);

	PInstance.Person_ForOneMoreDay();

	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0U);

}


TEST_CASE("Test Person class - Sickness functionality") {

	/* Test purpose:
	 * - Check that Vaccination is working.
	 *
	 * Test steps:
	 * 1. Configure a Disease with certain capabilities
	 * 2. Infect a person with that disease
	 * 3.  Vaccinate this person directly.
	 * 4. Check that he has recovered and can't be infected anymore
	 *
	 * */
	PersonTestHook PInstance;


	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");


	PInstance.Person_DiseaseGetInfected(&DInstance);

	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 20U);

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Sick);

	PInstance.Person_DiseaseGetVaccinated();

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Vaccinated);

	PInstance.Person_DiseaseGetInfected(&DInstance);

	REQUIRE(PInstance.PersonTHook_GetNumOfInfectDays() == 0U);

	REQUIRE(PInstance.Person_GetCurrentStatus() == Person_Vaccinated);

}



TEST_CASE("Test Person class - transmission functionality") {

	/* Test purpose:
	 * - Check that a person can infect another.
	 *
	 * Test steps:
	 * 1. Configure a Disease with certain capabilities
	 * 2. Infect Person_A with that disease
	 * 3. Get 2 people in the same place (Person_A, Person_B).
	 * 4. Check that Person_B is correctly infected.
	 * 5. Get Person_A vaccinated.
	 * 6. Get same 2 people in the same place again (Person_A, Person_B)
	 * 7. Check that Person_A is still okay.
	 * */
	PersonTestHook PInstanceA, PInstanceB;


	Disease DInstance((Disease_IntType) 20,
						(Disease_FloatType) 1.0,
						(Disease_StrType) "UnKnoWn");


	/* Infect person A*/
	PInstanceA.Person_DiseaseGetInfected(&DInstance);
	REQUIRE(PInstanceA.PersonTHook_GetNumOfInfectDays() == 20U);
	REQUIRE(PInstanceA.Person_GetCurrentStatus() == Person_Sick);

	for(int i = 0 ; i < 4; i++){
		PInstanceA.Person_ForOneMoreDay();
	}
	REQUIRE(PInstanceA.PersonTHook_GetNumOfInfectDays() == 16U);

	/* Touch instance B */
	PInstanceA.Person_PersonTouch(&PInstanceB);
	/* Check instance B configuration */
	REQUIRE(PInstanceB.PersonTHook_GetNumOfInfectDays() == 20U);
	REQUIRE(PInstanceB.Person_GetCurrentStatus() == Person_Sick);

	/* Vaccinate person A */
	PInstanceA.Person_DiseaseGetVaccinated();
	REQUIRE(PInstanceA.Person_GetCurrentStatus() == Person_Vaccinated);
	REQUIRE(PInstanceA.PersonTHook_GetNumOfInfectDays() == 0U);

	/* Person B touches A*/
	PInstanceB.Person_PersonTouch(&PInstanceA);
	REQUIRE(PInstanceA.PersonTHook_GetNumOfInfectDays() == 0U);
	REQUIRE(PInstanceA.Person_GetCurrentStatus() == Person_Vaccinated);

}



TEST_CASE("POPO") {

	Disease D_Ptr(20, 1.0, (Disease_StrType) "CoCo");

	PopulationTestHook Pop(40000, 1.0, 0, (Population_string) "Nop", &D_Ptr);

	REQUIRE(Pop.PopulationTestHook_SanityCheck() == true);

}


TEST_CASE("POPO2") {

	Disease D_Ptr(20, 1.0, (Disease_StrType) "CoCo");

	PopulationTestHook Pop(40000000, 0.1, 0, (Population_string) "Nop", &D_Ptr);

	REQUIRE(Pop.PopulationTestHook_SanityCheck() == true);

}


TEST_CASE("POPO3") {

	Disease D_Ptr(20, 1.0, (Disease_StrType) "CoCo");

	PopulationTestHook Pop(40000, 0.4, 1, (Population_string) "Nop", &D_Ptr);

	REQUIRE(Pop.PopulationTestHook_SanityCheck() == true);

}


TEST_CASE("POPO4") {

	Disease D_Ptr(20, 1.0, (Disease_StrType) "CoCo");

	PopulationTestHook Pop(40000, 0, 1, (Population_string) "Nop", &D_Ptr);

	REQUIRE(Pop.PopulationTestHook_SanityCheck() == true);

	for(int i = 0; i < 100; i++)
		Pop.Population_Runnable();

}




