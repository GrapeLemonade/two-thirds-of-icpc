#include "core.h"
#include <vector>
#include <string>

using namespace std;

const int n = 26;
int m;
vector<string> s;

void init_words(char* words[], int len){
	for(int i = 0;i < len;i++){
		string S = words[i];
		if(S.length() == 1) continue; // 单个字母不算单词
		s.push_back(S);
	}
	m = s.size();
}

vector<int> v[26];
int sk[26], top;
bool ins[26], vis[26];
int dfn[26], low[26], cnt;
int col[26], tot;


void dfs(int i){
	sk[++top] = i, vis[i] = ins[i] = true;
	dfn[i] = low[i] = ++cnt;
	for(auto x : v[i]){
		int to = s[x].back() - 'a';
		if(vis[to]){ 
			if(ins[to]) low[i] = min(low[i], low[to]);
		}else{
			dfs(to);
			low[i] = min(low[i], low[to]);
		}
	}
	if(dfn[i] == low[i]){
		col[i] = ++tot;
		while(1){
			int x = sk[top--];
			ins[x] = false;
			if(x == i) break;
			col[x] = col[i];
		}
	}
}

void get_SCC(){
	for(int i = 0;i < m;i++) v[s[i][0] - 'a'].push_back(i);
	for(int i = 0;i < n;i++) if(!vis[i]) dfs(i);
}

void check_loop(){

}

int get_all(char* result[]){

}

int get_max(char* result[], char head, char tail, bool enable_loop, bool enable_self_loop){

}
