void init_words(char* words[], int len);

void get_SCC();

void check_loop();

int get_all(char* result[]);

int get_max_DAG(char* result[], char head, char tail, bool enable_self_loop, bool weighted);
