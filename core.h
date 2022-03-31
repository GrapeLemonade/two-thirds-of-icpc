void init_words(char* words[], int len);

void get_SCC();

void check_loop();

int get_all(char* result[]);

int get_max_DAG(char* result[], char head, char tail, bool enable_self_loop, bool weighted);

int get_max(char* result[], char head, char tail, bool weighted);

int engine(char* words[], int len, char* result[], char head, char tail, bool count, bool weighted, bool enable_self_loop, bool enable_ring);