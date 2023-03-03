
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>

#if (defined(_WIN32) || defined(__WIN32__))
#include <direct.h>
#define mkdir(A, B) _mkdir(A)
#endif

#include <string.h>

#include "Log.h"

static char *directoryName = "logs";
struct log
{
	FILE *stream;
	char *name;
};
typedef struct log log_t;

log_t *logOpen(char *name)
{
	mkdir(directoryName, 777);

	log_t *log = malloc(sizeof(log_t));

	log->name = malloc(sizeof(directoryName) + sizeof(name) + 1);
	strcpy(log->name, directoryName);
	strcat(log->name, "/");
	strcat(log->name, name);

	log->stream = fopen(log->name, "a");

	return log;
}

void logWrite(log_t *log, char *text)
{

	fprintf(log->stream, "%s\n", text);
	fflush(log->stream);
}

void logClose(log_t *log)
{
	fclose(log->stream);
	free(log);
}