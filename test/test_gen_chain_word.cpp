#include "pch.h"
#include "CppUnitTest.h"
#include "../src/core.cpp"
#include <vector>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test_gen_chain_word
{

	void test(char* words[], int len, char* ans[], int ans_len, char head, char tail, bool enable_loop){
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_word(words, len, result, head, tail, enable_loop);
		Assert::AreEqual(ans_len, out_len);
		for(int i = 0;i < ans_len;i++){
			if(result != NULL) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
			else Assert::Fail();
		}
	}

	int cnt = 0;

	TEST_CLASS(test_gen_chain_word)
	{
	public:
		TEST_METHOD(Example)
		{
			char* words[] = {"algebra", "apple", "zoo", "elephat", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
			char* ans[] = {"algebra", "apple", "elephat", "trick"};
			test(words, 11, ans, 4, 0, 0, false);
		}

		TEST_METHOD(SimpleChain)
		{
			char* words[] = {"ab", "bc", "cd", "de", "ef"};
			char* ans[] = {"ab", "bc", "cd", "de", "ef"};
			test(words, 5, ans, 5, 0, 0, false);
		}

		TEST_METHOD(TenSelfLoop)
		{
			char* words[] = {"aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa"};
			char* ans[] = {"aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa"};
			test(words, 10, ans, 10, 0, 0, true);
		}
	};
}
