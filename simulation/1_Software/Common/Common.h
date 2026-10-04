#pragma once

#include <iostream>
#include <string>
#include <cassert>



#define PRIVATE static

//#define __DEBUG_CODE__
//#define __ASSERTION_MODE__
//#define _DEPLOYMENT_MODE_


/* 				Pre-processing directives 				*/
#ifdef _DEPLOYMENT_MODE_
#define ABSL_PATH (std::string)"/usr/src/test/examples/manual_populations/"

#else
#define ABSL_PATH (std::string)""

#endif

inline void _DEBUG_PRINT_(std::string Str) {

#ifdef __DEBUG_CODE__
	std::cout << Str << std::endl;
#endif

}


inline void _ASSERT_(bool Condition, std::string Str) {

#ifdef __ASSERTION_MODE__
	if(!Condition){
		std::cout << Str;
	}
	assert(Condition == true && "Assertion error");

#endif

}
