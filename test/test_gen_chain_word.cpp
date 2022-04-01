#include "pch.h"
#include "CppUnitTest.h"
#include "../src/core.cpp"
#include <vector>
#include <algorithm>
#include <cassert>

#pragma warning(disable:6385)
#pragma warning(disable:6386)

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

	unsigned int seed = 998244353;

	unsigned int rand(){
		seed ^= seed << 13;
		seed ^= seed >> 7;
		seed ^= seed << 17;
		return seed;
	}

	char** generator(int n, bool DAG, int len, unsigned int Seed){
		seed = Seed ^ n ^ len;
		char** words = (char**)malloc(len * sizeof(char*));
		for(int i = 0;i < len;i++){
			assert(words != NULL);
			words[i] = (char*)malloc(4 * sizeof(char));
			assert(words[i] != NULL);
			words[i][0] = (char)(rand() % n + 'a');
			words[i][1] = (char)(i + 'a');
			words[i][2] = (char)(rand() % n + 'a');
			if(DAG && words[i][0] >= words[i][2]){
				if(words[i][0] == words[i][2]){
					if(words[i][2] == n - 1 + 'a') words[i][0]--;
					else words[i][2]++;
				}else swap(words[i][0], words[i][2]);
			}
			words[i][3] = 0;
		}
		return words;
	}

	int brute_force(char* words[], int len, char head, char tail){
		assert(words != NULL);
		assert(len <= 10);
		int a[10] = {0}, b[10] = {0};
		for(int i = 0;i < len;i++){
			a[i] = i;
			b[i] = (int)strlen(words[i]);
		}
		int ans = 0;
		do{
			assert(words[a[0]] != NULL);
			if(head && words[a[0]][0] != head) continue;
			for(int i = 1;i < len;i++){
				assert(words != NULL);
				assert(words[a[i - 1]] != NULL);
				assert(words[a[i]] != NULL);
				if(words[a[i]][0] != words[a[i - 1]][2]) break;
				if(!tail || words[a[i]][b[a[i]] - 1] == tail) ans = max(ans, i + 1);
			}
		}while(std::next_permutation(a, a + len));
		if(ans == 1) ans = 0;
		return ans;
	}

	int f[1 << 20][20];
	int dp(char* words[], int len, char head, char tail){
		assert(words != NULL);
		assert(len <= 20);
		int b[20];
		for(int i = 0;i < len;i++) b[i] = (int)strlen(words[i]);
		for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++) f[i][j] = (int)-1e9;
		for(int i = 0;i < len;i++){
			assert(words[i] != NULL);
			if(!head || words[i][0] == head) f[1ll << i][i] = 1;
		}
		for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++) if(i & (1 << j)){
			assert(words[j] != NULL);
			for(int k = 0;k < len;k++) if(!(i & (1 << k))){
				assert(words[k] != NULL);
				if(words[j][b[j] - 1] == words[k][0]) f[i | (1ll << k)][k] = max(f[i | (1ll << k)][k], f[i][j] + 1);
			}
		}
		int ans = 0;
		for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++){
			assert(words[j] != NULL);
			if(!tail || words[j][b[j] - 1] == tail) ans = max(ans, f[i][j]);
		}
		if(ans == 1) ans = 0;
		return ans;
	}

	void checker(char* words[], int len, char* result[], int res_len){
		for(int i = 0;i < res_len;i++){
			bool tag = false;
			for(int j = 0;j < len;j++){
				assert(words != NULL);
				assert(result != NULL);
				if(strcmp(words[j], result[i]) == 0){
					tag = true;
					break;
				}
			}
			Assert::AreEqual(tag, true);
		}
	}

	void stress(int n, bool DAG, int len, unsigned int seed, char head, char tail){
		char** words = generator(n, DAG, len, seed);
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_word(words, len, result, head, tail, !DAG);
		int ans_len;
		if(len <= 8){
			ans_len = dp(words, len, head, tail);
			//if(ans_len != brute_force(words, len, head, tail)){
			//	assert(0);
			//}
			Assert::AreEqual(ans_len, brute_force(words, len, head, tail));
		}else ans_len = dp(words, len, head, tail);
		Assert::AreEqual(ans_len, out_len);
		checker(words, len, result, out_len);
	}



	TEST_CLASS(test_gen_chain_word){
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
			for(int len = 1;len <= 18;len++){
				for(int i = 0;i < 5;i++){
					seed = (len << 7) | (i << 13) ^ 123;
					stress(n, false, len, i, 0, 0);
					stress(n, true, len, i, 0, 0);
				}
			}
		}

		/*
		* 对拍，head 有限制 tail 无限制
		*/ 
		TEST_METHOD(stresses_1_0){
			for(int len = 1;len <= 18;len++){
				for(int i = 0;i < 5;i++){
					seed = (len << 7) | (i << 13) ^ 345;
					char head = rand() % n + 'a';
					stress(n, false, len, i, head, 0);
					stress(n, true, len, i, head, 0);
				}
			}
		}

		/*
		* 对拍，head 无限制 tail 有限制
		*/ 
		TEST_METHOD(stresses_0_1){
			for(int len = 1;len <= 18;len++){
				for(int i = 0;i < 5;i++){
					seed = (len << 7) | (i << 13) ^ 567;
					char tail = rand() % n + 'a';
					stress(n, false, len, i, 0, tail);
					stress(n, true, len, i, 0, tail);
				}
			}
		}

		/*
		* 对拍，head 和 tail 均有限制
		*/ 
		TEST_METHOD(stresses_1_1){
			for(int len = 1;len <= 18;len++){
				for(int i = 0;i < 5;i++){
					seed = (len << 7) | (i << 13) ^ 789;
					char head = rand() % n + 'a', tail = rand() % n + 'a';
					stress(n, false, len, i, head, tail);
					stress(n, true, len, i, head, tail);
				}
			}
		}

		
	};
}
