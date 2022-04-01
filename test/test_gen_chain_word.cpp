#include "pch.h"
#include "CppUnitTest.h"
#include "../src/core.cpp"
#include <vector>
#include <algorithm>
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test_gen_chain_word_correctness{

	void test(char* words[], int len, char* ans[], int ans_len, char head, char tail, bool enable_loop){
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_word(words, len, result, head, tail, enable_loop);
		Assert::AreEqual(ans_len, out_len);
		for(int i = 0;i < ans_len;i++){
			if(result != NULL) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
			else Assert::Fail();
		}
	}

	void stress(int n, bool DAG, int len, unsigned int seed, char head, char tail){
		char** words = generator(n, DAG, len, seed);
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_word(words, len, result, head, tail, !DAG);
		int ans_len = dp(words, len, head, tail, false);
		if(len <= 8) Assert::AreEqual(ans_len, brute_force(words, len, head, tail, false));
		Assert::AreEqual(ans_len, out_len);
		checker(words, len, result, out_len);
	}

	unsigned int seed = 998244353;

	unsigned int rnd(){
		seed ^= seed << 13;
		seed ^= seed >> 7;
		seed ^= seed << 17;
		return seed;
	}

	TEST_CLASS(test_gen_chain_word_correctness){
	public:

		/*
		* -w 样例
		*/ 
		TEST_METHOD(example_w){
			char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
			char* ans[] = {"algebra", "apple", "elephant", "trick"};
			test(words, 11, ans, 4, 0, 0, false);
		}

		/*
		* -h 样例
		*/ 
		TEST_METHOD(example_h){
			char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
			char* ans[] = {"elephant", "trick"};
			test(words, 11, ans, 2, 'e', 0, false);
		}

		/*
		* -t 样例
		*/ 
		TEST_METHOD(example_t){
			char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
			char* ans[] = {"algebra", "apple", "elephant"};
			test(words, 11, ans, 3, 0, 't', false);
		}

		/*
		* -h -t 同时使用
		*/ 
		TEST_METHOD(together_h_t){
			char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
			char* ans[] = {"algebra", "apple", "elephant"};
			test(words, 11, ans, 3, 'a', 't', false);
		}

		/*
		* 长度为 5 的链
		*/ 
		TEST_METHOD(simple_chain){
			char* words[] = {"ab", "bc", "cd", "de", "ef"};
			char* ans[] = {"ab", "bc", "cd", "de", "ef"};
			test(words, 5, ans, 5, 0, 0, false);
		}

		/*
		* 4 个点 6 条边的 DAG
		*/ 
		TEST_METHOD(max_DAG_4_vertices){
			char* words[] = {"ab", "ac", "ad", "bc", "bd", "cd"};
			char* ans[] = {"ab", "bc", "cd"};
			test(words, 6, ans, 3, 0, 0, false);
		}

		/*
		* 3 个点的链 + 3 个自环
		*/ 
		TEST_METHOD(simple_chain_with_self_loop){
			char* words[] = {"aa", "ab", "bb", "bc", "cc"};
			char* ans[] = {"aa", "ab", "bb", "bc", "cc"};
			test(words, 5, ans, 5, 0, 0, false);
		}

		/*
		* 3 个点的链 + 3 个自环 vs 4 个点的链
		*/ 
		TEST_METHOD(simple_chain_with_self_loop_vs_simple_chain){
			char* words[] = {"aa", "ab", "bb", "bc", "cc", "de", "ef", "fg", "gh"};
			char* ans[] = {"aa", "ab", "bb", "bc", "cc"};
			test(words, 9, ans, 5, 0, 0, false);
		}

		/*
		* 5 个孤立自环点
		*/ 
		TEST_METHOD(isolated_vertex_with_self_loop){
			char* words[] = {"aa", "bb", "cc", "dd", "ee"};
			char* ans[] = {"none"};
			test(words, 5, ans, 0, 0, 0, false);
		}

		/*
		* 长度为 5 的链 附带一些单个字母的单词
		*/ 
		TEST_METHOD(simple_chain_and_single_character){
			char* words[] = {"ab", "bc", "cd", "de", "ea", "a", "b", "c", "d", "e"};
			char* ans[] = {"ab", "bc", "cd", "de", "ea"};
			test(words, 10, ans, 5, 0, 0, true);
		}

		/*
		* -r 样例
		*/ 
		TEST_METHOD(example_r){
			char* words[] = {"element", "heaven", "table", "teach", "talk"};
			char* ans[] = {"table", "element", "teach", "heaven"};
			test(words, 5, ans, 4, 0, 0, true);
		}

		/*
		* 10 个 aa
		*/ 
		TEST_METHOD(only_self_loop_aa){
			char* words[] = {"aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa"};
			char* ans[] = {"xx"};
			test(words, 10, ans, 0, 0, 0, true);
		}


		/*
		* 对拍，head 和 tail 均无限制
		*/ 
		TEST_METHOD(stresses_0_0){
			for(int n = 2;n <= 26;n += 2){
				for(int len = 1;len <= 19;len += 3){
					for(int i = 0;i < 5;i++){
						stress(n, false, len, i, 0, 0);
						stress(n, true, len, i, 0, 0);
					}
				}
			}
		}

		/*
		* 对拍，head 有限制 tail 无限制
		*/ 
		TEST_METHOD(stresses_1_0){
			for(int n = 2;n <= 26;n += 2){
				for(int len = 1;len <= 19;len += 3){
					for(int i = 0;i < 5;i++){
						char head = rnd() % n + 'a';
						stress(n, false, len, i, head, 0);
						stress(n, true, len, i, head, 0);
					}
				}
			}
		}

		/*
		* 对拍，head 无限制 tail 有限制
		*/ 
		TEST_METHOD(stresses_0_1){
			for(int n = 2;n <= 26;n += 2){
				for(int len = 1;len <= 19;len += 3){
					for(int i = 0;i < 5;i++){
						char tail = rnd() % n + 'a';
						stress(n, false, len, i, 0, tail);
						stress(n, true, len, i, 0, tail);
					}
				}
			}
		}

		/*
		* 对拍，head 和 tail 均有限制
		*/ 
		TEST_METHOD(stresses_1_1){
			for(int n = 2;n <= 26;n += 2){
				for(int len = 1;len <= 19;len += 3){
					for(int i = 0;i < 5;i++){
						char head = rnd() % n + 'a', tail = rnd() % n + 'a';
						stress(n, false, len, i, head, tail);
						stress(n, true, len, i, head, tail);
					}
				}
			}
		}

		
	};
}

