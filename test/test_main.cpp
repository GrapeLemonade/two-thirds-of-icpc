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
			try{
				const char* args[] = {"WordList.exe"};
				main_serve(1, args);
			}catch(invalid_argument const &e){
				Assert::AreEqual(0, strcmp("missing arguments", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* 重复选项 
		*/
		TEST_METHOD(option_duplicated){
			try{
				const char* args[] = {"WordList.exe", "-h", "a", "-h", "a"};
				main_serve(5, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("option duplicated -- 'h'", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* -h, -t 缺少参数
		*/
		TEST_METHOD(missing_argument_head_tail){
			try{
				const char* args[] = {"WordList.exe", "-h", "a", "-t"};
				main_serve(4, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("option requires an argument -- 't'", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* -h, -t 参数长度大于 1
		*/
		TEST_METHOD(argument_too_long_head_tail){
			try{
				const char* args[] = {"WordList.exe", "-h", "123"};
				main_serve(3, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("argument of option -h should be a single alphabet, 123 received", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* -h, -t 参数不是单个字母
		*/
		TEST_METHOD(argument_not_alphabet_head_tail){
			try{
				const char* args[] = {"WordList.exe", "-h", "1"};
				main_serve(3, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("argument of option -h should be an alphabet, 1 received", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* 非法选项
		*/
		TEST_METHOD(invalid_option){
			try{
				const char* args[] = {"WordList.exe", "-q", "123"};
				main_serve(3, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("invalid option -- 'q'", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* 多个文件参数
		*/
		TEST_METHOD(too_many_file_arguments){
			try{
				const char* args[] = {"WordList.exe", "-n", "1.in", "2.in"};
				main_serve(4, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("multiple text files given: \"1.in\" and \"2.in\"", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* -n 不独立使用
		*/
		TEST_METHOD(option_n_with_other){
			try{
				const char* args[] = {"WordList.exe", "-n", "1.in", "-r"};
				main_serve(4, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("cannot combine -n with other options", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* -m 和 -h,-t,-r 混合使用
		*/
		TEST_METHOD(option_m_with_h_t_r){
			int cnt = 0;
			try{
				const char* args[] = {"WordList.exe", "-m", "1.in", "-h", "a"};
				main_serve(5, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("cannot combine -m with -h, -t or -r", e.what()));
				cnt++;
			}
			try{
				const char* args[] = {"WordList.exe", "-m", "1.in", "-t", "a"};
				main_serve(5, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("cannot combine -m with -h, -t or -r", e.what()));
				cnt++;
			}
			try{
				const char* args[] = {"WordList.exe", "-m", "1.in", "-r"};
				main_serve(4, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("cannot combine -m with -h, -t or -r", e.what()));
				cnt++;
			}
			Assert::AreEqual(3, cnt);
		}

		/*
		* -m,-w,-c 混合使用
		*/
		TEST_METHOD(option_m_w_c_together){
			int cnt = 0;
			try{
				const char* args[] = {"WordList.exe", "-m", "-w", "1.in"};
				main_serve(4, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("conflicting option combinations", e.what()));
				cnt++;
			}
			try{
				const char* args[] = {"WordList.exe", "-m", "-c", "1.in"};
				main_serve(4, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("conflicting option combinations", e.what()));
				cnt++;
			}
			try{
				const char* args[] = {"WordList.exe", "-w", "-c", "1.in"};
				main_serve(4, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("conflicting option combinations", e.what()));
				cnt++;
			}
			Assert::AreEqual(3, cnt);
		}

		/*
		* -n,-m,-w,-c 均没有，但有其它选项
		*/
		TEST_METHOD(no_option_n_m_w_c){
			try{
				const char* args[] = { "WordList.exe", "-h", "a", "-t", "b", "-r"};
				main_serve(6, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("missing -n, -w, -m or -c", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* 没有任何一个选项
		*/
		TEST_METHOD(no_option_at_all){
			try{
				const char* args[] = { "WordList.exe", "1.in"};
				main_serve(2, args);
			}catch(invalid_argument const& e){
				Assert::AreEqual(0, strcmp("no option specified", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* 文件不存在
		*/
		TEST_METHOD(no_such_file){
			try{
				const char* args[] = {"WordList.exe", "-n", "no_such_file.in"};
				main_serve(3, args);
			}catch(runtime_error const& e){
				Assert::AreEqual(0, strcmp("no_such_file.in: No such file", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* 输入路径不是一个正常的文件
		*/
		TEST_METHOD(not_regular_file){
			try{
				create_directory("not_regular_file.in");
				const char* args[] = {"WordList.exe", "-n", "not_regular_file.in"};
				main_serve(3, args);
			}catch(runtime_error const& e){
				Assert::AreEqual(0, strcmp("not_regular_file.in: Not a regular file", e.what()));
				remove_all("not_regular_file.in");
				return;
			}
			Assert::Fail();
		}

		/*
		* 无法读取输入文件
		*/
		TEST_METHOD(cant_read){
			try{
				const char* args[] = {"WordList.exe", "-n", "..\\..\\test\\data\\cant_read.txt"};
				main_serve(3, args);
			}catch(runtime_error const& e){
				Assert::AreEqual(0, strcmp("..\\..\\test\\data\\cant_read.txt: Cannot open as read-only", e.what()));
				return;
			}
			Assert::Fail();
		}


		/*
		* 输入文件中没有单词
		*/
		TEST_METHOD(no_words){
			try{
				ofstream output;
				output.open("no_words.in", ios::out | ios::binary | ios::trunc);
				output.close();
				const char* args[] = {"WordList.exe", "-n", "no_words.in"};
				main_serve(3, args);
			}catch(runtime_error const& e){
				Assert::AreEqual(0, strcmp("no_words.in: File does not contain words", e.what()));
				return;
			}
			Assert::Fail();
		}

		/*
		* 无法写 solution.txt
		*/
		TEST_METHOD(cant_write){
			try{
				ofstream output;
				output.open("cant_write.in", ios::out | ios::binary | ios::trunc);
				output << "abc" << endl;
				output.close();
				create_directory("solution.txt");
				const char* args[] = {"WordList.exe", "-w", "cant_write.in"};
				main_serve(3, args);
			}catch(runtime_error const& e){
				Assert::AreEqual(0, strcmp("solution.txt: Cannot open for writing", e.what()));
				remove_all("solution.txt");
				return;
			}
			Assert::Fail();
		}
	};
}



