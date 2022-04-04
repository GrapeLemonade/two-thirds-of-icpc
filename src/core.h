#ifdef CORE
#define EXPOSED_FUNCTION extern "C" _declspec(dllexport)
#else
#define EXPOSED_FUNCTION extern "C" _declspec(dllimport)
#endif

EXPOSED_FUNCTION
int engine(
	const char* words[],
	int len,
	char* result[],
	char head,
	char tail,
	bool count,
	bool weighted,
	bool enable_self_loop,
	bool enable_ring);

EXPOSED_FUNCTION
const char* gui_engine(const char* input, int type, char head, char tail, bool weighted);

EXPOSED_FUNCTION
int gen_chain_word(const char* words[], int len, char* result[], char head, char tail, bool enable_loop);

EXPOSED_FUNCTION
int gen_chains_all(const char* words[], int len, char* result[]);

EXPOSED_FUNCTION
int gen_chain_word_unique(const char* words[], int len, char* result[]);

EXPOSED_FUNCTION
int gen_chain_char(const char* words[], int len, char* result[], char head, char tail, bool enable_loop);