#include "core.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <map>

using namespace std;

typedef long long ll;

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
	return (int)ans.size();
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
	int x = head ? head - 'a' : (int)(max_element(f, f + 26) - f), len = f[x];
	while(1){
		if(h[x] != -1) ans.push_back(s[h[x]]);
		if(g[x] == -1) break;
		ans.push_back(s[g[x]]);
		x = s[g[x]].back() - 'a';
	}
	vector_to_result(result);
	return len;
}

map<tuple<ll, ll, int>, pair<int, int> > mp;
pair<ll, ll> val;
vector<pair<int, int> > w[26][26];
vector<int> V[26];
int pos[26][26];
int Tail;

void modify_val(int i){
	if(i < 64) val.first += (1ll << i);
	else val.second += 1ll << (i - 64);
}

void dfs_max(int i){
	if(mp.find(make_tuple(val.first, val.second, i)) != mp.end()) return;
	int ans = Tail == -1 || i == Tail ? 0 : (int)-1e9, id = -1;
	pair<ll, ll> Val = val;
	if(pos[i][i] < w[i][i].size()){
		modify_val(w[i][i][pos[i][i]].second), pos[i][i]++;
		dfs_max(i);
		ans = mp[make_tuple(val.first, val.second, i)].first + w[i][i][(long long)pos[i][i] - 1].first; 
		id = i;
		val = Val, pos[i][i]--;
	}else{
		for(auto j : V[i]) if(pos[i][j] < w[i][j].size()){
			if(col[i] == col[j]) modify_val(w[i][j][pos[i][j]].second);
			else val = {0, 0};
			pos[i][j]++;
			dfs_max(j);
			int sum = mp[make_tuple(val.first, val.second, j)].first + w[i][j][(long long)pos[i][j] - 1].first;
			if(ans < sum) ans = sum, id = j;
			val = Val, pos[i][j]--;
		}
	}
	mp[make_tuple(val.first, val.second, i)] = {ans, id};
}

int get_max(char* result[], char head, char tail, bool weighted){
	for(int i = 0;i < m;i++){
		w[s[i][0] - 'a'][s[i].back() - 'a'].push_back({weighted ? (int)s[i].length() : 1, i});
		V[s[i][0] - 'a'].push_back(s[i].back() - 'a');
	}
	for(int i = 0;i < 26;i++) for(int j = 0;j < 26;j++) sort(w[i][j].begin(), w[i][j].end());
	for(int i = 0;i < 26;i++) sort(V[i].begin(), V[i].end()), V[i].erase(unique(V[i].begin(), V[i].end()), V[i].end());
	Tail = tail ? tail - 'a' : -1;
	int x = -1;
	for(int i = 0;i < 26;i++) if(!head || head - 'a' == i){
		dfs_max(i);
		if(x == -1 || mp[make_tuple(0, 0, i)].first > mp[make_tuple(0, 0, x)].first) x = i;
	}
	int len = mp[make_tuple(0, 0, x)].first;
	while(1){
		int j = mp[make_tuple(val.first, val.second, x)].second;
		if(j == -1) break;
		ans.push_back(s[w[x][j][pos[x][j]].second]);
		if(col[x] == col[j]) modify_val(w[x][j][pos[x][j]].second);
		else val = {0, 0};
		pos[x][j]++;
		x = j;
	}
	vector_to_result(result);
	return len;
}

int engine(
	char* words[],
	int len,
	char* result[],
	char head,
	char tail,
	bool count,
	bool weighted,
	bool enable_self_loop,
	bool enable_ring){
	
	init_words(words, len);
	get_SCC();
	if(!enable_ring) check_loop();
	if(!count) return get_all(result);
	else if(enable_ring) return get_max(result, head, tail, weighted);
	else return get_max_DAG(result, head, tail, enable_self_loop, weighted);
}