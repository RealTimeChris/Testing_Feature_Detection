// Testing_Feature_Detection.cpp : Defines the entry point for the application.
//

#include "Testing_Feature_Detection.h"

using namespace std;

int main()
{
	simd_dispatcher{}.opAnd(23, 23);
	simd_dispatcher{}.initialize();
	cout << "CURRENT IMPLEMENTATION: " << JSONIFIER_CAN_ALWAYS_RUN_HASWELL << endl;
	cout << simdjson::internal::detect_supported_architectures() << endl;
	return 0;
}