namespace test_gen_chain_char_correctness{

	void test(char* words[], int len, char* ans[], int ans_len, char head, char tail, bool enable_loop){
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_char(words, len, result, head, tail, enable_loop);
		Assert::AreEqual(ans_len, out_len);
		for(int i = 0;i < ans_len;i++){
			if(result != NULL) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
			else Assert::Fail();
		}
	}

	void stress(int n, bool DAG, int len, unsigned int seed, char head, char tail){
		char** words = generator(n, DAG, len, seed);
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_char(words, len, result, head, tail, !DAG);
		int ans_len = dp(words, len, head, tail, true);
		if(len <= 8) Assert::AreEqual(ans_len, brute_force(words, len, head, tail, true));
		int cnt = 0;
		assert(result != NULL);
		for(int i = 0;i < out_len;i++) cnt += (int)strlen(result[i]);
		Assert::AreEqual(ans_len, cnt);
		checker(words, len, result, out_len);
	}

	unsigned int seed = 998244353;

	unsigned int rnd(){
		seed ^= seed << 13;
		seed ^= seed >> 7;
		seed ^= seed << 17;
		return seed;
	}

	TEST_CLASS(test_gen_chain_char_correctness){
	public:

		/*
		* -c 样例
		*/ 
		TEST_METHOD(example_c){
			char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
			char* ans[] = {"pseudopseudohypoparathyroidism", "moon"};
			test(words, 11, ans, 2, 0, 0, false);
		}

		/*
		* 对拍，head 和 tail 均无限制
		*/ 
		TEST_METHOD(stresses_0_0){
			for(int n = 2;n <= 26;n += 2){
				for(int len = 1;len <= 19;len += 3){
					for(int i = 0;i < 5;i++){
						stress(n, false, len, i, 0, 0);
						stress(n, true, len, i, 0, 0);
					}
				}
			}
		}

		/*
		* 对拍，head 有限制 tail 无限制
		*/ 
		TEST_METHOD(stresses_1_0){
			for(int n = 2;n <= 26;n += 2){
				for(int len = 1;len <= 19;len += 3){
					for(int i = 0;i < 5;i++){
						char head = rnd() % n + 'a';
						stress(n, false, len, i, head, 0);
						stress(n, true, len, i, head, 0);
					}
				}
			}
		}

		/*
		* 对拍，head 无限制 tail 有限制
		*/ 
		TEST_METHOD(stresses_0_1){
			for(int n = 2;n <= 26;n += 2){
				for(int len = 1;len <= 19;len += 3){
					for(int i = 0;i < 5;i++){
						char tail = rnd() % n + 'a';
						stress(n, false, len, i, 0, tail);
						stress(n, true, len, i, 0, tail);
					}
				}
			}
		}

		/*
		* 对拍，head 和 tail 均有限制
		*/ 
		TEST_METHOD(stresses_1_1){
			for(int n = 2;n <= 26;n += 2){
				for(int len = 1;len <= 19;len += 3){
					for(int i = 0;i < 5;i++){
						char head = rnd() % n + 'a', tail = rnd() % n + 'a';
						stress(n, false, len, i, head, tail);
						stress(n, true, len, i, head, tail);
					}
				}
			}
		}
	};
}

namespace test_gen_chains_all_correctness{

	void test(char* words[], int len, char* ans[], int ans_len){
		char** result = (char**)malloc(10000);
		int out_len = gen_chains_all(words, len, result);
		Assert::AreEqual(ans_len, out_len);
		for(int i = 0;i < ans_len;i++){
			if(result != NULL) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
			else Assert::Fail();
		}
	}

	TEST_CLASS(test_gen_chains_all_correctness){
	public:

		/*
		* -n 样例
		*/ 
		TEST_METHOD(example_n){
			char* words[] = {"woo", "oom", "moon", "noox"};
			char* ans[] = {"moon noox", "oom moon", "oom moon noox", "woo oom", "woo oom moon", "woo oom moon noox"};
			test(words, 4, ans, 6);
		}

	};
}

namespace test_gen_chain_word_unique_correctness{

	void test(char* words[], int len, char* ans[], int ans_len){
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_word_unique(words, len, result);
		Assert::AreEqual(ans_len, out_len);
		for(int i = 0;i < ans_len;i++){
			if(result != NULL) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
			else Assert::Fail();
		}
	}

	TEST_CLASS(test_gen_chain_word_unique_correctness){
	public:

		/*
		* -w 样例
		*/ 
		TEST_METHOD(example_w){
			char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
			char* ans[] = {"apple", "elephant", "trick"};
			test(words, 11, ans, 3);
		}

	};
}



