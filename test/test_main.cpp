#include "pch.h"
#include "CppUnitTest.h"
#include "../src/main.cpp"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test_main_exception {

	TEST_CLASS(test_main_exception) {
	public:

		/*
		* 参数过少
		*/
		TEST_METHOD(missing_arguments){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe"};
				main_serve(1, args);
			});
		}

		/*
		* 重复选项 
		*/
		TEST_METHOD(option_duplicated){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-h", "a", "-h", "a"};
				main_serve(5, args);
			});
		}

		/*
		* -h, -t 缺少参数
		*/
		TEST_METHOD(missing_argument_head_tail){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-h", "a", "-t"};
				main_serve(4, args);
			});
		}

		/*
		* -h, -t 参数长度大于 1
		*/
		TEST_METHOD(argument_too_long_head_tail){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-h", "123"};
				main_serve(3, args);
			});
		}

		/*
		* -h, -t 参数不是单个字母
		*/
		TEST_METHOD(argument_not_alphabet_head_tail){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-h", "1"};
				main_serve(3, args);
			});
		}

		/*
		* 非法选项
		*/
		TEST_METHOD(invalid_option){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-q", "123"};
				main_serve(3, args);
			});
		}

		/*
		* 多个文件参数
		*/
		TEST_METHOD(too_many_file_arguments){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-n", "1.in", "2.in"};
				main_serve(4, args);
			});
		}

		/*
		* -n 不独立使用
		*/
		TEST_METHOD(option_n_with_other){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-n", "1.in", "-r"};
				main_serve(4, args);
			});
		}

		/*
		* -m 和 -h,-t,-r 混合使用
		*/
		TEST_METHOD(option_m_with_h_t_r){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-m", "1.in", "-h", "a"};
				main_serve(5, args);
			});
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-m", "1.in", "-t", "a"};
				main_serve(5, args);
			});
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-m", "1.in", "-r"};
				main_serve(4, args);
			});
		}

		/*
		* -m -w -c 混合使用
		*/
		TEST_METHOD(option_m_w_c_together){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-m", "-w", "1.in"};
				main_serve(4, args);
			});
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-m", "-c", "1.in"};
				main_serve(4, args);
			});
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = {"WordList.exe", "-w", "-c", "1.in"};
				main_serve(4, args);
			});
		}

		/*
		* -n -m -w -c 均没有
		*/
		TEST_METHOD(no_option_n_m_w_c){
			Assert::ExpectException<std::invalid_argument>([](){
				const char* args[] = { "WordList.exe", "-h", "a", "-t", "b", "-r" };
				main_serve(6, args);
			});
		}
	};
}



