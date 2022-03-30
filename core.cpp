#include "core.h"
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

const int n = 26;
int m;
vector<string> s;

void init_words(char* words[], int len){
	for(int i = 0;i < len;i++){
		string S = words[i];
		if(S.length() == 1) continue; // ������ĸ���㵥��
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
	for(int i = 0;i < n;i++){
		int last = -1;
		for(auto j : v[i]){
			if(s[j].back() - 'a' == i){
				if(last == -1) last = j;
				else throw invalid_argument("Word ring detected: " + s[last] + " " + s[j]);
			}
		}
	}
	get_SCC();
	if(tot < 26){
		bool siz[27] = {0}, vis[26] = {0};
		for(int i = 0;i < 26;i++){
			if(siz[col[i]]){
				int x = i;
				string S;
				while(!vis[x]){
					S += " ";
					S += s[x];
					vis[x] = true;
					for(auto j : v[x]){
						int to = s[j].back() - 'a';
						if(col[to] == col[i]){
							x = to;
							break;
						}
					}
				}
				throw invalid_argument("Word ring detected: " + S);
			}
			siz[col[i]] = true;
		}
	}
}

int get_all(char* result[]){
		
}

int get_max(char* result[], char head, char tail, bool enable_loop, bool enable_self_loop){

}
