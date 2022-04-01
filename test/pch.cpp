// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <cassert>
#include <string>

#pragma warning(disable:6385)
#pragma warning(disable:6386)


// 当使用预编译的头时，需要使用此源文件，编译才能成功。

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

unsigned int seed = 998244353;

unsigned int rnd(){
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
		int len = rnd() % 10 + 2;
		words[i] = (char*)malloc((len + 1) * sizeof(char));
		assert(words[i] != NULL);
		words[i][0] = rnd() % n + 'a';
		words[i][1] = (char)(i + 'a');
		for(int j = 2;j < len;j++) words[i][j] = (char)(rnd() % n + 'a');
		if(DAG && words[i][0] >= words[i][len - 1]){
			if(words[i][0] == words[i][len - 1]){
				if(words[i][0] == n - 1 + 'a') words[i][0]--;
				else words[i][len - 1]++;
			}else std::swap(words[i][0], words[i][len - 1]);
		}
		words[i][len] = 0;
	}
	return words;
}

int brute_force(char* words[], int len, char head, char tail, bool weighted){
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
		int sum = weighted ? b[a[0]] : 1;
		for(int i = 1;i < len;i++){
			assert(words != NULL);
			assert(words[a[i - 1]] != NULL);
			assert(words[a[i]] != NULL);
			if(words[a[i]][0] != words[a[i - 1]][b[a[i - 1]] - 1]) break;
			sum += weighted ? b[a[i]] : 1;
			if(!tail || words[a[i]][b[a[i]] - 1] == tail) ans = std::max(ans, sum);
		}
	}while(std::next_permutation(a, a + len));
	return ans;
}

int f[1 << 20][20];
int dp(char* words[], int len, char head, char tail, bool weighted){
	assert(words != NULL);
	assert(len <= 20);
	int b[20];
	for(int i = 0;i < len;i++) b[i] = (int)strlen(words[i]);
	for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++) f[i][j] = (int)-1e9;
	for(int i = 0;i < len;i++){
		assert(words[i] != NULL);
		if(!head || words[i][0] == head) f[1ll << i][i] = weighted ? b[i] : 1;
	}
	for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++) if(i & (1 << j)){
		assert(words[j] != NULL);
		for(int k = 0;k < len;k++) if(!(i & (1 << k))){
			assert(words[k] != NULL);
			if(words[j][b[j] - 1] == words[k][0]) f[i | (1ll << k)][k] = std::max(f[i | (1ll << k)][k], f[i][j] + (weighted ? b[k] : 1));
		}
	}
	int ans = 0;
	for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++) if(i & (1 << j)){
		if(i == (1 << j)) continue;
		assert(words[j] != NULL);
		if(!tail || words[j][b[j] - 1] == tail) ans = std::max(ans, f[i][j]);
	}
	return ans;
}

void checker(char* words[], int len, char* result[], int out_len){
	for(int i = 0;i < out_len;i++){
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

