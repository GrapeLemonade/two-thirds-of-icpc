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
	s.clear();
	for(int i = 0;i < len;i++){
		string S = words[i];
		if(S.length() == 1) continue; // 单个字母不算单词
		s.push_back(S);
	}
	m = (int)s.size();
}

vector<int> v[30];
int sk[30], top;
bool ins[30], vis[30];
int dfn[30], low[30], cnt;
int col[30], tot;

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
	for(int i = 0;i < 26;i++) v[i].clear(), vis[i] = false;
	cnt = tot = 0;
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
					vis[x] = true;
					for(auto j : v[x]){
						int to = s[j].back() - 'a';
						if(col[to] == col[i]){
							if(!S.empty()) S += " ";
							S += s[j];
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
	char* p = (char*) malloc(siz + 1);
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
	ans.clear();
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
	int len = (int)ans.size();
	vector_to_result(result);
	return len;
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
	int x = -1, sum = (int)-1e9;
	for(int i = 0;i < m;i++) if(!head || head == s[i][0]){
		int u = s[i][0] - 'a', v = s[i].back() - 'a';
		if(u == v){
			if(f[u] == (weighted ? (int)s[i].length() : 1)) continue;
			if(sum < f[u]) sum = f[u], x = i;
		}else{
			if(f[v] <= 0) continue;
			int Sum = f[v] + (weighted ? (int)s[i].length() : 1);
			if(sum < Sum) sum = Sum, x = i;
		}
	}
	if(x != -1){
		if(s[x][0] == s[x].back()) x = s[x][0] - 'a';
		else ans.push_back(s[x]), x = s[x].back() - 'a';
		while(1){
			if(h[x] != -1) ans.push_back(s[h[x]]);
			if(g[x] == -1) break;
			ans.push_back(s[g[x]]);
			x = s[g[x]].back() - 'a';
		}
	}
	int len = (int)ans.size();
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
		ans = mp[make_tuple(val.first, val.second, i)].first + w[i][i][pos[i][i] - 1ll].first; 
		id = i;
		val = Val, pos[i][i]--;
	}else{
		for(auto j : V[i]) if(pos[i][j] < w[i][j].size()){
			if(col[i] == col[j]) modify_val(w[i][j][pos[i][j]].second);
			else val = {0, 0};
			pos[i][j]++;
			dfs_max(j);
			int sum = mp[make_tuple(val.first, val.second, j)].first + w[i][j][pos[i][j] - 1ll].first;
			if(ans < sum) ans = sum, id = j;
			val = Val, pos[i][j]--;
		}
	}
	mp[make_tuple(val.first, val.second, i)] = {ans, id};
}

int get_max(char* result[], char head, char tail, bool weighted){
	mp.clear(), val = {0, 0};
	for(int i = 0;i < 26;i++) for(int j = 0;j < 26;j++) w[i][j].clear(), pos[i][j] = 0;
	for(int i = 0;i < 26;i++) V[i].clear();
	for(int i = 0;i < m;i++){
		w[s[i][0] - 'a'][s[i].back() - 'a'].push_back({weighted ? (int)s[i].length() : 1, i});
		V[s[i][0] - 'a'].push_back(s[i].back() - 'a');
	}
	for(int i = 0;i < 26;i++) for(int j = 0;j < 26;j++) sort(w[i][j].begin(), w[i][j].end(), greater<pair<int, int> >());
	for(int i = 0;i < 26;i++) sort(V[i].begin(), V[i].end()), V[i].erase(unique(V[i].begin(), V[i].end()), V[i].end());
	Tail = tail ? tail - 'a' : -1;
	for(int i = 0;i < 26;i++) if(!head || head - 'a' == i) dfs_max(i);
	int x = -1, sum = (int)-1e9;
	for(int i = 0;i < m;i++) if(!head || head == s[i][0]){
		int u = s[i][0] - 'a', v = s[i].back() - 'a';
		val = {0, 0};
		if(col[u] == col[v]) modify_val(i);
		if(mp[make_tuple(val.first, val.second, v)].first <= 0) continue;
		int Sum = mp[make_tuple(val.first, val.second, v)].first + (weighted ? (int)s[i].length() : 1);
		if(sum < Sum) sum = Sum, x = i;
	}
	val = {0, 0};
	if(x != -1){
		ans.push_back(s[x]);
		if(col[s[x][0] - 'a'] == col[s[x].back() - 'a']) modify_val(x), pos[s[x][0] - 'a'][s[x].back() - 'a']++;
		x = s[x].back() - 'a';
		while(1){
			int j = mp[make_tuple(val.first, val.second, x)].second;
			if(j == -1) break;
			ans.push_back(s[w[x][j][pos[x][j]].second]);
			if(col[x] == col[j]) modify_val(w[x][j][pos[x][j]].second);
			else val = {0, 0};
			pos[x][j]++;
			x = j;
		}
	}
	int len = (int)ans.size();
	vector_to_result(result);
	return len;
}


/*
* type = 0, enable self loop, disable ring, get all chains.
* type = 1, enable self loop, disable ring, get max chain.
* type = 2, disable self loop, disable ring, get max chain.
* type = 3, enable ring, get max chain.
*/

int engine(
	char* words[],
	int len,
	char* result[],
	char head,
	char tail,
	int type,
	bool weighted = false){

	init_words(words, len);
	if(type == 3){
		sort(s.begin(), s.end());
		s.erase(unique(s.begin(), s.end()), s.end());
		m = (int)s.size();
	}
	get_SCC();
	if(type < 3) check_loop();
	if(type == 0) return get_all(result);
	if(type == 3) return get_max(result, head, tail, weighted);
	return get_max_DAG(result, head, tail, type == 1, weighted);
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
	bool enable_ring) {

	int type = 0;
	if (count) type = 0;
	else if (enable_ring) type = 3;
	else if (enable_self_loop) type = 1;
	else type = 2;
	return engine(words, len, result, head, tail, type, weighted);
}

int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop){
	return engine(words, len, result, head, tail, enable_loop ? 3 : 1);
}

int gen_chains_all(char* words[], int len, char* result[]){
	return engine(words, len, result, 0, 0, 0);
}

int gen_chain_word_unique(char* words[], int len, char* result[]){
	return engine(words, len, result, 0, 0, 2);
}

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop){
	return engine(words, len, result, head, tail, enable_loop ? 3 : 1, true);
}