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

const char** generator(int n, bool DAG, int len, unsigned int Seed){
	seed = Seed ^ n ^ len;
	char** words = (char**)malloc(len * sizeof(char*));
	for(int i = 0;i < len;i++){
		//assert(words != nullptr);
		int len = rnd() % 10 + 3;
		words[i] = (char*)malloc((len + 1ll) * sizeof(char));
		//assert(words[i] != nullptr);
		if (words[i] != nullptr){
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
	}
	return (const char**)words;
}

int brute_force(const char* words[], int len, char head, char tail, bool weighted){
	//assert(words != nullptr);
	//assert(len <= 10);
	int a[10] = {0}, b[10] = {0};
	for(int i = 0;i < len;i++){
		a[i] = i;
		b[i] = (int)strlen(words[i]);
	}
	int ans = 0;
	do{
		//assert(words[a[0]] != nullptr);
		if(head && words[a[0]][0] != head) continue;
		int sum = weighted ? b[a[0]] : 1;
		for(int i = 1;i < len;i++){
			//assert(words != nullptr);
			//assert(words[a[i - 1]] != nullptr);
			//assert(words[a[i]] != nullptr);
			if(words[a[i]][0] != words[a[i - 1]][b[a[i - 1]] - 1]) break;
			sum += weighted ? b[a[i]] : 1;
			if(!tail || words[a[i]][b[a[i]] - 1] == tail) ans = std::max(ans, sum);
		}
	}while(std::next_permutation(a, a + len));
	return ans;
}

int f[1 << 20][20];
int dp(const char* words[], int len, char head, char tail, bool weighted){
	//assert(words != nullptr);
	//assert(len <= 20);
	int b[20];
	for(int i = 0;i < len;i++) b[i] = (int)strlen(words[i]);
	for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++) f[i][j] = (int)-1e9;
	for(int i = 0;i < len;i++){
		//assert(words[i] != nullptr);
		if(!head || words[i][0] == head) f[1ll << i][i] = weighted ? b[i] : 1;
	}
	for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++) if(i & (1 << j)){
		//assert(words[j] != nullptr);
		for(int k = 0;k < len;k++) if(!(i & (1 << k))){
			//assert(words[k] != nullptr);
			if(words[j][b[j] - 1] == words[k][0]) f[i | (1ll << k)][k] = std::max(f[i | (1ll << k)][k], f[i][j] + (weighted ? b[k] : 1));
		}
	}
	int ans = 0;
	for(int i = 0;i < (1 << len);i++) for(int j = 0;j < len;j++) if(i & (1 << j)){
		if(i == (1 << j)) continue;
		//assert(words[j] != nullptr);
		if(!tail || words[j][b[j] - 1] == tail) ans = std::max(ans, f[i][j]);
	}
	return ans;
}

void checker(const char* words[], int len, char* result[], int out_len){
	for(int i = 0;i < out_len;i++){
		bool tag = false;
		for(int j = 0;j < len;j++){
			//assert(words != nullptr);
			//assert(result != nullptr);
			if(strcmp(words[j], result[i]) == 0){
				tag = true;
				break;
			}
		}
		Assert::AreEqual(tag, true);
	}
}

