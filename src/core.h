int engine(
	char* words[],
	int len,
	char* result[],
	char head,
	char tail,
	int type,
	bool weighted);

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

int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

int gen_chains_all(char* words[], int len, char* result[]);

int gen_chain_word_unique(char* words[], int len, char* result[]);

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);