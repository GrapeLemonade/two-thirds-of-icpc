#include "core.h"
#include <vector>
#include <string>

using namespace std;

vector<string> s;

void init_words(char* words[], int len){
	for(int i = 0;i < len;i++){
		string S = words[i];
		if(S.length() == 1) continue; // 单个字母不算单词
		s.push_back(S);
	}
}

void get_SCC(){

}

void check_loop(){

}

int get_all(char* result[]){

}

int get_max(char* result[], char head, char tail, bool enable_loop, bool enable_self_loop){

}
