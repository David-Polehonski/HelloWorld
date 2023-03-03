
struct log;
typedef struct log log_t;

log_t *logOpen(char *name);
void logWrite(log_t *log, char *text);
void logClose(log_t *log);