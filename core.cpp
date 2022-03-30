#include "core.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

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
	m = (int)s.size();
}

vector<int> v[26];
int sk[26], top;
bool ins[26], vis[26];
int dfn[26], low[26], cnt;
int col[26], tot;

void dfs_SCC(int i){
	sk[++top] = i, vis[i] = ins[i] = true;
	dfn[i] = low[i] = ++cnt;
	for(auto j : v[i]){
		int to = s[j].back() - 'a';
		if(vis[to]){ 
			if(ins[to]) low[i] = min(low[i], low[to]);
		}else{
			dfs_SCC(to);
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
	for(int i = 0;i < n;i++) if(!vis[i]) dfs_SCC(i);
}

void check_loop(){
	for(int i = 0;i < n;i++){
		int last = -1;
		for(auto j : v[i]){
			if(s[j].back() - 'a' == i){
				if(last == -1) last = j;
				else throw logic_error("Word ring detected: " + s[last] + " " + s[j]);
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
				throw logic_error("Word ring detected: " + S);
			}
			siz[col[i]] = true;
		}
	}
}

vector<string> ans;

void vector_to_result(char* result[]){
	int siz = 0;
	for(auto &i : ans) siz += (int)i.length() + 1;
	char* p = (char*) malloc(siz);
	for(int i = 0;i < ans.size();i++){
		result[i] = p;
		for(auto j : ans[i]){
			assert(p);
			*p = j;
			p++;
		}
		assert(p);
		*p = 0;
		p++;
	}
}

vector<int> now;

void dfs_all(int i, bool loop){
	if(now.size() > 1){
		string S;
		for(auto j : now){
			if(!S.empty()) S += " ";
			S += s[j];
		}
		ans.push_back(S);
	}
	for(auto j : v[i]){
		int to = s[j].back() - 'a';
		if(i == to && loop) continue;
		now.push_back(j);
		dfs_all(to, i == to);
		now.pop_back();
	}
}

int get_all(char* result[]){
	for(int i = 0;i < 26;i++) dfs_all(i, false);
	vector_to_result(result);
	return (int)now.size();
}

int get_max_DAG(char* result[], char head, char tail, bool enable_self_loop, bool weighted){
	int a[26], f[26], g[26], h[26];
	for(int i = 0;i < 26;i++) a[col[i] - 1] = i;
	for(int i = 0;i < 26;i++){
		int x = a[i];
		f[x] = !tail || tail - 'a' == x ? 0 : (int)-1e9;
		g[x] = h[x] = -1;
		for(auto j : v[x]){
			int to = s[j].back() - 'a';
			if(to == x) continue;
			int val = f[to] + (weighted ? (int)s[j].length() : 1);
			if(val > f[x]) f[x] = val, g[x] = j;
		}
		if(!enable_self_loop) continue;
		for(auto j : v[x]){
			int to = s[j].back() - 'a';
			if(to == x){
				f[x] += weighted ? (int)s[j].length() : 1;
				h[x] = j;
				break;
			}
		}
	}
	int x = head ? head - 'a' : (int)(max_element(f, f + 26) - f);
	while(1){
		if(h[x] != -1) ans.push_back(s[h[x]]);
		if(g[x] == -1) break;
		ans.push_back(s[g[x]]);
		x = s[g[x]].back() - 'a';
	}
	vector_to_result(result);
	return f[x];
}
