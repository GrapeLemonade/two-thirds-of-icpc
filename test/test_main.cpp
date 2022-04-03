#include "pch.h"
#include "CppUnitTest.h"
#include "../src/main.cpp"
#include <stdexcept>



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test_main_exception {

	TEST_CLASS(test_main_exception) {
	public:

		/*
		* -w ÑùÀý
		*/
		TEST_METHOD(stresses) {
			char* args[] = { (char*)"t" };
			Assert::ExpectException<std::invalid_argument>([&] { main_serve(1, args); });
		}
	};
}



