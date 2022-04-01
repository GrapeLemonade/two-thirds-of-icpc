int engine(
	char* words[],
	int len,
	char* result[],
	char head,
	char tail,
	int type,
	bool weighted);

_declspec(dllexport)
int engine(
	char* words[],
	int len,
	char* result[],
	char head,
	char tail,
	bool count,
	bool weighted,
	bool enable_self_loop,
	bool enable_ring);

_declspec(dllexport)
int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

_declspec(dllexport)
int gen_chains_all(char* words[], int len, char* result[]);

_declspec(dllexport)
int gen_chain_word_unique(char* words[], int len, char* result[]);

_declspec(dllexport)
int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);